#include <iostream>
#include <string>
#include <bitset>

// 将字符转换为DNA序列
std::string charToDNA(char c) {
    std::bitset<8> ascii(c); // 将字符转换为8位ASCII
    std::string dna = "";
    for (int i = 7; i >= 0; i -= 2) {
        // 每2位二进制转换为一个DNA碱基
        switch (ascii[i] * 2 + ascii[i - 1]) {
            case 0: dna = "A" + dna; break;
            case 1: dna = "T" + dna; break;
            case 2: dna = "G" + dna; break;
            case 3: dna = "C" + dna; break;
        }
    }
    return dna;
}

// 将DNA序列转换为字符
char dnaToChar(const std::string& dna) {
    std::bitset<8> ascii(0);
    for (size_t i = 0; i < dna.size(); ++i) {
        ascii <<= 2;
        switch (dna[i]) {
            case 'A': break; // 00不需要改变
            case 'T': ascii.set(0); break; // 01
            case 'G': ascii.set(1); break; // 10
            case 'C': ascii.set(0); ascii.set(1); break; // 11
        }
    }
    return static_cast<char>(ascii.to_ulong());
}

// 检查是否是有效的DNA序列
bool isValidDNA(const std::string& dna) {
    for (char c : dna) {
        if (c != 'A' && c != 'G' && c != 'T' && c != 'C') {
            return false;
        }
    }
    return true;
}

// 将字符串转换为DNA序列
std::string stringToDNA(const std::string& text) {
    std::string dna = "";
    for (char c : text) {
        dna += charToDNA(c);
    }
    return dna;
}

// 将DNA序列转换为字符串
std::string dnaToString(const std::string& dna) {
    std::string text = "";
    for (size_t i = 0; i < dna.size(); i += 4) {
        text += dnaToChar(dna.substr(i, 4));
    }
    return text;
}

int main() {
    std::string input, output;
    char choice;
    std::cout << "Welcome to the Text-DNA Converter!" << std::endl;

    do {
        std::cout << "\nChoose an option:\n"
                  << "1. Convert sequences to DNA\n"
                  << "2. Convert DNA to sequences\n"
                  << "Q. Quit\n"
                  << "Your choice: ";
        std::cin >> choice;
        if (choice == 'Q' || choice == 'q') {
            break;
        }

        std::cout << "Enter the sequence: ";
        std::cin.ignore(); // 忽略之前读取choice后留下的换行符
        std::getline(std::cin, input); // 使用getline来允许空格

        switch (choice) {
            case '1':
                output = stringToDNA(input);
                std::cout << "DNA Sequence: " << output << std::endl;
                break;
            case '2':
                if (isValidDNA(input)) {
                    output = dnaToString(input);
                    std::cout << "Converted Sequence: " << output << std::endl;
                } else {
                    std::cout << "Invalid DNA sequence. Please enter a sequence containing only A, G, T, and C." << std::endl;
                    continue;
                }
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                continue;
        }
    } while (true);

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
