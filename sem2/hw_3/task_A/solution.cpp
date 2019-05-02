#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>

using namespace std;
typedef unsigned long long ull;
const int INF = 1000000;


class Graph {
	private:
		vector< vector< pair<int, int> > > edges;

	public:
		Graph () {
			edges.resize(0);
		}

		void addEdge (int from, int to, int weight) {
			if (from >= edges.size())
				edges.resize(from + 1, vector< pair<int, int> >(0));

			edges[from].push_back(make_pair(to, weight));
		}
		
		void getEdges (int vertex, vector< pair<int, int> > &edges) const {
			edges.clear();
			edges = this->edges[vertex];
		}

		size_t size () const {
			return edges.size();
		}

		void clear () {
			edges.resize(0);
		}

		void print () {
			for (int vertex = 0; vertex < edges.size(); vertex ++) {
				cout << vertex << ": ";
		
				for (pair<int, int> edge: edges[vertex]) {
					cout << "(" << edge.first << "; " << edge.second << ") ";
				}
		
				cout << endl;
			}
		}
};

class DisjointSets {
	private:
		vector<int> parents;
		vector<int> ranks;

	public:
		DisjointSets (size_t size) {
			parents = vector<int>(size);
			ranks = vector<int>(size, 0);

			for (int i = 0; i < size; i ++)
				parents[i] = i;
		}

		int find (int element) {
			int root = element;

			while (parents[root] != root)
				root = parents[root];

			int current = element, previous;

			while (parents[current] != root) {
				int previous = current;
				current = parents[current];
				parents[previous] = root;
			}

			return root;
		}

		bool unite (int element1, int element2) {
			element1 = find(element1);
			element2 = find(element2);

			if (element1 != element2) {
				if (ranks[element1] < ranks[element2])
					swap(element1, element2);

				else if (ranks[element1] == ranks[element2])
					ranks[element1] += 1;

				parents[element2] = element1;
				
				return true;
			}

			return false;
		}
};


bool compareEdgesPrim (const pair<int, int> &first, const pair<int, int> &second) {
	if (first.second == second.second)
		return first.first < second.first;

	return first.second < second.second;
}

void buildSpanningTreePrim (const Graph &graph, Graph &tree) {
	tree.clear();
	int verticesCount = graph.size();

	vector< pair<int, int> > minimalEdges(verticesCount, make_pair(-1, INF));
	vector<bool> used(verticesCount, false);
	minimalEdges[0].second = 0;

	set<pair<int, int>,  bool(*)(const pair<int, int> &first, const pair<int, int> &second)> edgesQueue(&compareEdgesPrim);
	edgesQueue.insert(make_pair(0, 0));

	for (int i = 0; i < verticesCount; i ++) {
		int vertex = edgesQueue.begin()->first;
		edgesQueue.erase(edgesQueue.begin());
		used[vertex] = true;
	 
		if (minimalEdges[vertex].first != -1) {
			tree.addEdge(vertex, minimalEdges[vertex].first, minimalEdges[vertex].second);
			tree.addEdge(minimalEdges[vertex].first, vertex, minimalEdges[vertex].second);
		}

		vector< pair<int, int> > edges;
		graph.getEdges(vertex, edges);
	 
		for (pair<int, int> edge: edges) {
			int to = edge.first;
			int cost = edge.second;

			if (cost < minimalEdges[to].second && !used[to]) {
				edgesQueue.erase(make_pair (to, minimalEdges[to].second));
				minimalEdges[to].second = cost;
				minimalEdges[to].first = vertex;
				edgesQueue.insert(make_pair (to, minimalEdges[to].second));
			}
		}
	}
}

bool compareEdgesKruskal (const tuple<int, int, int> &first, const tuple<int, int, int> &second) {
	if (get<2>(first) == get<2>(second))  {
		if (get<1>(first) == get<1>(second))
			return get<0>(first) > get<0>(second);

		return get<0>(first) > get<0>(second);
	}

	return get<2>(first) > get<2>(second);
}

void buildSpanningTreeKruskal (const Graph &graph, Graph &tree) {
	tree.clear();
	int verticesCount = graph.size();
	
	DisjointSets components(verticesCount);
	vector<tuple<int, int, int> > edgesQueue;

	for (int vertex = 0; vertex < verticesCount; vertex ++) {
		vector< pair<int, int> > edges;
		graph.getEdges(vertex, edges);

		for (int i = 0; i < edges.size(); i ++)
			edgesQueue.push_back(make_tuple(vertex, edges[i].first, edges[i].second));
	}

	sort(edgesQueue.begin(), edgesQueue.end(), compareEdgesKruskal);

	while (!edgesQueue.empty()) {
		tuple<int, int, int> edge = edgesQueue[edgesQueue.size() - 1];
		edgesQueue.pop_back();

		int from = get<0>(edge);
		int to = get<1>(edge);
		int weight = get<2>(edge);

		if (components.unite(from, to)) {
			tree.addEdge(from, to, weight);
			tree.addEdge(to, from, weight);
		}
	}
}

ull getSummaryWeight (const Graph &graph) {
	ull sum = 0;

	for (int vertex = 0; vertex < graph.size(); vertex ++) {
		vector< pair<int, int> > edgesFromVertex;
		graph.getEdges(vertex, edgesFromVertex);

		for (pair<int, int> edge: edgesFromVertex)
			sum += edge.second;
	}

	return sum / 2;
}


int main () {
	ifstream fin("kruskal.in");
	ofstream fout("kruskal.out");

	int verticesCount;
	int edgesCount;
	fin >> verticesCount >> edgesCount;

	Graph graph;
	Graph tree;

	for (int i = 0; i < edgesCount; i ++) {
		int from, to;
		int weight;
		fin >> from >> to >> weight;

		graph.addEdge(from - 1, to - 1, weight);
		graph.addEdge(to - 1, from - 1, weight);
	}

	buildSpanningTreeKruskal(graph, tree);
	fout << getSummaryWeight(tree) << endl;

	return 0;
}
