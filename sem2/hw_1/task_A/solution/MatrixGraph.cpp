#include "MatrixGraph.h"
#include <vector>
#include <iostream>


//Constructors:

MatrixGraph::MatrixGraph (int size) {
	graph = std::vector<std::vector <char> >(size, std::vector <char> (size, false));
}

MatrixGraph::MatrixGraph (const Graph *graph) {
	this->graph = std::vector<std::vector <char> >(graph->getVerticiesCount(), std::vector<char>(graph->getVerticiesCount(), false));
	
	for (int i = 0; i < this->graph.size(); i ++) {
		std::vector<int> next;
		graph->getNextVerticies(i, next);

		for (int j = 0; j < next.size(); j ++) {
			addEdge(i, next[j]);
		}
	}
}


//Setters:

bool MatrixGraph::scan () {
	std::cout << "Введите число вершин:" << std::endl;
	int size;
	std::cin >> size;

	if (size > 0) {
		graph = std::vector< std::vector<char> >(size, std::vector<char>(size, false));
	}

	else {
		return false;
	}

	std::cout << "Введите матрицу смежности графа:" << std::endl;
	
	for (int i = 0; i < size; i ++) {
		for (int j = 0; j < size; j ++) {
			double el;
			std::cin >> el;
			graph[i][j] = el > 0;
		}
	}

	return true;
}


//Getters:

void MatrixGraph::print () {
	std::cout << "Вы ввели граф:" << std::endl;

	for (int i = 0; i < graph.size(); i ++) {

		for (int j = 0; j < graph.size(); j ++) {
			std::cout << (graph[i][j] ? 1 : 0) << " ";
		}

		std::cout << std::endl;
	}
}


//Interface:

bool MatrixGraph::addEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < graph.size() && to < graph.size()) {
		graph[from][to] = 1;
		return true;
	}

	return false;
}

bool MatrixGraph::removeEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < graph.size() && to < graph.size()) {
		graph[from][to] = 0;
		return true;
	}

	return false;
}

bool MatrixGraph::getPrevVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < graph.size()) {
		verticies.clear();

		for (int i = 0; i < graph.size(); i ++) {
			if (graph[i][vertex] == 1) {
				verticies.push_back(i);
			}
		}

		return true;
	}

	return false;
}

bool MatrixGraph::getNextVerticies (int vertex, std::vector<int> &verticies) const {
	if (vertex >= 0 && vertex < graph.size()) {
		verticies.clear();

		for (int i = 0; i < graph.size(); i ++) {
			if (graph[vertex][i] == 1) {
				verticies.push_back(i);
			}
		}

		return true;
	}

	return false;
}

int MatrixGraph::getVerticiesCount () const {
	return graph.size();
}
