
#include <iostream>
#include "types/decimal.hpp"

using namespace g80::odbc;
auto main() -> int {


    decimal d1{0, 4};
    
    std::cout << "Input number: ";
    std::cin >> d1;

    auto d2 = 10.0f + d1;
    std::cout << "Hello: " << d2 << "\n";

    return 0;

    decimal def{6.0, 4}, def2{2.0, 5};
    decimal constr_from_int1{"245.56", 2};
    decimal constr_from_int2{123};
    decimal constr_from_fp1{123.456f, 2};
    decimal constr_from_fp2{123.456f};
    decimal constr_from_dec(constr_from_fp1);
    
    // def = 2.0;
    // def *= "-555.555";
    // def2 = def; // decimal{100.36, 2}; 

    def /= def2;

    std::cout << constr_from_int1.get_as_ldouble() << "\n";
    std::cout << constr_from_int2.get_as_ldouble() << "\n";
    std::cout << constr_from_fp1.get_as_ldouble() << "\n";
    std::cout << constr_from_fp2.get_as_ldouble() << "\n";
    std::cout << constr_from_dec.get_as_ldouble() << "\n";
    std::cout << "def whole: " << def.get_whole() << "\n";
    std::cout << "def part: " << def.get_part() << "\n";
    std::cout << "def data: " << def.get_data() << "\n";
    std::cout << "def scale: " << (int) def.get_scale() << "\n";
    std::cout << "def scale mul: " << def.get_scale_mul() << "\n";
    
}
