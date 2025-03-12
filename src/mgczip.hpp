#ifndef mgczip_hpp
#define mgczip_hpp
#include "huffman/huff_encoder.hpp"
#include "huffman/huff_decoder.hpp"
#include "lzw/lzw_encoder.hpp"
#include "lzw/lzw_decoder.hpp"
#include "lz77/lz77_encoder.hpp"
#include "lz77/lz77_decoder.hpp"
using namespace std;

enum METHOD {
    HUFFMAN, LZW, LZ77, BOTH
};

class MGCZip {
    private:
        void calculateCompressionRatio(string filename);
        void writeCompressedFile(BitStream compressed, string filename);
    public:
        MGCZip();
        void compress(string filename, METHOD method);
        void decompress(string filename, METHOD method);
        void deflate(string filename);
};

MGCZip::MGCZip() {

}

void MGCZip::deflate(string filename) {
    StringBuffer sb;
    sb.readBinaryFile(filename);
    LZ77Encoder lz;
    vector<LZTriple> triples = lz.encodeToTriple(sb);
    string asString;
    for (LZTriple triple : triples) {
        asString += to_string(triple.offset) + to_string(triple.length) + string(1, triple.symbol);
    }
    StringBuffer sb2;
    sb2.init(asString);
    HuffEncoder huff;
    BitStream bs = huff.compress(sb2);
    writeCompressedFile(bs, filename);
    calculateCompressionRatio(filename);
}

void MGCZip::decompress(string filename, METHOD method) {
    StringBuffer strBuff;
    strBuff.readBinaryFile(filename);
    string outfile = filename.substr(0, filename.size() - 4)  + ".2";
    switch (method) {
        case HUFFMAN: { HuffDecoder huff; huff.uncompress(strBuff, outfile); } break;
        case LZ77: { LZ77Decoder lz; lz.uncompress(strBuff, outfile); } break;
        case LZW: { LZWDecoder lzw; lzw.uncompress(strBuff, outfile); } break;
        default:
            break;
    }
}

void MGCZip::compress(string filename, METHOD method) {
    StringBuffer strBuff;
    strBuff.readBinaryFile(filename);
    BitStream bs;
    switch (method) {
        case HUFFMAN: {
                HuffEncoder huff;
                bs = huff.compress(strBuff); 
            }
            break;
        case LZ77: {
                LZ77Encoder lz;
                bs = lz.compress(strBuff); 
            }
            break;
        case LZW: {
                LZWEncoder lzw;
                bs = lzw.compress(strBuff); 
            }
            break;
        default:
            break;
    }
    writeCompressedFile(bs, filename);
    calculateCompressionRatio(filename);
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