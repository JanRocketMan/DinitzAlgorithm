#pragma once
#include <cassert>
#include <cstddef>
#include <vector>
#include <limits>
#include <unordered_map>

// Copyright by Jan RocketMan, 2017.

// CFLowGraph is a class which represents the Oriented Flow Network. It is considered that it has only one source
// and sink (with indexes 0 and (N - 1) respectively), that there is no multiple edges or loops
// and that the size of the Network doesn't exceed some predefined value N.

// It is made as an unordered_map of different vertexes, that store vectors of outcoming edges. As far as for many 
// algorithms of computing max flow it is important to declare a reverse edge with zero capacity for each straight one
// we will also save a link to reverse edge.

// Edge of CFlowGraph
struct flow_edge {
	flow_edge() = delete;
	flow_edge(size_t _end, int _capacity, size_t index_of_reverse) : end(_end), capacity(_capacity),
		flow(0), indexOfReverse(index_of_reverse) {}

	size_t const end;
	int capacity;
	int flow;
	size_t indexOfReverse; // Index of reverse edge in vector for end.
};


struct point {
	std::vector<flow_edge> out;
};

// CFlowGraph store all points in unordered_map<size_t, point> and has some useful methods.
class CFLowGraph {
public:
	CFLowGraph() = delete;
	CFLowGraph(size_t N) : size(N)
	{
		G.reserve(N);
	}
	~CFLowGraph() = default;


	int GetFlow(size_t index)
	{
		assert(index < size);

		int ans = 0;
		for (auto it = G[index].out.begin(); it != G[index].out.end(); it++)
			ans += it->flow;

		return ans;
	}

	// As far as we don't have multiple edges, we will not check their existance during the declaration.
	void DeclareEdge(size_t from, size_t to, int capacity, int reverse_capacity = 0)
	{
		assert(from < size && to < size && from != to);

		flow_edge straight(to, capacity, G[to].out.size()), reverse(from, reverse_capacity, G[from].out.size());

		G[from].out.push_back(straight);
		G[to].out.push_back(reverse);
	}

	void DeclareVertex(size_t index, size_t numberOfAdjacent)
	{
		G[index].out.reserve(numberOfAdjacent);
	}

	// AddFlow will decrease a flow on the reverse edge as well.
	void AddFlow(size_t from, size_t to, int val)
	{
		assert(from < size && to < size && from != to);

		std::vector<flow_edge>::iterator it = findedge(from, to), it2 = findedge(to, from);

		assert(it != G[from].out.end() && it2 != G[to].out.end());
		assert(it->capacity >= it->flow + val);
		assert(it->flow == -it2->flow);

		it->flow += val;
		it2->flow -= val;
	}

	// This version of AddFlow has also a "hint" - iterator pos that shows edge to boost the speed.
	void AddFlow(size_t from, size_t to, int val, std::vector<flow_edge>::iterator pos)
	{
		pos->flow += val;
		G[to].out[pos->indexOfReverse].flow -= val;
	}


	std::vector<flow_edge>::iterator GetBegin(size_t index)
	{
		assert(index < size);

		std::unordered_map<size_t, point>::iterator it = G.find(index);
		assert(it != G.end());

		return it->second.out.begin();
	}

	std::vector<flow_edge>::iterator GetEnd(size_t index)
	{
		assert(index < size);

		std::unordered_map<size_t, point>::iterator it = G.find(index);
		assert(it != G.end());

		return it->second.out.end();
	}

	size_t const GetSize() const
	{
		return size;
	}

	std::vector<bool> GetReachableFromSource()
	{
		std::vector<bool> visited(size, false);
		findReachableVertexes(0, visited);
		return visited;
	}
private:
	std::vector<flow_edge>::iterator findedge(size_t from, size_t to)
	{
		std::vector<flow_edge>::iterator it;
		for (it = G[from].out.begin(); it != G[from].out.end(); it++)
			if (it->end == to) break;
		return it;
	}

	// We use a recurse dfs to find all reachable vertexes. Saturated edges are ignored.
	void findReachableVertexes(size_t vertex, std::vector<bool>& visited)
	{
		visited[vertex] = true;
		for (auto it = G[vertex].out.begin(); it != G[vertex].out.end(); it++) {
			if (!visited[it->end] && it->capacity != it->flow) {
				findReachableVertexes(it->end, visited);
			}
		}
	}

	std::unordered_map<size_t, point> G;
	size_t const size;
};
