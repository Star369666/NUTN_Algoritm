#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void get_POIs_location(vector<vector<double>>&);
void closest(vector<vector<double>>);
double get_distance(double, double, double, double);
double found_smaller_distance(double, double, vector<int>&, int, int);

int main(void) {
	vector<vector<double>> location;

	get_POIs_location(location);
	closest(location);

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

void closest(vector<vector<double>> location) {
	double min_distance, current_distance;
	int i, j;
	vector<int> closest_POI;

	min_distance = get_distance(location[0][0], location[0][1], location[1][0], location[1][1]);
	for (i = 0; i < location.size() - 1; i++) {
		for (j = i + 1; j < location.size(); j++) {
			current_distance = get_distance(location[i][0], location[i][1], location[j][0], location[j][1]);
			min_distance = found_smaller_distance(current_distance, min_distance, closest_POI, i, j);
		}
	}

	for (i = 0; i < closest_POI.size(); i += 2) {
		cout << "Found a pair of the closest POIs: (" << location[closest_POI[i]][0] << ", " << location[closest_POI[i]][1] << ") and (" << location[closest_POI[i + 1]][0] << ", " << location[closest_POI[i + 1]][1] << ").\n";
	}
	cout << "The distance of the closest POIs is " << min_distance << "\n";
}

double get_distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double found_smaller_distance(double current_distance, double min_distance, vector<int>& closest_POI, int i, int j) {
	if (current_distance <= min_distance) {
		if (current_distance < min_distance && !closest_POI.empty()) {
			closest_POI.clear();
		}
		closest_POI.push_back(i);
		closest_POI.push_back(j);
		return current_distance;
	}

	return min_distance;
}