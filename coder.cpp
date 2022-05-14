#include <iostream>
#include <string>
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
void coder(Node* root, string str, unordered_map<char, string> &huffCode) {
	if (root == NULL)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->key] = str;
	}

	encode(root->left, str + "0", huffCode);
	encode(root->right, str + "1", huffCode);
}

int main()
{

}


