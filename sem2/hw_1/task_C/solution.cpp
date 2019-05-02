#include <iostream>
#include <vector>
#include <queue>


using std::queue;
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


unsigned minPathsCount (const Graph &graph, const unsigned start, const unsigned end) {
	vector<int> enterTimes(graph.getVerticesCount(), -1);
	vector<unsigned> minPathsToVertexCount(graph.getVerticesCount(), 0);
	queue<unsigned> verticesQueue;
	bool allowAddition = true;
	
	verticesQueue.push(start);
	enterTimes[start] = 0;
	minPathsToVertexCount[start] = 1;

	while (!verticesQueue.empty()) {
		unsigned vertex = verticesQueue.front();
		verticesQueue.pop();

		vector<unsigned> nextVertices;
		graph.getNextVertices(vertex, nextVertices);

		for (const unsigned nextVertex: nextVertices) {
			if (enterTimes[nextVertex] == -1 || enterTimes[nextVertex] == enterTimes[vertex] + 1) {
				if (nextVertex == end)
					allowAddition = false;	

				if (allowAddition && enterTimes[nextVertex] != enterTimes[vertex] + 1)
					verticesQueue.push(nextVertex);

				minPathsToVertexCount[nextVertex] += minPathsToVertexCount[vertex];
				enterTimes[nextVertex] = enterTimes[vertex] + 1;
			}
		}
	}

	return minPathsToVertexCount[end];
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
	Graph graph = readGraph(is);
	unsigned start, end;
	is >> start >> end;
	os << minPathsCount(graph, start, end) << endl;
}


/* Main: */

int main () {
	solveProblem(cin, cout);
	return 0;
}
