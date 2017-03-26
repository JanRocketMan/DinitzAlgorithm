#pragma once
#include "CFLowGraph.h"
#include <ctime>
#include <queue>
#include <iostream>

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
		for (size_t i = 0; i < N; i++) {
			start[i] = G.GetBegin(i);
		}
		dist.assign(N, inf2);
		std::queue<size_t> q;
		dist[s] = 0;
		q.push(s);

		while (!q.empty()) {
			size_t currv = q.front();
			q.pop();
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

		return (dist[t] != inf2);
	}

	// To compute max flow, we will calculate current distance to s and will take into account only those
	// edges, that lay in the LayerTree of the graph, which contains only non-saturated edges that 
	// lead from vertexes with distance n to vertexes with distance n + 1.
	int FindMaxFlow()
	{
		int ans = 0, delta = 0;
		float avg_bfs = 0, avg_dfs = 0;
		std::vector<float> bfs_times, dfs_times;
		clock_t bfs_clock = clock();

		clock_t cl2 = clock();
		while (UpdateLayer()) {
			bfs_clock = clock() - bfs_clock;
			bfs_times.push_back((float)(bfs_clock) / CLOCKS_PER_SEC);
			do {
				clock_t dfs_clock = clock();
				delta = pushing_dfs(s, inf);
				dfs_times.push_back((float)(clock() - dfs_clock) / CLOCKS_PER_SEC);
				ans += delta;
			} while (delta != 0);
			bfs_clock = clock();
			// times.push_back(((float) (clock() - newclock) ) / CLOCKS_PER_SEC);
		}
		std::cout << "Dinitz Time: " << (float)(clock() - cl2) / CLOCKS_PER_SEC << std::endl;
		for (size_t i = 0; i < bfs_times.size(); i++) {
			avg_bfs += (bfs_times[i] / bfs_times.size());
		}
		for (size_t i = 0; i < dfs_times.size(); i++) {
			avg_dfs += (dfs_times[i] / dfs_times.size());
		}
		std::cout << "Total Bfs Time: " << avg_bfs << " * " << bfs_times.size() << " = " << avg_bfs * bfs_times.size() << std::endl;
		std::cout << "Total Dfs Time: " << avg_dfs << " * " << dfs_times.size() << " = " << avg_dfs * dfs_times.size() << std::endl;
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
