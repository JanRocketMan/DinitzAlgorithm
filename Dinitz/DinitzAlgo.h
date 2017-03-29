#pragma once
#include "CFLowGraph.h"
#include <queue>

// Algorithm for computing max flow in CFLowGraph presented by Y.Dinitz. Complexity is O((V^2)E).
// It is capable only to calculate maximum flow between vertexes s(source) and t(sink), which indexes
// are initialized in the contructor.

class DinitzAlgorithm {
public:
	DinitzAlgorithm() = delete;
	DinitzAlgorithm(CFLowGraph& _G, size_t source, size_t sink) : G(_G), s(source), t(sink)
	{
		N = G.GetSize();
		dist.reserve(N);
		start.reserve(N);
	}
	~DinitzAlgorithm() = default;


	// UpdateLayer is a bfs made for recalculating distances between source and other vertexes in the residual edge.
	// It ignores all saturated edges and Returns whether the sink had been reached or not.
	bool UpdateLayer()
	{
		dist.assign(N, inf2);
		std::queue<size_t> q;
		dist[s] = 0;
		q.push(s);

		while (!q.empty()) {
			size_t currv = q.front();
			q.pop();
			start[currv] = G.GetBegin(currv);

			if (currv == t) break;
			for (std::vector<flow_edge>::iterator it = G.GetBegin(currv); it != G.GetEnd(currv); it++) {
				if (it->capacity == it->flow) continue;
				size_t adjv = it->end;

				if (dist[adjv] == inf2) {
					dist[adjv] = dist[currv] + 1;
					q.push(adjv);
				}
			}
		}

		while (!q.empty()) {
			size_t currv = q.front();
			q.pop();
			start[currv] = G.GetBegin(currv);
		}

		return (dist[t] != inf2);
	}

	// To compute max flow, we will calculate current distance to s and will take into account only those
	// edges, that lay in the LayerTree of the graph, which contains only non-saturated edges that 
	// lead from vertexes with distance n to vertexes with distance n + 1.
	int FindMaxFlow()
	{
		for (size_t i = 0; i < N; i++) {
			start.push_back(G.GetBegin(0));
		}
		int ans = 0, delta = 0;
		while (UpdateLayer()) {
			do {
				delta = pushing_dfs(s, inf);
				ans += delta;
			} while (delta != 0);
		}
		return ans;

	}

private:

	// Pushing dfs is a dfs that pushes maximum flow we can push through the path in layer tree.
	// To optimise it, we save an iterator that shows the first non-saturated possible edge in tree.
	int pushing_dfs(size_t currv, int canpush)
	{
		if (currv == t || canpush == 0) return canpush;

		for (auto pos = start[currv]; pos != G.GetEnd(currv); pos++) {
			size_t adjv = pos->end;
			if (dist[adjv] != dist[currv] + 1) {
				start[currv]++;
				continue;
			}

			assert(pos->capacity >= pos->flow);

			int newpush = pushing_dfs(adjv, std::min(canpush, pos->capacity - pos->flow));
			if (newpush == 0) {
				start[currv]++;
				continue;
			}
			else {
				G.AddFlow(currv, adjv, newpush, pos);
				if (pos->capacity == pos->flow) start[currv]++;
				return newpush;
			}
		}
		return 0;
	}

	std::vector<size_t> dist;
	std::vector<std::vector<flow_edge>::iterator> start;


	int inf = std::numeric_limits<int>::max(), inf2 = std::numeric_limits<size_t>::max();
	size_t N, s, t;
	CFLowGraph& G;
};