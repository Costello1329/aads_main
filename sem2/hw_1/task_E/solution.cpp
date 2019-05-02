#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;


/* Graph class and BFS search */

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

bool checkVertexReachability (const Graph &graph, const unsigned start, const unsigned end) {
	vector<bool> used(graph.getVerticesCount(), false);
	queue<unsigned> verticesQueue;
	
	verticesQueue.push(start);
	used[start] = true;

	while (!verticesQueue.empty()) {
		const unsigned vertex = verticesQueue.front();
		verticesQueue.pop();

		vector<unsigned> nextVertices;
		graph.getNextVertices(vertex, nextVertices);

		for (const unsigned nextVertex: nextVertices) {
			if (!used[nextVertex]) {
				used[nextVertex] = true;
				verticesQueue.push(nextVertex);
			}
		}
	}

	return used[end];
}


/* Field processing: */

struct Field {
	vector<pair<int, int> > columnsCoords;
	unsigned columnRadius;
	int leftWallCoord;
	int rightWallCoord;
};

double distance (const pair<int, int> &coords1, const pair<int, int> &coords2) {
	return sqrt(pow(coords1.first - coords2.first, 2) + pow(coords1.second - coords2.second, 2));
}

bool checkDiameter (const double diameter, const Field &field) {
	if (diameter <= 0)
		return true;

	if (diameter > field.rightWallCoord - field.leftWallCoord)
		return false;

	unsigned columnsCount = field.columnsCoords.size();
	Graph graph(columnsCount + 2);

	unsigned vertex = 0;

	for (const pair<int, int> columnCoord: field.columnsCoords) {
		if (columnCoord.first - field.leftWallCoord - field.columnRadius < diameter) {
			graph.addEdge(0, vertex + 1);
			graph.addEdge(vertex + 1, 0);
		}

		if (field.rightWallCoord - columnCoord.first - field.columnRadius < diameter) {
			graph.addEdge(vertex + 1, columnsCount + 1);
			graph.addEdge(columnsCount + 1, vertex + 1);
		}

		vertex ++;
	}

	for (unsigned i = 0; i < columnsCount; i ++) {
		for (unsigned j = i + 1; j < columnsCount; j ++) {
			if (distance(field.columnsCoords[i], field.columnsCoords[j]) - 2 * field.columnRadius < diameter) {
				graph.addEdge(i + 1, j + 1);
				graph.addEdge(j + 1, i + 1);
			}
		}
	}

	return !checkVertexReachability(graph, 0, columnsCount + 1);
}


double findMaximalDiameter (const Field &field) {
	double leftBound = 0;
	double rightBound = field.rightWallCoord - field.leftWallCoord;

	while (true) {
		bool checkMin = checkDiameter(leftBound, field);
		bool checkMax = checkDiameter(rightBound, field);
		double delta = (rightBound - leftBound) / 2;

		if (!checkMin && !checkMax)
			leftBound -= delta;

		else if (checkMin && checkMax)
			rightBound += delta;

		else {
			leftBound += delta / 2;
			rightBound -= delta / 2;

			if (delta < 0.00001)
				break;
		}
	}

	return (rightBound + leftBound) / 2;
}


/* Input and output processing: */

Field readField (istream &is) {
	Field field;
	unsigned columnsCount;
	is >> field.leftWallCoord >> field.rightWallCoord >> field.columnRadius >> columnsCount;

	for (int i = 0; i < columnsCount; i ++) {
		int x, y;
		is >> x >> y;

		//if (x > field.leftWallCoord - field.columnRadius && x < field.rightWallCoord + field.columnRadius)
			field.columnsCoords.push_back(make_pair(x, y));
	}

	return field;
}

void solveProblem (istream &is, ostream &os) {
	Field field = readField(is);
	double maxD = findMaximalDiameter(field);
	printf("%.3f\n", maxD > 0 ? maxD : 0.000);
}


/* Main: */

int main () {
	solveProblem(cin, cout);
	return 0;
}
