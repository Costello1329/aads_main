#include <iostream>
#include <vector>


using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;


class Graph {
	private:
		vector<vector<unsigned> > graph;

		bool validateVertex (unsigned vertex) const {
			if (vertex >= graph.size())
				return false;

			else
				return true;
		}

	public:
		Graph (const unsigned verticesCount) {
			graph = vector<vector<unsigned> >(verticesCount, vector<unsigned>(0));
		}

		void addEdge (const unsigned from, const unsigned to) {
			if (!validateVertex(from) || !validateVertex(to))
				return;

			graph[from].push_back(to);
		}

		void getNextVertices (const unsigned vertex, vector<unsigned> &nextVertices) const {
			if (!validateVertex(vertex))
				return;

			nextVertices = graph[vertex];
		}

		unsigned getVerticesCount () const {
			return graph.size();
		}
};

/* DFS: */


enum VertexColor {
	white = 0,
	gray = 1,
	black = 2
};

void dfs (const Graph &graph, const unsigned prevVertex, const unsigned vertex, vector<unsigned> &colors, vector<unsigned> &enterTimes, int &minCycleLength) {
	colors[vertex] = gray;

	vector<unsigned> nextVertices;
	graph.getNextVertices(vertex, nextVertices);

	for (const unsigned nextVertex: nextVertices) {
		if (nextVertex == prevVertex)
			continue;

		if (colors[nextVertex] == white) {
			enterTimes[nextVertex] = enterTimes[vertex] + 1;
			dfs(graph, vertex, nextVertex, colors, enterTimes, minCycleLength);
		}

		else if (colors[nextVertex] == gray) {
			int currentCycleLength = enterTimes[vertex] - enterTimes[nextVertex] + 1;
			
			if (currentCycleLength < minCycleLength || minCycleLength == -1)
				minCycleLength = currentCycleLength;
		}
	}

	colors[vertex] = black;
}

int findMinLengthCycle (const Graph &graph) {
	int minCycleLength = -1;

	for (unsigned vertex = 0; vertex < graph.getVerticesCount(); vertex ++) {
		if (minCycleLength == 3)
			break;

		vector<unsigned> colors(graph.getVerticesCount(), white);
		vector<unsigned> enterTimes(graph.getVerticesCount(), 0);

		dfs(graph, vertex, vertex, colors, enterTimes, minCycleLength);
	}

	return minCycleLength;
}


/* Input and output processing: */

Graph readGraph (istream &is) {
	unsigned verticesCount, edgesCount;
	is >> verticesCount >> edgesCount;

	Graph graph(verticesCount);

	for (unsigned i = 0; i < edgesCount; i ++) {
		unsigned from, to;
		is >> from >> to;	
		graph.addEdge(from, to);
		graph.addEdge(to, from);
	}

	return graph;
}

void solveProblem (istream &is, ostream &os) {
	os << findMinLengthCycle(readGraph(is)) << endl;
}


/* Main: */

int main () {
	solveProblem(cin, cout);
	return 0;
}
