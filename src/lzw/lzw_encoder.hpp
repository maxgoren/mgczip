#ifndef lzw_encoder_hpp
#define lzw_encoder_hpp
#include <iostream>
#include "../bitstream.hpp"
#include "../prefixtrie.hpp"
#include "../stringbuffer.hpp"

class LZWEncoder {
    private:
        int num_chars;
        int max_code_words;
        int bit_width;
        BitStream squeeze(StringBuffer istrbuff);
        void addHeader(BitStream& bs);
    public:
        LZWEncoder();
        BitStream compress(StringBuffer strbuff);
};

LZWEncoder::LZWEncoder() {
    num_chars = 256;
    max_code_words = 4096;
    bit_width = 12;
}

BitStream LZWEncoder::compress(StringBuffer sb) {
    return squeeze(sb);
}

void LZWEncoder::addHeader(BitStream& resultStream) {
    resultStream.writeChar('M',8);
    resultStream.writeChar('G', 8);
    resultStream.writeChar('C', 8);
    resultStream.writeChar('L', 8);
}

BitStream LZWEncoder::squeeze(StringBuffer istrbuff) {
    string str;
    while (!istrbuff.done()) {
        str.push_back(istrbuff.get());
        istrbuff.advance();
    }
    string output;
    PrefixTrie<int> st;
    BitStream bs;
    addHeader(bs);
    for (int i = 0; i < num_chars; i++) {
        string key;
        key.push_back((char)i);
        st.insert(key, i);
    }
    int code = num_chars+1;
    while (str.size() > 0) {
        string s = st.longestPrefix(str);
        bs.writeInt(st.get(s), bit_width);
        cout<<st.get(s)<<": "<<s<<endl;
        int t = s.length();
        if (t < str.length() && code < max_code_words) {
            st.insert(str.substr(0, t+1), code++);
        }
        str = str.substr(t);
    }
    bs.writeInt(num_chars, bit_width);
    cout<<(num_chars&0xFFF)<<endl;
    return bs;
}

#endif