#ifndef lz77_decoder_hpp
#define lz77_decoder_hpp
#include <iostream>
#include <fstream>
#include <vector>
#include "../bitstream.hpp"
#include "lz77_defs.hpp"
using namespace std;

class LZ77Decoder {
    private:
        void validateHeader(BitStream& bs);
    public:
        LZ77Decoder();
        string uncompress(StringBuffer triple, string outfile);
};

LZ77Decoder::LZ77Decoder() {
    
}


string LZ77Decoder::uncompress(StringBuffer sb, string outfile) {
    BitStream input;
    while (!sb.done()) {
        input.writeChar(sb.get(), 8);
        sb.advance();
    }
    string output;
    input.start();
    LZTriple m;
    while (!input.done()) {
        m.offset = input.readInt(12);
        m.length = input.readInt(12);
        m.symbol = input.readChar();
        if (m.length > 0) {
            int spos = output.size() - m.offset;
            for (int i = 0; i < m.length; i++)
                output.push_back(output[spos+i]);
        }
        output.push_back(m.symbol);
    }
    ofstream opf(outfile);
    if (opf.is_open()) {
        for (unsigned char c : output) {
            opf << c;
        }
    }
    opf.close();       
    return output;
}

void LZ77Decoder::validateHeader(BitStream& bs) {
    bool pass = true;
    string header = "MGC7";
    for (char c : header) {
        if (c != bs.readChar()) {
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

#endif