
#include <iostream>
#include "types/decimal.hpp"

using namespace g80::odbc;
auto main() -> int {

    decimal constr_from_int1{123, 2};
    decimal constr_from_int2{123};
    decimal constr_from_fp1{123.456f, 2};
    decimal constr_from_fp2{123.456f};
    decimal constr_from_dec(constr_from_fp1, 3);
    decimal constr_from_dec2(constr_from_fp1);


    std::cout << constr_from_int1.get_as_ldouble() << "\n";
    std::cout << constr_from_int2.get_as_ldouble() << "\n";
    std::cout << constr_from_fp1.get_as_ldouble() << "\n";
    std::cout << constr_from_fp2.get_as_ldouble() << "\n";
    std::cout << constr_from_dec.get_as_ldouble() << "\n";
    std::cout << constr_from_dec2.get_as_ldouble() << "\n";

}