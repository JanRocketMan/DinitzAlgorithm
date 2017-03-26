#include "CFlowGraph.h"
#include "DinitzAlgo.h"

#include <iostream>
#include <map>
#include <ctime>

void process_test()
{
	size_t m = 0, n = 0;
	std::vector< std::vector<size_t> > Familiar;

	std::cin >> m >> n; // m - number of boys, n - number of girls
	Familiar.reserve(m);

	for (size_t i = 0; i < m; i++) {
		size_t g = 0;
		std::cin >> g;
		while (g != 0) {
			Familiar[i].push_back(g);
			std::cin >> g;
		}
		std::sort(Familiar[i].begin(), Familiar[i].end());
	}

	CFLowGraph G(n + m + 2);
}

int main() {
	size_t k = 0;

	std::cin >> k;

	for (size_t i = 0; i < k; i++) {
		process_test();
	}
	
	return 0;
}