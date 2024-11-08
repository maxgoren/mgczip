#include "huffman.hpp"

MGCZip::MGCZip() {
    huffmanTree = nullptr;
}

MGCZip::~MGCZip() {
    cleanup(huffmanTree);
}

void MGCZip::compress(string filename) {
    StringBuffer sbuff = readBinaryFile(filename);
    BitStream compressed = squeeze(sbuff);
    writeCompressedFile(compressed, filename);
}

void MGCZip::uncompress(string filename) {
    StringBuffer compressed = readBinaryFile(filename);
    BitStream inf;
    while (!compressed.done()) { 
        inf.writeChar(compressed.get(), 8); 
        compressed.advance(); 
    }
    string uncompressed = unsqueeze(inf);
    string new_file_name = filename.substr(0, filename.size() - 4)  + ".2";
    ofstream opf(new_file_name);
    if (opf.is_open()) {
        for (unsigned char c : uncompressed) {
            opf << c;
            cout<<c;
        }
        cout<<endl;
    }
    opf.close();       
}

void MGCZip::printEncodingTable() {
    for (auto it = encoding.iterator(); !it.done(); it.next()) {
        cout<<it.get().key()<<": "<<it.get().value()<<endl;
    }
}

void MGCZip::cleanup(link node) {
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

AVLMap<char, link> MGCZip::computeFrequencies(StringBuffer data) {
    AVLMap<char, link> freq;
    while (!data.done()) {
        char c = data.get();
        if (freq.find(c).empty()) {
            freq.insert(c, new HuffmanNode(c, 1, nullptr, nullptr));
        } else {
            freq[c]->frequency++;
        }
        data.advance();
    }
    return freq;
}

bool MGCZip::isLeaf(link h) {
    return (h->left == nullptr && h->right == nullptr);
}

void MGCZip::generateEncodingTable(link h, string prefix) {
    if (h != nullptr) {
        if (isLeaf(h)) {
            encoding.insert(h->symbol, prefix);
        } else {
            generateEncodingTable(h->left, prefix + "0");
            generateEncodingTable(h->right, prefix + "1");
        }
    }
}

void MGCZip::encodeTrie(HuffmanNode* x) {
    if (isLeaf(x)) {
        trieStream.writeBit(true);
        trieStream.writeChar(x->symbol, 8);
        return;
    }
    trieStream.writeBit(false);
    encodeTrie(x->left);
    encodeTrie(x->right);
}

HuffmanNode* MGCZip::decodeTrie() {
    if (trieStream.readBit()) {
        return new HuffmanNode(trieStream.readChar(), 0, nullptr, nullptr);
    }
    return new HuffmanNode('\0', 0, decodeTrie(), decodeTrie());
}

void MGCZip::validateHeader() {
    bool pass = true;
    string header = "leet";
    for (char c : header) {
        if (c != trieStream.readChar()) {
            pass = false;
        }
    }
    if (!pass) {
        cout<<"Invalid Header, aborting."<<endl;
        exit(0);
    }
}

void MGCZip::writeCompressedFile(BitStream compressed, string filename) {
    std::ofstream file(filename+".mgz", std::ios::out|std::ios::binary);
    compressed.start();
    for (int j = 0; j < compressed.size(); ) {
        int i = 0;
        unsigned char byte = 0;
        while (j < compressed.size() && i < 8) {
            if (compressed.readBit()) {
                byte |= (1 <<(7-i));
            }
            i++;
            j++;
        }
        file.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }
    file.close();
}

StringBuffer MGCZip::readBinaryFile(string filename) {
    StringBuffer data;
    data.readBinaryFile(filename);
    return data;
}

void MGCZip::buildHuffmanTree(StringBuffer data) {
    MinHeap<link, HuffCmp> pq;
    AVLMap<char, link> freq = computeFrequencies(data);
    for (auto it = freq.iterator(); !it.done(); it.next()) {
        pq.push(it.get().value());
        cout<<it.get().value()->symbol<<"("<<it.get().value()->frequency<<") ";
    }
    cout<<endl;
    while (pq.size() > 1) {
        link a = pq.pop();
        link b = pq.pop();
        link tmp = new HuffmanNode('&', a->frequency + b->frequency, a, b);
        pq.push(tmp);
    }
    huffmanTree = pq.pop();
}

BitStream MGCZip::squeeze(StringBuffer data) {
    string prefix, output;
    BitStream result;
    result.writeChar('l',8);
    result.writeChar('e', 8);
    result.writeChar('e', 8);
    result.writeChar('t', 8);
    buildHuffmanTree(data);
    levelorder(huffmanTree);
    generateEncodingTable(huffmanTree, prefix);
    printEncodingTable();
    cout<<"--------------"<<endl;
    encodeTrie(huffmanTree);
    trieStream.start();
    for (int i = 0; i < trieStream.size(); i++)
        result.writeBit(trieStream.readBit());
    while (!data.done()) {
        output += encoding.find(data.get()).value();
        data.advance();
    }
    result.writeInt(output.size(), 8);
    for (unsigned char c : output) {
        result.writeBit(c == '1');
    }
    return result;
}
string MGCZip::unsqueeze(BitStream encoded) {
    int i = 0;
    string result;
    link x = nullptr;
    trieStream = encoded;
    trieStream.start();
    validateHeader();
    huffmanTree = decodeTrie();
    levelorder(huffmanTree);
    int length = trieStream.readInt();
    cout<<"Length: "<<length<<endl;
    i = trieStream.offset();
    while (i < trieStream.size()) {
        if (x == nullptr) {
            x = huffmanTree;
        }
        if (isLeaf(x)) {
            result.push_back(x->symbol);
            x = nullptr;
        } else {
            if (trieStream.readBit()) {
                x = x->right;
            } else {
                x = x->left;
            }
            i++;
        }
    }
    result.push_back(x->symbol);
    return result;
}

void MGCZip::levelorder(link h) {
    queue<link> fq;
    fq.push(h);
    while (!fq.empty()) {
        int nc = fq.size();
        while (nc > 0) {
            link t = fq.front();
            fq.pop();
            nc--;
            if (t != nullptr) {
                cout<<"["<<t->symbol<<":("<<t->frequency<<")]";
                fq.push(t->left);
                fq.push(t->right);
            }
        }
        cout<<endl;
    }
}