
#include <iostream>
#include "types/decimal.hpp"

using namespace g80::odbc;
auto main() -> int {

    decimal def, def2;
    decimal constr_from_int1{123, 2};
    decimal constr_from_int2{123};
    decimal constr_from_fp1{123.456f, 2};
    decimal constr_from_fp2{123.456f};
    decimal constr_from_dec(constr_from_fp1);
    
    def = -555.55;
    def2 = decimal{100.36, 2}; 

    std::cout << constr_from_int1.get_as_ldouble() << "\n";
    std::cout << constr_from_int2.get_as_ldouble() << "\n";
    std::cout << constr_from_fp1.get_as_ldouble() << "\n";
    std::cout << constr_from_fp2.get_as_ldouble() << "\n";
    std::cout << constr_from_dec.get_as_ldouble() << "\n";
    std::cout << "def2: " << def2.get_part() << "\n";
    
}
