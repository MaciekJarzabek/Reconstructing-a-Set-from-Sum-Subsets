#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<cmath>
#include<chrono>
using namespace std;

vector<int> ReadFile();
vector<int> Set;
vector<int> Map;
vector<bool> Used;
bool search_complete = false;
double amount;

vector<int> ReadFile() {
	fstream plik;
	plik.open("instancja.txt", ios::in);
	if (plik.good() == false) {
		cout << "Nie ma pliku";
		exit(0);
	}
	vector<int> input_instance;
	int length;
	while (plik >> length) {
		input_instance.push_back(length);
	}
	plik.close();
	return input_instance;
}

int Cuts(int K) {
	K = 8 * K;
	K = K + 1;
	K = sqrt(K);
	K = K - 3;
	K = K / 2;
	return K;
}

int FirstElement() {
	int first = Set[Set.size() - 1] - Set[Set.size() - 2];
	for (int a = 0; a < Set.size(); a++) {
		if (Set[a] == first) {
			Used[a] = true;
			break;
		}
	}
	return first;
}

void WriteOutTheMap(vector<int> zbior) {
	for (int a = 0; a < zbior.size(); a++) {
		cout << zbior[a] << ", ";
	}
	cout << endl;
}

bool UsedSet(int number) {
	for (int a = 0; a < Set.size(); a++) {
		if (Used[a] == true) {
			continue;
		}
		if (Set[a] == number) {
			Used[a] = true;
			return true;
		}
	}
	return false;
}

bool Check() {
	vector<bool> in;
	for (int a = 0; a < Map.size() - 1; a++) {
		in.push_back(false);
	}
	int tmp = Map[Map.size() - 1];
	for (int a = Map.size() - 2; a >= 0; a--) {
		if (UsedSet(tmp + Map[a])) {
			in[a] = true;
			tmp = tmp + Map[a];
		}
	}
	for (int a = 0; a < in.size(); a++) {
		if (in[a] == false) {
			return false;
		}
	}
	return true;
}

bool CalculateInstanceLength() {
	double a, b, c;
	a = 1;
	b = 3;
	c = (2 * Set.size()) - 2;
	double delt = sqrt((b * b) + (4 * a * c));
	double k2 = (-b - delt) / (2 * a);
	if (floor(k2) == k2) {
		amount = k2 + 1;
		return true;
	}
	else {
		return false;
	}
}

void BacktrackSearch(int s, int cuts_number) {
	if (Map.size() == cuts_number)
	{
		for (int a = 0; a < Used.size(); a++) {
			if (Used[a] == false) {
				break;
			}
		}
		cout << "Result: ";

		for (int a = 0; a < Map.size(); a++) {
			cout << Map[a] << " | ";
		}

		cout << "\n";
		search_complete = true;
		return;
	}
	for (int a = 0; a < Set.size(); a++) {
		if (Used[a] == true)
			continue;
		Map.push_back(Set[a]);

		vector<bool> backup_used = Used;
		if (Check()) {
			Used[a] = true;
			BacktrackSearch(Set[a], cuts_number);
			if (search_complete)
				break;
			Used = backup_used;
		}
		else {
			Used = backup_used;
			Map.pop_back();
		}
	}

	Map.pop_back();

	return;
}

int main() {
	auto start = chrono::high_resolution_clock::now();

	Set = ReadFile();
	sort(Set.begin(), Set.end());
	int cuts = Cuts(Set.size());
	if (cuts == -1) {
		cout << "Cannot construct map";
		return 0;
	}
	for (int i = 0; i < Set.size(); i++) {
		Used.push_back(false);
	}
	int first = FirstElement();
	Map.push_back(first);
	if (CalculateInstanceLength() == true) {
		BacktrackSearch(first, cuts + 1);
	}
	else
	{
		cout << "Incorrect instance length" << endl;
	}
	if (search_complete == false) {
		cout << "No solution found " << endl;
		return 0;
	}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	cout << "Execution time: " << duration.count() << "ms" << endl;

	return 1;
}