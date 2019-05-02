#ifndef LISTGRAPH_H
#define LISTGRAPH_H
#include "Graph.h"
#include <vector>


class ListGraph : public Graph {
	private:
		std::vector< std::vector<int> > graph;

	public:
		//Constructors
		ListGraph () {};
		ListGraph (int size);
		ListGraph (const Graph *graph);

		//Setters
		bool scan ();

		//Getters
		void print ();

		//Interface
		bool addEdge (int from, int to);
		bool removeEdge (int from, int to);
		bool getPrevVerticies (int vertex, std::vector<int> &verticies) const;
		bool getNextVerticies (int vertex, std::vector<int> &verticies) const;
		int getVerticiesCount () const;
};


#endif
