# Decimal Type
--
What's a decimal class (g80::odbc::decimal)? 

A C++ data type intended to handle SQL decimal types.
Compared with floating-point precisions, a decimal has a fixed decimal places called a scale.
Useful for currency and accounting where you exactly need 2 decimal places for book keeping (scale of 2).

Parts of a decimal number
--
```c++
/*
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
*/
```

Available Constructors:
--

```c++
decimal blank;                              // Equivalent to 0.00, defaults to 2 decimal places
decimal from_floating_fp {1234.56, 3};      // Equivalent to 1234.560, with 3 decimal places (default is 2)
decimal from_integral {1234, 3};            // Equivalent to 1234.000, with 3 decimal places (default is 2)
decimal from_str {L"1234.56", 3}            // Equivalent to 1234.560, with 3 decimal places (default is 2)
                                            // Note: No move constructor since the class only uses primitive types
```

Assignments:
--

```c++
decimal assign_from_floating_fp{0, 4}       // Defines a decimal with a scale of 4
assign_from_floating_fp = {1234.56}         // decimal becomes 1234.5600
assign_from_floating_fp = {1234.56789}      // decimal becomes 1234.5679
assign_from_floating_fp = {33}              // decimal becomes 33.0000
assign_from_floating_fp = {"333.45"}        // decimal becomes 33.4500
```

Arithmetic:
--
```c++
/*
    Supports the following basic arithmetic operations
    (as of this writing - 2022-08-30)

    +=, -=, *=, /=, +, -, *, /

*/
```

Equality Operations:
--
```c++
/*
    Supports the following basic arithmetic operations
    (as of this writing - 2022-08-30)

    ==, !=, >, >=, <, <=

*/
```

Stream Operations:
--
```c++
/*
    Supports the following basic arithmetic operations
    (as of this writing - 2022-08-30)

    <<, >>

*/
```

Helpers:
--
```c++

 auto get_data() const -> int64_t;
 auto get_whole() const -> int64_t;
 auto get_part() const -> int64_t;
 auto get_scale() const -> int8_t;
 auto get_scale_mul() const -> int64_t;
 auto get_as_ldouble() const -> long double;

```