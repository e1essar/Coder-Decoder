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
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

void Code(Node* root, string bincode, unordered_map<char, string>& HuffCode) {
    if (root == NULL) return;

    if (!root->left && !root->right) {
        HuffCode[root->key] = bincode;
    }

    Code(root->left, bincode + "0", HuffCode);
    Code(root->right, bincode + "1", HuffCode);
}

void HuffmanCoder(const char* input_text = "text.txt", const char* output_text = "encode.txt") {
    FILE* input;
    fopen_s(&input, input_text, "r");
    if (!input) exit(1);
    unsigned char key;
    map<char, int> freq;
    while (!feof(input)) {
        key = fgetc(input);
        if (!feof(input)) {
            freq[key]++;
        }
    }

    char count_letters = freq.size();

    fclose(input);

    FILE* output;
    fopen_s(&output, output_text, "wb +");
    fopen_s(&input, input_text, "rb");
    if (!input || !output) exit(1);

    fputc(count_letters, output);
    for (auto pair : freq) {
        fputc(pair.first, output);
        fputc(pair.second, output);
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

    unordered_map<char, string> HuffCode;
    Code(root, "", HuffCode);

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair : HuffCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    char fkey, symbol = 0;
    int countOFbits = 0;

    while (!feof(input))
    {
        fkey = fgetc(input);
        if (!feof(input))
        {
            if (countOFbits + HuffCode[fkey].length() <= 8)
            {
                for (int i = 0; i < HuffCode[fkey].length(); i++)
                {
                    symbol = (symbol << 1) | (HuffCode[fkey][i] - '0');
                }
                countOFbits += HuffCode[fkey].length();
            }
            else
            {
                if (HuffCode[fkey].length() - countOFbits + 8 <= 8)
                {
                    for (int j = 0; j < 8 - countOFbits; j++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }
                    fputc(symbol, output);
                    symbol = 0;
                    for (int j = 8 - countOFbits; j < HuffCode[fkey].length(); j++) // 1011110(1|00000011)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }
                    countOFbits = HuffCode[fkey].length() - 8 + countOFbits;
                }
                else
                {
                    for (int j = 0; j < 8 - countOFbits; j++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }

                    fputc(symbol, output);
                    int i = 8 - countOFbits;
                    countOFbits = 0;
                    symbol = 0;

                    for (i; i < HuffCode[fkey].length(); i++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][i] - '0');
                        countOFbits++;
                        if (countOFbits == 8)
                        {
                            fputc(symbol, output);
                            countOFbits = 0;
                            symbol = 0;
                        }
                    }
                }
            }
        }
        else
        {
            symbol = symbol << (8 - countOFbits);
            fputc(symbol, output);
        }
    }

    fclose(input);
    fclose(output);
}

void compressValue(const char* input_text = "text.txt", const char* output_text = "encode.txt")
{
    long long file_size = 0;
    long long compress_size = 0;
    //FILE* input; FILE* output;
    //fopen_s(&input, input_text, "r");
    //fopen_s(&output, output_text, "r");

    struct stat sb {};
    struct stat se {};
    // Finding compression ratio
    if (!stat(input_text, &sb)) {
        file_size = sb.st_size;
    }
    else {
        perror("stat");
    }
    if (!stat(output_text, &se)) {
        compress_size = se.st_size;
    }
    else {
        perror("stat");
    }
    cout << "\nCompress value is: " << (compress_size + 0.0) / file_size << "\n";
    return;
}

int main() {
    HuffmanCoder();
    //compressValue();
}