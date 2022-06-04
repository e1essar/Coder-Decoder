#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>
using namespace std;

struct Node {   // struct Tree
    char key;
    unsigned int freq;
    Node* left;
    Node* right;
};

Node* AllocNode(char key, unsigned int freq, Node* left, Node* right) { // Creating new Node
    Node* node = new Node();
    node->key = key;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
};

struct Compare {    // Comparing leafs
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// main function
void HuffmanDecoder(const char* input_text = "encode.txt", const char* output_text = "decoded.txt") {
    FILE* input;
    fopen_s(&input, input_text, "rb"); // opening input text
    
    unsigned char key;
    char unic_count = 0;
    int count = 0;
    map<char, int> freq;

    if (input != NULL) unic_count = fgetc(input); // getting count of unic letters

    for (int i = 0; i < unic_count; i++) { // getting alphabet letter-frequency and counting size of the text
        key = fgetc(input);
        fread(&freq[key], sizeof(int), 1, input);
        count += freq[key];
    }

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair : freq) {
        cout << pair.first << " " << pair.second << '\n';
    }

    priority_queue<Node*, vector<Node*>, Compare> pq;   // queue with priority >

    for (auto pair : freq) {
        pq.push(AllocNode(pair.first, pair.second, nullptr, nullptr));  // push leafs into the queue
    }

    Node* temp;

    while (pq.size() != 1) {    // creating the HuffmanTree

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        if (left->freq == right->freq && left->key < right->key) {  // sorting
            temp = left;
            left = right;
            right = temp;
        }

        int sum = left->freq + right->freq;
        pq.push(AllocNode('\0', sum, left, right));     // node with low freq leafs
    }

    Node* root = pq.top();

    FILE* output;
    fopen_s(&output, output_text, "wb +");  // opening output file
    char symbol;    // symbol we get from encoded text

    while (!feof(input)) {
        symbol = fgetc(input);  // reading symbol from encoded text
        if (!feof(input)) {
            for (int i = 7; i >= 0 && count != 0; i--) {
                if ((symbol >> i) & 1) {        // if we get "1" in bincode
                    if (root->right) {          // if it is not leaf
                        root = root->right;
                    }
                    else {                      // if it is leaf
                        key = root->key;        // get the letter
                        fputc(key, output);     // put this letter into output
                        count--;
                        root = pq.top()->right; // returning the root
                    }
                }
                else {                          // if we get "0" in bincode
                    if (root->left) {           // if it is not leaf
                        root = root->left;
                    }
                    else {                      // if it is leaf
                        key = root->key;        // get the letter
                        fputc(key, output);     // put this letter into output
                        count--;
                        root = pq.top()->left;  // returning the root
                    }
                }
            }
        }
    }

    fclose(input);
    fclose(output);
}

// function to check input text and decoded one
void Equality(const char* input_text = "text.txt", const char* output_text = "decoded.txt") {
    FILE* input; FILE* output;
    fopen_s(&input, input_text, "r");
    fopen_s(&output, output_text, "r");

    char letter_text, letter_decode;
    bool flag = true;

    while (!feof(input) && flag) {
        flag = false;
        letter_text = fgetc(input);
        letter_decode = fgetc(output);
        if (letter_text == letter_decode) flag = true;
        else break;
    }

    if (flag) cout << '\n' << "Texts are equal" << '\n';
    else cout << '\n' <<  "Texts are not equal" << '\n';

    fclose(input);
    fclose(output);
}

int main() {
    HuffmanDecoder();
    Equality();
}