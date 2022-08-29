#pragma once
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace g80 {
    namespace odbc {


        /**
         * 
         *      a: 250
         *      b: 15 
         * 
         *      +
         * 
         *      max_allowance: 255 - a: 0
         * 
         *      if b <= max_allowance, co = 0
         *      else co = max - b
         * 
         *      
         * 
         */

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



        public:

            // Blank constructor
            decimal() : 
                scale_{2},
                scale_mul_{100},
                data_{0} {
            }

            // Regular Constructor for integral types
            template<typename T> requires std::is_integral<T>::value || std::is_floating_point<T>::value
            decimal(const T i, const int8_t s = 2) : 
                scale_{s}, 
                scale_mul_{get_scale_mul(scale_)}, 
                data_{static_cast<int64_t>(i * scale_mul_ + (std::is_integral<T>::value ? 0 : (i >= 0 ? 0.5 : -0.5)))} {
            }

            // Regular Constructor for integral/fp types in string format
            decimal(const std::string &n, const int8_t s = 2) : 
                scale_{s}, 
                scale_mul_{get_scale_mul(scale_)}, 
                data_{static_cast<int64_t>(std::stold(n) * scale_mul_ + (std::stold(n) >= 0 ? 0.5 : -0.5))} {
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
            template<typename T> requires std::is_integral<T>::value || std::is_floating_point<T>::value
            auto operator=(const T t) -> decimal & {
                data_ = static_cast<int64_t>(t * scale_mul_ + (std::is_integral<T>::value ? 0 : (t >= 0 ? 0.5 : -0.5)));
                return *this;
            }

            // Copy assignment for decimal
            auto operator=(const decimal &d) -> decimal & {
                scale_ = d.scale_;
                scale_mul_ = d.scale_mul_;
                data_ = d.data_;
                return *this;
            }

            auto get_whole() const -> int64_t {
                return data_ / scale_mul_;
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
    }
}