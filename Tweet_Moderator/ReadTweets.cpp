// replace my comments with your own
// except: you can use my comment about one-word lookahead

// you may modify the code that I have supplied if you wish


using namespace std;

#include "ReadTweets.h"
#include <cctype>
#include <cstdlib>
#include <iostream>

string removePunct(string word)
{ // should return a copy of the word with all leading and trailing punctuation      
  // removed
  // punctuation elsewhere in the word should not be removed
  // note that a word could havce more than one leading/trailing punctuation symbols
  // you need to write this
    int start = 0, end = 0;
    for (string::iterator it = word.begin(); it != word.end(); ++it) {
    if (!ispunct(*it)) {
      break;
    }
    ++start;
    }
    if (start == word.size()) {
    return "";
    }
    for (string::reverse_iterator it = word.rbegin(); it != word.rend(); ++it) {
    if (!ispunct(*it)) {
      break;
    }
    ++end;
    }
    return word.substr(start, word.size() - start - end);
}

void ReadTweets::close() {
    wordfile.close();
}

ReadTweets::ReadTweets(const char *fname)
{   wordfile.open(fname);//open file
    if (!wordfile)
    {   cout << "Failed to open " << fname << endl;
        exit(1);
    }
    wordfile >> nextword;
    eoffound = false;
}

string ReadTweets::getNextWord()
{ // should return next word in file, converted to lower case
  // empty string should be returned if next word contains no letters
  
  // uses a one-word lookahead to avoid any problems relating to when if end-of-file
  // is detected due to absence/presence of newline at end of file

  // incomplete

    string word = nextword;
    wordfile >> nextword;
    if (wordfile.eof()) eoffound = true;
    word = removePunct(word);
    for (auto &c : word) {
    c = tolower(c);
    }
    // need to check that word contains a letter, and if not return an empty string;
    // also need to convert to lower case before returning
    return word;
}

bool ReadTweets::isNextWord()
{ return !eoffound;
}

bool ReadTweets::getTweets()
{ // you must write this
    // This function has been implemented in the main.cpp file.
    return false;
}

bool ReadTweets::getTweetsToShow()
{ // you must write this
    // This function has been implemented in the main.cpp file.
    return false;
}
