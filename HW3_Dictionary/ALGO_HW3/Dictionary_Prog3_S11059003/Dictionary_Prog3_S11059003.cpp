#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
using namespace std;

#define MAX_RANDOM 999999

struct node {
	string element;
	int priority;
	struct node* leftChild;
	struct node* rightChild;
	node(string element, int rand) : element(element), leftChild(NULL), rightChild(NULL), priority(rand) {}
};

class treap {
private:
	node* root;

	void left_rotation(node*&);
	void right_rotation(node*&);
	void insert(node*&, string);
	void remove(node*&, string);
public:
	treap() {}
	void insertNode(string);
	void deleteNode(string);
	void searchNode(string);
	void inorder();
	void inorder_helper(node*);
};

vector<string> getArticle();
string get_test_word();
vector<string> get_test_file();

int main(void) {
	treap Treap;
	vector<string> article = getArticle();
	vector<string> test_words;
	string test_word;
	double start, end;
	int i, choose = 4;

	start = clock();
	for (i = 0; i < article.size(); i++) {
		Treap.insertNode(article[i]);
	}
	cout << "\n";

	cout << "Enter a number to choose what to do(with a file) [1] Insert a word, [2] Delete a word, [3] Search a word, [4] Search part of the input file: ";
	cin >> choose;
	cout << "\n";

	switch (choose) {
	case 1:
		test_word = get_test_word();
		Treap.insertNode(test_word);
		break;
	case 2:
		test_word = get_test_word();
		Treap.deleteNode(test_word);
		break;
	case 3:
		test_word = get_test_word();
		Treap.searchNode(test_word);
		break;
	case 4:
		test_words = get_test_file();
		for (i = 0; i < test_words.size(); i++) {
			Treap.searchNode(test_words[i]);
		}
		break;
	default:
		break;
	}

	/*cout << "The final tree is shown with inorder traversal:";
	Treap.inorder();
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

void treap::insertNode(string element) {
	insert(root, element);
}

void treap::deleteNode(string element) {
	remove(root, element);
}

void treap::searchNode(string element) {
	node* current = root;
	while (current) {
		if (element == current->element) {
			cout << "Found " << element << " \n";
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

void treap::inorder() {
	inorder_helper(root);
}

void treap::inorder_helper(node* root) {
	if (root) {
		inorder_helper(root->leftChild);
		cout << " " << root->element << " ";
		inorder_helper(root->rightChild);
	}
	return;
}

void treap::left_rotation(node*& root) {
	node* x = root->rightChild;
	root->rightChild = x->leftChild;
	x->leftChild = root;
	root = x;
}

void treap::right_rotation(node*& root) {
	node* x = root->leftChild;
	root->leftChild = x->rightChild;
	x->rightChild = root;
	root = x;
}

void treap::insert(node*& root, string element) {
	random_device rd;
	uniform_int_distribution<int> dist(1, MAX_RANDOM);

	if (!root) {
		root = new node(element, dist(rd));
	}
	else {
		if (element == root->element) {
			cout << element << " already exists\n";
			return;
		}
		else if (element > root->element) {
			insert(root->rightChild, element);
		}
		else {
			insert(root->leftChild, element);
		}

		if (root->leftChild && root->leftChild->priority > root->priority) {
			right_rotation(root);
		}
		else if (root->rightChild && root->rightChild->priority < root->priority) {
			left_rotation(root);
		}
	}
}

void treap::remove(node*& root, string element) {
	if (root->element == element) {
		cout << element << " is deleted\n";
		if (!root->leftChild) {
			root = root->rightChild;
		}
		else if (!root->rightChild) {
			root = root->leftChild;
		}
		else {
			if (root->leftChild->priority > root->rightChild->priority) {
				right_rotation(root);
				remove(root->rightChild, element);
			}
			else if (root->leftChild->priority < root->rightChild->priority) {
				left_rotation(root);
				remove(root->leftChild, element);
			}
		}
	}
	else {
		if (element < root->element) {
			remove(root->leftChild, element);
		}
		else if (element > root->element) {
			remove(root->rightChild, element);
		}
	}
}