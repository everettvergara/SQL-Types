# ODBC SQL-Types
ODBC Data Types for SQL

Decimal Type: g80::odbc::decimal
--

What's a decimal? As compared to floating point precisions, 
a decimal has a fixed decimal places called a scale.
Useful for currency and accounting.

Parts of a decimal number
--

|<- precision ->|
12345678.99999999
            ^ scale

This class ranges from {-2^64 to 2^64-1} / scale_mul
where scale: 0  = No decimal, scale_mul = 1
             1  = 1 decimal place, scale_mul = 10
             2  = 2 decimal places, scale_mul = 100
             3  = 3 decimal places, scale_mul = 1000
             .  = .
             n  = n decimal places, scale_mul = 10^n


Constructors:
--

decimal blank;                                  // Defines a blank constructor, with a scale of 2 
                                                // decimal places (default)

template<typename T>                            // Defines a decimal from an integral or 
requires std::is_integral_v<T> ||               // floating-point types
std::is_floating_point_v<T>
decimal(const T t, const int8_t s = 2)

decimal(const std::wstring &n,                  // Defines a decimal from a std::wstring
const int8_t s = 2)                             // Because Windows ODBC uses wchar_t instead of char

decimal(const decimal &d)                       // Copy constructor

template<typename T>                            // Copy assignment for integral and floating types
requires std::is_integral_v<T> || 
std::is_floating_point_v<T>
auto operator=(const T t) -> decimal &

template<typename T>                            // Copy assignment for integral and floating types
requires std::is_integral_v<T> || 
std::is_floating_point_v<T>
auto operator=(const T t) -> decimal &

auto operator=(const decimal &d) -> decimal &   // Copy assignment for decimal types

auto operator=(const std::wstring &n)           // Copy assignment for wstring types
