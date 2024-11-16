#ifndef huff_encoder_hpp
#define huff_encoder_hpp
#include "../hashmap.hpp"
#include "../bitstream.hpp"
#include "../stringbuffer.hpp"
#include "../prique.hpp"
#include "huffnode.hpp"

class HuffEncoder {
    private:
        HashMap<char, string> encoding;
        link huffmanTree;
        BitStream resultStream;
        HashMap<char, link> computeFrequencies(StringBuffer data);
        void generateEncodingTable(link h, string prefix);
        void addHeader();
        void encodeTrie(HuffmanNode* x);
        void buildHuffmanTree(StringBuffer data);
        BitStream squeeze(StringBuffer data);
        void cleanup(link node);
    public:
        HuffEncoder();
        ~HuffEncoder();
        BitStream compress(StringBuffer strbuff);
};

HuffEncoder::HuffEncoder() {
    huffmanTree = nullptr;
}

HuffEncoder::~HuffEncoder() {
    cleanup(huffmanTree);
}

BitStream HuffEncoder::compress(StringBuffer sbuff) {
    return squeeze(sbuff);
}

void HuffEncoder::cleanup(link node) {
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}


HashMap<char, link> HuffEncoder::computeFrequencies(StringBuffer data) {
    HashMap<char, link> freq;
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


void HuffEncoder::generateEncodingTable(link h, string prefix) {
    if (h != nullptr) {
        if (isLeaf(h)) {
            encoding.insert(h->symbol, prefix);
        } else {
            generateEncodingTable(h->left, prefix + "0");
            generateEncodingTable(h->right, prefix + "1");
        }
    }
}

void HuffEncoder::encodeTrie(HuffmanNode* x) {
    if (isLeaf(x)) {
        resultStream.writeBit(true);
        resultStream.writeChar(x->symbol, 8);
        return;
    }
    resultStream.writeBit(false);
    encodeTrie(x->left);
    encodeTrie(x->right);
}

void HuffEncoder::buildHuffmanTree(StringBuffer data) {
    MinHeap<link, HuffCmp> pq;
    HashMap<char, link> freq = computeFrequencies(data);
    for (auto it : freq) {
        pq.push(it.value());
    }
    while (pq.size() > 1) {
        link a = pq.pop();
        link b = pq.pop();
        link tmp = new HuffmanNode('&', a->frequency + b->frequency, a, b);
        pq.push(tmp);
    }
    huffmanTree = pq.pop();
}

void HuffEncoder::addHeader() {
    resultStream.writeChar('M',8);
    resultStream.writeChar('G', 8);
    resultStream.writeChar('C', 8);
    resultStream.writeChar('H', 8);

}

BitStream HuffEncoder::squeeze(StringBuffer data) {
    string prefix, output;
    buildHuffmanTree(data);
    generateEncodingTable(huffmanTree, prefix);
    addHeader();
    encodeTrie(huffmanTree);
    while (!data.done()) {
        for (char c : encoding.find(data.get()).value()) {
            resultStream.writeBit(c == '1');
        }
        data.advance();
    }
    return resultStream;
}

#endif