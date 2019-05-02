#include <algorithm>
#include <iostream>
#include <vector>


using std::max;
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


/* Kosaraju's Algorithm: */

Graph transpose (const Graph &graph) {
	Graph transposed(graph.getVerticesCount());

	for (unsigned from = 0; from < graph.getVerticesCount(); from ++) {
		vector<unsigned> edges;
		graph.getNextVertices(from, edges);

		for (unsigned to: edges)
			transposed.addEdge(to, from);
	}

	return transposed;
}

void orderVertices (const Graph &graph, const unsigned vertex, vector<bool> &used, vector<unsigned> &order, const unsigned timer = 0) {
	used[vertex] = true;
	vector<unsigned> nextVertices;
	graph.getNextVertices(vertex, nextVertices);

	for (const unsigned nextVertex: nextVertices)
		if (!used[nextVertex])
			orderVertices(graph, nextVertex, used, order, timer + 1);

	order.push_back(vertex);
}

vector<unsigned> topSort (const Graph &graph) {
	vector<bool> used = vector<bool>(graph.getVerticesCount(), false);
	vector<unsigned> order;

	for (unsigned vertex = 0; vertex < used.size(); vertex ++)
		if (!used[vertex]) 
			orderVertices(graph, vertex, used, order);

	return order;
}

void markComponents (const Graph &graph, const unsigned vertex, const unsigned componentNumber, vector<bool> &used, vector<unsigned> &componentID) {
	used[vertex] = true;
	componentID[vertex] = componentNumber;

	vector<unsigned> nextVertices;
	graph.getNextVertices(vertex, nextVertices);

	for (const unsigned nextVertex: nextVertices)
		if (!used[nextVertex]) 
			markComponents(graph, nextVertex, componentNumber, used, componentID);
}

vector<unsigned> findSCC (const Graph &graph) {
	Graph transposedGraph = transpose(graph);
	vector<unsigned> order = topSort(transposedGraph);
	vector<bool> used(graph.getVerticesCount(), false);
	vector<unsigned> componentID(graph.getVerticesCount(), 0);

	unsigned componentsCount = 0;

	for (int i = graph.getVerticesCount() - 1; i >= 0; i --) {
		unsigned vertex = order[i];
		
		if (!used[vertex])
			markComponents(graph, vertex, componentsCount ++, used, componentID);
	}

	return componentID;
}

Graph condense (const Graph &graph) {
	vector<unsigned> components = findSCC(graph);

	unsigned maxComponentID = 0;

	for (const unsigned id: components)
		maxComponentID = max(maxComponentID, id);

	Graph condensed(maxComponentID + 1);

	for (unsigned vertex = 0; vertex < graph.getVerticesCount(); vertex ++) {
		vector<unsigned> nextVertices;
		graph.getNextVertices(vertex, nextVertices);
		
		for (const unsigned nextVertex: nextVertices)
			if (components[vertex] != components[nextVertex])
				condensed.addEdge(components[vertex], components[nextVertex]);
	}

	return condensed;
}


/* Main algorithm: */

unsigned findMinimalEdgesNumber (const Graph &graph) {
	if (graph.getVerticesCount() == 1)
		return 0;

	unsigned isolatedPoints = 0;
	unsigned sourcePoints = 0;
	unsigned stockPoints = 0;

	for (unsigned vertex = 0; vertex < graph.getVerticesCount(); vertex ++) {
		// Outer edges exist ?:

		vector<unsigned> nextVertices;
		graph.getNextVertices(vertex, nextVertices);
		bool outer = nextVertices.size() != 0;

		// Inner edges exist ?:
		bool inner = false;

		for (unsigned fromVertex = 0; fromVertex < graph.getVerticesCount(); fromVertex ++) {
			vector<unsigned> reverseEdges;
			graph.getNextVertices(fromVertex, reverseEdges);

			for (unsigned reverseEdge: reverseEdges) {
				if (reverseEdge == vertex) {
					inner = true;
					break;
				}
			}
		}

		// Classifying vertex:

		if (!outer && !inner)
			isolatedPoints ++;

		else if (!outer && inner)
			sourcePoints ++;
		
		else if (outer && !inner)
			stockPoints ++;
	}

	return isolatedPoints + max<unsigned>(sourcePoints, stockPoints);
}


/* Input and output processing: */

Graph readGraph (istream &is) {
	unsigned verticesCount, edgesCount;
	is >> verticesCount >> edgesCount;

	Graph graph(verticesCount);

	for (unsigned i = 0; i < edgesCount; i ++) {
		unsigned from, to;
		is >> from >> to;
		
		if (from != to)
			graph.addEdge(from - 1, to - 1);
	}

	return graph;
}

void solveProblem (istream &is, ostream &os) {
	os << findMinimalEdgesNumber(condense(readGraph(is))) << endl;
}


/* Main: */

int main () {
	solveProblem(cin, cout);
	return 0;
}
