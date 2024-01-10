/*
/********************************************************************
*                                                                  *
*     Highly optimized Logarithmic base finder written in C++      *
*				This object uses C++ exceptions					   *
*						© Lane Graham, 2024                        *
*			https://github.com/Chemiculs/clogarithm                *
*                                                                  *
*********************************************************************
*/

#include "clogarithm.hpp" // include c_logarithm.hpp

#include <climits>        // for UINT16_MAX

int main() {



    intptr_t __x = 1000000; // the target number to resolve logarithmic base(s) for

    clogarithm::clogarithm_t logarithm_resolver(__x); // instantiate the clogarithm object

    auto all_logarithms = logarithm_resolver.find_all_logarithmic_bases(); // resolve all logarithmic base(s) of x

    for (auto& i : *all_logarithms) // this will output: " base: 10, logarithmic exponent: 6 \n base: 100, logarithmic exponent: 3 \n base: 1000, logarithmic exponent: 2" 
        printf("base: %d, logarithmic exponent: %d\n", (int)i.base, (int)i.logarithm); // clogarithm_entry_t.base is the base of x which we are resolving the logarithmic base of, the logarithmic base of [base] is stored in clogarithm_entry_t.logarithm

    logarithm_resolver.clear_logarithm_engine(); // this needs to be called if you wish to begin searching for logarithms again through the same object after calling find_all_logarithmic_bases()

    //If you wanted to change the number being resolved for, you can call logarithm_resolver.set_x( <value> ) at any time;

    auto& first_logarithm = logarithm_resolver.find_next_logarithmic_base(); // find the lowest logarithmic base (iterator) of (x = 1000000 in our case)  

    if (first_logarithm.valid) // did we find a logarithm?
        printf("lowest logarithmic base: %d, logarithmic exponent: %d\n", (int)first_logarithm.base, (int)first_logarithm.logarithm); // this code will execute and print: " base: 10, logarithmic exponent: 6 "
    else
        printf("%s", "no logarithmic bases found!\n \n");



    // //
    // //
    // //
    // 
    // 
    // //
    // //
    // //
    // //
    // //
    // 
    // 
    // //
    // //
    // //
    
     
  
    // For our second example, we will resolve all logarithmic equations for all possible 16-bit unsigned numbers (excluding 0 && 1)  

    std::uint16_t _x = CLOGARITHM_BASE_B_MIN_POSITIVE; // minimum possible number to target as (X) (-1 - 1 cause UB)

    // increment X until UINT16_MAX is reached
    clogarithm::clogarithm_t resolver(_x);

    while (true) { 

        resolver.set_x(_x); // update the target (X) of the logarithm resolver

        auto all_logarithms = resolver.find_all_logarithmic_bases();

        for (auto& logarithm_descriptor : *all_logarithms)
            printf("logarithmic base %d of %d is %d \n", (int)logarithm_descriptor.base, _x, (int)logarithm_descriptor.logarithm);

        if (_x == UINT16_MAX)
            break;

        ++_x;
    }

    getchar();

    return 0;
}

