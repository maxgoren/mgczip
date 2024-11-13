#include <iostream>
#include "mgczip.hpp"

void printUsage() {
    cout<<"Not enough arguments"<<endl;
    cout<<"mgczip -c to compress file"<<endl;
    cout<<"       -ch <filename> compress using huffman coding"<<endl;
    cout<<"       -c7 <filename> compress using lz77"<<endl;
    cout<<"       -cw <filename> compress using lzw"<<endl;

    cout<<"mgczip -d to compress file"<<endl;
    cout<<"       -dh <filename> decompress using huffman coding"<<endl;
    cout<<"       -d7 <filename> decompress using lz77"<<endl;
    cout<<"       -dw <filename> decompress using lzw"<<endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 0;
    }
    MGCZip mz;
    string doto = argv[2];
    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'c': {
                    switch (argv[1][2]) {
                        case 'h': { mz.compress(doto, HUFFMAN); break; }
                        case '7': { mz.compress(doto, LZ77); break; }
                        case 'w': { mz.compress(doto, LZW); break; }
                        case 'p': { mz.deflate(doto); break; }
                        default: break;
                    }
                }
                break;
            case 'd': {
                    switch (argv[1][2]) {
                        case 'h': { mz.decompress(doto, HUFFMAN); break; }
                        case '7': { mz.decompress(doto, LZ77); break; }
                        case 'w': { mz.decompress(doto, LZW); break; }
                        default: break;
                    }
                }
                break;
            default:
                cout<<"Invalid argument provided"<<endl;
            break;
        }
    }
    return 0;
}