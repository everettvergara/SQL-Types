#pragma once
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace g80 {
    namespace odbc {

        class decimal {

        private:
            int8_t scale_;
            int64_t scale_mul_;
            int64_t data_;

            static inline auto get_scale_mul(int8_t s) -> int64_t {
                int64_t m{1};
                for(int8_t i{0}; i < s; ++i) m *= 10;
                return m;
            }

            auto scaled_str_to_ld(const std::wstring &n) const -> long double {
                auto ld = std::stold(n) * scale_mul_;
                ld += ld >= 0 ? 0.50 : -0.5;
                return ld;
            }

        public:

            // Blank constructor
            decimal() : 
                scale_{2},
                scale_mul_{100},
                data_{0} {
            }

            // Regular Constructor for integral and floating point types
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            decimal(const T t, const int8_t s = 2) : 
                scale_{s}, 
                scale_mul_{get_scale_mul(scale_)}, 
                data_{static_cast<int64_t>(t * scale_mul_ + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)))} {
            }

            // Regular Constructor for integral/fp types in string format
            decimal(const std::wstring &n, const int8_t s = 2) : 
                scale_{s}, 
                scale_mul_{get_scale_mul(scale_)}, 
                data_{static_cast<int64_t>(scaled_str_to_ld(n))} {
            }

            // Copy Constructor for decimal
            decimal(const decimal &d) : 
                scale_{d.scale_}, 
                scale_mul_{d.scale_mul_}, 
                data_{d.data_} {
            }

            // Destructor
            ~decimal() = default;

            // Copy Assignment for integral and floating types
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            auto operator=(const T t) -> decimal & {
                data_ = static_cast<int64_t>(t * scale_mul_ + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // Copy assignment for decimal
            auto operator=(const decimal &d) -> decimal & {
                scale_ = d.scale_;
                scale_mul_ = d.scale_mul_;
                data_ = d.data_;
                return *this;
            }

            // Copy assignment for decimal
            auto operator=(const std::wstring &n) -> decimal & {
                data_ = {static_cast<int64_t>(scaled_str_to_ld(n))};
                return *this;
            }

            // operator += for decimal
            auto operator+=(const decimal &d) -> decimal & {
                auto gs = scale_ >= d.scale_ ? scale_ : d.scale_;
                data_ = this->data_on_scale(gs) + d.data_on_scale(gs);
                scale_ = gs;
                scale_mul_ = get_scale_mul(gs);
                return *this;
            }

            // operator += for integral and fp
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            auto operator+=(const T t) -> decimal & {
                data_ += static_cast<int64_t>(t * scale_mul_ + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // operator += for string
            auto operator+=(const std::wstring &n) -> decimal & {
                data_ += static_cast<int64_t>(scaled_str_to_ld(n));
                return *this;
            }

            // operator -= for decimal
            auto operator-=(const decimal &d) -> decimal & {
                auto gs = scale_ >= d.scale_ ? scale_ : d.scale_;
                data_ = this->data_on_scale(gs) - d.data_on_scale(gs);
                scale_ = gs;
                scale_mul_ = get_scale_mul(gs);
                return *this;
            }

            // operator -= for integral and fp
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            auto operator-=(const T t) -> decimal & {
                data_ -= static_cast<int64_t>(t * scale_mul_ + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // operator -= for string
            auto operator-=(const std::wstring &n) -> decimal & {
                data_ -= static_cast<int64_t>(scaled_str_to_ld(n));
                return *this;
            }

            // operator *= for decimal
            auto operator*=(const decimal &d) -> decimal & {
                auto gs = scale_ >= d.scale_ ? scale_ : d.scale_;
                auto gm = scale_mul_ >= d.scale_mul_ ? scale_mul_ : d.scale_mul_;
                data_ = this->data_on_scale(gs) * d.data_on_scale(gs) / gm;
                scale_ = gs;
                scale_mul_ = gm;
                return *this;
            }

            // operator *= for integral and fp
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            auto operator*=(const T t) -> decimal & {
                data_ = static_cast<int64_t>(t * data_ + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // operator *= for string
            auto operator*=(const std::wstring &n) -> decimal & {
                auto ld = std::stold(n);
                data_ = static_cast<int64_t>(ld * data_ + (ld >= 0 ? 0.5 : -0.5));
                return *this;
            }

            // operator /= for decimal
            auto operator/=(const decimal &d) -> decimal & {
                auto gs = scale_ >= d.scale_ ? scale_ : d.scale_;
                auto gm = scale_mul_ >= d.scale_mul_ ? scale_mul_ : d.scale_mul_;
                data_ = this->data_on_scale(gs) / d.data_on_scale(gs) * gm;
                scale_ = gs;
                scale_mul_ = gm;
                return *this;
            }

            // operator /= for integral and fp
            template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
            auto operator/=(const T t) -> decimal & {
                data_ = static_cast<int64_t>(data_ / t + (std::is_integral_v<T> ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // operator /= for string
            auto operator/=(const std::wstring &n) -> decimal & {
                auto ld = std::stold(n);
                data_ = static_cast<int64_t>(data_ / ld + (ld >= 0 ? 0.5 : -0.5));
                return *this;
            }

            friend auto operator==(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) == r.data_on_scale(gs);
            }

            friend auto operator!=(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) != r.data_on_scale(gs);
            }

            friend auto operator>(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) > r.data_on_scale(gs);
            }

            friend auto operator>=(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) >= r.data_on_scale(gs);
            }

            friend auto operator<(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) < r.data_on_scale(gs);
            }

            friend auto operator<=(const decimal &l, const decimal &r) -> bool {
                auto gs = l.scale_ >= r.scale_ ? l.scale_ : r.scale_;
                return l.data_on_scale(gs) <= r.data_on_scale(gs);
            }

            // Helpers
            auto get_data() const -> int64_t {
                return data_;
            }

            auto get_whole() const -> int64_t {
                return data_ / scale_mul_;
            }

            auto get_scale() const -> int8_t {
                return scale_;
            }

            auto get_scale_mul() const -> int64_t {
                return scale_mul_;
            }

            auto get_part() const -> int64_t {
                return data_ % scale_mul_;
            }

            auto get_as_ldouble() const -> long double {
                return static_cast<long double>(1.0 * data_ / scale_mul_);
            }

            inline auto data_on_scale(int8_t s) const -> int64_t {
                int8_t ds = s - scale_;
                if(ds == 0) return data_;
                auto mul = decimal::get_scale_mul(ds < 0 ? -ds : ds);
                if(ds > 0) return data_ * mul;
                return data_ / mul;
            }

            auto rescale(int8_t s) -> int64_t {
                return data_ = this->data_on_scale(s);
            }
        };

        template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_class_v<decimal>
        auto operator+(decimal l, const T &r) -> decimal {
            l += r;
            return l;
        }

        template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_class_v<decimal>
        auto operator-(decimal l, const T &r) -> decimal {
            l -= r;
            return l;
        }

        template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_class_v<decimal>
        auto operator*(decimal l, const T &r) -> decimal {
            l *= r;
            return l;
        }

        template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_class_v<decimal>
        auto operator/(decimal l, const T &r) -> decimal {
            l /= r;
            return l;
        }

        auto operator<<(std::ostream &o, const decimal &d) -> std::ostream & {
            o << d.get_as_ldouble();
            return o;
        }

        auto operator>>(std::istream &i, decimal &d) -> std::istream & {
            long double ld;
            i >> ld;
            d = ld;
            return i;
        }
    }
}