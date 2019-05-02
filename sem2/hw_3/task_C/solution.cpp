#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
const int INF = 1000000;


class Net {
	private:
		vector< vector< pair<int, int> > > edges;

	public:
		Net () {}

		Net (int size) {
			edges.resize(size);
		}

		void addEdgeOriented (int from, int to, int weight) {
			if (max(to, from) >= edges.size())
				edges.resize(max(to, from) + 1);

			edges[from].push_back(make_pair(to, weight));
		}

		void addEdgeUnoriented (int from, int to, int weight) {
			addEdgeOriented(from, to, weight);
			addEdgeOriented(to, from, weight);
		}

		void increaseWeightOriented (int from, int to, int delta) {
			if (max(from, to) < edges.size()) {
				for (int i = 0; i < edges[from].size(); i ++) {
					if (edges[from][i].first == to) {
						edges[from][i] = make_pair(to, edges[from][i].second + delta);
						return;
					}
				}
			}

			addEdgeOriented(from, to, delta);
		}

		void increaseWeightUnoriented (int from, int to, int delta) {
			increaseWeightOriented(from, to, delta);
			increaseWeightOriented(to, from, delta);
		}

		void getEdges (int vertex, vector< pair<int, int> > &edges) const {
			edges = this->edges[vertex];
		}

		int size () const {
			return edges.size();
		}

		void print () const {
			for (int vertex = 0; vertex < edges.size(); vertex ++) {
				cout << vertex << ": ";
		
				for (pair<int, int> edge: edges[vertex]) {
					cout << "(" << edge.first << "; " << edge.second << ") ";
				}
		
				cout << endl;
			}
		}
};

struct Vertex {
	char type;
	int index;

	Vertex () {
		type = '0';
		index = -1;
	}

	Vertex (char type, int index) {
		this->type = type;
		this->index = index;
	}
};

void buildNet (Net &net, string s, string p, vector<Vertex> &verticies) {
	vector<int> sIndexToVertex(s.size(), -1);
	vector<int> pIndexToVertex(p.size(), -1);

	for (int i = 0; i <= s.size() - p.size(); i ++) {
		for (int j = 0; j < p.size(); j ++) {
			if (s[i + j] == '?' && p[j] == '0') {
				if (sIndexToVertex[i + j] == -1)
					sIndexToVertex[i + j] = net.size();
				
				net.increaseWeightUnoriented(0, sIndexToVertex[i + j], 1);
			}

			else if (s[i + j] == '?' && p[j] == '1') {
				if (sIndexToVertex[i + j] == -1)
					sIndexToVertex[i + j] = net.size();

				net.increaseWeightUnoriented(1, sIndexToVertex[i + j], 1);
			}

			else if (s[i + j] == '0' && p[j] == '?') {
				if (pIndexToVertex[j] == -1)
					pIndexToVertex[j] = net.size();

				net.increaseWeightUnoriented(0, pIndexToVertex[j], 1);
			}

			else if (s[i + j] == '1' && p[j] == '?') {
				if (pIndexToVertex[j] == -1)
					pIndexToVertex[j] = net.size();

				net.increaseWeightUnoriented(1, pIndexToVertex[j] , 1);
			}

			else if (s[i + j] == '?' && p[j] == '?') {
				int size = net.size();

				if (pIndexToVertex[j] == -1)
					pIndexToVertex[j] = size ++;

				if (sIndexToVertex[i + j] == -1)
					sIndexToVertex[i + j] = size ++;

				net.increaseWeightUnoriented(sIndexToVertex[i + j], pIndexToVertex[j], 1);
			}
		}
	}

	verticies.resize(net.size());

	for (int i = 0; i < sIndexToVertex.size(); i ++)
		if (sIndexToVertex[i] != -1)
			verticies[sIndexToVertex[i]] = Vertex('s', i);

	for (int j = 0; j < pIndexToVertex.size(); j ++)
		if (pIndexToVertex[j] != -1)
			verticies[pIndexToVertex[j]] = Vertex('p', j);
}

int FindMaxFlowDFS (Net &net, vector<int> &distances, vector<int> &pointer, int vertex, int end, int minimalWeight = INF) {
	//cout << "hello" << endl;

	if (minimalWeight == 0 || vertex == end)
		return minimalWeight;

	vector< pair<int, int> > edges;
	net.getEdges(vertex, edges);

	for (int i = pointer[vertex]; i < edges.size(); i ++) {
		pointer[vertex]++;

		int to = edges[i].first;
		int weight = edges[i].second;

		if (distances[to] == distances[vertex] + 1 && weight > 0) {
			int newMinimalWeight = min(minimalWeight, weight);
			int delta = FindMaxFlowDFS(net, distances, pointer, to, end, newMinimalWeight);
			
			if (delta > 0) {
				net.increaseWeightOriented(vertex, to, - delta);
				net.increaseWeightOriented(to, vertex, delta);
				return delta;
			}
		}
	}

	return 0;
}

bool FindMaxFlowBFS (Net &net, int start, int end, vector<int> &distances) {
	queue<int> verticesQueue;
	verticesQueue.push(start);

	distances = vector<int>(net.size(), -1);
	distances[start] = 0;

	while (!verticesQueue.empty()) {
		int vertex = verticesQueue.front();
		verticesQueue.pop();

		vector< pair<int, int> > edges;
		net.getEdges(vertex, edges);

		for (pair<int, int> edge: edges) {
			if (distances[edge.first] == -1 && edge.second > 0) {
				distances[edge.first] = distances[vertex] + 1;
				verticesQueue.push(edge.first);
			}
		}
	}

	return (distances[end] != -1);
}

int runMaxStream(Net &net, int start, int end) {
	int maxFlow = 0;
	vector<int> distances(net.size());
	vector<int> pointer(net.size());

	while (FindMaxFlowBFS(net, start, end, distances)) {
		int currentFlow;
		pointer = vector<int> (net.size(), 0);
		
		do {
			currentFlow = FindMaxFlowDFS(net, distances, pointer, start, end);
			maxFlow += currentFlow;
		} while (currentFlow != 0);
	}

	return maxFlow;
}

void cutGraph (const Net &net, int start, vector<bool> &reachable) {
	reachable[start] = true;
	vector< pair<int, int> > edges;
	net.getEdges(start, edges);

	for (pair<int, int> edge: edges)
		if (!reachable[edge.first] && edge.second)
			cutGraph(net, edge.first, reachable);
}

int solveProblem (string &s, string &p) {
	vector<Vertex> verticies;
	Net net(2);
	buildNet(net, s, p, verticies);

	int hammingDistance = 0;

	for (int i = 0; i <= s.size() - p.size(); i ++)
		for (int j = 0; j < p.size(); j ++)
			if ((s[i + j] == '1' && p[j] == '0') || (s[i + j] == '0' && p[j] == '1'))
				hammingDistance ++;

	hammingDistance += runMaxStream(net, 0, 1);
	
	vector<bool> reachable(net.size(), false);
	cutGraph(net, 0, reachable);

	for (int vertex = 0; vertex < verticies.size(); vertex ++) {
		if (verticies[vertex].type == 's')
			s[verticies[vertex].index] = reachable[vertex] ? '0' : '1';
	
		else if (verticies[vertex].type == 'p')
			p[verticies[vertex].index] = reachable[vertex] ? '0' : '1';
	}

	return hammingDistance;
}


int main () {
	string s, p;
	cin >> s >> p;
	cout << solveProblem(s, p) << endl;
	cout << s << endl << p << endl;
	return 0;
}
