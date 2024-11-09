#ifndef lzw_decoder_hpp
#define lzw_decoder_hpp
#include <iostream>
#include "../bitstream.hpp"
#include "../stringbuffer.hpp"



class LZWDecoder {
    private:
        int num_chars;
        int max_code_words;
        int bit_width;
        string decompress(BitStream input);
        void validateHeader(BitStream& bs);
    public:
        LZWDecoder();
        void uncompress(StringBuffer strbuff, string outfile);
};

LZWDecoder::LZWDecoder() {
    num_chars = 256;
    max_code_words = 4096;
    bit_width = 12;
}

void LZWDecoder::uncompress(StringBuffer sb, string outfile) {
    BitStream bs;
    while (!sb.done()) {
        bs.writeChar(sb.get(), 8);
        sb.advance();
    }
    bs.start();
    validateHeader(bs);
    string uncompressed = decompress(bs);
    ofstream opf(outfile);
    if (opf.is_open()) {
        for (unsigned char c : uncompressed) {
            opf << c;
        }
    }
    opf.close();       
}

void LZWDecoder::validateHeader(BitStream& bs) {
    bool pass = true;
    string header = "MGCL";
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

string LZWDecoder::decompress(BitStream input) {
    string st[max_code_words];
    int i;
    for (i = 0; i < num_chars; i++) {
        st[i].push_back((char)i);
    }
    st[i++] = "";
    unsigned int codeword = input.readInt(12);
    if (codeword == num_chars)
        return "";
    string val = st[codeword];
    string output;
    while (input.offset() < input.size()) {
        output.append(val);
        codeword = input.readInt(12);
        if (codeword == num_chars)
            return output;
        string s = st[codeword];
        if (i == codeword) { s = val; s.push_back(val[0]); }
        if (i < max_code_words) {
            val.push_back(s[0]);
            st[i++] = val;
        } 
        val = s;           
    }
    return output;
}

#endif