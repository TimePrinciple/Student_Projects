/**
 * ReadTweets Interface for CE221 assignment 2
 */

#ifndef READTWEETS_H
#define READTWEETS_H
/**
 * ReadTweets class. Provides mechanisms to read a text file, and return
 * lower-case versions of words from that file.
 */
using namespace std;

#include <string>
#include <fstream>
#include <set>
#include <map>
#include "User.h"
 class ReadTweets
 {
   public:

    /**
     * Constructor. Opens the file with the given filename and associates in
     * with the wordfile stream.
     * Initailises the other members.
     * Prints an error message then terminates the program if thr file cannot be opened.
     * Must not perform any other processing
     * @param filename - a C string naming the file to read.
     */
     ReadTweets(const char *filename);

    /**
     * Closes the file.
     */
     void close();

    /**
     * Returns a string, being the next word in the file. 
     * All letters should be converted to lower case
     * Leading and trailing punctuation symbols should not be included in the word
     * but punctuation elsewhere should not be removed
     * Returns an empty string if next word contains no letters
     * @return - string - next word.
     */
     string getNextWord();

    /**
     * Returns true if there is a further word in the file, false if we have reached the
     * end of file.
     * @return - bool - !eof
     */
     bool isNextWord();

     /**
      * This method should get all tweets based on a user registration number
      * the return type is bool so that true means the function was succesfully executed
      * and false otherwise. Tweets should be stored in the Tweets set.
      **/

     bool getTweets();

     /**
      * The method getTweetsToShow should select randomly five tweets from the file and store them in the set 
      * private member array tweets2show. The return type is bool so that true means the function was succesfully 
      * executed and false otherwise
      * */
     bool getTweetsToShow();

   private:
     ifstream wordfile;
     string nextword;
     bool eoffound;
     set<string> tweets2show;
     multimap<User,string> tweets; //the tweets can also be another object
 };

 #endif
