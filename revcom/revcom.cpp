/* @file revcom.cpp
 * @author Ryan David Najac <rdn2108>
 * @brief simple function to get the reverse an complement of an DNA sequence.
 */
#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

/*
 * @brief format printing results with this function
 * @par label
 * @par start_time
 * @par end_time
 * @ Returns
 *  Nothing
 */
void print_data(const string& label,
               const chrono::high_resolution_clock::time_point& start_time,
               const chrono::high_resolution_clock::time_point& end_time)
{
	auto dur = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
	cout << left << setw(20) << label  << dur.count() << "ms\n";
}

/*
 * @brief calculate the reverse complement of a DNA sequence
 * @par dna
 * @ret string
 */
string revcom(const string& dna) {
	string oligo = dna;

	for (char& base : oligo) {
		switch (base) {
		case 'A': base = 'T'; break;
		case 'T': base = 'A'; break;
		case 'C': base = 'G'; break;
		case 'G': base = 'C'; break;
		}
	}
	reverse(oligo.begin(), oligo.end());
	return oligo;
}

int main(int argc, char *argv[])
{	
	string sequence;

	/* set up timers */
	chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point end_time = chrono::high_resolution_clock::now();
	
	cout << "Enter a DNA sequence: ";
	cin >> sequence;
	start_time = chrono::high_resolution_clock::now();
	string reverse_complement = revcom(sequence);
	end_time = chrono::high_resolution_clock::now();
	
	cout << endl;
	cout << left << setw(20) << "DNA Sequence: " << sequence << endl;
	cout << left << setw(20) << "Reverse Complement: " << reverse_complement << endl;
	print_data("revcom", start_time, end_time);

	return 0;
}

