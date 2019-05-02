#ifndef GRAPH_H
#define GRAPH_H
#include <vector>


class Graph {
	public:
		//Constructors
		virtual	~Graph () {}

		//Setters
		virtual bool scan () = 0;

		//Getters
		virtual void print () = 0;

		//Interface
		virtual bool addEdge (int from, int to) = 0;
		virtual bool removeEdge (int from, int to) = 0;
		virtual bool getNextVerticies (int vertex, std::vector<int> &verticies) const = 0;
		virtual bool getPrevVerticies (int vertex, std::vector<int> &verticies) const = 0;
		virtual int getVerticiesCount () const = 0;
};


#endif
