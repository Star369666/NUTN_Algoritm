#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

struct node {
	string element;
	struct node* leftChild;
	struct node* rightChild;
};

vector<string> getArticle();
string get_test_word();
vector<string> get_test_file();
node* build_root(string);
node* check_exist_node(node*, string);
node* insertNode(node*, string);
node* deleteNode(node*, string);
node* searchNode(node*, string);
void inorder(node*);

int main(void) {
	node* root = NULL;
	vector<string> article = getArticle();
	vector<string> test_words;
	string test_word;
	double start, end;
	int i, choose = 4;

	start = clock();
	root = insertNode(root, article[0]);
	for (i = 1; i < article.size(); i++) {
		insertNode(root, article[i]);
	}
	cout << "\n";

	cout << "Enter a number to choose what to do(with a file) [1] Insert a word, [2] Delete a word, [3] Search a word, [4] Search part of the input file: ";
	cin >> choose;
	cout << "\n";

	switch (choose) {
	case 1:
		test_word = get_test_word();
		insertNode(root, test_word);
		break;
	case 2:
		test_word = get_test_word();
		deleteNode(root, test_word);
		break;
	case 3:
		test_word = get_test_word();
		searchNode(root, test_word);
		break;
	case 4:
		test_words = get_test_file();
		for (i = 0; i < test_words.size(); i++) {
			searchNode(root, test_words[i]);
		}
		break;
	default:
		break;
	}

	/*cout << "The final tree is shown with inorder traversal:";
	inorder(root);
	cout << ".\n";*/
	end = clock();
	cout << "The execution time is: " << end - start << "ms.\n";
	return 0;
}

vector<string> getArticle() {
	ifstream in_file;
	string file_name, line;
	vector<string> element;

	cout << "Enter the article file name in (.txt) format: ";
	cin >> file_name;
	in_file.open(file_name);

	if (!in_file.is_open()) {
		cout << "Failed to open the file.\n";
		exit(1);
	}

	while (getline(in_file, line)) {
		while (true) {
			element.push_back(line.substr(0, line.find(" ")));
			line = line.substr(line.find(" ") + 1, line.length());

			if (line.find(" ") == -1) {
				element.push_back(line);
				break;
			}
		}
	}

	return element;
}

string get_test_word() {
	ifstream in_file;
	string file_name, element;

	cout << "Enter the test file name in (.txt) format: ";
	cin >> file_name;
	in_file.open(file_name);

	if (!in_file.is_open()) {
		cout << "Failed to open the file.\n";
		exit(1);
	}

	getline(in_file, element);

	return element;
}

vector<string> get_test_file() {
	ifstream in_file;
	string file_name, line;
	vector<string> element;

	cout << "Enter the test file name in (.txt) format: ";
	cin >> file_name;
	in_file.open(file_name);

	if (!in_file.is_open()) {
		cout << "Failed to open the file.\n";
		exit(1);
	}

	while (getline(in_file, line)) {
		element.push_back(line);
	}

	return element;
}

node* build_root(string element) {
	node* root = new node;
	root->element = element;
	root->leftChild = NULL;
	root->rightChild = NULL;
	return root;
}

node* check_exist_node(node* root, string element) {
	while (root) {
		if (root->element == element) {
			return root;
		}
		else if (root->element > element) {
			root = root->leftChild;
		}
		else {
			root = root->rightChild;
		}
	}
	return NULL;
}

node* insertNode(node* root, string element) {
	if (check_exist_node(root, element)) {
		cout << "\"" << element << "\"" << " already exists\n";
		return root;
	}

	node* current = root;
	node* previous = NULL;

	while (current) {
		previous = current;
		if (element > current->element) {
			current = current->rightChild;
		}
		else if (element < current->element) {
			current = current->leftChild;
		}
	}

	if (!previous) {
		previous = build_root(element);
	}
	else {
		if (element > previous->element) {
			previous->rightChild = build_root(element);
		}
		else {
			previous->leftChild = build_root(element);
		}
	}

	cout << "Inserted " << element << " to BST.\n";
	return previous;
}



node* deleteNode(node* root, string element) {
	if (!root) {
		cout << "The BST is blank such that don't able to delete\n";
		return root;
	}

	node* current = root;
	node* previous = NULL;

	while (current && current->element != element) {
		previous = current;
		if (element > current->element) {
			current = current->rightChild;
		}
		else if (element < current->element) {
			current = current->leftChild;
		}
	}

	if (!current) {
		cout << element << " is not exist in the article\n";
		return root;
	}

	if (!current->leftChild || !current->rightChild) {
		node* replace;

		if (!current->leftChild) {
			replace = current->rightChild;
		}
		else {
			replace = current->leftChild;
		}

		if (!previous) {
			return replace;
		}

		if (current == previous->leftChild) {
			previous->leftChild = replace;
		}
		else {
			previous->rightChild = replace;
		}
		cout << "Deleted " << element << " from BST\n";
		free(current);
	}
	else {
		node* checker = NULL;
		node* successor = current->rightChild;

		while (successor->leftChild) {
			checker = successor;
			successor = successor->leftChild;
		}

		if (checker) {
			checker->leftChild = successor->rightChild;
		}
		else {
			checker->rightChild = successor->rightChild;
		}

		cout << "Deleted " << element << " from BST\n";
		current->element = successor->element;
		free(successor);
	}
	return root;
}

node* searchNode(node* root, string element) {
	while (root) {
		if (element == root->element) {
			cout << "Found " << element << " \n";
			return root;
		}
		else if (element < root->element) {
			root = root->leftChild;
		}
		else {
			root = root->rightChild;
		}
	}
	cout << "Not found " << element << " \n";
	return NULL;
}

void inorder(node* root) {
	if (root) {
		inorder(root->leftChild);
		cout << " " << root->element << " ";
		inorder(root->rightChild);
	}
	return;
}