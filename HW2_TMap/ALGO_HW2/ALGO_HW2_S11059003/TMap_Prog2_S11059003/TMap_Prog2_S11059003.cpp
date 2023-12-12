#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void get_POIs_location(vector<vector<double>>&);
vector<int> max_convex_hull(vector<vector<double>>);
double get_distance(double, double, double, double);
int check_same_side(double, double);

int main(void) {
	vector<vector<double>> location;

	get_POIs_location(location);
	max_convex_hull(location);

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

vector<int> max_convex_hull(vector<vector<double>> location)
{
	int i, j, k, flag1, flag2;
	double divide, coefficient;
	double perimeter = 0, area = 0;
	vector<int> convex_hull;

	for (i = 0; i < location.size() - 1; i++) {
		for (j = i + 1; j < location.size(); j++) {
			if (location[j][0] - location[i][0] == 0) {
				continue;
			}
			coefficient = (location[j][1] - location[i][1]) / (location[j][0] - location[i][0]);
			divide = location[i][1] - coefficient * location[i][0];
			k = 0;
			flag1 = 0;

			do {
				if (k != i && k != j) {
					flag1 = check_same_side(divide, location[k][1] - coefficient * location[k][0]);
				}
				k++;
			} while (flag1 == 0);

			flag2 = flag1;		//avoid n = 3
			for (; k < location.size() - 1; k++) {
				if (k != i && k != j) {
					flag2 = check_same_side(divide, location[k][1] - coefficient * location[k][0]);
					if (flag1 != flag2) {
						break;
					}
				}
			}
			if (flag1 == flag2) {
				convex_hull.push_back(i);
				convex_hull.push_back(j);
				perimeter += get_distance(location[i][0], location[i][1], location[j][0], location[j][1]);
				area += (location[i][0] * location[j][1] - location[j][0] * location[i][1]) / 2;
			}
		}
	}

	cout << "The convex-hull is finished as following:\n";
	for (i = 0; i < convex_hull.size(); i += 2) {
		cout << "(" << location[convex_hull[i]][0] << ", " << location[convex_hull[i]][1] << ") and (" << location[convex_hull[i + 1]][0] << ", " << location[convex_hull[i + 1]][1] << ")\n";
	}
	cout << "The max perimeter of the convex-hull is " << perimeter << "\nThe max area of the convex-hull is " << abs(area) << "\n";

	return convex_hull;
}

double get_distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int check_same_side(double standard, double compare) {
	if (standard > compare) {
		return 1;
	}

	return 2;
}