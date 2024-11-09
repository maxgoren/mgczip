#ifndef mgczip_hpp
#define mgczip_hpp
#include "huffman/huff_encoder.hpp"
#include "huffman/huff_decoder.hpp"
#include "lzw/lzw_encoder.hpp"
#include "lzw/lzw_decoder.hpp"
using namespace std;

enum METHOD {
    HUFFMAN, LZW, BOTH
};

class MGCZip {
    private:
        void calculateCompressionRatio(string filename);
        void writeCompressedFile(BitStream compressed, string filename);
        void compressWithBoth(string filename);
        void decompressWithBoth(string filename);
    public:
        MGCZip();
        void compress(string filename, METHOD method);
        void decompress(string filename, METHOD method);
};

MGCZip::MGCZip() {

}

void MGCZip::decompress(string filename, METHOD method) {
    HuffDecoder huff;
    LZWDecoder lzw;
    switch (method) {
        case HUFFMAN: huff.uncompress(filename); break;
        case LZW: lzw.uncompress(filename); break;
        case BOTH: decompressWithBoth(filename); break;
        default:
            break;
    }
}

void MGCZip::compress(string filename, METHOD method) {
    HuffEncoder huff;
    LZWEncoder lzw;
    BitStream bs;
    switch (method) {
        case HUFFMAN: 
            bs = huff.compress(filename); 
            break;
        case LZW: 
            bs = lzw.compress(filename); 
            break;
        case BOTH:
            compressWithBoth(filename);
        default:
            break;
    }
    writeCompressedFile(bs, filename);
    calculateCompressionRatio(filename);
}

void MGCZip::compressWithBoth(string filename) {
    HuffEncoder huff;
    LZWEncoder lzw;
    huff.compress(filename);
    lzw.compress(filename + ".mgz");
}

void MGCZip::decompressWithBoth(string filename) {
        HuffDecoder huff;
        LZWDecoder lzw;
        lzw.uncompress(filename);
        huff.uncompress(filename.substr(0, filename.size() - 4) + ".2");
}

void MGCZip::writeCompressedFile(BitStream compressed, string filename) {
    std::ofstream file(filename+".mgz", std::ios::out|std::ios::binary);
    compressed.start();
    int j = 0; 
    while (!compressed.done()) {
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

void MGCZip::calculateCompressionRatio(string filename) {
    auto slength = filesystem::file_size(filesystem::path(filename));
    auto flength = filesystem::file_size(filesystem::path(filename + ".mgz"));
    cout<<"Starting size: "<<slength<<" bytes"<<endl;
    cout<<"Ending   size: "<<flength<<" bytes"<<endl;
    cout<<(1 - ((double)flength/(double)slength))<<" compression ratio."<<endl;
}

#endif