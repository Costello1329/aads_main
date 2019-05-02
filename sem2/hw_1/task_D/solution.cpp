#include <algorithm>
#include <iostream>
#include <vector>

using std::max;
using std::vector;
using std::cin;
using std::cout;
using std::endl;

typedef vector<vector<unsigned> > Graph;


bool findSink (const Graph &graph, unsigned verticesCount) {
	int left;
	int up;
	int right;
	int down;

	for (int i = 0; i < verticesCount; i ++) {
		for (right = 1; right + i < verticesCount && graph[i][right + i] == 0; right ++);
		for (down = 1; down + i < verticesCount && graph[down + i][i] == 1; down ++);

		if (down + i < verticesCount || right + i < verticesCount) {
			i += max<int>(down, right) - 1;
			continue;
		}

		else {
			for (left = 1; -left + i >= 0 && graph[i][-left + i] == 0; left ++);
			for (up = 1; -up + i >= 0 && graph[-up + i][i] == 1; up ++);
	
			if (-up + i >= 0 || -left + i >= 0)
				return false;
	
			else
				return true;
		}
	}

	return false;
}


int main () {
	unsigned verticesCount = 0;
	cin >> verticesCount;

	vector<vector<unsigned> > graph = vector<vector<unsigned> >(verticesCount, vector<unsigned>(verticesCount, 0));

	for (unsigned i = 0; i < verticesCount; i ++)
		for (unsigned j = 0; j < verticesCount; j ++)
			cin >> graph[i][j];

	cout << (findSink(graph, verticesCount) ? "YES" : "NO") << endl;

	return 0;
}
