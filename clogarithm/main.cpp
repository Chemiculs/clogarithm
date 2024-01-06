#include "clogarithm.hpp" // include c_logarithm.hpp

int main() {

    intptr_t x = 1000000; // the target number to resolve logarithmic base(s) for

    clogarithm::clogarithm_t logarithm_resolver(x); // instantiate the clogarithm object

    auto all_logarithms = logarithm_resolver.find_all_logarithmic_bases(); // resolve all logarithmic base(s) of x

    for (auto& i : *all_logarithms) // this will output: " base: 10, logarithmic exponent: 6 \n base: 100, logarithmic exponent: 3 \n base: 1000, logarithmic exponent: 2" 
        printf("base: %d, logarithmic exponent: %d\n", (int)i.base, (int)i.logarithm); // clogarithm_entry_t.base is the base of x which we are resolving the logarithmic base of, the logarithmic base of [base] is stored in clogarithm_entry_t.logarithm

    logarithm_resolver.clear_logarithm_engine(); // this needs to be called if you wish to begin searching for logarithms again through the same object after calling find_all_logarithmic_bases()

    //If you wanted to change the number being resolved for, you can call logarithm_resolver.set_x( <value> ) at any time;

    auto& first_logarithm = logarithm_resolver.find_next_logarithmic_base(); // find the lowest logarithmic base (iterator) of (x = 1000000 in our case)  

    if (first_logarithm.valid) // did we find a logarithm?
        printf("base: %d, logarithmic exponent: %d\n", (int)first_logarithm.base, (int)first_logarithm.logarithm); // this code will execute and print: " base: 10, logarithmic exponent: 6 "
    else
        printf("%s", "no logarithmic bases found!\n \n");

    getchar();

    return 0;
}

