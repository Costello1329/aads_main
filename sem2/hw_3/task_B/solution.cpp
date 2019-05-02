#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <queue>
#include <list>

using std::vector;
using std::queue;
using std::list;
using std::pair;
using std::make_pair;
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::min;
using std::max;


// Net class:
template <class Edge>
class Net {
	private:
		typedef Edge* EdgePointer;
		typedef pair<EdgePointer, EdgePointer> Arc;
		typename Edge::IndexType source;
		typename Edge::IndexType sink;
		list<Arc>* edges;
		vector<list<Arc>*> innerEdges;
		vector<list<Arc>*> outerEdges;

		class UniversalEdgeIterator {
			friend class Net;

			private:
				list<Arc>* edgesList;
				typename list<Arc>::iterator edgeIterator;

				bool validated;
				enum UniversalIteratorType {inner, outer, all};
				UniversalIteratorType iteratorType;
	
				UniversalEdgeIterator (list<Arc> *edgesList, const UniversalIteratorType &iteratorType) {
					this->edgesList = edgesList;
					this->iteratorType = iteratorType;
					validated = false;
				}

				void validate () {
					if (!validated)
						begin();

					validated = true;
				}

			public:
				typedef UniversalIteratorType IteratorType;

				UniversalEdgeIterator () {
					edgesList = nullptr;
					validated = false;
				}

				// Iterator methods:
				void begin () {
					edgeIterator = edgesList->begin();
				}

				void move () {
					edgeIterator = next(edgeIterator);
				}
	
				bool end () const {
					return edgesList->size() == 0 || edgeIterator == edgesList->end();
				}

				// Getters:
				Edge getEdge () const {
					return *(edgeIterator->first);
				}

				Edge getReversedEdge () const {
					return *(edgeIterator->second);
				}

				// Setter:
				void increaseFlow (const typename Edge::FlowType &delta) const {
					edgeIterator->first->flow += delta;
					edgeIterator->second->flow -= delta;
				}

				// Other:
				IteratorType getIteratorType () const {
					return iteratorType;
				}
		};

		UniversalEdgeIterator* allEdgesIterator;
		vector<UniversalEdgeIterator*> innerEdgesIterators;
		vector<UniversalEdgeIterator*> outerEdgesIterators;

		void expand (const typename Edge::IndexType &newVerticesCount) {
			const typename Edge::IndexType &oldVerticesCount = getVerticesCount();

			innerEdges.resize(newVerticesCount);
			outerEdges.resize(newVerticesCount);
			innerEdgesIterators.resize(newVerticesCount);
			outerEdgesIterators.resize(newVerticesCount);

			for (unsigned i = oldVerticesCount; i < newVerticesCount; i ++) {
				innerEdges[i] = new list<Arc>();
				outerEdges[i] = new list<Arc>();
				innerEdgesIterators[i] = new EdgeIterator(&(*innerEdges[i]), EdgeIterator::IteratorType::inner);
				outerEdgesIterators[i] = new EdgeIterator(&(*outerEdges[i]), EdgeIterator::IteratorType::outer);
			}
		}

	public:
		typedef UniversalEdgeIterator EdgeIterator;
		typedef typename Edge::IndexType IndexType;

		// Constructors, destructors:
		Net (const IndexType &verticesCount = 0) {
			edges = new list<Arc>();
			allEdgesIterator = new EdgeIterator(&(*edges), EdgeIterator::IteratorType::all);
		}

		~Net () {
			for (const Arc &arc: *edges)
				delete arc.first;

			delete edges;
			delete allEdgesIterator;

			for (typename Edge::IndexType vertex = 0; vertex < getVerticesCount(); vertex ++) {
				delete innerEdges[vertex];
				delete outerEdges[vertex];
				delete innerEdgesIterators[vertex];
				delete outerEdgesIterators[vertex];
			}
		}

		// Setters:
		void setSource (const typename Edge::IndexType &source) {
			if (this->source >= getVerticesCount())
				expand(source + 1);

			this->source = source;
		}

		void setSink (const typename Edge::IndexType &sink) {
			if (this->sink >= getVerticesCount())
				expand(sink + 1);

			this->sink = sink;
		}

		void addEdge (const Edge &edge) {
			if (edge.from >= getVerticesCount() || edge.to >= getVerticesCount())
				expand(max<typename Edge::IndexType>(edge.from, edge.to) + 1);

			EdgePointer edgePointer = new Edge(edge);
			EdgePointer reversedEdgePointer = new Edge(edge.reversed());

			edges->push_back(make_pair(edgePointer, reversedEdgePointer));
			outerEdges[edgePointer->from]->push_back(make_pair(edgePointer, reversedEdgePointer));
			innerEdges[edgePointer->to]->push_back(make_pair(edgePointer, reversedEdgePointer));

			edges->push_back(make_pair(reversedEdgePointer, edgePointer));
			outerEdges[reversedEdgePointer->from]->push_back(make_pair(reversedEdgePointer, edgePointer));
			innerEdges[reversedEdgePointer->to]->push_back(make_pair(reversedEdgePointer, edgePointer));

			allEdgesIterator->validate();
			outerEdgesIterators[edgePointer->from]->validate();
			outerEdgesIterators[edgePointer->to]->validate();
			innerEdgesIterators[edgePointer->from]->validate();
			innerEdgesIterators[edgePointer->to]->validate();
		}

		// Getters:
		const typename Edge::IndexType &getSource () const {
			return source;
		}

		const typename Edge::IndexType &getSink () const {
			return sink;
		}

		EdgeIterator& getAllEdgesIterator () {
			return *allEdgesIterator;
		}

		EdgeIterator getConstAllEdgesIterator () const {
			return *allEdgesIterator;
		}

		EdgeIterator& getInnerEdgeIterator (const IndexType &vertex) {
			return *innerEdgesIterators[vertex];
		}

		EdgeIterator getConstInnerEdgeIterator (const IndexType &vertex) const {
			return *innerEdgesIterators[vertex];
		}

		EdgeIterator& getOuterEdgeIterator (const IndexType &vertex) {
			return *outerEdgesIterators[vertex];
		}

		EdgeIterator getConstOuterEdgeIterator (const IndexType &vertex) const {
			return *outerEdgesIterators[vertex];
		}

		// Info:
		IndexType getVerticesCount () const {
			return innerEdges.size();
		}

		IndexType getEdgesCount () const {
			return edges->size();
		}
};


// Flow addons:
template <class Edge>
const typename Edge::FlowType getFlowFromOrToVertex (typename Net<Edge>::EdgeIterator iter) {
	typename Edge::FlowType flow = 0;

	for (iter.begin(); !iter.end(); iter.move())
		flow += iter.getEdge().flow;

	return flow;
}

template <class Edge>
bool isFlowValid (const Net<Edge> &net) {
	typename Net<Edge>::EdgeIterator iter = net.getConstAllEdgesIterator();

	for (iter.begin(); !iter.end(); iter.move())
		if (iter.getEdge().flow > iter.getEdge().capacity || iter.getEdge().flow != -iter.getReversedEdge().flow)
			return false;

	for (typename Edge::IndexType vertex = 0; vertex < net.getVerticesCount(); vertex ++) {
		if (vertex == net.getSource() || vertex == net.getSink())
			break;

		typename Edge::FlowType innerFlow = getFlowFromOrToVertex<Edge>(net.getConstInnerEdgeIterator(vertex));
		typename Edge::FlowType outerFlow = getFlowFromOrToVertex<Edge>(net.getConstOuterEdgeIterator(vertex));

		if (innerFlow != outerFlow)
			return false;
	}

	return true;
}

template <class Edge>
typename Edge::FlowType getFlowWithoutCheck (const Net<Edge> &net) {
	return getFlowFromOrToVertex<Edge>(net.getConstOuterEdgeIterator(net.getSource()));
}

template <class Edge>
typename Edge::FlowType getFlowWithCheck (const Net<Edge> &net) {
	if (isFlowValid(net))
		return getFlowWithoutCheck(net);

	else
		throw ("Net exeption: invalid flow!");
}


// Global discharge algorithm:
template <class Edge>
void push (Net<Edge> &net, const typename Net<Edge>::EdgeIterator &iter, const typename Edge::FlowType &pushingFlow, vector<typename Edge::FlowType> &excess) {
	iter.increaseFlow(pushingFlow);
	Edge edge = iter.getEdge();

	if (edge.from != net.getSource() && edge.from != net.getSink())
		excess[edge.from] -= pushingFlow;

	if (edge.to != net.getSource() && edge.to != net.getSink())
		excess[edge.to] += pushingFlow;
}

template <class Edge>
void relabel (const Net<Edge> &net, const typename Edge::IndexType &vertex, vector<typename Edge::IndexType> &height) {
	typename Edge::IndexType minHeight = numeric_limits<typename Edge::IndexType>::max();

	typename Net<Edge>::EdgeIterator iter = net.getConstOuterEdgeIterator(vertex);

	for (iter.begin(); !iter.end(); iter.move()) {
		Edge edge = iter.getEdge();

		if (edge.residualCapacity() > 0)
			minHeight = min<typename Edge::IndexType>(minHeight, height[edge.to]);
	}

	height[vertex] = minHeight + 1;
}

template <class Edge>
inline void discharge (Net<Edge> &net, const typename Edge::IndexType &vertex, vector<typename Edge::FlowType> &excess, vector<typename Edge::IndexType> &height) {
	while (excess[vertex] > 0) {
		typename Net<Edge>::EdgeIterator &iter = net.getOuterEdgeIterator(vertex);

		if (iter.end()) {
			relabel(net, vertex, height);
			iter.begin();
		}

		else {
			Edge edge = iter.getEdge();

			if (edge.residualCapacity() > 0 && height[vertex] == height[edge.to] + 1) {
				typename Edge::FlowType pushingFlow = min<typename Edge::FlowType>(excess[edge.from], edge.residualCapacity());
				push(net, iter, pushingFlow, excess);
			}

			else
				iter.move();
		}
	}
}

template <class Edge>
void setMaxFlowWithPreflowPush (Net<Edge> &net) {
	vector<typename Edge::FlowType> excess(net.getVerticesCount(), 0);
	vector<typename Edge::IndexType> height(net.getVerticesCount(), 0);
	height[net.getSource()] = net.getVerticesCount();

	typename Net<Edge>::EdgeIterator iter = net.getConstOuterEdgeIterator(net.getSource());

	for (iter.begin(); !iter.end(); iter.move())
		push(net, iter, iter.getEdge().residualCapacity(), excess);

	while (true) {
		bool discharged = false;

		for (typename Edge::IndexType vertex = 0; vertex < net.getVerticesCount(); vertex ++) {
			if (excess[vertex] && vertex != net.getSource() && vertex != net.getSink()) {
				discharge(net, vertex, excess, height);
				discharged = true;
			}
		}

		if (!discharged)
			break;
	}
}


// Dinic Blocking flow algorithm:
template <class Edge>
typename Edge::FlowType tryPushFlow (Net<Edge> &layeredNet, const typename Edge::IndexType &vertex, const typename Edge::FlowType &pushingFlow = numeric_limits<typename Edge::FlowType>::max()) {
	if (vertex == layeredNet.getSink() || pushingFlow == 0)
		return pushingFlow;

	for (typename Net<Edge>::EdgeIterator iter = layeredNet.getConstOuterEdgeIterator(vertex); !iter.end(); iter.move()) {
		Edge edge = iter.getEdge();

		if (edge.capacity == 0)
			continue;

		typename Edge::FlowType newPushingFlow = min<typename Edge::FlowType>(pushingFlow, edge.residualCapacity());
		typename Edge::FlowType pushedFlow = tryPushFlow(layeredNet, edge.to, newPushingFlow);

		if (pushedFlow != 0) {
			iter.increaseFlow(pushedFlow);
			return pushedFlow;
		}
	}

	if (!layeredNet.getOuterEdgeIterator(vertex).end())
		layeredNet.getOuterEdgeIterator(vertex).move();
	
	return 0;
}

template <class Edge>
void setBlockingFlowWithDinic (Net<Edge> &layeredNet) {
	while (tryPushFlow(layeredNet, layeredNet.getSource()) != 0) {};
}


// Malhotra-Kumar-Maheshwari Blocking flow algorithm:
template <class Edge>
inline typename Edge::FlowType setPotentialOneSide (typename Net<Edge>::EdgeIterator iter) {
	typename Edge::FlowType potential = 0;

	for (iter.begin(); !iter.end(); iter.move())
		potential += iter.getEdge().residualCapacity();

	return potential;
}

template <class Edge>
inline typename Edge::FlowType getPotential (const Net<Edge> &net, const typename Edge::IndexType &vertex, const vector<typename Edge::FlowType> &innerPotential, const vector<typename Edge::FlowType> &outerPotential) {
	if (vertex == net.getSource())
		return outerPotential[vertex];

	else if (vertex == net.getSink())
		return innerPotential[vertex];

	else
		return min<typename Edge::FlowType>(outerPotential[vertex], innerPotential[vertex]);
}

template <class Edge>
inline void deleteVertexOneSide (const Net<Edge> &layeredNet, typename Net<Edge>::EdgeIterator iter, vector<typename Edge::FlowType> &innerPotential, vector<typename Edge::FlowType> &outerPotential, vector<bool> &isDeleted) {
	for (iter.begin(); !iter.end(); iter.move()) {
		Edge edge = iter.getEdge();

		if (edge.capacity == 0)
			continue;

		typename Edge::IndexType nextVertex = (iter.getIteratorType() == Net<Edge>::EdgeIterator::IteratorType::outer ? edge.to : edge.from);
		(iter.getIteratorType() == Net<Edge>::EdgeIterator::IteratorType::outer ? innerPotential[nextVertex] : outerPotential[nextVertex]) -= edge.residualCapacity();

		if (getPotential(layeredNet, nextVertex, innerPotential, outerPotential) == 0)
			deleteVertex(layeredNet, nextVertex, innerPotential, outerPotential, isDeleted);
	}
}

template <class Edge>
void deleteVertex (const Net<Edge> &layeredNet, const typename Edge::IndexType &vertex, vector<typename Edge::FlowType> &innerPotential, vector<typename Edge::FlowType> &outerPotential, vector<bool> &isDeleted) {
	if (isDeleted[vertex])
		return;

	isDeleted[vertex] = true;
	deleteVertexOneSide<Edge>(layeredNet, layeredNet.getConstOuterEdgeIterator(vertex), innerPotential, outerPotential, isDeleted);
	deleteVertexOneSide<Edge>(layeredNet, layeredNet.getConstInnerEdgeIterator(vertex), innerPotential, outerPotential, isDeleted);
}

template <typename Edge>
void pushFlowFromRoot (Net<Edge> &layeredNet, const typename Edge::IndexType &root, const typename Edge::FlowType &pushingFlow, const typename Edge::IndexType &destination, vector<typename Edge::FlowType> &innerPotential, vector<typename Edge::FlowType> &outerPotential, const vector<bool> &isDeleted) {
	// BFS init:
	queue<typename Edge::IndexType> verticesQueue;
	vector<bool> used(layeredNet.getVerticesCount(), false);
	verticesQueue.push(root);
	used[root] = true;

	// Other data init:
	vector<typename Edge::FlowType> excessOrLack(layeredNet.getVerticesCount(), 0);
	excessOrLack[root] = pushingFlow;

	// BFS:
	while (!verticesQueue.empty()) {
		typename Edge::IndexType vertex = verticesQueue.front();
		verticesQueue.pop();

		if (vertex == destination) {
			break;
		}

		for (typename Net<Edge>::EdgeIterator &iter = (destination == layeredNet.getSink() ? layeredNet.getOuterEdgeIterator(vertex) : layeredNet.getInnerEdgeIterator(vertex)); !iter.end(); iter.move()) {
			Edge edge = iter.getEdge();
			typename Edge::IndexType nextVertex = (destination == layeredNet.getSink() ? edge.to : edge.from);

			if (edge.capacity > 0 && !isDeleted[nextVertex]) {
				if (!used[nextVertex]) {
					verticesQueue.push(nextVertex);
					used[nextVertex] = true;
				}
				
				typename Edge::FlowType pushedFlow = min<typename Edge::FlowType>(excessOrLack[vertex], edge.residualCapacity());
				excessOrLack[vertex] -= pushedFlow;
				excessOrLack[nextVertex] += pushedFlow;
				iter.increaseFlow(pushedFlow);
				outerPotential[edge.from] -= pushedFlow;
				innerPotential[edge.to] -= pushedFlow;
			}

			if (excessOrLack[vertex] == 0)
				break;
		}
	}
}

template <class Edge>
void setBlockingFlowWithMalhotraKumarMaheshwari (Net<Edge> &layeredNet) {
	// Initialize potentials:
	vector<typename Edge::FlowType> innerPotential(layeredNet.getVerticesCount());
	vector<typename Edge::FlowType> outerPotential(layeredNet.getVerticesCount());

	for (typename Edge::IndexType vertex = 0; vertex < layeredNet.getVerticesCount(); vertex ++) {
		innerPotential[vertex] = setPotentialOneSide<Edge>(layeredNet.getConstInnerEdgeIterator(vertex));
		outerPotential[vertex] = setPotentialOneSide<Edge>(layeredNet.getConstOuterEdgeIterator(vertex));
	}

	// Initialize deleted vertices:
	vector<bool> isDeleted(layeredNet.getVerticesCount(), false);

	// Main cycle:
	while (true) {
		// Deleting traverse:
		for (typename Edge::IndexType vertex = 0; vertex < layeredNet.getVerticesCount(); vertex ++)
			if (!isDeleted[vertex] && getPotential(layeredNet, vertex, innerPotential, outerPotential) == 0)
				deleteVertex(layeredNet, vertex, innerPotential, outerPotential, isDeleted);

		if (isDeleted[layeredNet.getSource()] || isDeleted[layeredNet.getSink()])
			break;

		// Finding root:

		typename Edge::IndexType root = layeredNet.getSource();

		for (typename Edge::IndexType vertex = 0; vertex < layeredNet.getVerticesCount(); vertex ++)
			if (!isDeleted[vertex] && getPotential(layeredNet, vertex, innerPotential, outerPotential) <= getPotential(layeredNet, root, innerPotential, outerPotential))
				root = vertex;

		typename Edge::FlowType rootPotential = getPotential(layeredNet, root, innerPotential, outerPotential);
		// Pushing flow through the root:
		pushFlowFromRoot(layeredNet, root, rootPotential, layeredNet.getSink(), innerPotential, outerPotential, isDeleted);
		pushFlowFromRoot(layeredNet, root, rootPotential, layeredNet.getSource(), innerPotential, outerPotential, isDeleted);
	}
}


// Global Max Flow augmenting paths algorithm:
template <class Edge>
Net<Edge> buildLayeredNetwork (const Net<Edge> &net) {
	// BFS init:
	queue<typename Edge::IndexType> verticesQueue;
	vector<bool> used(net.getVerticesCount(), false);
	verticesQueue.push(net.getSource());

	// Layered Net init:
	Net<Edge> layeredNet(net.getVerticesCount());
	layeredNet.setSource(net.getSource());
	layeredNet.setSink(net.getSink());
	vector<typename Edge::IndexType> layers(net.getVerticesCount(), numeric_limits<typename Edge::IndexType>::max());
	layers[net.getSource()] = 0;

	// BFS:
	while (!verticesQueue.empty()) {
		typename Edge::IndexType vertex = verticesQueue.front();
		verticesQueue.pop();
	
		if (used[vertex])
			continue;

		used[vertex] = true;
		
		typename Net<Edge>::EdgeIterator iter = net.getConstOuterEdgeIterator(vertex);

		for (iter.begin(); !iter.end(); iter.move()) {
			Edge edge = iter.getEdge();

			if (edge.residualCapacity() > 0 && layers[edge.from] + 1 <= layers[edge.to]) {
				verticesQueue.push(edge.to);
				
				Edge residualEdge = edge;
				residualEdge.capacity = edge.residualCapacity();
				residualEdge.flow = 0;
				layeredNet.addEdge(residualEdge);
				layers[edge.to] = layers[edge.from] + 1;
			}
		}
	}

	return layeredNet;
}

template <class Edge>
void transferFlowFromLayeredNetworkToNetwork (Net<Edge> &layeredNet, Net<Edge> &net) {
	// BFS init:
	queue<typename Edge::IndexType> verticesQueue;
	vector<bool> used(net.getVerticesCount(), false);
	verticesQueue.push(net.getSource());

	// Layered Net init:
	vector<typename Edge::IndexType> layers(net.getVerticesCount(), numeric_limits<typename Edge::IndexType>::max());
	layers[net.getSource()] = 0;

	// BFS:
	while (!verticesQueue.empty()) {
		typename Edge::IndexType vertex = verticesQueue.front();
		verticesQueue.pop();

		if (used[vertex])
			continue;

		used[vertex] = true;
		
		typename Net<Edge>::EdgeIterator iter = net.getConstOuterEdgeIterator(vertex);
		typename Net<Edge>::EdgeIterator &layeredNetIter = layeredNet.getOuterEdgeIterator(vertex);
		layeredNetIter.begin();

		for (iter.begin(); !iter.end() && !layeredNetIter.end(); iter.move()) {
			Edge edge = iter.getEdge();

			if (edge.residualCapacity() > 0 && layers[edge.from] + 1 <= layers[edge.to]) {
				verticesQueue.push(edge.to);

				layers[edge.to] = layers[edge.from] + 1;

				while (layeredNetIter.getEdge().capacity == 0)
					layeredNetIter.move();
				
				iter.increaseFlow(layeredNetIter.getEdge().flow);
				layeredNetIter.move();	
			}
		}
	}
}

template <class Edge>
void setMaxFlowWithAugmentingPaths (Net<Edge> &net) {
	while (true) {
		Net<Edge> layeredNet = buildLayeredNetwork(net);

		setBlockingFlowWithMalhotraKumarMaheshwari(layeredNet);

		if (getFlowWithCheck(layeredNet) == 0)
			break;

		else
			transferFlowFromLayeredNetworkToNetwork(layeredNet, net);
	}
}


// --------------------------------------------------


// My custom Edge:
template <typename FirstType = unsigned, typename SecondType = long long>
struct SimpleEdge {
	typedef FirstType IndexType;
	typedef SecondType FlowType;
	IndexType from;
	IndexType to;
	FlowType flow;
	FlowType capacity;

	SimpleEdge (const IndexType &from, const IndexType &to, const FlowType &capacity) {
		this->from = from;
		this->to = to;
		this->flow = 0;
		this->capacity = capacity;
	}

	FlowType residualCapacity () const {
		return capacity - flow;
	}

	SimpleEdge reversed () const {
		return SimpleEdge(to, from, 0);
	}
};


// Custom printers:
template <class Edge>
void printEdge (const Edge &edge, ostream &os = cout) {
	os << edge.from << " -> " << edge.to << " (" << edge.flow << "/" << edge.capacity << ")";
}

template <class Edge>
void printNet (Net<Edge> &net, ostream &os = cout) {
	for (typename Edge::IndexType vertex = 0; vertex < net.getVerticesCount(); vertex ++) {
		typename Net<Edge>::EdgeIterator iter = net.getConstOuterEdgeIterator(vertex);

		for (iter.begin(); !iter.end(); iter.move()) {
			printEdge(iter.getEdge(), os);
			os << " ";
		}

		os << endl;
	}
}

// Solving Matan:
void inputMatan (istream &is, Net<SimpleEdge<unsigned, long long> > &net, long long &positiveSum) {
	unsigned verticesCount;
	is >> verticesCount;

	net = Net<SimpleEdge<unsigned, long long> >(verticesCount + 2);
	net.setSource(verticesCount);
	net.setSink(verticesCount + 1);

	positiveSum = 0;


	for (unsigned vertex = 0; vertex < verticesCount; vertex ++) {
		long long usability;
		cin >> usability;

		if (usability > 0) {
			positiveSum += usability;
			net.addEdge(SimpleEdge<unsigned, long long>(net.getSource(), vertex, usability));
		}

		else
			net.addEdge(SimpleEdge<unsigned, long long>(vertex, net.getSink(), - usability));
	}


	const long long maxCapacity = verticesCount * 1000 + 1;

	for (unsigned from = 0; from < verticesCount; from ++) {
		unsigned outerEdgesCount;
		cin >> outerEdgesCount;

		for (unsigned edgeIndex = 0; edgeIndex < outerEdgesCount; edgeIndex ++) {
			unsigned to;
			cin >> to;
			-- to;
			net.addEdge(SimpleEdge<unsigned, long long>(from, to, maxCapacity));
		}
	}
}

void outputMatan (ostream &os, const long long &maxUsability) {
	os << maxUsability << endl;
}

void solveMatan (istream &is, ostream &os) {
	Net<SimpleEdge<unsigned, long long> > net;
	long long positiveSum;
	inputMatan(is, net, positiveSum);
	
	setMaxFlowWithPreflowPush(net);
	long long maxFlow = getFlowWithoutCheck(net);
	
	outputMatan(os, positiveSum - maxFlow);	
}

// Solving Undirected maxFlow:
template <class Edge>
void inputUndirectedMaxFlow (istream &is, Net<Edge> &net) {
	typename Edge::IndexType verticesCount, edgesCount;
	is >> verticesCount >> edgesCount;

	net.setSource(0);
	net.setSink(verticesCount - 1);

	for (unsigned edgeIndex = 0; edgeIndex < edgesCount; edgeIndex ++) {
		typename Edge::IndexType from, to;
		typename Edge::FlowType capacity;
		cin >> from >> to >> capacity;
		-- from;
		-- to;

		net.addEdge(Edge(from, to, capacity));
		//net.addEdge(Edge(to, from, capacity));
	}
}

void outputUndirectedMaxFlow (ostream &os, const long long &maxFlow) {
	os << maxFlow << endl;
}

void solveUndirectedMaxFlow (istream &is, ostream &os) {
	Net<SimpleEdge<unsigned, long long> > net;
	inputUndirectedMaxFlow(is, net);

	setMaxFlowWithPreflowPush(net);
	long long maxFlow = getFlowWithCheck(net);
	
	outputUndirectedMaxFlow(os, maxFlow);	
}


// Main:
int main () {
	solveUndirectedMaxFlow(cin, cout);
	return 0;
}
