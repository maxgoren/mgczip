#ifndef decoder_hpp
#define decoder_hpp
#include "avlmap.hpp"
#include "bitstream.hpp"
#include "huffnode.hpp"
#include "stringbuffer.hpp"

class Decoder {
    private:
        AVLMap<char, string> encoding;
        link huffmanTree;
        BitStream trieStream;
        bool isLeaf(link h);
        HuffmanNode* decodeTrie();
        void validateHeader();
        StringBuffer readBinaryFile(string filename);
        string unsqueeze(BitStream encoded);
        void cleanup(link node);
    public:
        Decoder();
        ~Decoder();
        void uncompress(string filename);

};

Decoder::Decoder() {

}

Decoder::~Decoder() {
    cleanup(huffmanTree);
}

void Decoder::uncompress(string filename) {
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

void Decoder::cleanup(link node) {
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

bool Decoder::isLeaf(link h) {
    return (h->left == nullptr && h->right == nullptr);
}

HuffmanNode* Decoder::decodeTrie() {
    if (trieStream.readBit()) {
        return new HuffmanNode(trieStream.readChar(), 0, nullptr, nullptr);
    }
    return new HuffmanNode('\0', 0, decodeTrie(), decodeTrie());
}

void Decoder::validateHeader() {
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

StringBuffer Decoder::readBinaryFile(string filename) {
    StringBuffer data;
    data.readBinaryFile(filename);
    return data;
}

string Decoder::unsqueeze(BitStream encoded) {
    int i = 0;
    string result;
    link x = nullptr;
    trieStream = encoded;
    trieStream.start();
    validateHeader();
    huffmanTree = decodeTrie();
    Levelorder()(huffmanTree);
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

#endif