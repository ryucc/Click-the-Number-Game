#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Recursive function to build subsets
void dfs(int start, int target, int mask, vector<int>& results) {
    if (target == 0) {
        results.push_back(mask);
        return;
    }
    for (int i = start; i <= 12; i++) {
        if (i > target) break;
        dfs(i + 1, target - i, mask | (1 << (i - 1)), results);
    }
}

// Generate all subsets as bitmasks for sums 2–12
vector<vector<int>> combinations_for_sums(int min_sum = 0, int max_sum = 12) {
    vector<vector<int>> all_results;
    for (int target = min_sum; target <= max_sum; target++) {
        vector<int> combos;
        dfs(1, target, 0, combos);
        all_results.push_back(combos);
    }
    return all_results;
}

// Helper to print a bitmask as numbers
void print_mask(int mask) {
    cout << "{ ";
    for (int i = 0; i < 12; i++) {
        if (mask & (1 << i)) {
            cout << (i + 1) << " ";
        }
    }
    cout << "}";
}

vector<double> dice_sum_probabilities() {
    vector<double> prob(13, 0.0); // index = sum, 0–12

    for (int d1 = 1; d1 <= 6; d1++) {
        for (int d2 = 1; d2 <= 6; d2++) {
            int sum = d1 + d2;
            prob[sum] += 1.0 / 36.0;
        }
    }
    return prob;
}

double dp(vector<double> &memo, vector<vector<int>> &combos, vector<double> &dice_prob, int state) {
	if(memo[state] != -1) return memo[state];

    double prob = 0.0;
    for (int sum = 2; sum <= 12; sum++) {
        double best = 0.0;
        for (int mask : combos[sum]) {
            if ((state & mask) == mask) { // can remove this subset
                double p = 1.0 - dp(memo, combos, dice_prob, state ^ mask);
                if (p > best) best = p;
            }
        }
        prob += dice_prob[sum] * best;
    }
    memo[state] = prob;
//	cout << state << ": " << memo[state] << endl;
    return prob;
}
double show(vector<double> &memo, vector<vector<int>> &combos, vector<double> &dice_prob, int state) {


	print_mask(state);
	cout << ": " << memo[state] << endl;
    for (int sum = 2; sum <= 12; sum++) {
        double best = 0.0;
		int bm = 0;
		cout << sum << endl;
        for (int mask : combos[sum]) {
            if ((state & mask) == mask) { // can remove this subset
                double p = 1.0 - memo[state ^ mask];
                if (p > best) {
					best = p;
					bm = mask;
				}
				print_mask(mask);
				cout << ": " << p << endl;
            }
        }
    }
    return 0;
}

int main() {
    auto result = combinations_for_sums();
	auto probs = dice_sum_probabilities();
	vector<double> memo(4096, -1);

	dp(memo, result, probs, 4095);
	dp(memo, result, probs, 4094);
	cout << "[";
	for(int i = 0; i < 4096; i++) {
		cout << memo[i] << ", ";
	}
	cout << "]";

    return 0;
}

