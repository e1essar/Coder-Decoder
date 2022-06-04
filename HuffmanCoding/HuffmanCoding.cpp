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

void Code(Node* root, string bincode, unordered_map<char, string>& HuffCode) { // Creating binary code of symbol by the HuffmanTree
    if (root == NULL) return;
    if (!root->left && !root->right) {
        HuffCode[root->key] = bincode;
    }
    Code(root->left, bincode + "0", HuffCode);
    Code(root->right, bincode + "1", HuffCode);
}

// main function 
void HuffmanCoding(const char* input_text = "text.txt", const char* output_text = "encode.txt") {
    FILE* input;
    fopen_s(&input, input_text, "r"); // opening input text

    unsigned char key;
    map<char, int> freq;    // creating map letter-frequency
    
    while (!feof(input)) {  // counting freqs of letters in the text
        key = fgetc(input);
        if (!feof(input)) {
            freq[key]++;
        }
    }

    char unic_count = freq.size();  // count of unic letters

    fclose(input);

    FILE* output;
    fopen_s(&output, output_text, "wb +");
    fopen_s(&input, input_text, "rb");  // opening input and output files

    fputc(unic_count, output);  // putting unic_count into the head of output
    for (auto pair : freq) {
        fputc(pair.first, output);
        fwrite(&pair.second, sizeof(int), 1, output);   // putting letter-freq into the head of output
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

    unordered_map<char, string> HuffCode;   // creating Huffman codes by the Tree
    Code(root, "", HuffCode);

    cout << "Huffman Codes:\n" << '\n';
    for (auto pair : HuffCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    char symbol;    // symbol to put into the output
    int bits = 0;   // count of bits for unfulling the byte

    while (!feof(input)) {
        key = fgetc(input);     // reading letter from input file
        if (!feof(input)) {
            if (bits + HuffCode[key].length() <= 8) {   // if bincode of letter fits in the byte
                for (int i = 0; i < HuffCode[key].length(); i++) {
                    symbol = (symbol << 1) | (HuffCode[key][i] - '0');
                }
                bits += HuffCode[key].length();
            }
            else {  // if does not fit
                if (HuffCode[key].length() - bits + 8 <= 8) {   // if fits in the next byte
                    for (int j = 0; j < 8 - bits; j++) {        // put what fits
                        symbol = (symbol << 1) | (HuffCode[key][j] - '0');
                    }

                    fputc(symbol, output);
                    symbol = 0;

                    for (int j = 8 - bits; j < HuffCode[key].length(); j++) { // put what remains
                        symbol = (symbol << 1) | (HuffCode[key][j] - '0');
                    }
                    bits = HuffCode[key].length() - 8 + bits;
                }
                else {  // if does not fit in the next byte
                    for (int j = 0; j < 8 - bits; j++) {        // put what fits
                        symbol = (symbol << 1) | (HuffCode[key][j] - '0');
                    }

                    fputc(symbol, output);
                    int i = 8 - bits;
                    bits = 0;
                    symbol = 0;

                    for (i; i < HuffCode[key].length(); i++) {  // put what remains
                        symbol = (symbol << 1) | (HuffCode[key][i] - '0');
                        bits++;
                        if (bits == 8) {
                            fputc(symbol, output);
                            bits = 0;
                            symbol = 0;
                        }
                    }
                }
            }
        }
        else {  // put the last symbol
            symbol = symbol << (8 - bits);
            fputc(symbol, output);
        }
    }

    fclose(input);
    fclose(output);
}

// function to count the CompressValue
void CompressValue(const char* input_text = "text.txt", const char* output_text = "encode.txt") {
    long long file = 0;
    long long compressed_file = 0;
    struct stat f;

    if (!stat(input_text, &f))
        file = f.st_size;
    else perror("stat");
    
    if (!stat(output_text, &f))
        compressed_file = f.st_size; 
    else perror("stat");
    
    cout << "\nCompress value is: " << (compressed_file + 0.0) / file << "\n";
}

int main() {
    HuffmanCoding();
    CompressValue();
}