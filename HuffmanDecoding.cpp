#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>


using namespace std;

struct Node {
    char key;
    unsigned int freq;
    Node* left;
    Node* right;
};

Node* AllocNode(char key, unsigned int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->key = key;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void decode(Node* root, int& index, string str)
{
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (!root->left && !root->right)
    {
        cout << root->key;
        return;
    }

    index++;

    if (str[index] == '0')
        decode(root->left, index, str);
    else
        decode(root->right, index, str);
}

void HuffmanDecoder(const char* input_text = "encode.txt", const char* output_text = "decoded.txt") {
    FILE* input = fopen(input_text, "rb");
    unsigned char count = 0;
    unsigned char key;
    int cart = 0;
    if (input != NULL) {
        count = fgetc(input);
    }

    map<char, int> freq;
    for (int i = 0; i < count; i++) {
        key = fgetc(input);
        freq[key] = fgetc(input);
        cart += freq[key];
    }

    cout << (int)count << '\n';
    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair : freq) {
        cout << pair.first << " " << pair.second << '\n';
    }

    priority_queue<Node*, vector<Node*>, compare> pq;

    for (auto pair : freq) {
        pq.push(AllocNode(pair.first, pair.second, nullptr, nullptr));
    }

    Node* temp;

    while (pq.size() != 1)
    {

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        if (left->freq == right->freq && left->key < right->key) {
            temp = left;
            left = right;
            right = temp;
        }

        int sum = left->freq + right->freq;
        pq.push(AllocNode('\0', sum, left, right));
    }

    Node* root = pq.top();

    FILE* output;

    fopen_s(&output, output_text, "wb +");
    char symbol; char letter;
    while (!feof(input))
    {
        symbol = fgetc(input);
        if (!feof(input))
        {
            for (int i = 7; i >= 0 && cart != 0; i--)
            {
                if ((symbol >> i) & 1)
                {
                    if (root->right)
                    {
                        root = root->right;
                    }
                    else
                    {
                        letter = root->key;
                        fputc(letter, output);
                        cart--;
                        root = pq.top()->right;
                    }
                }
                else
                {
                    if (root->left)
                    {
                        root = root->left;
                    }
                    else
                    {
                        letter = root->key;
                        fputc(letter, output);
                        cart--;
                        root = pq.top()->left;
                    }
                }
            }
        }
    }
    fclose(input);
    fclose(output);
}

void AreEqual(const char* input_text = "text.txt", const char* output_text = "umom.txt")
{
    FILE* input; FILE* output;
    fopen_s(&input, input_text, "r");
    fopen_s(&output, output_text, "r");

    char a = fgetc(input), b = fgetc(output);
    bool flag = true;

    while (!feof(input) && flag)
    {
        flag = false;
        if ((a & b) == a)
        {
            a = fgetc(input);
            b = fgetc(output);
            flag = true;
        }
    }
    if (flag) cout << "Texts are equal" << '\n';
    else cout << "Texts are not equal" << '\n';

    fclose(input);
    fclose(output);
}

int main()
{
    HuffmanDecoder();
    //AreEqual();
}