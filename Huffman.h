#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    virtual bool Read(byte& value) = 0;
    virtual ~IInputStream() {}
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
    virtual ~IOutputStream() {}
};

void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

#endif // HUFFMAN_H
