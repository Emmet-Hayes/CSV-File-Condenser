/*Author: @Emmet Hayes
For usage with DemoAI's Text generator
Long Short Term Memory Recurrent Neural Network.
To generate music with the LSTM:
Step 1: Download MIDI files from anywhere on the internet
Step 2: Use the Midi to CSV executable to create CSV text files
Step 3: run this program to make condensed CSV text files, it
  will dramatically reduce the file size and improve the LSTM's ability to learn.
Step 4: train the Text Generator on a text file with all the condensed midi information
Step 5: run this program again to expand the generated text into a proper CSV file
Step 6: Use the CSV to Midi executable to create a new Midi file
Step 7: Use WMP or any other method to change midi to wav
This program strips a CSV file of unnecessarily long messages like note_on_c
and replaces them with symbolic characters that can later be re-expanded.
Here is a conversion table to illustrate:
Header == <            | Start_track == /   | Title_t == ~            | Copyright_t == (
Copyright by == )      | Text_t == |        | SMPTE_offset == !       | Time_signature == $
Key_signature == #     | Tempo == %         | Instrument_name_t == =  | End_track == \
System_exclusive == ?  | Control_c == &     | Program_c == ;          | Pitch_bend_c == +
MIDI_port == `         | Marker_t == ^      | Note_on_c == {          | Note_off_c == }
End_of_file == >       |
*/
#include <fstream>
#include <vector>
#include "MidiEvent.h"
using namespace std;

void condenseFile(string, vector<MidiEvent *>&);
void expandFile(string, vector<MidiEvent *>&);
void MergeSort(vector<string> &);
void Merger(vector<string> &, int, int, int);
void MergeSortHelper(vector<string> &);
int CompareData(const string &, const string &);
MidiEvent * createEvent(string);

int main(char ** argc, int argv) {
	vector<MidiEvent *> eventList;
	vector<MidiEvent *>::iterator mIt;
	string userChoice;
	cout << "If you'd like to condense a MIDI file type c.\n"
		<< "if you'd like to expand a MIDI file type e.\n";
	cin >> userChoice;
	string fn;
	cout << "enter the files name (no extension needed.): ";
	cin >> fn;
	string filePath = "converted_miditocsv/" + fn + ".txt";
	if (userChoice[0] == 'c')
		condenseFile(filePath, eventList);
	else if (userChoice[0] == 'e')
		expandFile(filePath, eventList);
	cout << "110 becomes: " << static_cast<char>(110 - '0') << endl;
	system("pause");

	return 0;
}

void condenseFile(string fileName, vector<MidiEvent *> &eventList) {
	ifstream inFile(fileName, ios::in);
	string ss = "./condensedCSVfiles" + fileName.substr(19);
	ofstream outFile(ss, ios::out);
	int lineNum = 0;
	cout << ss << endl;
	while (!inFile.eof()) {
		string checkMe;
		getline(inFile, checkMe);
		for (int i = 0; i < TYPE_COUNT; ++i)
			if (checkMe.find(x[i]) != string::npos)
				checkMe.replace(checkMe.find(x[i]), x[i].length(), y[i]);
		++lineNum;
		eventList.push_back(createEvent(checkMe));
	}
	inFile.close();
	cout << "Total Lines Read: " << lineNum + 1 << endl;
	int c = 0;
	for (vector<MidiEvent *>::iterator i = eventList.begin();
		i != eventList.end(); ++i, ++c) {
		MidiEvent * temp = *i;
		outFile << temp->backToString();
		if (c % 200 == 0) cout << temp->toCondensedString();
	}
	outFile.close();
}

void expandFile(string fileName, vector<MidiEvent *> &eventList) {
	ifstream inFile(fileName, ios::in);
	string ss = "./expandedCSVfiles" + fileName.substr(19);
	ofstream outFile(ss.c_str(), ios::out);
	vector<string> fileData;
	int lineNum = 0;
	cout << ss << endl;
	while (!inFile.eof()) {
		string checkMe;
		getline(inFile, checkMe);
		for (int i = 0; i < TYPE_COUNT; ++i)
			if (checkMe.find(y[i]) != string::npos)
				checkMe.replace(checkMe.find(y[i]), y[i].length(), x[i]);
		fileData.push_back(checkMe + "\n");
		++lineNum;
		eventList.push_back(createEvent(checkMe));
	}
	cout << "Total Lines Read: " << lineNum + 1 << endl;
	cout << "Sorting. please wait...\n";
	MergeSort(fileData);
	for (int i = 0; i < fileData.size(); ++i)
		outFile << fileData[i];
	inFile.close();
	outFile.close();
}

int CompareData(const string& A, const string& B) {
	stringstream ssA(A), ssB(B);
	string sTrackA, sTimeA, sTrackB, sTimeB;
	try {
		ssA >> sTrackA, ssA >> sTimeA, ssB >> sTrackB, ssB >> sTimeB;
		if (stoi(sTrackA) < stoi(sTrackB)) return 1;
		if (stoi(sTrackA) == stoi(sTrackB)) {
			if (stoi(sTimeA) < stoi(sTimeB)) return 1;
			else return 0;
		}
		else return 0;
	}
	catch (invalid_argument) {
		cout << "invalid argument found. ceasing function. (-1)\n";
		return (-1);
	}
	catch (out_of_range) {
		cout << "out of range. ceasing function. (-1)\n";
		return (-2);
	}
	return (A <= B);
}

MidiEvent * createEvent(string s) {
	stringstream ss(s);
	int typecheck;
	string sTrack, sTime, sType;
	MidiEvent * event = nullptr;
	try {
		ss >> sTrack;
		ss >> sTime;
		ss >> sType;
		typecheck = stoi(sTrack);
		typecheck = stoi(sTime);
		for (int i = 0; i < TYPE_COUNT; ++i) {
			if (sType.find(x[i]) != string::npos || sType.find(y[i]) != string::npos) {
				if (i == 1 || i == 11 || i == 20)
					event = new MidiEvent(s);
				else if (i == 0 || i == 13 || i == 15 || i == 18 || i == 19)
					event = new ChannelEvent(s);
				else if (i == 2 || i == 3 || i == 5 || i == 10 || i == 17)
					event = new TextEvent(s);
				else if (i == 6)
					event = new SmpteEvent(s);
				else if (i == 7)
					event = new TimeEvent(s);
				else if (i == 8)
					event = new KeyEvent(s);
				else if (i == 9)
					event = new NumberEvent(s);
				else if (i == 9 || i == 14 || i == 16)
					event = new TwoChanEvent(s);
				else if (i == 12)
					event = new SysExEvent(s);
			}
		}
	}
	catch (invalid_argument) {
		cout << "invalid argument: " << s << "\n";
		event = new MidiEvent(s);
	}
	catch (out_of_range) {
		cout << "out of range: " << s << "\n";
		event = new MidiEvent(s);
	}
	return event;
}

void Merger(vector<string> & vec, int lo, int  mi, int hi) {
	string * temp = new string[hi - lo + 1];//temporary merger array
	int i = lo, j = mi + 1;//i is for left-hand,j is for right-hand
	int k = 0;//k is for the temporary array
	stringstream ss;
	while (i <= mi && j <= hi) {
		if (CompareData(vec[i], vec[j]) > 0) temp[k++] = vec[i++];
		else temp[k++] = vec[j++];
	}
	while (i <= mi) //rest elements of left-half
		temp[k++] = vec[i++];
	while (j <= hi) //rest elements of right-half
		temp[k++] = vec[j++];
	for (k = 0, i = lo; i <= hi; ++i, ++k) //copy the mergered temporary array to the original array
		vec[i] = temp[k];
	delete[]temp;
}

void MergeSortHelper(vector<string> & vec, int lo, int hi) {
	int mid;
	if (lo < hi) {
		mid = (lo + hi) >> 1;
		MergeSortHelper(vec, lo, mid);
		MergeSortHelper(vec, mid + 1, hi);
		Merger(vec, lo, mid, hi);
	}
}

void MergeSort(vector<string> & vec) {
	MergeSortHelper(vec, 0, vec.size() - 1);
}