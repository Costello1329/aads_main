#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


class GameField {
	private:
		int field[16];

	public:
		const unsigned short godNumber = 80;


		//Operators:

		void operator = (const GameField &other) {
			for (int i = 0; i < 16; i ++)
				field[i] = other.get(i);
		}


		//Setters:

		void set (int el, int index) {
			field[index] = el;
		}

		void scan () {
			cin >> field[0] >> field[1] >> field[2] >> field[3] >> field[4] >> field[5] >> field[6] >> field[7] >> field[8] >> field[9] >> field[10] >> field[11] >> field[12] >> field[13] >> field[14] >> field[15];
		}


		//Getters:

		int get (int index) const {
			return field[index];
		}

		void print () const {
			cout << field[0] << ' ' << field[1] << ' ' << field[2] << ' ' << field[3] << endl << field[4] << ' ' << field[5] << ' ' << field[6] << ' ' << field[7] << endl << field[8] << ' ' << field[9] << ' ' << field[10] << ' ' << field[11] << endl << field[12] << ' ' << field[13] << ' ' << field[14] << ' ' << field[15] << endl;
		}


		//Descriptors:

		bool isSolvable () const { 
			int inversions = 0;

			for (int i = 0; i < 16; i ++) {
				if (field[i] != 0) {
					for (int j = 0; j < i; j ++)
						if (field[j] > field[i])
							inversions ++;
				}

				else
					inversions += 1 + (i >> 2);
			}
			 
			return inversions % 2 == 0;
		}

		int heuristic () const {
			int count = 0;

			//Manhattan distances:

			for (int i = 0; i < 16; i ++) {
				if (field[i] == 0)
					continue;

				int x = (field[i] - 1) % 4;
				int y = (field[i] - 1) >> 2;

				count += abs((i % 4) - x) + abs((i >> 2) - y);
			}

			//Linear conflict:

			bool participants[16];

			for (int i = 0; i < 16; i ++)
				participants[i] = false;

			//Linear conflict rows:

			for (int i = 0; i < 4; i ++) {
				for (int first = 0; first < 4; first ++) {
					for (int second = first + 1; second < 4; second ++) {
						int firstPosition = 4 * i + first;
						int secondPosition = 4 * i + second;
						int firstPlace = get(firstPosition) - 1;
						int secondPlace = get(secondPosition) - 1;

						if (firstPlace != -1 && secondPlace != -1 && firstPlace >> 2 == i && secondPlace >> 2 == i && firstPlace > secondPlace) {
							count += 2;
							participants[firstPlace + 1] = true;
							participants[secondPlace + 1] = true;
						}
					}
				}
			}

			//Linear conflict columns:

			for (int i = 0; i < 4; i ++) {
				for (int first = 0; first < 4; first ++) {
					for (int second = first + 1; second < 4; second ++) {
						int firstPosition = 4 * first + i;
						int secondPosition = 4 * second + i;
						int firstPlace = get(firstPosition) - 1;
						int secondPlace = get(secondPosition) - 1;

						if (firstPlace != -1 && secondPlace != -1 && firstPlace % 4 == i && secondPlace % 4 == i && firstPlace > secondPlace) {
							count += 2;
							participants[firstPlace + 1] = true;
							participants[secondPlace + 1] = true;
						}
					}
				}
			}

			//Corner conflict:

			if (get(1) == 2 && get(4) == 5 && get(0) != 1 && !participants[2] && !participants[5])
				count += 2;

			if (get(2) == 3 && get(7) == 8 && get(3) != 4 && !participants[3] && !participants[8])
				count += 2;

			if (get(8) == 9 && get(13) == 14 && get(12) != 13 && !participants[9] && !participants[14])
				count += 2;

			//Last move:

			if ((get(12) == 12 || get(13) == 12 || get(14) == 12 || get(15) == 12) && (get(3) == 15 || get(7) == 15 || get(11) == 15 || get(15) == 15))
				count += 2;

			return count;
		}


		//Transform:

		bool move (char direction) {
			int i;

			for (i = 0; i < 16; i ++)
				if (field[i] == 0)
					break;

			if (direction == 'L' && i % 4 != 3) {
				swap(field[i + 1], field[i]);
				return true;
			}

			else if (direction == 'U' && i >> 2 != 3) {
				swap(field[i + 4], field[i]);
				return true;
			}

			else if (direction == 'R' && i % 4 != 0) {
				swap(field[i - 1], field[i]);
				return true;
			}

			else if (direction == 'D' && i >> 2 != 0) {
				swap(field[i - 4], field[i]);
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
		char directions[4] = {'R', 'D', 'U', 'L'};
		char antiDirections[4] = {'L', 'U', 'D', 'R'};

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

	string path = "";
	IDAStar(start, path);
	cout << path.size() << endl << path << endl;
	return 0;
}
