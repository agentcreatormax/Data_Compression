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
    friend class EnhancedEncoding;

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

    void buildCode(Node* node, string code, map<char, string>& huffCode) {
        if (!node) return;
        if (!node->left && !node->right) {
            huffCode[node->ch] = code;
            return;
        }
        buildCode(node->left, code + "0", huffCode);
        buildCode(node->right, code + "1", huffCode);
    }

    public:
    Encode(string s_){
        s = s_;
        // build frequnecy table
        for(int i=0;i<s.size();i++){
            freq[s[i]]++;
        }
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

// the input of frequency table are provided in a non decreasing order
// which reduces time complexity from O(ClogC) to O(C)
class EnhancedEncoding{
    private:
    map<char,int> freq;

    public:
    // input is a map of character and its frequency in sorted order
    EnhancedEncoding(map<char,int> mp){
        freq = mp;
    }
    Node* BuildTree(){
        Node* prev = nullptr;
        for (auto it = freq.begin(); it != freq.end(); it++) {
            Node* cur = new Node(it->first, it->second);
            if(!prev){
                prev = cur;
            }
            else{
                Node* temp = nullptr;
                if(cur->freq <= prev->freq){
                    temp = new Node(cur, prev);
                } 
                else{
                    temp = new Node(prev, cur);
                }
                prev = temp;
            }
        }
        return prev;
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

int main()
{
    cout << "Do you want to Encode or Decode? (E/D): \n";
    char choice; cin >> choice;
    cin.ignore();

    if (choice == 'E' || choice == 'e') {
        cout << "Do you want Enhanced Encoding? (Y/N): ";
        cout << "(If yes, then you need to provide the frequency of each character in sorted way)\n";
        char enhancedChoice; cin >> enhancedChoice;
        cin.ignore();

        if (enhancedChoice == 'Y' || enhancedChoice == 'y') {
            // Enhanced Encoding
            int n;
            cout << "Enter number of unique characters: ";
            cin >> n;
            cin.ignore();

            map<char, int> freqMap;
            cout << "Enter character and its frequency (sorted by frequency):\n";
            for (int i = 0; i < n; i++) {
                char c; int f;
                cin >> c >> f;
                cin.ignore();
                freqMap[c] = f;
            }

            EnhancedEncoding enhancedEncoder(freqMap);
            Node* root = enhancedEncoder.BuildTree();

            cout << "Huffman Tree (Enhanced Encoding): ";
            enhancedEncoder.printTree(root);
            cout << "\n";

            // Note: EnhancedEncoding class does not provide encoding function directly,
            // so encoding string must be done manually if needed.

        } else {
            // Normal Encoding
            cout << "Enter string to encode: ";
            string input;
            getline(cin, input);

            Encode encoder(input);
            encoder.printFreqTable();

            Node* root = encoder.BuildTreeFromFrequencyTable();
            cout << "Huffman Tree: ";
            encoder.printTree(root);
            cout << "\n";

            string encodedStr = encoder.EncodedString();
            cout << "Encoded string: " << encodedStr << "\n";

        }

    } else if (choice == 'D' || choice == 'd') {
        // Decoding
        int n;
        cout << "Enter number of unique characters in code map: ";
        cin >> n;
        cin.ignore();

        map<char, string> codes;
        cout << "Enter character and its code:\n";
        for (int i = 0; i < n; i++) {
            char c; string code;
            cin >> c >> code;
            cin.ignore();
            codes[c] = code;
        }

        cout << "Enter encoded string to decode: ";
        string encodedStr;
        getline(cin, encodedStr);

        Decode decoder(codes, encodedStr);
        string decodedStr = decoder.TreeDecoder();
        if(decoder.check())
        cout << "Decoded string: " << decodedStr << "\n";
        else cout << decodedStr << "\n";

    } else {
        cout << "Invalid choice! Exiting.\n";
    }

    return 0;
}
