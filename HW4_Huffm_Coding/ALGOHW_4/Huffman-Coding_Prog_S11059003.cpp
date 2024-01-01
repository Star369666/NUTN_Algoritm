#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <bitset>
#include <sys/stat.h>
using namespace std;

#define _bitset 32		//狦﹃Τび干0はτ穦硑Θsize跑

struct huffman {
	int key;
	int frequency;
	struct huffman* left;
	struct huffman* right;
};

vector<vector<int>> read_pgm(vector<vector<int>>&, vector<string>&, vector<int>&);
void table(vector<vector<int>>&, int);
vector<huffman*> min_heap(vector<vector<int>>);
void heap_insertion(vector<huffman*>&, huffman*);
huffman* heap_min_deletion(vector<huffman>&);
void swap(huffman**, huffman**);
void build_huffman_tree(vector<huffman*>&);
huffman* new_huffman_node(int, int);
huffman* joint(huffman*, huffman*);
void build_encode_table(huffman*, unordered_map<int, string>&, unordered_map<string, int>&, string, vector<string>&);
void write_encode_file(unordered_map<int, string>, vector<vector<int>>, vector<vector<int>>&);
void read_encode_file_to_pgm(unordered_map<string, int>, vector<string>, vector<int>, vector<vector<int>>&);
void print_frequency(vector<vector<int>>);
void sort_and_print_hash_table(vector<string>, unordered_map<string, int>);
void quick_sort(vector<string>&, int, int);
void print_file_size(string, string);

int main(void) {
	vector<vector<int>> key, pgm, write_len;
	vector<int> pgm_length;
	vector<string> information, hash_code;
	vector<huffman*> tree;
	unordered_map<int, string> encode;
	unordered_map<string, int> decode;
	double start, end;

	start = clock();
	pgm = read_pgm(key, information, pgm_length);
	tree = min_heap(key);
	build_huffman_tree(tree);
	build_encode_table(tree[0], encode, decode, "", hash_code);
	write_encode_file(encode, pgm, write_len);
	read_encode_file_to_pgm(decode, information, pgm_length, write_len);
	print_frequency(key);
	sort_and_print_hash_table(hash_code, decode);
	end = clock();
	cout << "execution time: " << end - start << "ms.\n";

	return 0;
}

//弄pgm, pixel value+frequency, 玡4, –︽计
vector<vector<int>> read_pgm(vector<vector<int>>& key, vector<string>& information, vector<int>& pgm_len) {
	int i;
	int row_count = 1, count = 1, element = 0;	//count: 程valueindex
	string file_name, buffer, checker;
	vector<vector<int>> pgm;

	cout << "Enter the input file name with .pgm format: ";
	cin >> file_name;
	cout << "\n";
	ifstream file(file_name);
	getchar();	//睲传︽才腹

	print_file_size(file_name, "original");

	if (!file.is_open()) {
		cout << "Failed to open the file.\n";
		exit(1);
	}

	for (i = 0; i < 4; i++) {	//セ+称爹+*糴+程η
		getline(file, buffer);
		information.push_back(buffer);
	}
	buffer.clear();

	pgm.resize(row_count);
	while (getline(file, buffer)) {
		if (!file.eof()) {		//getline穦奔传︽
			buffer += "\n";
		}
		pgm.resize(row_count);
		for (i = 0; i < buffer.size(); i++) {	//value琌传︽┪计
			if (buffer[i] == ' ') {
				element = (int)' ';
				pgm[row_count - 1].push_back(element);
			}
			else if (buffer[i] == '\n') {
				element = (int)'\n';
				pgm[row_count - 1].push_back(element);
				row_count++;
			}
			else {
				for (checker.clear(); isdigit(buffer[i]); i++) {
					checker += buffer[i];
				}
				i--;
				element = stoi(checker);
				pgm[row_count - 1].push_back(element);
			}
			table(key, element);
		}
		pgm_len.push_back(pgm[row_count - 2].size());		//Τぶelement
		buffer.clear();
	}

	file.close();
	return pgm;
}

void table(vector<vector<int>>& key, int element) {	//承pixel value+frequency
	int i;
	for (i = 0; i < key.size(); i++) {	//狡valuefrquency+1
		if (!key[i].empty() && key[i][0] == element) {
			key[i][1]++;
			return;
		}
	}
	key.push_back({ element, 1 });	//穝valuefrequency琌1
}

vector<huffman*> min_heap(vector<vector<int>> key) {
	vector<huffman*> result;
	huffman* buffer;
	int i;

	for (i = 0; i < key.size(); i++) {
		buffer = new_huffman_node(key[i][0], key[i][1]);
		heap_insertion(result, buffer);
	}

	return result;
}

void heap_insertion(vector<huffman*>& heap, huffman* buffer) {
	int current = heap.size();

	heap.push_back(buffer);				//insert程Ю
	while (heap[current]->frequency < heap[(current - 1) / 2]->frequency) {	//浪琩Τ⊿Τゑ
		swap(&heap[current], &heap[current / 2]);
		current = (current - 1) / 2;		//穝index
	}
}

huffman* heap_min_deletion(vector<huffman*>& heap) {
	int current = 0;
	int last = heap.size() - 1;
	huffman* result = heap[0];

	swap(&heap[0], &heap[last]);			//root蛤程Юユ传
	heap.erase(heap.begin() + last);		//程Ю(程root)埃

	while ((2 * current + 1) < heap.size()) {		//Τオ娩传⊿オ娩﹚⊿娩
		if ((2 * current + 2) < heap.size() && heap[2 * current + 2]->frequency < heap[2 * current + 1]->frequency && heap[2 * current + 2]->frequency < heap[current]->frequency) {	 // +  < オ +  < 
			swap(&heap[current], &heap[2 * current + 2]);
			current = 2 * current + 2;
		}
		else if (heap[current]->frequency > heap[2 * current + 1]->frequency) {	//オ < 
			swap(&heap[current], &heap[2 * current + 1]);
			current = 2 * current + 1;
		}
		else if (heap[current]->frequency <= heap[2 * current + 1]->frequency) {	//オ常礚猭swap
			break;
		}
	}

	return result;
}

void swap(huffman** a, huffman** b) {
	huffman* temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void build_huffman_tree(vector<huffman*>& heap) {
	huffman* a, * b, * buffer;

	while (heap.size() > 1) {
		a = heap_min_deletion(heap);
		b = heap_min_deletion(heap);
		buffer = joint(a, b);
		heap_insertion(heap, buffer);
	}
}

huffman* new_huffman_node(int key, int frequency) {
	huffman* result = new huffman();
	result->key = key;
	result->frequency = frequency;
	result->left = NULL;
	result->right = NULL;
	return result;
}

huffman* joint(huffman* a, huffman* b) {
	huffman* root = new huffman;
	root->key = -1;		//ぃ琌腑(external)竊翴安砞⊿Τη琌-1pixel
	root->frequency = a->frequency + b->frequency;
	root->left = a;
	root->right = b;
	return root;
}

void build_encode_table(huffman* tree, unordered_map<int, string>& encode, unordered_map<string, int>& decode, string code, vector<string>& codes) {
	if (tree) {	//Inorder砐
		build_encode_table(tree->left, encode, decode, (code + "0"), codes);
		if (tree->key != -1) {
			encode[tree->key] = code;
			decode[code] = tree->key;
			codes.push_back(code);
		}
		build_encode_table(tree->right, encode, decode, (code + "1"), codes);
	}
}

//溃罽瓜
void write_encode_file(unordered_map<int, string> hash, vector<vector<int>> pgm, vector<vector<int>>& write_len) {		
	ofstream output_file("test.hc", ios::binary);
	string helper, sub;
	int i, j, fill;

	for (i = 0; i < pgm.size(); i++) {
		helper.clear();
		for (j = 0; j < pgm[i].size(); j++) {
			helper += hash[pgm[i][j]];
		}

		write_len.resize(i + 1);
		write_len[i].push_back(helper.size());
		fill = helper.size() % _bitset;		//狦ぃ琌计bit程Ю干0
		if (fill != 0) {
			fill = _bitset - fill;
			for (j = 0; j < fill; j++) {
				helper += "0";
			}
		}
		write_len[i].push_back(helper.size());
		
		for (j = 0; j < helper.size(); j += _bitset) {
			sub = helper.substr(j, _bitset);
			bitset<_bitset> binary(sub.c_str());
			output_file.write(reinterpret_cast<const char*>(&binary), sizeof(binary));
		}
	}

	print_file_size("test.hc", "compressed");
	cout << "\n";
	output_file.close();
}

//秆溃罽瓜
void read_encode_file_to_pgm(unordered_map<string, int> hash, vector<string> information, vector<int> pgm_len, vector<vector<int>>& write_len) {
	int i, helper;	
	int row_count = 0, column_count = 0;
	char previous;	//previous: 魁琌计┪传︽
	string all, all_helper, buffer;
	ifstream input_file("test.hc", ios::binary);
	ofstream output_file("testd.pgm");
	bitset<_bitset> binary;

	while (input_file.read(reinterpret_cast<char*>(&binary), sizeof(binary))) {	//Ω弄bit_sizebit
		all_helper.clear();
		all_helper += binary.to_string();
		for (i = 1; i < write_len[row_count][1] / _bitset; i++) {
			input_file.read(reinterpret_cast<char*>(&binary), sizeof(binary));
			all_helper += binary.to_string();
		}

		if (write_len[row_count][1] != write_len[row_count][0]) {		//耞程Ю狠琌Τ干0
			all += all_helper.substr(0, write_len[row_count][0]);
		}
		else {
			all += all_helper;
		}
		row_count++;
	}

	for (i = 0; i < information.size(); i++) {	//糶セ+称爹+*糴+程η
		output_file << information[i];
		output_file << "\n";
	}

	row_count = 0;
	for(i = 0; i < all.size(); i++) {
		buffer.clear();
		for (buffer += all[i]; hash.find(buffer) == hash.end(); buffer += all[i]) {
			i++;
		}
		helper = hash[buffer];

		if (helper == 10 && previous == ' ' && pgm_len[row_count] == column_count + 1) {		//传︽耞传︽瞷–程Ю
			output_file << char(helper);
			previous = char(helper);
			column_count = 0;
			row_count++;
			continue;
		}
		else if (helper == 32 && isdigit(previous)) {		//耞瞷计
			output_file << char(helper);
			previous = char(helper);
		}
		else {	//计耞
			output_file << helper;
			previous = '0';
		}
		column_count++;
	}
}

void print_frequency(vector<vector<int>> key) {
	int i;
	cout << "Keys with their frequencies shown:\n";
	for (i = 0; i < key.size(); i++) {
		cout << setiosflags(ios::left) << setw(5) << key[i][0] << " appeared " << setw(6) << key[i][1] << " times." << "\n";
	}
	cout << "\n\n";
}

void sort_and_print_hash_table(vector<string> code, unordered_map<string, int> hash) {
	int i;
	quick_sort(code, 0, code.size() - 1);
	cout << "Encoded codes with pixel values shown:\n";
	for (i = 0; i < code.size(); i++) {
		cout << setw(15) << code[i] << " is " << setw(5) << hash[code[i]] << "\n";
	}
	cout << "\n\n";
}

void quick_sort(vector<string>& code, int left, int right) {
	if (left >= right) {
		return;
	}

	int i = left;
	int j = right;
	string key = code[left];

	while (i != j) {
		while (i < j && code[j] > key) {
			j -= 1;
		}
		while (i < j && code[i] < key) {
			i++;
		}
		if (i < j) {
			string temp;
			temp = code[i];
			code[i] = code[j];
			code[j] = temp;
		}
	}

	code[left] = code[i];
	code[i] = key;

	quick_sort(code, i + 1, right);
	quick_sort(code, left, i - 1);
}

void print_file_size(string file_name, string status) {
	struct stat stat_buffer;
	int size = stat(file_name.c_str(), &stat_buffer);	//ъ0琌Θ
	if (size == 0) {
		cout << "The " << status << " file size is " << stat_buffer.st_size << " bytes\n";
	}
	else {
		cout << "Can't not get the file size\n";
	}
}