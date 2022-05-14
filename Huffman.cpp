#include <iostream>
#include <string>
using namespace std;

// дерево
struct Node {
	char key;
	int freq;
	Node* left, * right;
};

// выделение узла дерева
Node* GetNode(char key, int freq, Node* left, Node* right) {
	Node* node = new Node();

	node->key = key;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// порядок
struct compare {
	bool operator()(Node* l, Node* r) {
		return l->freq > r->freq;
	}
};


int main()
{

}


