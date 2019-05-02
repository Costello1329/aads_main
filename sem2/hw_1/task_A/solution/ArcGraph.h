#ifndef ARCGRAPH_H
#define ARCGRAPH_H
#include "Graph.h"
#include <vector>


class ArcGraph : public Graph {
	private:
		std::vector< std::pair<int, int> > graph;
		int size;

	public:
		//Constructors
		ArcGraph () {}
		ArcGraph (int size);
		ArcGraph (const Graph *graph);

		//Setters
		bool scan ();

		//Getters
		void print ();

		//Interface
		bool addEdge (int from, int to);
		bool removeEdge (int from, int to);
		bool getNextVerticies (int vertex, std::vector<int> &verticies) const;
		bool getPrevVerticies (int vertex, std::vector<int> &verticies) const;
		int getVerticiesCount () const;
};


#endif
