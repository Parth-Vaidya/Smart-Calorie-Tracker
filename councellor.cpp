// counsellor_portal.cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>
#include <stdexcept>
#include <set> // ** NEW: Required to easily check for valid usernames **

using namespace std;

class Counsellor {
    string counsellorName;
    string username;
    string password;

public:
    Counsellor(string cname = "default", string uname = "user", string pass = "123")
        : counsellorName(cname), username(uname), password(pass) {}

    string getCounsellorName() const { return counsellorName; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

class Course {
public:
    static void saveCourse(const string& cat, int cal, int f, int p, int c, const string& desc, const string& cname) {
        ofstream out("courses.txt", ios::app);
        out << cat << "," << cal << "," << f << "," << p << "," << c << "," << desc << "," << cname << "\n";
        out.close();
    }
};

class CounsellorPortal {
    map<string, Counsellor> counsellors;

public:
    CounsellorPortal() {
        loadCredentials();
    }

    void loadCredentials() {
        ifstream in("counsellors.txt");
        if (!in) return;
        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string cname, uname, pass;
            getline(iss, cname, ',');
            getline(iss, uname, ',');
            getline(iss, pass);
            counsellors[uname] = Counsellor(cname, uname, pass);
        }
        in.close();
    }

    void registerCounsellor(const string& cname, const string& uname, const string& pass) {
        if (counsellors.count(uname)) {
            cout << "Username already exists.\n";
            return;
        }
        ofstream out("counsellors.txt", ios::app);
        out << cname << "," << uname << "," << pass << "\n";
        out.close();
        counsellors[uname] = Counsellor(cname, uname, pass);
        cout << "Counsellor registered successfully.\n";
    }

    Counsellor* login(const string& uname, const string& pass) {
        map<string, Counsellor>::iterator it = counsellors.find(uname);
        if (it != counsellors.end() && it->second.getPassword() == pass) {
            cout << "\nLogin successful. Welcome " << it->second.getCounsellorName() << "!\n";
            return &it->second;
        }
        cout << "\nInvalid username or password.\n";
        return nullptr;
    }

    // ** THIS FUNCTION IS NOW MODIFIED **
    void viewEnrolledUsers(const string& counsellorName) {
        ifstream in("enrollments.txt");
        if (!in) {
            cout << "No enrollment data found. (enrollments.txt is missing or empty)\n";
            return;
        }

        map<string, vector<string>> enrolledUsersByCourse;
        set<string> validUsernames; // Keep track of users enrolled with this counsellor
        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string courseCat, fileCounsellorName, enrolledUsername;
            
            getline(iss, courseCat, ',');
            getline(iss, fileCounsellorName, ',');
            getline(iss, enrolledUsername);

            if (fileCounsellorName == counsellorName) {
                enrolledUsersByCourse[courseCat].push_back(enrolledUsername);
                validUsernames.insert(enrolledUsername); // Add to the set for validation
            }
        }
        in.close();

        cout << "\n--- Users Enrolled in Your Courses ---\n";
        if (enrolledUsersByCourse.empty()) {
            cout << "No users have enrolled in your courses yet.\n";
            return;
        }

        for (map<string, vector<string>>::const_iterator it = enrolledUsersByCourse.begin(); it != enrolledUsersByCourse.end(); ++it) {
            cout << "Course: " << it->first << "\n";
            const vector<string>& users = it->second;
            for (size_t i = 0; i < users.size(); ++i) {
                cout << "  - User: " << users[i] << "\n";
            }
            cout << "-------------------------------------\n";
        }

        // ** NEW LOGIC: Prompt to view a specific user's summary **
        while (true) {
            cout << "\nEnter a username to view their summary (or type 'back' to return): ";
            string usernameToView;
            getline(cin, usernameToView);

            if (usernameToView == "back") {
                break;
            }

            // Security check: ensure the requested user is enrolled with THIS counsellor
            if (validUsernames.find(usernameToView) == validUsernames.end()) {
                cout << "Error: User '" << usernameToView << "' is not enrolled in your courses.\n";
                continue;
            }

            // Construct filename and try to display the contents
            string filename = usernameToView + "_summary.txt";
            ifstream summaryFile(filename.c_str());

            if (summaryFile.is_open()) {
                cout << "\n--- Displaying Summary for " << usernameToView << " ---\n";
                string summaryLine;
                while (getline(summaryFile, summaryLine)) {
                    cout << summaryLine << endl;
                }
                cout << "--- End of Summary ---\n";
                summaryFile.close();
            } else {
                cout << "Could not find a summary file for user '" << usernameToView << "'.\n";
            }
        }
    }
};

int main() {
    CounsellorPortal portal;

    while (true) {
        cout << "\n--- Counsellor Menu ---\n";
        cout << "1. Register\n2. Login\n3. Exit\nChoose: ";
        int choice;
        cin >> choice;
        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            string cname, uname, pass;
            cout << "Enter Counsellor Name: "; getline(cin, cname);
            cout << "Enter Username: "; getline(cin, uname);
            cout << "Enter Password: "; getline(cin, pass);
            portal.registerCounsellor(cname, uname, pass);
        } 
        else if (choice == 2) {
            string uname, pass;
            cout << "Enter Username: "; getline(cin, uname);
            cout << "Enter Password: "; getline(cin, pass);
            Counsellor* c = portal.login(uname, pass);
            if (c) {
                while (true) {
                    cout << "\n--- Counsellor Dashboard (" << c->getCounsellorName() << ") ---\n";
                    cout << "1. Create Course\n2. View Enrolled Users\n3. Logout\nChoose: ";
                    int cchoice;
                    cin >> cchoice;
                     if(cin.fail()){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }
                    cin.ignore();

                    if (cchoice == 1) {
                        int cal, f, p, carb; 
                        string cat, desc;
                        cout << "Enter Course Category (e.g., Weight Loss): "; getline(cin, cat);
                        cout << "Enter Target Calories: "; cin >> cal;
                        cout << "Enter Target Fats (g): "; cin >> f;
                        cout << "Enter Target Protein (g): "; cin >> p;
                        cout << "Enter Target Carbs (g): "; cin >> carb; 
                        cin.ignore();
                        cout << "Enter Course Description: "; getline(cin, desc);
                        
                        Course::saveCourse(cat, cal, f, p, carb, desc, c->getCounsellorName());
                        cout << "Course created successfully.\n";
                    } 
                    else if (cchoice == 2) {
                        portal.viewEnrolledUsers(c->getCounsellorName());
                    } 
                    else {
                        cout << "Logging out...\n";
                        break;
                    }
                }
            }
        } 
        else {
            cout << "Exiting program. Goodbye!\n";
            break;
        }
    }
    return 0;
}