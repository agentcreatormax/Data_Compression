// This project is to Encode and decode the strings using Huffmann process
// for tie breakers of same frequency nodes we place the node whose root formed first to the left of the tree

#include <bits/stdc++.h>
using namespace std ;

class Node {
    protected:
    char ch;
    int freq;
    Node* left;
    Node* right;

    friend class Encode;
    friend class Decode;

    public:
    Node(){
        ch = '\0';
        freq = 0;
        left = nullptr;
        right = nullptr;
    }

    Node(char c, int f){
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    Node(Node* l, Node* r){
        left = l;
        right = r;
        ch = '\0';
        freq = l->freq+r->freq;
    }

    int getFreq() const{
        return freq;
    }
};

class comp{
    public:
    bool operator()(const Node* a, const Node* b){
        return a->getFreq() > b->getFreq();
    }
};

// given a string as a input , returns a code for each character and combined code
class Encode{
    private:
    map<char,int> freq;
    string s;

    public:
    Encode(string s_){
        s = s_;
        // build frequnecy table
        for(int i=0;i<s.size();i++){
            freq[s[i]]++;
        }
    }

    
    void buildCode(Node* node, string code, map<char, string>& huffCode) {
        if (!node) return;
        if (!node->left && !node->right) {
            huffCode[node->ch] = code;
            return;
        }
        buildCode(node->left, code + "0", huffCode);
        buildCode(node->right, code + "1", huffCode);
    }

    Node* BuildTreeFromFrequencyTable(){
        priority_queue<Node*, vector<Node*>, comp> pq;
        for(auto it=freq.begin();it!=freq.end();it++){
            pq.push(new Node(it->first, it->second));
        }
        while (pq.size() > 1) {
            Node* a = pq.top(); pq.pop();
            Node* b = pq.top(); pq.pop();
            Node* left, *right;
            if(a->freq<=b->freq){
                left = a;
                right = b;
            }
            else{
                left = b;
                right = a;
            }
            pq.push(new Node(left, right));
        }
        return pq.top();
    }

    string EncodedString(){
        Node* root = BuildTreeFromFrequencyTable();
        map<char, string> huffCode;
        buildCode(root, "", huffCode);

        string encoded;
        for (char c : s) {
            encoded += huffCode[c];
        }

        return encoded;
    }

    void printFreqTable(){
        for(auto it=freq.begin();it!=freq.end();it++){
            cout<<it->first<<" "<<it->second<<endl;
        }
        cout<<endl;
    }

    void printTree(Node* node){
        if(node==nullptr) return;
        if (!node->left && !node->right) {
            cout << node->ch;
            return;
        }

        cout << "(";
        printTree(node->left);
        cout << ",";
        printTree(node->right);
        cout << ")";
    }
};

class Decode{
    private:
    map<char,string> codes;
    string encoded;
    bool isEncodedStringValid = true;

    Node* BuildTreeFromCode(){
        Node* root = new Node();
        for(auto it=codes.begin(); it!=codes.end(); it++){
            string code = it->second;
            Node* cur = root;
            for(int i=0;i<code.size();i++){
                if(code[i]=='0'){
                    if(!cur->left) cur->left = new Node();
                    cur = cur->left;
                }
                else{
                    if(!cur->right) cur->right = new Node();
                    cur=cur->right;
                }
            }
            cur->ch = it->first;
        }
        return root;
    }

    public:
    Decode(map<char,string> mp, string Encoded_){
        codes = mp;
        encoded = Encoded_;
    }

    bool check(){
        return isEncodedStringValid;
    }
    
    string TreeDecoder(){
        Node* root = BuildTreeFromCode();
        string result;
        Node* curr = root;

        for(char bit : encoded){
            if(bit == '0'){
                if(!curr->left){
                    isEncodedStringValid = false;
                    return "Invalid encoded string";
                } 
                curr = curr->left;
            }
            else if(bit == '1'){
                if(!curr->right){
                    isEncodedStringValid = false;
                    return "Invalid encoded string";
                } 
                curr = curr->right;
            }
            else{
                isEncodedStringValid = false;
                return "Invalid encoded string";
            }
            // traverse untill the leaf is found
            if(!curr->left && !curr->right){
                result.push_back(curr->ch);
                curr = root; 
            }
        }
        if(curr!=root){
            isEncodedStringValid = false;
            return "Invalid encoded string";
        } 
        return result;
    }
};

int main() {
    int option;
    cout << "You want to compress or decompress?\n";
    cout << "1 -- compress\n";
    cout << "2 -- decompress\n";
    cout << "Enter your option: ";
    cin >> option;
    cin.ignore();

    if (option == 1) {
        string file1, file2;
        cout << "Enter the name of the file you want to compress: ";
        cin >> file1;
        cout << "Enter the name of the file you want to save the compressed data: ";
        cin >> file2;
        cin.ignore();

        ifstream input(file1);
        if (!input.is_open()) {
            cout << "Failed to open input file.\n";
            return 1;
        }

        string s, line;
        while (getline(input, line)) {
            s += line + '\n';
        }
        input.close();

        if (s.empty()) {
            cout << "Input file is empty.\n";
            return 1;
        }

        if (!s.empty() && s.back() == '\n') s.pop_back(); // Remove trailing newline

        Encode encoder(s);
        encoder.printFreqTable();  // Print frequency table

        Node* root = encoder.BuildTreeFromFrequencyTable();
        cout << "Huffman Tree: ";
        encoder.printTree(root);  // Print tree
        cout << '\n';

        map<char, string> huffCode;
        encoder.buildCode(root, "", huffCode);

        string encodedStr = encoder.EncodedString();

        ofstream output(file2);
        if (!output.is_open()) {
            cout << "Failed to open output file.\n";
            return 1;
        }

        output << huffCode.size() << '\n';
        for (auto& p : huffCode) {
            output << p.first << ' ' << p.second << '\n';
        }

        output << encodedStr << '\n';
        output.close();

        cout << "Compression completed. Data saved to " << file2 << '\n';
    }


    else if (option == 2) {
        string file, outputFile;
        cout << "Enter the name of the compressed file: ";
        cin >> file;
        cout << "Enter the name of the file you want to save the decompressed data: ";
        cin >> outputFile;
        cin.ignore();

        ifstream input(file);
        if (!input.is_open()) {
            cout << "Failed to open compressed file.\n";
            return 1;
        }

        int n;
        input >> n;
        input.ignore();

        map<char, string> codeMap;
        for (int i = 0; i < n; ++i) {
            char c;
            string code;
            input >> c >> code;
            input.ignore();
            codeMap[c] = code;
        }

        string encodedStr;
        getline(input, encodedStr);
        input.close();

        if (encodedStr.empty()) {
            cout << "Input file is empty.\n";
            return 1;
        }

        Decode decoder(codeMap, encodedStr);
        string decodedStr = decoder.TreeDecoder();

        if (!decoder.check()) {
            cout << "Decoding failed: " << decodedStr << '\n';
            return 1;
        }

        ofstream out(outputFile);
        if (!out.is_open()) {
            cout << "Failed to open output file.\n";
            return 1;
        }

        out << decodedStr << '\n';
        out.close();

        cout << "Decompression completed. Decoded data saved to " << outputFile << '\n';
    }


    else {
        cout << "Invalid option\n";
    }

    return 0;
}
