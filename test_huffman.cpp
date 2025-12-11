#include "Huffman.h"
#include <iostream>
#include <sstream>
#include <vector>

class StringInputStream : public IInputStream {
    const std::string& data;
    size_t pos;
public:
    StringInputStream(const std::string& s) : data(s), pos(0) {}
    bool Read(byte& value) override {
        if (pos >= data.size()) return false;
        value = data[pos++];
        return true;
    }
};

class StringOutputStream : public IOutputStream {
    std::string& data;
public:
    StringOutputStream(std::string& s) : data(s) {}
    void Write(byte value) override {
        data.push_back(value);
    }
};

int main() {
    // Тест 1: маленький файл (экономим на размере < 256 байт)
    {
        std::string original;
        for (int i = 0; i < 10; i++) {
            original += "aaabbbccc";
        }

        std::string compressed;
        std::string decompressed;

        {
            StringInputStream in(original);
            StringOutputStream out(compressed);
            Encode(in, out);
        }

        {
            StringInputStream in(compressed);
            StringOutputStream out(decompressed);
            Decode(in, out);
        }

        std::cout << "Test 1 (90 bytes):" << std::endl;
        std::cout << "  Original: " << original.size() << " bytes" << std::endl;
        std::cout << "  Compressed: " << compressed.size() << " bytes" << std::endl;
        std::cout << "  Overhead: " << (compressed.size() < original.size() ? 0 : compressed.size() - original.size()) << " bytes" << std::endl;
        std::cout << "  Ratio: " << (100.0 * compressed.size() / original.size()) << "%" << std::endl;
        std::cout << "  Match: " << (original == decompressed ? "YES" : "NO") << std::endl << std::endl;
    }

    // Тест 2: средний файл (экономим на размере < 65536 байт)
    {
        std::string original;
        for (int i = 0; i < 1000; i++) {
            original += "aaabbbccc";
        }

        std::string compressed;
        std::string decompressed;

        {
            StringInputStream in(original);
            StringOutputStream out(compressed);
            Encode(in, out);
        }

        {
            StringInputStream in(compressed);
            StringOutputStream out(decompressed);
            Decode(in, out);
        }

        std::cout << "Test 2 (9000 bytes):" << std::endl;
        std::cout << "  Original: " << original.size() << " bytes" << std::endl;
        std::cout << "  Compressed: " << compressed.size() << " bytes" << std::endl;
        std::cout << "  Saved: " << (original.size() - compressed.size()) << " bytes" << std::endl;
        std::cout << "  Ratio: " << (100.0 * compressed.size() / original.size()) << "%" << std::endl;
        std::cout << "  Match: " << (original == decompressed ? "YES" : "NO") << std::endl << std::endl;
    }

    // Тест 3: большой файл
    {
        std::string original;
        for (int i = 0; i < 10000; i++) {
            original += "aaabbbccc";
        }

        std::string compressed;
        std::string decompressed;

        {
            StringInputStream in(original);
            StringOutputStream out(compressed);
            Encode(in, out);
        }

        {
            StringInputStream in(compressed);
            StringOutputStream out(decompressed);
            Decode(in, out);
        }

        std::cout << "Test 3 (90000 bytes):" << std::endl;
        std::cout << "  Original: " << original.size() << " bytes" << std::endl;
        std::cout << "  Compressed: " << compressed.size() << " bytes" << std::endl;
        std::cout << "  Saved: " << (original.size() - compressed.size()) << " bytes ("
                  << (100.0 * (original.size() - compressed.size()) / original.size()) << "%)" << std::endl;
        std::cout << "  Ratio: " << (100.0 * compressed.size() / original.size()) << "%" << std::endl;
        std::cout << "  Match: " << (original == decompressed ? "YES" : "NO") << std::endl;
    }

    return 0;
}
