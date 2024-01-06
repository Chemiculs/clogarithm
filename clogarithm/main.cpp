#include "clogarithm.h" // include c_logarithm.hpp

int main() {

	intptr_t x = 0; // the target number to resolve logarithmic base(s) for

	printf("%s", "Please enter a number to find all integral logarithms of:\n");

	scanf_s("%d", &x);

	c_logarithm::clogarithm logarithm_resolver(x); // instantiate the clogarithm object

	auto all_logarithms = logarithm_resolver.find_all_logarithmic_bases(); // resolve all logarithmic base(s) of x

	for (auto& i : *all_logarithms)
		printf("base: %d, logarithmic exponent: %d\n", (int) i.base, (int) i.logarithm); // clogarithm_entry_t.base is the base of x which we are resolving the logarithmic base of, the logarithmic base of [base] is stored in clogarithm_entry_t.logarithm

	printf("%s", "Please enter a number to find the first logarithmic base of:\n");

	scanf_s("%d", &x);

	logarithm_resolver.clear_logarithm_engine(); // this needs to be called if you wish to begin searching for logarithms again through the same object after calling find_all_logarithmic_bases()

	auto& first_logarithm = logarithm_resolver.find_next_logarithmic_base(); // find the lowest logarithmic base (iterator) of (x)  

	if (first_logarithm.valid) // did we find a logarithm?
		printf("base: %d, logarithmic exponent: %d\n", (int)first_logarithm.base, (int)first_logarithm.logarithm);
	else
		printf("%s", "no logarithmic bases found!\n");

	getchar();

	return 0;
}