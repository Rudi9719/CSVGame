#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct level {
  int id;
  string title;
  string body;
  int choiceCount;
  vector<int> choiceIDs;
  vector<string> choices;
}lvl;

// Separator for 'csv'
string separator = ",,";
// Global game_csv is the file to load
string game_csv;

// load level for int
level GotoLevel(int);

int main(int argc, char **argv) {
  // Initialize nextLevel to 0 (load first level)
  int next_level = 0;
  game_csv = "/sys/sdf/share/csvgame/story.csv";
  for (int i = 1; i < argc; i++) {
    string test_arg = string(argv[i]);
    if (test_arg == "-f") {
      if (argc > i+1) {
	game_csv = string(argv[i+1]);
      }
    }
    if (test_arg == "-l") {
      if (argc > i+1) {
	next_level = atoi(argv[i+1]);
      }
    }
  }

  // Setup the level from next_level
  level  l = GotoLevel(next_level);
  while(l.choiceIDs.size() > 0) {
    // Display the level info
    cout << endl << l.id << "\t" << l.title << endl << endl << l.body << endl <<endl;
    // Loop through choiceID vector size and print IDs and options
    for (int i = 0; i < l.choiceIDs.size(); i++) {
      cout << l.choiceIDs.at(i) << ": " << l.choices.at(i) << endl;
    }
    do {
      // accept in the next level
      cout << "Goto page: ";
      cin >> next_level;
    } while (!count(l.choiceIDs.begin(), l.choiceIDs.end(), next_level));
    l = GotoLevel(next_level);
  }
}

level GotoLevel(int num) {
  // Read file, from the global game_csv converted to a C string (const char array)
  fstream file(game_csv.c_str());
  // Go to the beginning of the file
  file.seekg(ios::beg);
  // skip to the line we want
  for(int i = 0; i < num; i++) {
    // ignore the crap we don't want
    file.ignore(numeric_limits<streamsize>::max(),'\n');
  }
  // make a new level
  level l;
  // make a new line
  string line;
  // read what we have from the file, to the string line
  getline(file, line);

  //LevelID,Title,Body,ChoiceCount,ChoiceID1,....ChoiceIDN,Choice1,....ChoiceN
  l.id = atoi(line.substr(0, line.find(separator)).c_str());
  line.erase(0, line.find(separator) + separator.length());

  l.title = line.substr(0, line.find(separator));
  line.erase(0, line.find(separator) + separator.length());;

  l.body = line.substr(0, line.find(separator));
  line.erase(0, line.find(separator) + separator.length());

  l.choiceCount = atoi(line.substr(0, line.find(separator)).c_str());
  line.erase(0, line.find(separator) + separator.length());

  for(int i = 0; i < l.choiceCount; i++) {
    int choice = atoi(line.substr(0, line.find(separator)).c_str());
    l.choiceIDs.push_back(choice);
    line.erase(0, line.find(separator) + separator.length());
  }
  for(int i = 0; i < l.choiceCount; i++) {
    string choice = line.substr(0, line.find(separator));
    l.choices.push_back(choice);
    line.erase(0, line.find(separator) + separator.length());
  }
  return l;
}
