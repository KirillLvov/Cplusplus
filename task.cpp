#include <iostream>
#include <fstream> 
#include <map>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

string run(string field, string command) {
	if(command.compare(string("u")) == 0)
		for_each(field.begin(), field.end(), [](char & c){c = ::tolower(c);});
	if(command.compare(string("U")) == 0)
		for_each(field.begin(), field.end(), [](char & c){c = ::toupper(c);});
	if(command[0] == 'R') {
		char first = command[1];
		char second = command[2];
		int pos = field.find(first);
		while(pos != string::npos) {
			field.replace(pos, 1, string(1,second));
			pos = field.find(first);
		}
	}
	return field;
}

int main(int argc, char* argv[]) {

	// A map which contains the number of field, which is needed to modify, as a key, and the queue of commands for this field, as a value   
	map<int, queue<string>> commands;
	int field_number, colon_pos;
	string str, command;

	// Iterate through main argument - commands
	for(int i=2; i<argc; i++) {
		str = string(argv[i]);
		colon_pos = str.find(':');
		// Get the number of the field
		field_number = stoi(str.substr(0,colon_pos));
		// Get a command name
		command = str.substr(colon_pos+1,str.length()-colon_pos);
		
		// Insert the command to the map
		auto search = commands.find(field_number);
		// If there is no such field number, create a new key-value pair 
		if (search == commands.end()) {
			queue<string> q;
			q.push(command);
			commands.insert(pair<int,queue<string>>(field_number,q));
		}
		// If there is such field number yet, just add a new command to corresponding queue
		else
			commands[field_number].push(command);
	}

	// Open file and read it line by line
	ifstream f;
	f.open(argv[1]);
	string line;
	int start_pos, tab_pos, curr_field_num;
	string field, new_field;
	while(getline(f, line)) {
		start_pos = 0;
		curr_field_num = 0;
		// Iterate through the fields number that we should modify in each line
		for(map<int,queue<string>>::iterator it = commands.begin(); it != commands.end(); it++) {
			// Skip a corresponding number of fields to come to the field we should modify
			for(int i=curr_field_num; i<it->first; i++) {
				tab_pos = line.find('\t', start_pos);
				start_pos = tab_pos+1;
			}
			tab_pos = line.find('\t', start_pos);
			// Get the field we should modify
			field = line.substr(start_pos, tab_pos-start_pos);
			// Apply a set of command on the field
			while (!it->second.empty()) {
				new_field = run(field, it->second.front());
				it->second.pop();
				field = new_field;
			}
			// Modify the line
			line.replace(start_pos, tab_pos-start_pos, new_field);
			// Go to the next iteration
			curr_field_num = it->first+1;
			start_pos = tab_pos+1;
		}
		// Print the modified line
		cout << line << endl;
	}
	f.close();
}
