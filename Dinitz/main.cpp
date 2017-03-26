#include "CFlowGraph.h"
#include "DinitzAlgo.h"

#include <algorithm>
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
		std::vector<size_t> Next_Familiar;
		size_t g = 0;
		std::cin >> g;
		while (g != 0) {
			Next_Familiar.push_back(g);
			std::cin >> g;
		}
		std::sort(Next_Familiar.begin(), Next_Familiar.end());
		Familiar.push_back(Next_Familiar);
	}

	CFLowGraph G(n + m + 2);

	G.DeclareVertex(0, m);
	for (size_t i = 0; i < m; i++) {
		G.DeclareVertex(i + 1, Familiar[i].size());
		G.DeclareEdge(0, i + 1, 1);
	}

	for (size_t i = 0; i < m; i++) {
		size_t nextpos = 0;
		for (size_t j = 0; j < n; j++) {
			if (j + 1 != Familiar[i][nextpos]) {
				G.DeclareEdge(i + 1, m + j + 1, 1);
			} else {
				nextpos++;
			}
		}
	}

	for (size_t i = 0; i < n; i++) {
		G.DeclareEdge(i + m + 1, n + m + 1, 1);
	}

	DinitzAlgorithm D(G, 0, G.GetSize() - 1);
	D.FindMaxFlow();

	std::pair < std::vector<size_t>, std::vector<size_t> > Ans = G.GetMaxIndependentSet(m + 1);

	std::cout << Ans.first.size() + Ans.second.size() << std::endl;
	std::cout << Ans.first.size() << " " << Ans.second.size() << std::endl;

	for (size_t i = 0; i < Ans.first.size(); i++) {
		std::cout << Ans.first[i] << " ";
	}
	std::cout << std::endl;

	for (size_t i = 0; i < Ans.second.size(); i++) {
		std::cout << Ans.second[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
	size_t k = 0;

	std::cin >> k;

	for (size_t i = 0; i < k; i++) {
		process_test();
		std::cout << std::endl;
	}
	return 0;
}