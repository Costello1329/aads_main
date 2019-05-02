#include "SetGraph.h"
#include <vector>
#include <iostream>
#include <algorithm>


//Constructors:

SetGraph::SetGraph (int size) {
	to = std::vector<std::unordered_set <int> >(size);
	from = std::vector<std::unordered_set <int> >(size);
}

SetGraph::SetGraph (const Graph *graph) {
	to = std::vector<std::unordered_set <int> >(graph->getVerticiesCount());
	from = std::vector<std::unordered_set <int> >(graph->getVerticiesCount());
	
	for (int i = 0; i < graph->getVerticiesCount(); i ++) {
		std::vector<int> verticies;
		graph->getNextVerticies(i, verticies);

		for (int j = 0; j < verticies.size(); j ++) {
			addEdge(i, verticies[j]);
		}
	}
}


//Setters:

bool SetGraph::scan () {
	std::cout << "Введите количество вершин:" << std::endl;
	int size;
	std::cin >> size;

	if (size > 0) {
		from = std::vector<std::unordered_set <int> >(size);
		to = std::vector<std::unordered_set <int> >(size);
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

void SetGraph::print () {
	std::cout << "Вы ввели граф:" << std::endl;

	std::cout << std::endl << "from:" << std::endl;

	for (int i = 0; i < from.size(); i ++) {
		std::cout << i << ": ";

		for (int el: from[i]) {
			std::cout << el << " ";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl << "to:" << std::endl;

	for (int i = 0; i < to.size(); i ++) {
		std::cout << i << ": ";

		for (int el: to[i]) {
			std::cout << el << " ";
		}

		std::cout << std::endl;
	}
}


//Interface:

bool SetGraph::addEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < this->to.size() && to < this->to.size()) {
		this->to[from].insert(to);
		this->from[to].insert(from);
		return true;
	}

	return false;
}

bool SetGraph::removeEdge (int from, int to) {
	if (from >= 0 && to >= 0 && from < this->to.size() && to < this->to.size()) {
		bool returnCode = false;

		for(int el: this->to[from]) {
			if (el == to) {
				this->to[from].erase(el);
				returnCode = true;
			}
		}

		for(int el: this->from[to]) {
			if (el == from) {
				this->from[to].erase(el);
				returnCode = true;
			}
		}

		return returnCode;
	}

	return false;
}

bool SetGraph::getPrevVerticies (int vertex, std::vector <int> &verticies) const {
	if (vertex >= 0 && vertex < to.size()) {
		verticies.clear();

		for(int i : from[vertex]) {
			verticies.push_back(i);
		}
	
		return true;
	}

	return false;
}

bool SetGraph::getNextVerticies (int vertex, std::vector <int> &verticies) const {
	if (vertex >= 0 && vertex < to.size()) {
		verticies.clear();

		for(int i : to[vertex]) {
			verticies.push_back(i);
		}

		return true;
	}

	return false;
}

int SetGraph::getVerticiesCount() const {
	return to.size();
}
