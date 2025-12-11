#include "Huffman.h"
#include <vector>
#include <queue>
#include <map>
#include <cstring>
#include <algorithm>

struct HuffNode {
    byte symbol;
    unsigned long frequency;
    HuffNode* left;
    HuffNode* right;

    HuffNode(byte sym, unsigned long freq)
        : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}

    HuffNode(HuffNode* l, HuffNode* r)
        : symbol(0), frequency(l->frequency + r->frequency), left(l), right(r) {}
};

struct CompareNode {
    bool operator()(HuffNode* a, HuffNode* b) const {
        return a->frequency > b->frequency;
    }
};

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

void BuildCodes(HuffNode* node, std::vector<bool>& code, std::map<byte, std::vector<bool>>& codes) {
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

void DeleteTree(HuffNode* node) {
    if (!node) return;
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

void WriteTree(HuffNode* node, BitWriter& writer) {
    if (!node) return;

    if (!node->left && !node->right) {
        writer.WriteBit(true);
        writer.WriteByte(node->symbol);
        return;
    }

    WriteTree(node->left, writer);
    WriteTree(node->right, writer);
    writer.WriteBit(false);
}

// Подсчет количества узлов для вычисления размера дерева
int CountTreeNodes(HuffNode* node) {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return 1 + CountTreeNodes(node->left) + CountTreeNodes(node->right);
}

HuffNode* ReadTree(BitReader& reader, int symbolCount) {
    std::vector<HuffNode*> stack;
    int symbolsRead = 0;

    bool bit;
    while (symbolsRead < symbolCount || stack.size() > 1) {
        if (!reader.ReadBit(bit)) break;

        if (bit) {
            byte symbol;
            if (!reader.ReadByte(symbol)) break;
            stack.push_back(new HuffNode(symbol, 0));
            symbolsRead++;
        } else {
            if (stack.size() < 2) break;
            HuffNode* right = stack.back(); stack.pop_back();
            HuffNode* left = stack.back(); stack.pop_back();
            stack.push_back(new HuffNode(left, right));
        }
    }

    if (stack.empty()) return nullptr;
    return stack[0];
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<byte> data;
    byte b;

    // Читаем все данные и считаем частоты
    std::map<byte, unsigned long> frequency;
    while (original.Read(b)) {
        data.push_back(b);
        frequency[b]++;
    }

    if (data.empty()) {
        return;
    }

    // Создаем кучу узлов
    std::priority_queue<HuffNode*, std::vector<HuffNode*>, CompareNode> heap;

    for (auto& p : frequency) {
        heap.push(new HuffNode(p.first, p.second));
    }

    // Строим дерево Хаффмана
    HuffNode* root = nullptr;
    if (heap.size() == 1) {
        HuffNode* single = heap.top();
        heap.pop();
        root = new HuffNode(single, new HuffNode(single->symbol, 0));
    } else {
        while (heap.size() > 1) {
            HuffNode* left = heap.top(); heap.pop();
            HuffNode* right = heap.top(); heap.pop();
            HuffNode* parent = new HuffNode(left, right);
            heap.push(parent);
        }
        root = heap.top();
        heap.pop();
    }

    // Строим таблицу кодов
    std::map<byte, std::vector<bool>> codes;
    std::vector<bool> code;
    BuildCodes(root, code, codes);

    BitWriter writer(compressed);

    // Пишем количество уникальных символов
    writer.WriteByte(frequency.size());

    // Пишем размер данных с переменной длиной
    unsigned long dataSize = data.size();
    if (dataSize < 256) {
        writer.WriteBit(false);
        writer.WriteByte(dataSize & 0xFF);
    } else if (dataSize < 65536) {
        writer.WriteBit(true);
        writer.WriteBit(false);
        writer.WriteByte((dataSize >> 8) & 0xFF);
        writer.WriteByte(dataSize & 0xFF);
    } else {
        writer.WriteBit(true);
        writer.WriteBit(true);
        writer.WriteByte((dataSize >> 24) & 0xFF);
        writer.WriteByte((dataSize >> 16) & 0xFF);
        writer.WriteByte((dataSize >> 8) & 0xFF);
        writer.WriteByte(dataSize & 0xFF);
    }

    // Пишем дерево
    WriteTree(root, writer);

    // Кодируем данные
    for (byte symbol : data) {
        for (bool bit : codes[symbol]) {
            writer.WriteBit(bit);
        }
    }

    writer.Flush();

    DeleteTree(root);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    BitReader reader(compressed);

    // Читаем количество символов
    byte symbolCount;
    if (!reader.ReadByte(symbolCount)) {
        return;
    }

    if (symbolCount == 0) {
        return;
    }

    // Читаем размер данных с переменной длиной
    bool bit1;
    if (!reader.ReadBit(bit1)) {
        return;
    }

    unsigned long dataSize = 0;

    if (!bit1) {
        // 1 байт
        byte b;
        if (!reader.ReadByte(b)) return;
        dataSize = b;
    } else {
        bool bit2;
        if (!reader.ReadBit(bit2)) return;

        if (!bit2) {
            // 2 байта
            byte b1, b2;
            if (!reader.ReadByte(b1) || !reader.ReadByte(b2)) return;
            dataSize = ((unsigned long)b1 << 8) | (unsigned long)b2;
        } else {
            // 4 байта
            byte b1, b2, b3, b4;
            if (!reader.ReadByte(b1) || !reader.ReadByte(b2) || !reader.ReadByte(b3) || !reader.ReadByte(b4)) {
                return;
            }
            dataSize = ((unsigned long)b1 << 24) | ((unsigned long)b2 << 16) |
                       ((unsigned long)b3 << 8) | (unsigned long)b4;
        }
    }

    // Восстанавливаем дерево
    HuffNode* root = ReadTree(reader, symbolCount);

    if (!root) {
        return;
    }

    // Декодируем данные
    HuffNode* current = root;
    bool bit;
    unsigned long decoded = 0;

    while (reader.ReadBit(bit) && decoded < dataSize) {
        current = bit ? current->right : current->left;

        if (!current) break;

        if (!current->left && !current->right) {
            original.Write(current->symbol);
            decoded++;
            current = root;
        }
    }

    DeleteTree(root);
}
