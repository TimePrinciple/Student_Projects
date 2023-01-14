#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "ReadTweets.h"
#include "Manager.h"
#include "User.h"
#include <sstream>
using namespace std;

enum Field {
    UserID,
    TweetID,
    content,
    Date,
    Time
};

bool Time_reached = true;
Field prev_state = Field::Time;
Field parse_word (string &word) {
    // A three-bit bit map
    // all_number | has_two_dashes | has_two_colons
    // Then 100 is UserID or TweetID
    // 110 is Date and 101 is Time.
    int bit_map = 0;
    int dashes = 0, colons = 0;
    bool is_content = false;
    for (char &c : word) {
        if (!isdigit(c)) {
            if (c == '-') {
                ++dashes;
            } else if (c == ':') {
                ++colons;
            } else {
                return Field::content;
            }
        }
    }
    // If the loop completed without returning, then the
    // word is not content.
    // Switch the all_number bit on.
    bit_map += 4;
    if (dashes == 2) {
        bit_map += 2;
    }
    if (colons == 2) {
        bit_map += 1;
    }
    
    // If Time is the previously parsed, the next all_number
    // word is UserID.
    if (bit_map == 4 && prev_state == Field::Time) {
        prev_state = Field::UserID;
        return Field::UserID;
    } else if (bit_map == 6 && word.size() == 10) {
        prev_state = Field::Date;
        return Field::Date;
    } else if (bit_map == 5 && prev_state == Field::Date) {
        prev_state = Field::Time;
        return Field::Time;
    }

    return Field::content; 
}

void clear_screen() {
    cout << string(100, '\n');
}

template <class T>
bool login(T &handler) {
    if (is_same<T, User>::value) {
        return true;
    } else {
        return false;
    }
    return true;
}

bool cmp(const pair<string, float> &a, const pair<string, float> &b) {
    return a.second < b.second;
}

int main () {
    // Load user info from user.txt file.
    cout << "Loading infomation..." << endl;
    ifstream user_info;
    user_info.open("users.txt");
    if (!user_info.is_open()) {
        cout << "Error encoutered, specified file might not exist." << endl;
        cout << "Exiting..." << endl;
        return 1;
    }
    
    // Login module.
    cout << "-----------------------------" << endl;
    cout << "  Twitter Content Moderator" << endl;
    cout << "If you have any question, or any further request, please contact [farronwatcher@outlook.com]" << endl;
    cout << "-----------------------------" << endl;
    cout << "Please input your User Name:" << endl;
    cout << "-----------------------------" << endl;
    string user_input;
    getline(cin, user_input);
    string line;
    bool info_found = false;
    while (!user_info.eof()) {
        getline(user_info, line);
        if (line.find(user_input) != string::npos) {
            info_found = true;
            break;
        }
    }
    // Close the file.
    user_info.close();

    // Verify the user input.
    int id;
    float accumulative_score = 0, average_score = 0;
    bool is_mgr = false;
    if (info_found) {
        istringstream ss(line);
        string first_name, last_name; 
        ss >> id >> first_name >> last_name;
        if (!ss.eof()) {
            // Field remains, User.
            ss >> accumulative_score >> average_score;
            cout << "--------------------------------" << endl;
            cout << "User: " << user_input << " Verified." << endl;
        } else {
            // Field read up, Manager.
            is_mgr = true;
            cout << "--------------------------------" << endl;
            cout << "Manager: " << user_input << " Verified." << endl;
        }
        cout << "Welcome, logining you in..." << endl;
    } else {
        cout << "Information not found, creating new user..." << endl;
        id = rand() % 100000000;
        string first_name, last_name;
        ofstream info;
        info.open("./users.txt", ios_base::app);
        info << id << " " << user_input << " " << 0 << " " << 0 << endl;
        cout << "--------------------------------" << endl;
        cout << "User created, logining you in..." << endl;
        info.close();
    }

    // Construct the handler using different constructor.
    // Login the operator.
    bool mode;
    if (is_mgr) {
        Manager handler = Manager(user_input, id);
        mode = login<Manager>(handler);
    } else {
        User handler = User(user_input, id);
        mode = login<User>(handler);
    }

    if (mode) {
        // Pre-load stage, load and parse the file into the memoey.
        vector<string> UserIDs;
        vector<string> TweetIDs;
        vector<string> contents;
        vector<string> Dates;
        vector<string> Times;

        vector<string> blocked_list;
        ifstream ifs;
        ifs.open("./blocked_content.txt");
        while (!ifs.eof()) {
            string word;
            ifs >> word;
            blocked_list.emplace_back(word);
        }
        ifs.close();

        // Construct an input stream use the sample file, and collect
        // them accordingly.
        ReadTweets reader = ReadTweets("./tweets_sample.txt");
        int i = 0;
        bool error_encoutered = false;
        bool modified;
        while (reader.isNextWord()) {
            string word = reader.getNextWord();
            switch (parse_word(word)) {
                case Field::UserID: {
                    modified = false;
                    UserIDs.emplace_back(word);
                    word = reader.getNextWord();
                    TweetIDs.emplace_back(word);
                    break;
                }
                case Field::content: {
                    // Pre-process the word by blocked list.
                    if (find(blocked_list.begin(), blocked_list.end(), word) != blocked_list.end()) {
                        string w;
                        for (int i = 0; i < word.size(); ++i) {
                            w += "#";
                        }
                        // Moderate the content to a string of '#'s.
                        word = w;
                        modified = true;
                    }
                    if (contents.size() == i) {
                        contents.emplace_back(word);
                    } else {
                        contents[i] = contents[i] + " " + word;
                    }
                    break;
                }
                case Field::Date: {
                    if (modified) {
                        contents[i] += " #moderatedtweet";
                    }
                    Dates.emplace_back(word);
                    break;
                }
                case Field::Time: {
                    Times.emplace_back(word);
                    ++i;
                    break;
                }
                default: {
                    cout << "Error encoutered" << endl;
                    error_encoutered = true;
                }
            };
            if (error_encoutered) break;
        }
        reader.close();

        // Information loaded, start printing the control panel.
        clear_screen();
    user_tag:
        cout << "-----------------------------------------------" << endl;
        cout << "Welcome! User: " << user_input << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "1) Read tweet" << endl;
        cout << "2) Write tweet" << endl;
        cout << "3) Your statistics" << endl;
        cout << "4) Display top 10 most dangerous users" << endl;
        cout << "5) Exit game" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Type the number precedes the option to perfrom:" << endl;
        int option;
        cin >> option;
        clear_screen();
        cout << "-----------------------------------------------" << endl;
        bool quit = false;
        switch (option) {
            case 1: {
                int r = rand() % contents.size();
                r = 18;
                cout << "Dispalying the " << r << "th tweet: " << endl;
                cout << "UserID: " << UserIDs[r] << endl;
                cout << "TweetID: " << TweetIDs[r] << endl;
                cout << "Content: " << contents[r] << endl;
                cout << "Date: " << Dates[r] << endl;
                cout << "Time: " << Times[r] << endl;
                cout << "-----------------------------------------------" << endl;
                break;
            }
            case 2: {
                cout << "Content: ";
                string c;
                getchar();
                getline(cin, c);
                cout << "Date: ";
                string d;
                cin >> d;
                cout << "Time: ";
                string t;
                cin>> t;
                cout << "Sending tweet..." << endl;
                ofstream f;
                f.open("./tweets_sample.txt", ios_base::app);
                f << id << "\t" << rand() % 1000 << "\t" << c << "\t" << d << "\t" << t << endl;
                f.close();
                break;
            }
            case 3: {
                cout << "Displaying your statistics: " << endl;
                cout << "-----------------------------------------------" << endl;
                cout << "Scores per tweet: " << endl;
                string id_string = to_string(id);
                for (int i = 0; i < UserIDs.size(); ++i) {
                    if (UserIDs[i] == id_string) {
                        cout << "-----------------------------------------------" << endl;
                        cout << contents[i] << endl << Dates[i] << endl << Times[i] << endl;
                        string c = contents[i];
                        int blured_words = 0;
                        int n = 0;
                        istringstream iss(c);
                        while (!iss.eof()) {
                            string w;
                            iss >> w;
                            if (w[0] == '#') {
                                ++blured_words;
                            }
                            ++n;
                        }
                        cout << "Scores of this tweet: " << 10 * ((n - blured_words) / float(n)) << endl;
                    }
                }
                cout << "-----------------------------------------------" << endl;
                cout << "Totol score: " << accumulative_score << endl;
                cout << "Average score: " << average_score << endl;
                break;
            }
            case 4: {
                ifstream ifs;
                ifs.open("./users.txt");
                vector<pair<string, float>> rank;
                while (!ifs.eof()) {
                    string line;
                    getline(ifs, line);
                    cout << line << endl;
                    istringstream iss(line);
                    string _, fi, la;
                    float ave_s;
                    iss >> _ >> fi >> la;
                    if (iss.eof()) {
                        continue;
                    }
                    iss >> ave_s >> ave_s;
                    rank.emplace_back(pair<string, float>(fi + la, ave_s));
                }
                ifs.close();
                sort(rank.begin(), rank.end(), cmp);
                cout << "Dispalying the Top 10 dangerous users" << endl;
                cout << "-----------------------------------------------" << endl;
                for (auto &e : rank) {
                    cout << "User name: " << e.first << "    Average score: " << e.second << endl;
                }
                cout << "-----------------------------------------------" << endl;
                break;
            }
            case 5: {
                quit = true;
                break;
            }
        }
        if (!quit) {
            goto user_tag;
        } else {
            cout << "Bye!" << endl;
        }
    } else {
    clear_screen();
    manager_tag:
        cout << "-----------------------------------------------" << endl;
        cout << "Welcome! Manager: " << user_input << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "1) Reset moderation content" << endl;
        cout << "2) Add blocked content" << endl;
        cout << "3) Add another manager" << endl;
        cout << "4) Set the sample file" << endl;
        cout << "5) Exit game" << endl;
        cout << "6) Print blocked content list" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Type the number precedes the option to perfrom:" << endl;
        int option;
        cin >> option;
        clear_screen();
        cout << "-----------------------------------------------" << endl;
        bool quit = false;
        switch (option) {
            case 1: {
                ofstream ofs;
                ofs.open("./blocked_content.txt");
                ofs << " ";
                cout << "Content has been reset." << endl;
                ofs.close();
                break;
            }
            case 2: {
                ofstream ofs;
                ofs.open("./blocked_content.txt", ios_base::app);
                cout << "Type the word you want to add: " << endl;
                string word;
                cin >> word;
                ofs << " " << word;
                ofs.close();
                break;
            }
            case 3: {
                ofstream ofs;
                ofs.open("./users.txt", ios_base::app);
                cout << "Type the User name of Manager to be added: " << endl;
                string word;
                getline(cin, word);
                ofs << rand() % 20 << " " << word << endl;
                ofs.close();
                break;
            }
            case 4: {
                string f;
                cout << "Type the name of file you want to change to: " << endl;
                cin >> f;
                cout << "File set!" << endl;
                break;
            }
            case 5: {
                quit = true;
                break;
            }
            case 6: {
                ifstream ifs;
                ifs.open("./blocked_content.txt");
                cout << "Printing the blocked cotent list: " << endl;
                cout << "-----------------------------------------------" << endl;
                while (!ifs.eof()) {
                    string word;
                    ifs >> word;
                    cout << word << endl;
                }
                ifs.close();
                break;
            }
        }
        if (!quit) {
            goto manager_tag;
        } else {
            cout << "Bye!" << endl;
        }
    }

    return 0;
}
