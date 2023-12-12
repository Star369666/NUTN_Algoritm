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
	struct node* parent;
};

class splay_tree {
private:
	node* root;
public:
	splay_tree() {}
	void insertNode(string);
	void deleteNode(string);
	void searchNode(string);
	void inorder();
	void inorder_helper(node*);
	void left_rotation(node*);
	void right_rotation(node*);
	void splay(node*);
};

vector<string> getArticle();
string get_test_word();
vector<string> get_test_file();


int main(void) {
	splay_tree splayT;
	node* root = NULL;
	vector<string> article = getArticle();
	vector<string> test_words;
	string test_word;
	double start, end;
	int i, choose = 4;

	start = clock();
	for (i = 0; i < article.size(); i++) {
		splayT.insertNode(article[i]);
	}
	cout << "\n";

	cout << "Enter a number to choose what to do(with a file) [1] Insert a word, [2] Delete a word, [3] Search a word, [4] Search part of the input file: ";
	cin >> choose;
	cout << "\n";

	switch (choose) {
	case 1:
		test_word = get_test_word();
		splayT.insertNode(test_word);
		break;
	case 2:
		test_word = get_test_word();
		splayT.deleteNode(test_word);
		break;
	case 3:
		test_word = get_test_word();
		splayT.searchNode(test_word);
		break;
	case 4:
		test_words = get_test_file();
		for (i = 0; i < test_words.size(); i++) {
			splayT.searchNode(test_words[i]);
		}
		break;
	default:
		break;
	}

	/*cout << "The final tree is shown with inorder traversal:";
	splayT.inorder();
	cout << "\n";*/
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

void splay_tree::insertNode(string element) {
	node* insert = new node;
	insert->parent = NULL;
	insert->leftChild = NULL;
	insert->rightChild = NULL;
	insert->element = element;

	node* current = root;
	node* previous = NULL;

	while (current) {
		previous = current;
		if (insert->element == current->element) {
			cout << element << " already exists\n";
			return;
		}
		else if (insert->element < current->element) {
			current = current->leftChild;
		}
		else {
			current = current->rightChild;
		}
	}

	insert->parent = previous;
	if (!previous) {
		root = insert;
	}
	else if (insert->element < previous->element) {
		previous->leftChild = insert;
	}
	else {
		previous->rightChild = insert;
	}

	cout << element << " is inserted\n";
	splay(insert);
}

void splay_tree::deleteNode(string element) {
	node* first = root;
	node* second = NULL;
	node* third = NULL;

	while (root) {
		if (root->element <= element) {
			first = first->rightChild;
		}
		else {
			first = first->leftChild;
		}
	}

	if (!first) {
		cout << element << "not found to delete\n";
	}

	splay(first);	//split the tree
	if (first->rightChild) {
		third = first->rightChild;
		third->parent = NULL;
	}
	else {
		third->parent = NULL;
	}

	second = first;
	second->rightChild = NULL;
	first = NULL;

	if (third->leftChild) {	//remove element
		cout << element << " is deleted\n";
		third->leftChild->parent = NULL;
	}

	if (!third->leftChild) {	//join the tree
		root = second;
	}
	else if (!second) {
		root = third->leftChild;
	}

	node* predecessor = third->leftChild;
	while (!predecessor->rightChild) {
		predecessor = predecessor->rightChild;
	}

	splay(predecessor);
	first->rightChild = third;
	third->parent = first;

	root = predecessor;
	delete(second);
	second = NULL;
}

void splay_tree::searchNode(string element) {
	node* current = root;
	while (current) {
		if (element == current->element) {
			cout << "Found " << element << " \n";
			splay(current);
			return;
		}
		else if (element < current->element) {
			current = current->leftChild;
		}
		else {
			current = current->rightChild;
		}
	}
	cout << "Not found " << element << " \n";
}

void splay_tree::inorder() {
	inorder_helper(root);
}

void splay_tree::inorder_helper(node* root) {
	if (root) {
		inorder_helper(root->leftChild);
		cout << " " << root->element << " ";
		inorder_helper(root->rightChild);
	}
	return;
}

void splay_tree::left_rotation(node* x) {
	node* y = x->rightChild;
	x->rightChild = y->leftChild;
	if (y->leftChild) {
		y->leftChild->parent = x;
	}
	y->parent = x->parent;

	if (!x->parent) {
		root = y;
	}
	else if (x == x->parent->leftChild) {
		x->parent->leftChild = y;
	}
	else {
		x->parent->rightChild = y;
	}
	y->leftChild = x;
	x->parent = y;
}

void splay_tree::right_rotation(node* x) {
	node* y = x->leftChild;
	x->leftChild = y->rightChild;
	if (y->rightChild) {
		y->rightChild->parent = x;
	}
	y->parent = x->parent;

	if (!x->parent) {
		root = y;
	}
	else if (x == x->parent->rightChild) {
		x->parent->rightChild = y;
	}
	else {
		x->parent->leftChild = y;
	}
	y->rightChild = x;
	x->parent = y;
}

void splay_tree::splay(node* x) {
	while (x->parent) {
		if (!x->parent->parent) {	//only two node
			if (x == x->parent->leftChild) {	//zig
				right_rotation(x->parent);
			}
			else {	//zag
				left_rotation(x->parent);
			}
		}
		else if (x == x->parent->leftChild && x->parent == x->parent->parent->leftChild) {
			// zig-zig
			right_rotation(x->parent->parent);
			right_rotation(x->parent);
		}
		else if (x == x->parent->rightChild && x->parent == x->parent->parent->rightChild) {
			// zag-zag
			left_rotation(x->parent->parent);
			left_rotation(x->parent);
		}
		else if (x == x->parent->rightChild && x->parent == x->parent->parent->leftChild) {
			// zig-zag
			left_rotation(x->parent);
			right_rotation(x->parent);
		}
		else {
			// zag-zig
			right_rotation(x->parent);
			left_rotation(x->parent);
		}
	}
}