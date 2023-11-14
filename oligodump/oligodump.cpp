#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

//#include "../convert/convert.hpp"
// Function to convert a character to its DNA representation
std::string charToDNA(char ch)
{
	std::string binary = "";

	for (int i = 7; i >= 0; --i)
		binary += ((ch & (1 << i)) ? "1" : "0");

	std::string dna = "";
	for (size_t i = 0; i < binary.length(); i += 2) {
		if (binary.substr(i, 2) == "00")
			dna += "A";
		else if (binary.substr(i, 2) == "10")
			dna += "G";
		else if (binary.substr(i, 2) == "01")
			dna += "T";
		else if (binary.substr(i, 2) == "11")
			dna += "C";
	}
	return dna;
}
using namespace std;

/*
 * @brief dump oligo conversion like hexdump
 */
void oligodump(const void *data, size_t size)
{
	const unsigned char *buffer = static_cast<const unsigned char *>(data);

	// loop through entire data
	for (size_t i = 0; i < size; i += 16) {
		
		// print number column
		cout << setw(8) << setfill('0') << hex << i << ": ";

		// print oligos
		for (size_t j = 0; j < 16; ++j) {
			if (i + j < size) {
				char ch = buffer[i + j];
                		string dna = charToDNA(ch);
                		cout << dna;
			} else {
				cout << ' ';
			}
			if (j % 8 == 7)
				cout << " ";
		}

		// print original chars
		for (size_t j = 0; j < 16 && i + j < size; ++j) {
			char c = buffer[i + j];
			cout << (isprint(c) ? c : '.');
		}

		cout << endl;
	}
}

/*
 * @brief open file and get filesize
 * @par filename
 * @par file_size
 * @ret ifstream
 */
ifstream open_file(const char *filename, streampos &file_size)
{
	ifstream file(filename, ios::binary);

	if (file) {
		file.seekg(0, ios::end);
		file_size = file.tellg();
		file.seekg(0, ios::beg);
	}
	return file;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}

	const char *filename = argv[1];
	streampos file_size;
	ifstream file = open_file(filename, file_size);

	if (!file) {
		cerr << "Error opening file: " << filename << endl;
		return 1;
	}

	vector<char> buffer(file_size);
	file.read(buffer.data(), file_size);

	oligodump(buffer.data(), buffer.size());

	return 0;
}

