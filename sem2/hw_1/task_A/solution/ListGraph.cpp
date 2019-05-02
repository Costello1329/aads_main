#include "ListGraph.h"
#include <vector>
#include <iostream>
#include <algorithm>


//Constructors:

ListGraph::ListGraph (int size) {
	graph = std::vector< std::vector<int> >(size, std::vector<int>(0, 0));
}

ListGraph::ListGraph (const Graph *graph) {
	for (int i = 0; i < graph->getVerticiesCount(); i ++) {
		std::vector<int> next;
		graph->getNextVerticies(i, next);

		for (int j = 0; j < next.size(); j ++) {
			addEdge(i, next[j]);
		}
	}
}


//Setters:

bool ListGraph::scan () {
	std::cout << "Введите количество вершин:" << std::endl;
	int size;
	std::cin >> size;

	if (size > 0) {
		graph = std::vector< std::vector<int> >(size);
	}

	else {
		return false;
	}

	std::cout << "Введите количество ребер:" << std::endl;
	int edgesCount = 0;
	std::cin >> edgesCount;

	std::cout << "Введите ребра:" << std::endl;

	for (int i = 0; i < edgesCount; i ++) {
		int from, to;
		std::cin >> from >> to;
		addEdge(from, to);
	}

	return true;
}


//Getters:

void ListGraph::print () {
	std::cout << "Вы ввели граф:" << std::endl;

	for (int i = 0; i < graph.size(); i ++) {
		std::cout << i << ": ";

		for (int j = 0; j < graph[i].size(); j ++) {
			std::cout << graph[i][j] << " ";
		}

		std::cout << std::endl;
	}
}


//Interface:

bool ListGraph::addEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < graph.size() && to < graph.size()) {
		for (int el: graph[from]) {
			if (el == to) {
				return false;
			}
		}		

		graph[from].push_back(to);
		std::sort(graph[from].begin(), graph[from].end());
		return true;
	}

	return false;
}

bool ListGraph::removeEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < graph.size() && to < graph.size()) {
		for (int i = 0; i < graph[from].size(); i ++) {
			if (graph[from][i] == to) {
				graph[from].erase(graph[from].begin() + i);
			}
		}

		return true;
	}

	return false;
}

bool ListGraph::getPrevVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < graph.size()) {
		verticies.clear();

		for (int i = 0; i < graph.size(); i ++) {
			for (int j = 0; j < graph[i].size(); j ++) {
				if (graph[i][j] == vertex) {
					verticies.push_back(i);
				}
			}
		}

		return true;
	}

	return false;
}

bool ListGraph::getNextVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < graph.size()) {
		verticies.clear();

		verticies = graph[vertex];
		return true;
	}

	return false;
}

int ListGraph::getVerticiesCount () const {
	return graph.size();
}
