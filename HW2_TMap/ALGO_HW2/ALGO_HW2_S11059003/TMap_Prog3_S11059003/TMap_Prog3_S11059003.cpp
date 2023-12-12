#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void get_POIs_location(vector<vector<double>>&);
void shortest_Path(vector<vector<double>>);
double get_distance(double, double, double, double);
double compare_distance(double, double, int, int, int*, int*);
int binary_search(vector<int>, int, int, int);

int main(void) {
	vector<vector<double>> location;

	get_POIs_location(location);
	shortest_Path(location);

	return 0;
}

void get_POIs_location(vector<vector<double>>& location) {
	ifstream in_file;
	string file_name;
	char longitude, latitude;
	double longitude_value, latitude_value;

	cout << "Enter the map file name in (.txt) format: ";
	cin >> file_name;
	in_file.open(file_name + ".txt");

	if (!in_file.is_open()) {
		cout << "Failed to open the file.\n";
		exit(1);
	}

	while (!in_file.eof()) {
		in_file >> longitude >> longitude_value >> latitude >> latitude_value;

		if (longitude == 'W') {
			longitude_value *= -1;
		}
		if (latitude_value == 'S') {
			latitude_value *= -1;
		}
		location.push_back({ longitude_value, latitude_value });
	}
	in_file.close();
}

void shortest_Path(vector<vector<double>> location) {
	int i, j, used_index;
	int k = 0, next_index = 0, current_index = 0;
	vector<int> used_POI, min_POI;		//space: n+2n
	double min_distance, current_distance;
	double total_distance = 0;

	cout << "The minimum n POIs traversal is finished as following:\n";

	for (i = 0; i < location.size(); i++) {
		used_POI.push_back(i);
	}

	for (i = 0; i < location.size() - 1; i++) {
		if (used_POI[0] == current_index) {
			min_distance = get_distance(location[used_POI[1]][0], location[used_POI[1]][1], location[current_index][0], location[current_index][1]);
		}
		else {
			min_distance = get_distance(location[used_POI[0]][0], location[used_POI[0]][1], location[current_index][0], location[current_index][1]);
		}

		for (j = 0; j < used_POI.size(); j++) {
			current_distance = get_distance(location[used_POI[j]][0], location[used_POI[j]][1], location[current_index][0], location[current_index][1]);
			min_distance = compare_distance(min_distance, current_distance, current_index, used_POI[j], &used_index, &next_index);
		}

		current_index = next_index;
		total_distance += min_distance;
		min_POI.push_back(used_index);
		min_POI.push_back(next_index);
		used_index = binary_search(used_POI, used_index, 0, used_POI.size() - 1);
		used_POI.erase(used_POI.begin() + used_index);
		cout << "(" << location[min_POI[k]][0] << ", " << location[min_POI[k]][1] << ") and (" << location[min_POI[k + 1]][0] << ", " << location[min_POI[k + 1]][1] << ")\n";
		k += 2;
	}

	cout << "(" << location[used_POI[0]][0] << ", " << location[used_POI[0]][1] << ") and (" << location[0][0] << ", " << location[0][1] << ")\n";
	total_distance += get_distance(location[used_POI[0]][0], location[used_POI[0]][1], location[0][0], location[0][1]);
	cout << "The minimum distance is " << total_distance << "\n";
}

double get_distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double compare_distance(double min_distance, double current_distance, int i, int j, int* index1, int* index2) {
	if (i != j) {
		if (min_distance >= current_distance) {
			*index1 = i;
			*index2 = j;
			return current_distance;
		}
	}

	return min_distance;
}

int binary_search(vector<int> array, int target, int left, int right) {
	int middle;

	do {
		middle = (left + right) / 2;

		if (array[middle] == target) {
			return middle;
		}
		else if (array[middle] > target) {
			right = middle - 1;
		}
		else {
			left = middle + 1;
		}

	} while (left <= right);

	return -1;
}