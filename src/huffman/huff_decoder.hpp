#ifndef huff_decoder_hpp
#define huff_decoder_hpp
#include "../bitstream.hpp"
#include "../stringbuffer.hpp"
#include "huffnode.hpp"

class HuffDecoder {
    private:
        link huffmanTree;
        BitStream trieStream;
        HuffmanNode* decodeTrie();
        void validateHeader();
        string unsqueeze(BitStream encoded);
        void cleanup(link node);
    public:
        HuffDecoder();
        ~HuffDecoder();
        void uncompress(StringBuffer sbuff, string outfile);

};

HuffDecoder::HuffDecoder() {
    huffmanTree = nullptr;
}

HuffDecoder::~HuffDecoder() {
    cleanup(huffmanTree);
}

void HuffDecoder::uncompress(StringBuffer compressed, string outfile) {
    BitStream inf;
    while (!compressed.done()) { 
        inf.writeChar(compressed.get(), 8); 
        compressed.advance(); 
    }
    string uncompressed = unsqueeze(inf);
    ofstream opf(outfile);
    if (opf.is_open()) {
        for (unsigned char c : uncompressed) {
            opf << c;
            //cout<<c;
        }
        //cout<<endl;
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
        } else {
            cout<<c;
        }
    }
    if (!pass) {
        cout<<"Invalid Header, aborting."<<endl;
        exit(0);
    } else {
        cout<<", Headers Match! Decompressing..."<<endl;
    }
}

string HuffDecoder::unsqueeze(BitStream encoded) {
    string result;
    link x = nullptr;
    trieStream = encoded;
    trieStream.start();
    validateHeader();
    huffmanTree = decodeTrie();
    int length = trieStream.readInt();
    while (!trieStream.done()) {
        if (x == nullptr) {
            x = huffmanTree;
        }
        if (isLeaf(x)) {
            result.push_back(x->symbol);
            x = nullptr;
        } else {
            x = trieStream.readBit() ? x->right:x->left;
        }
    }
    if (isLeaf(x)) result.push_back(x->symbol);
    return result;
}

#endif