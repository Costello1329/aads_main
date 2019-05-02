#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class Graph {
	private:
		vector< vector< pair <int, double> > > graph;

	public:
		Graph (int size) {
			graph = vector< vector< pair<int, double> > >(size);
		}

		void addEdge (int from, int to, double weight);
		void getNextVerticies (int vertex, vector< pair<int, double> > &verticies) const;
		int size () const;
};

void Graph::addEdge (int from, int to, double weight) {
	if (from >= 0 && to >= 0 && from < graph.size() && to < graph.size()) {
		graph[from].push_back(make_pair(to, weight));
	}
}

void Graph::getNextVerticies (int vertex, vector< pair<int, double> > &verticies) const {
	if (vertex >= 0 && vertex < graph.size()) {
		verticies.clear();
		verticies = graph[vertex];
	}
}

int Graph::size () const {
	return graph.size();
}


void fordBellmann (const Graph &graph, vector<double> &minPathes, int start) {
	const int inf = 1000000;
	int size = graph.size();
	minPathes = vector<double>(size, inf);
	minPathes[start] = 0;

	for (int i = 0; i < size - 1; i ++) {
		for (int current = 0; current < size; current ++) {
			vector< pair<int, double> > nextVerticies;
			graph.getNextVerticies(current, nextVerticies);

			for (pair<int, double> next: nextVerticies) {
				minPathes[next.first] = min(minPathes[next.first], minPathes[current] + next.second);
			}
		}
	}
}

bool findMinimalCycle (const Graph &graph, vector<double> minPathes) {
	int size = graph.size();

	for (int current = 0; current < size; current ++) {
		vector< pair<int, double> > nextVerticies;
		graph.getNextVerticies(current, nextVerticies);

		for (pair<int, double> next: nextVerticies) {
			if (minPathes[next.first] > minPathes[current] + next.second) {
				return true;
			}
		}
	}

	return false;
}


int main () {
	int verticies;
	cin >> verticies;
	Graph graph(verticies);

	for (int i = 0; i < verticies; i ++) {
		for (int j = 0; j < verticies; j ++) {
			if (i == j) {
				continue;
			}

			double weight;
			cin >> weight;

			if (weight != -1) {
				graph.addEdge(i, j, -log(weight));
			}
		}
	}

	vector<double> minPathes;
	fordBellmann(graph, minPathes, 0);
	cout << (findMinimalCycle(graph, minPathes) ? "YES" : "NO") << endl;

	return 0;
}
