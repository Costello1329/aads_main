#include "ArcGraph.h"
#include <vector>
#include <iostream>
#include <algorithm>


bool compare (std::pair<int, int> edge1, std::pair<int, int> edge2) {
	if (edge1.first == edge2.first) {
		return edge1.second < edge2.second;
	}

	else {
		return edge1.first < edge2.first;
	}
}


//Constructors:

ArcGraph::ArcGraph (int size) {
	this->size = size;
}

ArcGraph::ArcGraph (const Graph *graph) {
	this->size = graph->getVerticiesCount();

	for (int i = 0; i < graph->getVerticiesCount(); i ++) {
		std::vector<int> next;
		graph->getNextVerticies(i, next);

		for (int j = 0; j < next.size(); j ++) {
			addEdge(i, next[j]);
		}
	}
}


//Setters:

bool ArcGraph::scan () {
	graph.clear();
	std::cout << "Введите число вершин:" << std::endl;
	int size;
	std::cin >> size;

	if (size > 0) {
		this->size = size;
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

void ArcGraph::print () {
	for (int i = 0; i < graph.size(); i ++) {
		std::cout << graph[i].first << " " << graph[i].second << std::endl;
	}
}


//Interface:

bool ArcGraph::addEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < size && to < size) {
		for (int i = 0; i < graph.size(); i ++) {
			if (graph[i].first == from && graph[i].second == to) {
				return false;
			}
		}

		std::sort(graph.begin(), graph.end(), compare);
		graph.push_back(std::make_pair(from, to));
		return true;
	}

	return false;
}

bool ArcGraph::removeEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < size && to < size) {
		for (int i = 0; i < graph.size(); i ++) {
			if (graph[i].first == from && graph[i].second == to) {
				graph.erase(graph.begin() + i);
				return true;
			}
		}
	}

	return false;
}

bool ArcGraph::getNextVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < size) {
		verticies.clear();

		for (int i = 0; i < graph.size(); i ++) {
			if (graph[i].first == vertex) {
				verticies.push_back(graph[i].second);
			}
		}

		return true;
	}

	return false;
}

bool ArcGraph::getPrevVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < size) {
		verticies.clear();

		for (int i = 0; i < graph.size(); i ++) {
			if (graph[i].second == vertex) {
				verticies.push_back(graph[i].first);
			}
		}

		return true;
	}

	return false;
}

int ArcGraph::getVerticiesCount () const {
	return size;
}
