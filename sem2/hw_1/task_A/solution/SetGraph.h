#ifndef CSETGRAPH_H
#define CSETGRAPH_H
#include "Graph.h"
#include <unordered_set>


class SetGraph : public Graph
{
	private:
		std::vector<std::unordered_set <int> > to;
		std::vector<std::unordered_set <int> > from;

	public:
		//Constructors:
		SetGraph () {}
		SetGraph (int size);
		SetGraph (const Graph *graph);

		//Setters:
		bool scan ();

		//Getters:
		void print ();

		//Interface:
		bool addEdge (int from, int to);
		bool removeEdge (int from, int to);
		bool getPrevVerticies(int vertex, std::vector <int> &verticies) const;
		bool getNextVerticies(int vertex, std::vector <int> &verticies) const;
		int getVerticiesCount() const;
};


#endif
