#include <iostream>
#include <vector>

using namespace std;

class Graph {
	private:
		vector< vector <int> > graph;
		int size;
		const int INF = 1000;

	public:
		Graph (int size) {
			this->size = size;
			graph = vector< vector<int> >(size, vector<int>(size, 0));
		}

		void scan () {
			for (int i = 0; i < size; i ++) {
				for (int j = 0; j < size; j ++) {
					int number;
					cin >> number;

					if (number != 0 || i == j) {
						graph[i][j] = number;
					}

					else {
						graph[i][j] = INF;
					}
				}
			}
		}

		void floydTransformation () {
			vector< vector<int> > matrix = graph;

			for (int k = 0; k < size; k ++) {
				for (int i = 0; i < size; i ++) {
					for (int j = 0; j < size; j ++) {
						if (matrix[i][k] < INF && matrix[k][j] < INF) {
							matrix[i][j] = min (matrix[i][j], matrix[i][k] + matrix[k][j]);
						}
					}
				}
			}

			for (int i = 0; i < size; i ++) {
				for (int j = 0; j < size; j ++) {
					for (int t = 0; t < size; t ++) {
						if (matrix[i][t] < INF && matrix[t][t] < 0 && matrix[t][j] < INF) {
							matrix[i][j] = -INF;
						}
					}
				}
			}

			for (int i = 0; i < size; i ++) {
				for (int j = 0; j < size; j ++) {
					if (matrix[i][j] == INF) {
						cout << "inf ";
					}

					else if (matrix[i][j] == -INF) {
						cout << "-inf ";
					}

					else {
						cout << matrix[i][j] << " ";
					}
				}

				cout << endl;
			}
		}
};

int main () {
	int verticies;
	cin >> verticies;

	Graph graph(verticies);
	graph.scan();
	graph.floydTransformation();
}