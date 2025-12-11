#include "Huffman.h"
#include <vector>
#include <queue>
#include <map>

class BitWriter {
    IOutputStream& stream;
    byte buffer;
    int bitCount;

public:
    BitWriter(IOutputStream& s) : stream(s), buffer(0), bitCount(0) {}

    void WriteBit(bool bit) {
        buffer = (buffer << 1) | (bit ? 1 : 0);
        bitCount++;
        if (bitCount == 8) {
            stream.Write(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }

    void WriteByte(byte b) {
        for (int i = 7; i >= 0; i--) {
            WriteBit((b >> i) & 1);
        }
    }

    void Flush() {
        if (bitCount > 0) {
            buffer <<= (8 - bitCount);
            stream.Write(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }
};

class BitReader {
    IInputStream& stream;
    byte buffer;
    int bitCount;
    bool endReached;

public:
    BitReader(IInputStream& s) : stream(s), buffer(0), bitCount(0), endReached(false) {
        if (!stream.Read(buffer)) {
            endReached = true;
        }
    }

    bool ReadBit(bool& bit) {
        if (endReached && bitCount >= 8) {
            return false;
        }
        if (bitCount == 8) {
            if (!stream.Read(buffer)) {
                endReached = true;
                return false;
            }
            bitCount = 0;
        }
        bit = (buffer >> (7 - bitCount)) & 1;
        bitCount++;
        return true;
    }

    bool ReadByte(byte& b) {
        b = 0;
        for (int i = 0; i < 8; i++) {
            bool bit;
            if (!ReadBit(bit)) return false;
            b = (b << 1) | (bit ? 1 : 0);
        }
        return true;
    }
};

struct HuffNode {
    byte symbol;
    HuffNode* left;
    HuffNode* right;

    HuffNode(byte s) : symbol(s), left(nullptr), right(nullptr) {}
    HuffNode(HuffNode* l, HuffNode* r) : symbol(0), left(l), right(r) {}
};

struct NodeWrapper {
    unsigned long frequency;
    HuffNode* node;

    // Для priority_queue (min-heap)
    bool operator>(const NodeWrapper& other) const {
        return frequency > other.frequency;
    }
};

void DeleteTree(HuffNode* node) {
    if (!node) return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

void BuildCodes(HuffNode* node, std::vector<bool>& code, std::vector<std::vector<bool>>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
        return;
    }

    code.push_back(false);
    BuildCodes(node->left, code, codes);
    code.pop_back();

    code.push_back(true);
    BuildCodes(node->right, code, codes);
    code.pop_back();
}

void WriteTree(HuffNode* node, BitWriter& writer) {
    if (!node->left && !node->right) {
        writer.WriteBit(true);
        writer.WriteByte(node->symbol);
    } else {
        writer.WriteBit(false);
        WriteTree(node->left, writer);
        WriteTree(node->right, writer);
    }
}


HuffNode* ReadTree(BitReader& reader) {
    bool isLeaf;
    if (!reader.ReadBit(isLeaf)) return nullptr;

    if (isLeaf) {
        byte symbol;
        if (!reader.ReadByte(symbol)) return nullptr;
        return new HuffNode(symbol);
    } else {
        HuffNode* left = ReadTree(reader);
        HuffNode* right = ReadTree(reader);
        return new HuffNode(left, right);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<byte> inputData;
    unsigned long frequencies[256] = {0};
    byte val;


    while (original.Read(val)) {
        inputData.push_back(val);
        frequencies[val]++;
    }

    if (inputData.empty()) return;


    std::priority_queue<NodeWrapper, std::vector<NodeWrapper>, std::greater<NodeWrapper>> pq;
    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            pq.push({frequencies[i], new HuffNode((byte)i)});
        }
    }


    if (pq.size() == 1) {
        NodeWrapper top = pq.top(); pq.pop();
        HuffNode* root = new HuffNode(top.node, new HuffNode(top.node->symbol));
        pq.push({top.frequency, root});
    } else {
        while (pq.size() > 1) {
            NodeWrapper l = pq.top(); pq.pop();
            NodeWrapper r = pq.top(); pq.pop();
            pq.push({l.frequency + r.frequency, new HuffNode(l.node, r.node)});
        }
    }

    HuffNode* root = pq.top().node;

    std::vector<std::vector<bool>> codes(256);
    std::vector<bool> currentCode;
    BuildCodes(root, currentCode, codes);

    BitWriter writer(compressed);

    unsigned long size = inputData.size();
    if (size < 256) {
        writer.WriteBit(false); // 0 prefix
        writer.WriteByte((byte)size);
    } else if (size < 65536) {
        writer.WriteBit(true); writer.WriteBit(false); // 10 prefix
        writer.WriteByte((byte)(size >> 8));
        writer.WriteByte((byte)(size & 0xFF));
    } else {
        writer.WriteBit(true); writer.WriteBit(true); // 11 prefix
        writer.WriteByte((byte)(size >> 24));
        writer.WriteByte((byte)(size >> 16));
        writer.WriteByte((byte)(size >> 8));
        writer.WriteByte((byte)(size & 0xFF));
    }

    WriteTree(root, writer);

    for (byte b : inputData) {
        const std::vector<bool>& code = codes[b];
        for (bool bit : code) {
            writer.WriteBit(bit);
        }
    }

    writer.Flush();
    DeleteTree(root);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    BitReader reader(compressed);

    unsigned long dataSize = 0;
    bool flag1;
    if (!reader.ReadBit(flag1)) return;

    if (!flag1) {
        byte b;
        if (!reader.ReadByte(b)) return;
        dataSize = b;
    } else {
        bool flag2;
        if (!reader.ReadBit(flag2)) return;
        if (!flag2) {
            byte b1, b2;
            if(!reader.ReadByte(b1) || !reader.ReadByte(b2)) return;
            dataSize = ((unsigned long)b1 << 8) | b2;
        } else {
            byte b1, b2, b3, b4;
            if(!reader.ReadByte(b1) || !reader.ReadByte(b2) ||
               !reader.ReadByte(b3) || !reader.ReadByte(b4)) return;
            dataSize = ((unsigned long)b1 << 24) | ((unsigned long)b2 << 16) |
                       ((unsigned long)b3 << 8) | b4;
        }
    }

    if (dataSize == 0) return;

    HuffNode* root = ReadTree(reader);
    if (!root) return;

    HuffNode* current = root;
    unsigned long decodedCount = 0;
    bool bit;

    while (decodedCount < dataSize && reader.ReadBit(bit)) {
        if (bit) current = current->right;
        else current = current->left;

        if (!current->left && !current->right) {
            original.Write(current->symbol);
            decodedCount++;
            current = root;
        }
    }

    DeleteTree(root);
}
