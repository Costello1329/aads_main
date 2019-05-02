#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class GameField {
	private:
		int field[9];

	public:
		const unsigned short godNumber = 31;


		//Operators:

		void operator = (const GameField &other) {
			for (int i = 0; i < 9; i ++) {
				field[i] = other.get(i);
			}
		}


		//Setters:

		void set (int a, int b, int c, int d, int e, int f, int g, int h, int i) {
			field[0] = a;
			field[1] = b;
			field[2] = c;
			field[3] = d;
			field[4] = e;
			field[5] = f;
			field[6] = g;
			field[7] = h;
			field[8] = i;
		}

		void setAt (int el, int index) {
			field[index] = el;
		}

		void scan () {
			cin >> field[0] >> field[1] >> field[2] >> field[3] >> field[4] >> field[5] >> field[6] >> field[7] >> field[8];
		}


		//Getters:

		int get (int index) const {
			return field[index];
		}

		void print () const {
			cout << field[0] << " " << field[1] << " " << field[2] << endl <<  field[3] << " " << field[4] << " " << field[5] << endl << field[6] << " " << field[7] << " " << field[8] << endl;
		}


		//Transform:

		int heuristic () const {
			int count = 0;

			for (int i = 0; i < 9; i ++) {
				if (field[i] == 0) {
					continue;
				}

				int x = (field[i] - 1) % 3;
				int y = (field[i] - 1) / 3;

				count += abs((i % 3) - x) + abs((i / 3) - y);
			}

			bool linearParticipants[9];

			for (int i = 0; i < 9; i ++) {
				linearParticipants[i] = false;
			}

			//Linear conflict rows:

			for (int i = 0; i < 3; i ++) {
				for (int first = 0; first < 3; first ++) {
					for (int second = first + 1; second < 3; second ++) {
						int firstPosition = 3 * i + first;
						int secondPosition = 3 * i + second;
						int firstPlace = get(firstPosition) - 1;
						int secondPlace = get(secondPosition) - 1;

						if (firstPlace != -1 && secondPlace != -1 && firstPlace / 3 == i && secondPlace / 3 == i && firstPlace > secondPlace) {
							count += 2;
							linearParticipants[firstPlace + 1] = true;
							linearParticipants[secondPlace + 1] = true;
						}
					}
				}
			}

			//Linear conflict columns:

			for (int i = 0; i < 3; i ++) {
				for (int first = 0; first < 3; first ++) {
					for (int second = first + 1; second < 3; second ++) {
						int firstPosition = 3 * first + i;
						int secondPosition = 3 * second + i;
						int firstPlace = get(firstPosition) - 1;
						int secondPlace = get(secondPosition) - 1;

						if (firstPlace != -1 && secondPlace != -1 && firstPlace % 3 == i && secondPlace % 3 == i && firstPlace > secondPlace) {
							count += 2;
							linearParticipants[firstPlace + 1] = true;
							linearParticipants[secondPlace + 1] = true;
						}
					}
				}
			}

			//Corner conflict

			if (get(1) == 2 && get(3) == 4 && get(0) != 1 && !linearParticipants[2] && !linearParticipants[4]) {
				count += 2;
			}

			if (get(1) == 2 && get(5) == 6 && get(2) != 3 && !linearParticipants[2] && !linearParticipants[6]) {
				count += 2;
			}

			if (get(3) == 4 && get(7) == 8 && get(6) != 7 && !linearParticipants[4] && !linearParticipants[8]) {
				count += 2;
			}

			if (get(5) == 6 && get(7) == 8 && get(8) != 0 && !linearParticipants[6] && !linearParticipants[8]) {
				count += 2;
			}

			return count;
		}

		bool isSolvable () const {
			int inv = 0;

			for (int i = 0; i < 9; i ++) {
				for (int j = i + 1; j < 9; j ++) {
					if (field[i] != 0 && field[j] != 0 && field[i] > field[j]) {
						inv ++;
					}
				}
			}
			 
			return (inv % 2 == 0);
		}

		bool move (char direction) {
			int i;

			for (i = 0; i < 9; i ++) {
				if (field[i] == 0) {
					break;
				}
			}

			if (direction == 'R' && i % 3 != 2) {
				swap(field[i + 1], field[i]);
				return true;
			}

			else if (direction == 'D' && i / 3 != 2) {
				swap(field[i + 3], field[i]);
				return true;
			}

			else if (direction == 'L' && i % 3 != 0) {
				swap(field[i - 1], field[i]);
				return true;
			}

			else if (direction == 'U' && i / 3 != 0) {
				swap(field[i - 3], field[i]);
				return true;
			}

			else
				return false;
		}
};


bool expand (GameField &field, string &path, int depth, char prev = '0') {
	int heuristic = field.heuristic();

	if (heuristic + path.size() > depth)
		return false;

	if (heuristic == 0)
		return true;

	else {
		GameField currentField = field;
		char directions[4] = {'D', 'R', 'U', 'L'};
		char antiDirections[4] = {'U', 'L', 'D', 'R'};

		for (int i = 0; i < 4; i ++) {
			char direction = directions[i];
			field = currentField;

			if (direction == prev || !field.move(direction))
				continue;

			path += direction;

			if (expand(field, path, depth, antiDirections[i]))
				return true;

			else
				path.pop_back();
		}

		return false;
	}
}

void IDAStar (const GameField start, string &path) {
	for (int iteration = start.heuristic(); iteration <= start.godNumber; iteration += 2) {
		GameField field = start;

		if (expand(field, path, iteration))
			return;
	}
}


int main () {
	GameField start;
	start.scan();

	if (!start.isSolvable()) {
		cout << "-1" << endl;
		return 0;
	}

	string path;
	IDAStar(start, path);
	cout << path.size() << endl << path << endl;
	return 0;
}
