# mgczip
Compress ya self before you wreck ya self!

Lossless data compression with mgczip provides > 50% reduction in space usage for _many_ file types. 
Supported encodings are Huffman encoding & LZW encoding. Mgczip is intended for use on single files,
support for archiving might be added in future release. Might add lz77 if i'm feeeling frisky.


## installation and usage

      make
      sudo make install
      make clean

      mgczip -ch <filename> to compress file using huffman coding
      mgczip -cl <filename> to compress file using LZW
      mgczip -dh <filename> to decompress file using huffman coding
      mgczip -dl <filename> to decompress file using LZW
  
