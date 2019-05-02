#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
	private:
		vector< vector< pair <int, int> > > graph;

	public:
		Graph (int size) {
			graph = vector< vector< pair<int, int> > >(size);
		}

		void addEdge (int from, pair<int, int> to) {
			graph[from].push_back(to);
		}

		void getNextVerticies (int vertex, vector<pair <int, int> > &verticies) const {
			verticies = graph[vertex];
		}

		int size () const {
			return graph.size();
		}
};


void Dijkstra (const Graph &graph, vector<int> &minPathes, int start) {
	const int INF = 1000000;
	minPathes = vector<int>(graph.size(), INF);
	minPathes[start] = 0;

	set<pair <int, int> > container;
	container.insert(make_pair(start, minPathes[start]));

	while (!container.empty()) {
		int current = container.begin()->first;
		container.erase(container.begin());

		vector<pair <int, int> > nextVerticies;
		graph.getNextVerticies(current, nextVerticies);

		for (pair<int, int> next: nextVerticies) {
			int vertex = next.first,
				weight = next.second;
			
			if (minPathes[current] + weight < minPathes[vertex]) {
				container.erase(make_pair(vertex, minPathes[vertex]));
				minPathes[vertex] = minPathes[current] + weight;
				container.insert(make_pair(vertex, minPathes[vertex]));
			}
		}
	}
}


int main () {
	int verticies, edges;
	cin >> verticies >> edges;
	
	Graph graph(verticies);

	for (int i = 0; i < edges; i ++) {
		int from, to, weight;
		cin >> from >> to >> weight;
		graph.addEdge(from, make_pair(to, weight));
		graph.addEdge(to, make_pair(from, weight));
	}

	int from, to;
	cin >> from >> to;

	vector<int> minPathes;
	Dijkstra(graph, minPathes, from);

	cout << minPathes[to] << endl;

	return 0;
}
