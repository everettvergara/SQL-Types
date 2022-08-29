
#include <iostream>
#include "types/decimal.hpp"

using namespace g80::odbc;
auto main() -> int {

    decimal constr_from_int(123, 2);
    decimal constr_from_fp(123.456f, 2);

    std::cout << constr_from_int.get_as_ldouble() << "\n";
    std::cout << constr_from_fp.get_as_ldouble() << "\n";
}