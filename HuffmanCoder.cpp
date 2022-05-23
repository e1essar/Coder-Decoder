#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// Tree
struct Node {
	char key;
	int freq;
	Node* left, * right;
};

// node alloc
Node* GetNode(char key, int freq, Node* left, Node* right) {
	
	Node* node = new Node();

	node->key = key;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// compare to order
struct compare {
	bool operator()(Node* l, Node* r) {
		return l->freq > r->freq;
	}
};

// Huffman encode
void coder(Node* root, string str, unordered_map<char, string>& huffCode) {
	if (root == NULL) // если корень пуст
		return;

	if (!root->left && !root->right) {
		huffCode[root->key] = str;
	}

	coder(root->left, str + "0", huffCode);
	coder(root->right, str + "1", huffCode);
}

string HuffmanTree(string text)
{
	// count frequency of appearance of each character and store it in a map
	unordered_map<char, int> freq; // создаем мапу freq с ключом типа char и соответствием int 
	for (char key : text) { // в цикле пока текст не закончился считаем частоту каждого символа
		freq[key]++;
	}

	// Create a priority queue to store live nodes of Huffman tree;
	priority_queue<Node*, vector<Node*>, compare> pq;

	// Create a leaf node for each character and add it to the priority queue.
	for (auto pair : freq) {
		pq.push(GetNode(pair.first, pair.second, nullptr, nullptr));//создание узла-листа одного символа с частотой
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority (lowest frequency) from the queue
		Node* left = pq.top(); pq.pop();//вытаскиваем по очереди символы с наименьшей частотой
		Node* right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;//сумму из двух ветвей узла
		pq.push(GetNode('\0', sum, left, right));//в очередь помещаем новый узел с наследниками left right
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();//в корень помещаем оставшийся элемент в очереди

	// traverse the Huffman Tree and store Huffman Codes in a map. Also prints them
	unordered_map<char, string> huffCode;//создаем мапу char=string
	coder(root, "", huffCode);//вызываем функцию coder 

	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair : huffCode) {
		cout << pair.first << " " << pair.second << '\n';//печатаем пару символ-кодировка
	}

	cout << "\nOriginal string was :\n" << text << '\n';

	// print encoded string
	string code = "";
	for (char key : text) {
		code += huffCode[key];//составляем строку Хаффмана по тексту
	}

	cout << "\nEncoded string is :\n" << code << '\n';
	return code;

}

// Huffman coding algorithm
int main()
{
	string code;
	string text;
	fstream in;
	in.open("C:\\Users\\igor_\\OneDrive\\Desktop\\text.txt");
	getline(in, text, '\0');
	int size = 0;
	in.seekg(0, std::ios::end);
	size = in.tellg();
	in.close();

	code = HuffmanTree(text);

	fstream out;
	out.open("C:\\Users\\igor_\\OneDrive\\Desktop\\text.txt");
	out << code;
	int size1 = 0;
	out.seekg(0, std::ios::end);
	size1 = out.tellg();
	out.close();

	cout << "\n" << size << " " << size1;

	return 0;
}
