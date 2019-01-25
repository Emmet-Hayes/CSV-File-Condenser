#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void condenseFile(string);

int main(char ** argc, int argv) {
	string cppStr = argc[0];
	condenseFile(cppStr);
	system("pause");
	return 0;
}

void condenseFile(string fileName) {
	fstream inFile, outFile;
	inFile.open(fileName, 'r');
	outFile.open("condensed" + fileName, 'w');
	while (!inFile.eof()) {
		string checkMe;
		getline(inFile, checkMe);
		if (checkMe.find("Header") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), "!");
		} if (checkMe.find("Start_track") != string::npos) {
			checkMe.replace(checkMe.find("Start_track"), checkMe.length(), "@");
		} if (checkMe.find("Title_t") != string::npos) {
			checkMe.replace(checkMe.find("Title_t"), checkMe.length(), "#");
		} if (checkMe.find("Time_signature") != string::npos) {
			checkMe.replace(checkMe.find("Time_signature"), checkMe.length(), "$");
		} if (checkMe.find("Tempo") != string::npos) {
			checkMe.replace(checkMe.find("Tempo"), checkMe.length(), "%");
		} if (checkMe.find("End_track") != string::npos) {
			checkMe.replace(checkMe.find("End_track"), checkMe.length(), "^");
		} if (checkMe.find("Start_track") != string::npos) {
			checkMe.replace(checkMe.find("Start_track"), checkMe.length(), "&");
		} if (checkMe.find("Title_t") != string::npos) {
			checkMe.replace(checkMe.find("Title_t"), checkMe.length(), "*");
		} if (checkMe.find("System_exclusive") != string::npos) {
			checkMe.replace(checkMe.find("System_exclusive"), checkMe.length(), "x");
		} if (checkMe.find("Control_c") != string::npos) {
			checkMe.replace(checkMe.find("Control_c"), checkMe.length(), "y");
		} if (checkMe.find("Program_c") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), "z");
		} if (checkMe.find("Pitch_bend_c") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), "w");
		} if (checkMe.find("Note_on_c") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), ";");
		} if (checkMe.find("Note_off_c") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), ":");
		} if (checkMe.find("End_of_file") != string::npos) {
			checkMe.replace(checkMe.find("Header"), checkMe.length(), "?");
		}
		outFile.write(checkMe.c_str(), sizeof(checkMe.c_str()));
	}
	inFile.close();
	outFile.close();
}