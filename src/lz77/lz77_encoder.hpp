#ifndef lz77_encoder_hpp
#define lz77_encoder_hpp
#include <iostream>
#include <vector>
#include "lz77_defs.hpp"
#include "../stringbuffer.hpp"
#include "../bitstream.hpp"
using namespace std;

class LZ77Encoder {
    private:
        vector<LZTriple> parseToTriples(string input);
        void addHeader(BitStream& bs);
    public:
        LZ77Encoder();
        BitStream compress(StringBuffer sb);
};

LZ77Encoder::LZ77Encoder() {

}

BitStream LZ77Encoder::compress(StringBuffer sb) {
    BitStream bs;
    for (LZTriple t : parseToTriples(sb.getBuffer())) {
        bs.writeInt(t.offset, 12);
        bs.writeInt(t.length, 12);
        bs.writeChar(t.symbol, 8);
    }
    return bs;
}

void LZ77Encoder::addHeader(BitStream& bs) {
    bs.writeChar('M',8);
    bs.writeChar('G', 8);
    bs.writeChar('C', 8);
    bs.writeChar('7', 8);
}

vector<LZTriple> LZ77Encoder::parseToTriples(string input) {
    vector<LZTriple> results;
    int i = 0; 
    while (i < input.size()) {
        int j = max(0, i-window_size);
        int match_left = j;
        int match_len = 0;
        while (j < i) {
            if (input[j] == input[i]) {
                int wp = j, lp = i;
                while (input[wp] == input[lp]) {
                    wp++; lp++;
                }
                if (wp - j > match_len) {
                    match_len = wp-j;
                    match_left = j;
                    j += match_len+1;
                }
            }
            j++;
        }
        LZTriple t;
        if (match_len > 0) {
            t.offset = i-match_left;
            t.length = match_len;
            t.symbol = input[i+match_len];
            i += match_len+1;
        } else {
            t.offset = 0;
            t.length = 0;
            t.symbol = input[i];
            i++;
        }
        results.push_back(t);
    }
    return results;
}

#endif