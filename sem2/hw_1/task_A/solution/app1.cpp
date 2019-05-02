#include <iostream>
#include <vector>
#include <string>
#include "MatrixGraph.cpp"
#include "ListGraph.cpp"
#include "ArcGraph.cpp"
#include "SetGraph.cpp"

using namespace std;

int main () {
	Graph *graph;

	cout << "Выберите тип графа:" << endl;
	cout << "1) Matrix" << endl;
	cout << "2) List" << endl;
	cout << "3) Arc" << endl;
	cout << "4) Set" << endl;

	while (true) {
		int num;
		cin >> num;

		if (num == 1) {
			graph = new MatrixGraph;
			break;
		}

		else if (num == 2) {
			graph = new ListGraph;
			break;
		}

		else if (num == 3) {
			graph = new ArcGraph;
			break;
		}

		else if (num == 4) {
			graph = new SetGraph;
			break;	
		}
	}

	while (!graph->scan());

	cout << endl;
	cout << "0) help" << endl;
	cout << "1) print" << endl;
	cout << "2) scan" << endl;
	cout << "3) addEdge {from} {to}" << endl;
	cout << "4) removeEdge {from} {to}" << endl;
	cout << "5) getPrevVerticies {vertex}" << endl;
	cout << "6) getNextVerticies {vertex}" << endl;
	cout << "7) getVerticiesCount" << endl;
	cout << "8) exit" << endl;
	cout << endl;

	while (true) {
		string command;
		cin >> command;

		if (command == "help") {
			cout << "1) print" << endl;
			cout << "2) scan" << endl;
			cout << "3) addEdge {from} {to}" << endl;
			cout << "4) removeEdge {from} {to}" << endl;
			cout << "5) getPrevVerticies {vertex}" << endl;
			cout << "6) getNextVerticies {vertex}" << endl;
			cout << "7) getVerticiesCount" << endl;
			cout << "8) exit" << endl;
		}

		else if (command == "print") {
			graph->print();
		}

		else if (command == "scan") {
			cout << (graph->scan() ? "граф сохранен" : "ошибка") << endl;
		}

		else if (command == "addEdge") {
			int from, to;
			cin >> from >> to;
			cout << (graph->addEdge(from, to) ? "добавлено" : "ошибка") << endl;
		}

		else if (command == "removeEdge") {
			int from, to;
			cin >> from >> to;
			cout << (graph->removeEdge(from, to) ? "удалено" : "ошибка") << endl;
		}

		else if (command == "getPrevVerticies") {
			int vertex;
			cin >> vertex;
			vector<int> prev;

			if(!graph->getPrevVerticies(vertex, prev)) {
				cout << "ошибка" << endl;
				continue;
			}

			else {
				cout << "Родители " << vertex << ": ";
	
				for (int i = 0; i < prev.size(); i ++) {
					cout << prev[i] << " ";
				}
	
				if (prev.size() == 0) {
					cout << "нет родителей";
				}

				cout << endl;
			}
		}

		else if (command == "getNextVerticies") {
			int vertex;
			cin >> vertex;
			vector<int> next;

			if(!graph->getNextVerticies(vertex, next)) {
				cout << "ошибка" << endl;
			}

			else {
				cout << "Дети " << vertex << ": ";

				for (int i = 0; i < next.size(); i ++) {
					cout << next[i] << " ";
				}

				if (next.size() == 0) {
					cout << "нет детей";
				}

				cout << endl;
			}
		}

		else if (command == "getVerticiesCount") {
			cout << graph->getVerticiesCount() << endl;
		}

		else if (command == "exit") {
			break;
		}

		else {
			cout << "Неизвестная команда." << endl <<  "Введите 'help', чтобы посмотреть список команд." << endl;
			continue;
		}

		cout << endl;
	}

	delete graph;
	return 0;
}
