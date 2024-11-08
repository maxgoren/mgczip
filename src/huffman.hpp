#ifndef huffman_hpp
#define huffman_hpp
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <bitset>
#include <algorithm>
#include "avlmap.hpp"
#include "prique.hpp"
#include "bitstream.hpp"
#include "stringbuffer.hpp"
#include "huffnode.hpp"
using namespace std;



class MGCZip {
    private:
        AVLMap<char, string> encoding;
        link huffmanTree;
        BitStream trieStream;
        void printEncodingTable();
        AVLMap<char, link> computeFrequencies(StringBuffer data);
        bool isLeaf(link h);
        void generateEncodingTable(link h, string prefix);
        void encodeTrie(HuffmanNode* x);
        HuffmanNode* decodeTrie();
        void validateHeader();
        void writeCompressedFile(BitStream compressed, string filename);
        StringBuffer readBinaryFile(string filename);
        void buildHuffmanTree(StringBuffer data);
        BitStream squeeze(StringBuffer data);
        string unsqueeze(BitStream encoded);
        void levelorder(link rootnode);
        void cleanup(link node);
    public:
        MGCZip();
        ~MGCZip();
        void compress(string filename);
        void uncompress(string filename);
};

#endif