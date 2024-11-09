#ifndef huff_decoder_hpp
#define huff_decoder_hpp
#include "../avlmap.hpp"
#include "../bitstream.hpp"
#include "../stringbuffer.hpp"
#include "huffnode.hpp"

class HuffDecoder {
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
        HuffDecoder();
        ~HuffDecoder();
        void uncompress(string filename);

};

HuffDecoder::HuffDecoder() {

}

HuffDecoder::~HuffDecoder() {
    cleanup(huffmanTree);
}

void HuffDecoder::uncompress(string filename) {
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

void HuffDecoder::cleanup(link node) {
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

bool HuffDecoder::isLeaf(link h) {
    return (h->left == nullptr && h->right == nullptr);
}

HuffmanNode* HuffDecoder::decodeTrie() {
    if (trieStream.readBit()) {
        return new HuffmanNode(trieStream.readChar(), 0, nullptr, nullptr);
    }
    return new HuffmanNode('\0', 0, decodeTrie(), decodeTrie());
}

void HuffDecoder::validateHeader() {
    bool pass = true;
    string header = "MGCH";
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

StringBuffer HuffDecoder::readBinaryFile(string filename) {
    StringBuffer data;
    data.readBinaryFile(filename);
    return data;
}

string HuffDecoder::unsqueeze(BitStream encoded) {
    string result;
    link x = nullptr;
    trieStream = encoded;
    trieStream.start();
    validateHeader();
    huffmanTree = decodeTrie();
    Levelorder()(huffmanTree);
    int length = trieStream.readInt();
    cout<<"Length: "<<length<<endl;
    while (!trieStream.done()) {
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
        }
    }
    result.push_back(x->symbol);
    return result;
}

#endif