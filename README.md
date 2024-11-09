# mgczip
Compress ya self before you wreck ya self!

Lossless data compression with mgczip provides > 50% reduction in space usage 
for _most_ text thanks to huffman encoding. 

## installation and usage

      make
      sudo make install
      make clean

      mgczip -c <filename> to compress file
      mgczip -d <filename> to decompress file
  