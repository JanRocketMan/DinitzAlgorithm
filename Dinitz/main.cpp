#include "CFlowGraph.h"
#include "DinitzAlgo.h"

#include <iostream>
#include <string>

int main() {
	std::string pattern, Text;
	size_t curr_distance = 0, ans = 0;

	std::cin >> Text >> pattern;

	CFLowGraph G(Text.size() + pattern.size() + 2);

	G.DeclareEdge(0, Text.size() + pattern.size() + 1, 0);
	for (size_t i = 0; i < pattern.size(); i++) {
		for (size_t j = i; j < i + Text.size() - pattern.size() + 1; j++) {
			if (pattern[i] == '0' && Text[j] == '1') {
				curr_distance++;
			}
			else if (pattern[i] == '0' && Text[j] == '?') {
				G.DeclareEdge(0, j + 1, 1);
			}
			else if (pattern[i] == '1' && Text[j] == '0') {
				curr_distance++;
			}
			else if (pattern[i] == '1' && Text[j] == '?') {
				G.DeclareEdge(j + 1, Text.size() + pattern.size() + 1, 1);
			}
			else if (pattern[i] == '?' && Text[j] == '0') {
				G.DeclareEdge(0, Text.size() + i + 1, 1);
			}
			else if (pattern[i] == '?' && Text[j] == '1') {
				G.DeclareEdge(Text.size() + i + 1, Text.size() + pattern.size() + 1, 1);
			} 
			else if (pattern[i] == '?' && Text[j] == '?') {
				G.DeclareEdge(j + 1, Text.size() + i + 1, 1, 1);
			}
		}
	}

	DinitzAlgorithm D(G, 0, G.GetSize() - 1);

	ans = curr_distance + D.FindMaxFlow();
	std::cout << ans << std::endl;

	std::vector<bool> PredictedZero = G.GetReachableFromSource();

	for (size_t i = 0; i < pattern.size(); i++) {
		if (pattern[i] == '?') {
			if (PredictedZero[Text.size() + i + 1] == true) {
				pattern[i] = '0';
			}
			else {
				pattern[i] = '1';
			}
		}
	}

	for (size_t i = 0; i < Text.size(); i++) {
		if (Text[i] == '?') {
			if (PredictedZero[i + 1] == true) {
				Text[i] = '0';
			}
			else {
				Text[i] = '1';
			}
		}
	}

	std::cout << Text << std::endl;
	std::cout << pattern << std::endl;

	// system("pause");
	return 0;
}