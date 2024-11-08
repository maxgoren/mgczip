#ifndef encoder_hpp
#define encoder_hpp
#include "avlmap.hpp"
#include "bitstream.hpp"
#include "huffnode.hpp"
#include "stringbuffer.hpp"
#include "prique.hpp"

class Encoder {
    private:
        AVLMap<char, string> encoding;
        link huffmanTree;
        BitStream trieStream;
        void printEncodingTable();
        AVLMap<char, link> computeFrequencies(StringBuffer data);
        bool isLeaf(link h);
        void generateEncodingTable(link h, string prefix);
        void encodeTrie(HuffmanNode* x);
        void writeCompressedFile(BitStream compressed, string filename);
        StringBuffer readBinaryFile(string filename);
        void buildHuffmanTree(StringBuffer data);
        BitStream squeeze(StringBuffer data);
        void cleanup(link node);
    public:
        Encoder();
        ~Encoder();
        void compress(string filename);
};

Encoder::Encoder() {

}

Encoder::~Encoder() {
    cleanup(huffmanTree);
}

void Encoder::compress(string filename) {
    StringBuffer sbuff = readBinaryFile(filename);
    BitStream compressed = squeeze(sbuff);
    writeCompressedFile(compressed, filename);
}

void Encoder::cleanup(link node) {
    if (node != nullptr) {
        cleanup(node->left);
        cleanup(node->right);
        delete node;
    }
}

void Encoder::printEncodingTable() {
    for (auto it = encoding.iterator(); !it.done(); it.next()) {
        cout<<it.get().key()<<": "<<it.get().value()<<endl;
    }
}

AVLMap<char, link> Encoder::computeFrequencies(StringBuffer data) {
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

bool Encoder::isLeaf(link h) {
    return (h->left == nullptr && h->right == nullptr);
}

void Encoder::generateEncodingTable(link h, string prefix) {
    if (h != nullptr) {
        if (isLeaf(h)) {
            encoding.insert(h->symbol, prefix);
        } else {
            generateEncodingTable(h->left, prefix + "0");
            generateEncodingTable(h->right, prefix + "1");
        }
    }
}

void Encoder::encodeTrie(HuffmanNode* x) {
    if (isLeaf(x)) {
        trieStream.writeBit(true);
        trieStream.writeChar(x->symbol, 8);
        return;
    }
    trieStream.writeBit(false);
    encodeTrie(x->left);
    encodeTrie(x->right);
}

void Encoder::writeCompressedFile(BitStream compressed, string filename) {
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

StringBuffer Encoder::readBinaryFile(string filename) {
    StringBuffer data;
    data.readBinaryFile(filename);
    return data;
}

void Encoder::buildHuffmanTree(StringBuffer data) {
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

BitStream Encoder::squeeze(StringBuffer data) {
    string prefix, output;
    BitStream result;
    result.writeChar('l',8);
    result.writeChar('e', 8);
    result.writeChar('e', 8);
    result.writeChar('t', 8);
    buildHuffmanTree(data);
    Levelorder()(huffmanTree);
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

#endif