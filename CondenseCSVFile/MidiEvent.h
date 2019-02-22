#pragma once
#ifndef _MIDI_EVENT_H
#define _MIDI_EVENT_H
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

const string x[TYPE_COUNT] = { "Header", "Start_track", "Title_t", "Copyright_t", "Copyright by", "Text_t",
"SMPTE_offset", "Time_signature", "Key_signature", "Tempo", "Instrument_name_t", "End_track", "System_exclussive",
"Control_c", "Program_c", "Pitch_bend_c", "MIDI_port", "Marker_t", "Note_on_c", "Note_off_c", "End_of_file" };
const string y[TYPE_COUNT] = { "<", "/", "~", "(", ")", "|", "!", "$", "#", "%", "=", "\\", "?", "&", ";", "+", "`", "^", "{", "}", ">" };
const int TYPE_COUNT = 21, TRACK_LIMIT = 20, TIME_LIMIT = 320000, TEMPO_LIMIT = 1000000;

class MidiEvent {
protected:
	int track, time;
	string type;
public:
	MidiEvent() {
		track = 0, time = 0, type = "NO TYPE";
		validateEventParameters();
	}
	MidiEvent(string s) : MidiEvent() {
		stringstream ss(s);
		string sTrack, sTime, sType;
		try {
			ss >> sTrack, ss >> sTime, ss >> sType;
			track = stoi(sTrack), time = stoi(sTime), type = sType;
			validateEventParameters();
		}
		catch (invalid_argument) {
			cout << "invalid argument: " << s << "\n";
		}
		catch (out_of_range) {
			cout << "out of range: " << s << "\n";
		}
	}
	virtual string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + "\n";
	}
	virtual string toCondensedString() {
		return to_string(static_cast<char>(track - '0')) + ", " + to_string(time)
			+ ", " + type + "\n";
	}
	virtual void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[1] && type != y[11] && type != y[20]) type = "NO TYPE";
	}
};
class TextEvent : public MidiEvent {
private:
	string text = "t"; //Title_t, Copyright_t, Instrument_name_t, Marker_t, Cue_point_t etc
public:
	TextEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sText, sNumber;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sText;
		track = stoi(sTrack), time = stoi(sTime), type = sType; // text = sText;
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + text + "\"\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track - '0')) + ", " + to_string(time)
			+ ", " + type + " " + text + "\"\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[2] && type != y[3] && type != y[5] && type != y[10] && type != y[17]) type = "NO TYPE";
	}
};
class NumberEvent : public MidiEvent {
private:
	int number; //Sequence_number, MIDI_port, Channel_prefix, Tempo
public:
	NumberEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sText, sNumber;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sNumber;
		track = stoi(sTrack), time = stoi(sTime), type = sType, number = stoi(sNumber);
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + to_string(number) + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track - '0')) + ", " + to_string(static_cast<char>(time - '0')) + ", " + type + " " + to_string(number) + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[9]) type = "NO TYPE";
		if (number < 0 || number > TEMPO_LIMIT) number %= TEMPO_LIMIT;
	}

};
class TimeEvent : public MidiEvent {
private:
	int timesig[4];
public:
	TimeEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sNum, sDenom, sClick, sNotesQ;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sNum, ss >> sDenom, ss >> sClick, ss >> sNotesQ;
		track = stoi(sTrack), time = stoi(sTime), type = sType, timesig[0] = stoi(sNum), timesig[1] = stoi(sDenom), timesig[2] = stoi(sClick), timesig[3] = stoi(sNotesQ);
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + to_string(timesig[0]) + ", " + to_string(timesig[1]) + ", " + to_string(timesig[2]) + ", " + to_string(timesig[3]) + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track)) + ", " + to_string(time) + ", " + type + " " + to_string(timesig[0]) + ", " + to_string(timesig[1]) + ", " + to_string(timesig[2]) + ", " + to_string(timesig[3]) + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[7]) type = "NO TYPE";
		if (timesig[0] < 0 || timesig[0] > 64) timesig[0] %= 64;
		if (timesig[1] < 2 || timesig[1] > 64) timesig[1] = (timesig[1] * 2) % 64;
		if (timesig[2] < 0 || timesig[2] > 256) timesig[2] %= 256;
		if (timesig[3] < 0 || timesig[3] > 256) timesig[3] %= 256;
	}
};
class KeyEvent : public MidiEvent {
private:
	int key;
	string third = "d";
public:
	KeyEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sKey, sPositive;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sKey, ss >> sPositive;
		track = stoi(sTrack), time = stoi(sTime), type = sType, key = stoi(sKey);
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + to_string(key) + ", " + third + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track)) + ", " + to_string(time) + ", " + type + " " + to_string(key) + ", " + third + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[8]) type = "NO TYPE";
		if (key < -7 || key > 7) key = (key % 15) - 7;
	}
};
class SmpteEvent : public MidiEvent {
private:
	int smpte[5];
public:
	SmpteEvent(string s) : MidiEvent() {
		stringstream ss(s);
		string sTrack, sTime, sType, sHour, sMin, sSec, sFrame, sFracframe;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sHour, ss >> sMin, ss >> sSec, ss >> sFrame, ss >> sFracframe;
		track = stoi(sTrack), time = stoi(sTime), type = sType, smpte[0] = stoi(sHour), smpte[1] = stoi(sMin), smpte[2] = stoi(sSec), smpte[3] = stoi(sFrame), smpte[4] = stoi(sFracframe);
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " +
			type + " " + to_string(smpte[0]) + ", " + to_string(smpte[1]) + ", " + to_string(smpte[2]) + ", " + to_string(smpte[3]) + ", " + to_string(smpte[4]) + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track)) + ", " + to_string(time) + ", " +
			type + " " + to_string(smpte[0]) + ", " + to_string(smpte[1]) + ", " + to_string(smpte[2]) + ", " + to_string(smpte[3]) + ", " + to_string(smpte[4]) + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT;
		if (type != y[6]) type = "NO TYPE";
	}
};
class ChannelEvent : public MidiEvent {
private:
	int channel[3];
public:
	ChannelEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sChannel, sNote, sVelocity;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sChannel, ss >> sNote, ss >> sVelocity;
		track = stoi(sTrack), time = stoi(sTime), type = sType, channel[0] = stoi(sChannel), channel[1] = stoi(sNote), channel[2] = stoi(sVelocity);
		validateEventParameters();
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " +
			type + " " + to_string(channel[0]) + ", " + to_string(channel[1]) + ", " + to_string(channel[2]) + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track)) + ", " + to_string(time) + ", " +
			type + " " + to_string(channel[0]) + ", " + to_string(channel[1]) + ", " + to_string(channel[2]) + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT + 1;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT + 1;
		if (type != y[0] && type != y[13] && type != y[15] && type != y[18] && type != y[19]) type = "NO TYPE";
		if (channel[0] < 0 || channel[0] > TRACK_LIMIT) channel[0] %= TRACK_LIMIT + 1;
		if (channel[1] < 0 || channel[1] > CHAR_MAX) channel[1] %= CHAR_MAX + 1;
		if (channel[2] < 0 || channel[2] > CHAR_MAX) channel[2] %= CHAR_MAX + 1;
	}
};
class TwoChanEvent : public MidiEvent {
private:
	int channel[2];
public:
	TwoChanEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sChannel, sValue;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sChannel, ss >> sValue;
		track = stoi(sTrack), time = stoi(sTime), type = sType, channel[0] = stoi(sChannel), channel[1] = stoi(sValue);
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + to_string(channel[0]) + ", " + to_string(channel[1]) + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track + '0')) + ", " + to_string(time) + ", " + type + " " + to_string(channel[0]) + ", " + to_string(channel[1]) + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT + 1;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT + 1;
		if (type != y[12]) type = "NO TYPE";
		if (channel[0] < 0 || channel[0] > 128) channel[0] %= 128;
		if (channel[1] < 0 || channel[1] > 128) channel[1] %= 128;
	}
};
class SysExEvent : public MidiEvent {
private:
	string data;
	int length;
public:
	SysExEvent(string s) {
		stringstream ss(s);
		string sTrack, sTime, sType, sLength, sData;
		ss >> sTrack, ss >> sTime, ss >> sType, ss >> sLength, ss >> sData;
		track = stoi(sTrack), time = stoi(sTime), type = sType, length = stoi(sLength), data = sData;
	}
	string backToString() {
		return to_string(track) + ", " + to_string(time) + ", " + type + " " + to_string(length) + ", " + data + "\n";
	}
	string toCondensedString() {
		return to_string(static_cast<char>(track + '0')) + ", " + to_string(time) + ", " + type + " " + to_string(length) + ", " + data + "\n";
	}
	void validateEventParameters() {
		if (track < 0 || track > TRACK_LIMIT) track %= TRACK_LIMIT + 1;
		if (time < 0 || time > TIME_LIMIT) time %= TIME_LIMIT + 1;
		if (type != y[14]) type = "NO TYPE";
	}
};
#endif