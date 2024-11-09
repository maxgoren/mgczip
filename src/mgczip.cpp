#include <iostream>
#include "mgczip.hpp"

void printUsage() {
    cout<<"Not enough arguments"<<endl;
    cout<<"mgczip -c to compress file"<<endl;
    cout<<"       -ch <filename> compress using huffman coding"<<endl;
    cout<<"       -cl <filename> compress using lz78"<<endl;
    cout<<"       -cz <filename> huffman & lz78"<<endl;

    cout<<"mgczip -d to compress file"<<endl;
    cout<<"       -dh <filename> decompress using huffman coding"<<endl;
    cout<<"       -dl <filename> decompress using lz78"<<endl;
    cout<<"       -dz <filename> huffman & lz78"<<endl;
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
                        case 'h': mz.compress(doto, HUFFMAN); break;
                        case 'l': mz.compress(doto, LZW); break;
                        case 'z': mz.compress(doto, BOTH); break;
                        default: break;
                    }
                }
                break;
            case 'd': {
                    switch (argv[1][2]) {
                        case 'h': mz.decompress(doto, HUFFMAN); break;
                        case 'l': mz.decompress(doto, LZW); break;
                        case 'z': mz.decompress(doto, BOTH); break;
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