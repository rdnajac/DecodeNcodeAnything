#pragma once

#include <string>

std::string charToDNA(char ch);
char dnaToChar(const std::string& dna);
bool isValidDNA(const std::string& dna);
std::string textToDNA(const std::string& text);
std::string dnaToText(const std::string& dna);

