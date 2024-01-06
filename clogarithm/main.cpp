#include "c_logarithm.h"

int main() {

	intptr_t x = 0;

	printf("%s", "Please enter a number to find all integral logarithms of:\n");

	scanf_s("%d", &x);
	
	c_logarithm::clogarithm logarithm_resolver(x);

	auto all_logarithms = logarithm_resolver.find_all_logarithmic_bases();

	for (auto& i : *all_logarithms)
		printf("base: %d, logarithmic exponent: %d\n", (int)i.base, (int)i.logarithm);

	getchar();

	return 0;
}