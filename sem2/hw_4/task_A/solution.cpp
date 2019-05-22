#include <iostream>
#include <vector>
#include <math.h>
#include <limits>
using namespace std;

class SparseTable {
  private:
  	typedef pair<int, size_t> Pair;
	vector<vector<Pair> > table;

  public:
	SparseTable (const vector<int> &sequence) {
		const size_t length = sequence.size();
		const size_t height = static_cast<size_t>(log2(sequence.size())) + 1;
		table.resize(height);

		for (size_t i = 0; i < length; i ++) {
			table[0].push_back(make_pair(sequence[i], i));
		}

		for (size_t i = 1; i < height; i ++) {
			const size_t currentLength = length - pow(2, i) + 1;
			table[i].resize(currentLength);

			for (size_t j = 0; j < currentLength; j ++) {
				table[i][j] = min<Pair>(table[i - 1][j], table[i - 1][j + pow(2, i - 1)]); 
			}
		}
	}

	Pair RangeMinimumQuery (const size_t left, const size_t right) {
		if (left > right) {
			throw "out of range!";
		}

		const size_t length = right - left + 1;
		const size_t power = static_cast<size_t>(log2(length));
		const size_t flooredLength = pow(2, power);

		const Pair leftValue = table[power][left];
		const size_t padding = length - flooredLength;
		const Pair rightValue = table[power][left + padding];

		return min<Pair>(leftValue, rightValue);
	}
};


int main() {
	size_t length, queriesCount;
	cin >> length >> queriesCount;

	vector<int> sequence(length);
	
	for (size_t i = 0; i < length; i ++) {
		cin >> sequence[i];
	}

	SparseTable sparseTable(sequence);

	for (size_t i = 0; i < queriesCount; i ++) {
		size_t left, right;
		cin >> left >> right;
		left --;
		right --;
		size_t firstMinimumIndex = sparseTable.RangeMinimumQuery(left, right).second;
		int value;

		if (firstMinimumIndex == left) {
			pair<int, size_t> rightQuery = sparseTable.RangeMinimumQuery(left + 1, right);
			value = rightQuery.first;
		} else if (firstMinimumIndex == right) {
			pair<int, size_t> leftQuery = sparseTable.RangeMinimumQuery(left, right - 1);
			value = leftQuery.first;
		} else {
			pair<int, size_t> leftQuery = sparseTable.RangeMinimumQuery(left, firstMinimumIndex - 1);
			pair<int, size_t> rightQuery = sparseTable.RangeMinimumQuery(firstMinimumIndex + 1, right);
			value = min<size_t>(leftQuery.first, rightQuery.first);
		}

		cout << value << endl;
	}

	return 0;
}
