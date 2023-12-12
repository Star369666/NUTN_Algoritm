#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void get_POIs_location(vector<vector<double>>&);
void Convex_Hull_TSP_Algorithm(vector<vector<double>>, vector<int>);
double get_distance(double, double, double, double);
vector<int> max_convex_hull(vector<vector<double>>);
int check_same_side(double, double);
void is_able_to_in_line(vector<vector<double>>, vector<int>, double, double, double, vector<vector<double>>&);
void link_points_in_the_side(vector<vector<double>>, vector<vector<double>>, vector<int>, int, int, double&);

int main(void) {
	vector<vector<double>> location;

	get_POIs_location(location);
	vector<int> convex_hull = max_convex_hull(location);
	Convex_Hull_TSP_Algorithm(location, convex_hull);

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

double get_distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<int> max_convex_hull(vector<vector<double>> location)
{
	int i, j, flag2;
	int k = 0, flag1 = 0;
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

	return convex_hull;
}

int check_same_side(double standard, double compare) {
	if (standard > compare) {
		return 1;
	}

	return 2;
}

void Convex_Hull_TSP_Algorithm(vector<vector<double>> location, vector<int> convex_hull) {
	int i, j;
	int minimum_index = 0;
	vector<int> rest_POI, side_point_count, traveller;
	double coefficient, constant, reflection_coefficient, rest_POI_x, rest_POI_y, minimum_distance, current_distance;
	double total_distance = 0;
	vector<vector<double>> line_coefficient, able_reflection_side, convex_hull_rest_POI;
	bool flag;

	line_coefficient.resize(convex_hull.size() / 2);
	side_point_count.resize(convex_hull.size() / 2);

	fill(side_point_count.begin(), side_point_count.end(), 0);

	for (i = 0; i < location.size(); i++) {
		flag = true;
		for (j = 0; j < convex_hull.size(); j++) {
			if (i == convex_hull[j]) {
				flag = false;
			}
		}
		if (flag) {
			rest_POI.push_back(i);
		}
	}

	for (i = 0; i < convex_hull.size(); i += 2) {
		coefficient = (location[convex_hull[i]][1] - location[convex_hull[i + 1]][1]) / (location[convex_hull[i]][0] - location[convex_hull[i + 1]][0]);
		constant = location[convex_hull[i]][1] - coefficient * location[convex_hull[i]][0];
		line_coefficient[i / 2].push_back(location[convex_hull[i]][1] - location[convex_hull[i + 1]][1]);
		line_coefficient[i / 2].push_back(location[convex_hull[i]][0] - location[convex_hull[i + 1]][0]);
		line_coefficient[i / 2].push_back((location[convex_hull[i]][0] - location[convex_hull[i + 1]][0]) * constant);
	}

	for (i = 0; i < rest_POI.size(); i++) {
		able_reflection_side.clear();
		minimum_index = 0;
		for (j = 0; j < line_coefficient.size(); j++) {
			reflection_coefficient = -1 * (line_coefficient[j][0] * location[rest_POI[i]][0] - line_coefficient[j][1] * location[rest_POI[i]][1] + line_coefficient[j][2]) / (pow(line_coefficient[j][0], 2) - pow(line_coefficient[j][1], 2));
			rest_POI_x = location[rest_POI[i]][0] + reflection_coefficient * line_coefficient[j][0];
			rest_POI_y = location[rest_POI[i]][1] + reflection_coefficient * line_coefficient[j][1];
			if (j == 0) {
				minimum_distance = get_distance(location[rest_POI[i]][0], location[rest_POI[i]][1], rest_POI_x, rest_POI_y);
			}
			else {
				current_distance = get_distance(location[rest_POI[i]][0], location[rest_POI[i]][1], rest_POI_x, rest_POI_y);
				if (minimum_distance > current_distance) {
					minimum_distance = current_distance;
					minimum_index = j;
				}
			}
			is_able_to_in_line(location, convex_hull, j * 1.0, rest_POI_x, rest_POI_y, able_reflection_side);
		}

		if (able_reflection_side.empty()) {
			convex_hull_rest_POI.push_back({ rest_POI[i] * 1.0, minimum_index * 1.0, rest_POI_x, rest_POI_y });
		}
		else {
			minimum_distance = get_distance(location[rest_POI[i]][0], location[rest_POI[i]][1], able_reflection_side[0][1], able_reflection_side[0][2]);
			minimum_index = 0;
			for (j = 1; j < able_reflection_side.size(); j++) {
				current_distance = get_distance(location[rest_POI[i]][0], location[rest_POI[i]][1], able_reflection_side[j][1], able_reflection_side[j][2]);
				if (minimum_distance > current_distance) {
					minimum_distance = current_distance;
					minimum_index = j;

				}
			}
			convex_hull_rest_POI.push_back({ rest_POI[i] * 1.0, minimum_index * 1.0, able_reflection_side[minimum_index][1], able_reflection_side[minimum_index][2] });
		}
		side_point_count[minimum_index]++;
	}

	cout << "The Convex-Hull-TSP Algorithm is finished as following:\n";
	for (i = 0; i < side_point_count.size(); i++) {
		if (side_point_count[i] == 0) {
			cout << "(" << location[convex_hull[2 * i]][0] << ", " << location[convex_hull[2 * i]][1] << ") to (" << location[convex_hull[2 * i + 1]][0] << ", " << location[convex_hull[2 * i + 1]][1] << ")\n";
			total_distance += get_distance(location[convex_hull[2 * i]][0], location[convex_hull[2 * i]][1], location[convex_hull[2 * i + 1]][0], location[convex_hull[2 * i + 1]][1]);
		}
		else {
			link_points_in_the_side(location, convex_hull_rest_POI, convex_hull, i, side_point_count[i], total_distance);
		}
	}
	cout << "The Convex-Hull-TSP Algorithm distance is " << total_distance;
}

void is_able_to_in_line(vector<vector<double>> location, vector<int> convex_hull, double side_number, double x, double y, vector<vector<double>>& able_in_line)
{
	double left_x, left_y, right_x, right_y;

	if (location[convex_hull[2 * side_number]][0] > location[convex_hull[2 * side_number + 1]][0]) {
		left_x = location[convex_hull[2 * side_number + 1]][0];
		right_x = location[convex_hull[2 * side_number]][0];

		if (location[convex_hull[2 * side_number]][1] > location[convex_hull[2 * side_number + 1]][1]) {
			left_y = location[convex_hull[2 * side_number + 1]][1];
			right_y = location[convex_hull[2 * side_number]][1];
		}
		else {
			left_y = location[convex_hull[2 * side_number]][1];
			right_y = location[convex_hull[2 * side_number + 1]][1];
		}
	}
	else {
		left_x = location[convex_hull[2 * side_number]][0];
		right_x = location[convex_hull[2 * side_number + 1]][0];

		if (location[convex_hull[2 * side_number]][1] > location[convex_hull[2 * side_number + 1]][1]) {
			left_y = location[convex_hull[2 * side_number + 1]][1];
			right_y = location[convex_hull[2 * side_number]][1];
		}
		else {
			left_y = location[convex_hull[2 * side_number]][1];
			right_y = location[convex_hull[2 * side_number + 1]][1];
		}
	}

	if (left_x <= x && x <= right_x && left_y <= y && y <= right_y) {
		able_in_line.push_back({ side_number, x, y });
	}
}

void link_points_in_the_side(vector<vector<double>> location, vector<vector<double>> convex_hull_rest_POI, vector<int> convex_hull, int index, int count, double& total_distance)
{
	int j, l;
	int k = 0, next_index = 0, previous_index;

	cout << "(" << location[convex_hull[2 * index]][0] << ", " << location[convex_hull[2 * index]][1] << ") to ";
	previous_index = convex_hull[2 * index];

	while (k < convex_hull_rest_POI.size() && convex_hull_rest_POI[k][1] != index) {
		k++;
	}

	for (j = 0; j < count; j++) {
		for (l = k; l < convex_hull_rest_POI.size(); l++) {
			next_index = k;
			if (location[convex_hull[2 * index]][0] > location[convex_hull[2 * index + 1]][0] && convex_hull_rest_POI[l][1] == index)
			{
				if (convex_hull_rest_POI[l][2] > convex_hull_rest_POI[next_index][2]) {
					next_index = l;
				}
			}
			else if (location[convex_hull[2 * index]][0] < location[convex_hull[2 * index + 1]][0] && convex_hull_rest_POI[l][1] == index)
			{
				if (convex_hull_rest_POI[l][2] < convex_hull_rest_POI[next_index][2]) {
					next_index = l;
				}
			}
		}

		total_distance += get_distance(location[previous_index][0], location[previous_index][1], location[convex_hull_rest_POI[next_index][0]][0], location[convex_hull_rest_POI[next_index][0]][1]);
		previous_index = convex_hull_rest_POI[next_index][0];

		cout << "(" << location[convex_hull_rest_POI[next_index][0]][0] << ", " << location[convex_hull_rest_POI[next_index][0]][1] << ")\n(" << location[convex_hull_rest_POI[next_index][0]][0] << ", " << location[convex_hull_rest_POI[next_index][0]][1] << ") to ";
		convex_hull_rest_POI.erase(convex_hull_rest_POI.begin() + next_index);

		k = 0;
		while (k < convex_hull_rest_POI.size() && convex_hull_rest_POI[k][1] != index) {
			k++;
		}
	}

	total_distance += get_distance(location[previous_index][0], location[previous_index][1], location[convex_hull[2 * index + 1]][0], location[convex_hull[2 * index + 1]][1]);
	cout << "(" << location[convex_hull[2 * index + 1]][0] << ", " << location[convex_hull[2 * index + 1]][1] << ")\n";
}