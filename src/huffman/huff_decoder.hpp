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
        BitStream stringBufferToBitStream(StringBuffer compressed);
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

BitStream HuffDecoder::stringBufferToBitStream(StringBuffer compressed) {
     BitStream inf;
    while (!compressed.done()) { 
        inf.writeChar(compressed.get(), 8); 
        compressed.advance(); 
    }
    return inf;
}

void HuffDecoder::uncompress(StringBuffer compressed, string outfile) {
    string uncompressed = unsqueeze(stringBufferToBitStream(compressed));
    ofstream opf(outfile);
    if (opf.is_open()) {
        for (unsigned char c : uncompressed) {
            opf << c;
        }
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
    link left = decodeTrie();
    link right = decodeTrie();
    return new HuffmanNode('\0', 0, left, right);
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
    int i = 0;
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
        cout<<".";
        i++;
    }
    if (isLeaf(x)) result.push_back(x->symbol);
    cout<<" Done! "<<i<<" bits read."<<endl;
    return result;
}

#endif