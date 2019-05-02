#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
#include "Graph.h"
#include <vector>


class MatrixGraph : public Graph {
	std::vector<std::vector <char> > graph;

	public:
		//Constructors:
		MatrixGraph () {}
		MatrixGraph (int size);
		MatrixGraph (const Graph *givenGraph);

		//Getters:
		void print();

		//Setters:
		bool scan();

		//Interface:
		bool addEdge (int from, int to);
		bool removeEdge (int from, int to);
		bool getPrevVerticies (int vertex, std::vector<int> &verticies) const;
		bool getNextVerticies (int vertex, std::vector<int> &verticies) const;
		int getVerticiesCount () const;
};


#endif
