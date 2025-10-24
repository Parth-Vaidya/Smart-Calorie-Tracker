#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include <cctype>
#include <limits>
#include <ctime>
#include <queue>
#include <set>

using namespace std;


// NEW: Base Class 'Account'
class Account {
protected: // Accessible by derived classes like User and Counsellor
    string username;
    string password;

public:
    Account(string u, string p) : username(u), password(p) {}

    string getUsername() const {
        return username;
    }

    string checkPassword() const {
        return password;
    }

    // A pure virtual function. This makes Account an "abstract class".
    // Any class that inherits from Account MUST provide its own version of this function.
    virtual void displayDashboard() const = 0;

    // NEW: Virtual destructor is good practice for base classes.
    virtual ~Account() {}
};


//User now inherits from Account
class User : public Account {
public:
    // The constructor now calls the base class constructor
    User(string u, string p) : Account(u, p) {}

    // MODIFIED: Providing the required implementation for the virtual function.
    // The 'override' keyword is good practice to ensure we are correctly overriding a base class function.
    void displayDashboard() const override {
        cout << "\n--- Welcome to the User Dashboard, " << getUsername() << "! ---\n";
        cout << "Here you can track meals and enroll in courses.\n";
    }
};


struct CourseInfo {
    string category;
    string counsellorName;
    string fullDetails;
};

// Forward declaration of Tracker is needed for the friend function prototype
class Tracker;

// NEW: Prototype for our friend function
void printDetailedSummary(const Tracker& tracker);

class Tracker {
private:
    string currentUser;
    map<string, vector<int>> foodDatabase;
    map<string, vector<int>> dailyIntake;

public:
    
    // OOP CONCEPT 2: FRIEND FUNCTION
    // NEW: This line gives the printDetailedSummary function special access
    // to the private members of this class (currentUser and dailyIntake).
    friend void printDetailedSummary(const Tracker& tracker);

    void loadFoodDatabase() {
        ifstream in("foodDB.txt");
        if (!in) return;
        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string food;
            int cal, pro, carb, fat;
            getline(iss, food, ',');
            iss >> cal >> pro >> carb >> fat;
            foodDatabase[food] = {cal, pro, carb, fat};
        }
        in.close();
    }

    void saveFoodDatabase() {
        ofstream out("foodDB.txt");
        for (const auto& pair : foodDatabase) {
            out << pair.first << "," << pair.second[0] << " " << pair.second[1] << " " << pair.second[2] << " " << pair.second[3] << "\n";
        }
        out.close();
    }

    void registerUser() {
        string un, p;
        cout << "-----Register New User-----\n";
        cout << "Enter Username: ";
        getline(cin, un);

        ifstream in("users.txt");
        string line;
        bool exists = false;
        while (getline(in, line)) {
            istringstream iss(line);
            string firstWord;
            getline(iss, firstWord, ',');
            if (firstWord == un) {
                exists = true;
                break;
            }
        }
        in.close();

        if (exists) {
            cout << "Username already exists! Please choose another.\n";
            return;
        }

        cout << "Enter Password: ";
        getline(cin, p);
        line = un + "," + p;

        ofstream out("users.txt", ios::app);
        if (!out) {
            cout << "File could not be opened\n";
            return;
        }
        out << line << "\n";
        out.close();
        cout << "User registered successfully!\n";
    }

    bool loginUser() {
        string u, p;
        cout << "-------User LogIn-------\n";
        cout << "Enter Username: ";
        getline(cin, u);
        cout << "Enter Password: ";
        getline(cin, p);

        ifstream in("users.txt");
        if (!in) {
            cout << "Error: could not open users.txt\n";
            return false;
        }

        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string fileUser, filePass;
            getline(iss, fileUser, ',');
            getline(iss, filePass, ',');
            if (fileUser == u && filePass == p) {
                cout << "LogIn Successful\n";
                currentUser = u;
                in.close();
                // Demonstrate Polymorphism with a User object
                User loggedInUser(u, p);
                loggedInUser.displayDashboard();
                return true;
            }
        }
        cout << "Username or Password does not match.\n";
        in.close();
        return false;
    }

    void enterMeal() {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4) {
            cout << "Invalid input. Please choose a number from 1 to 4.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore();
        string meal;
        if (choice == 1) meal = "Breakfast";
        else if (choice == 2) meal = "Lunch";
        else if (choice == 3) meal = "Snacks";
        else if (choice == 4) meal = "Dinner";

        cout << "Start adding food items for " << meal << ". Type 'done' when finished.\n";
        string food;
        while (true) {
            cout << "Food: ";
            getline(cin, food);
            if (food == "done") break;

            if (foodDatabase.find(food) == foodDatabase.end()) {
                cout << "Food not found. Enter macros for " << food << ":\n";
                int cal, pro, carb, fat;
                cout << "Calories: "; cin >> cal;
                cout << "Protein: "; cin >> pro;
                cout << "Carbs: "; cin >> carb;
                cout << "Fats: "; cin >> fat;
                cin.ignore();
                foodDatabase[food] = {cal, pro, carb, fat};
                saveFoodDatabase();
            }

            vector<int> macros = foodDatabase[food];
            if (dailyIntake.find(meal) == dailyIntake.end()) {
                dailyIntake[meal] = {0, 0, 0, 0};
            }
            for (int i = 0; i < 4; i++) {
                dailyIntake[meal][i] += macros[i];
            }
            cout << food << " added successfully!\n";
        }
        cout << meal << " entries completed.\n";
        saveDailySummary();
    }
    
    void viewAndSelectCourse() {
        ifstream in("courses.txt");
        if (!in) {
            cout << "\nSorry, no courses are available at the moment.\n";
            return;
        }
        vector<CourseInfo> courses;
        string line;
        cout << "\n--- Available Fitness Courses ---\n";
        while (getline(in, line)) {
            istringstream iss(line);
            string cat, desc, cname;
            getline(iss, cat, ',');
            getline(iss, desc, ','); // Simplified for this example
            getline(iss, cname);
            stringstream courseDetails;
            courseDetails << "Category: " << cat << "\n"
                          << "  Description: " << desc << "\n"
                          << "  Created by: " << cname;
            courses.push_back({cat, cname, courseDetails.str()});
        }
        in.close();
        if (courses.empty()) {
            cout << "No courses found.\n";
            return;
        }
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << i + 1 << ". " << courses[i].fullDetails << "\n---------------------------------\n";
        }
        cout << "Enter the number of the course you want to select (or 0 to cancel): ";
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice > 0 && choice <= courses.size()) {
            const CourseInfo& selectedCourse = courses[choice - 1];
            ofstream out("enrollments.txt", ios::app);
            out << selectedCourse.category << "," << selectedCourse.counsellorName << "," << currentUser << "\n";
            out.close();
            cout << "\n*** You have successfully enrolled in the '" << selectedCourse.category << "' course! ***\n";
        } else {
            cout << "\nInvalid selection or action cancelled.\n";
        }
    }
    
    void displayDailySummary() {
        time_t now = time(0);
        char* dt = ctime(&now);
        int totalCal = 0, totalPro = 0, totalCarb = 0, totalFat = 0;

        cout << "\n====== Daily Nutrition Summary for " << currentUser << " ======\n";
        cout << "Report generated on: " << dt;
        cout << "------------------------------------\n";

        for(const auto& pair : dailyIntake) {
            cout << pair.first << " -> Cal:" << pair.second[0] << " Pro:" << pair.second[1] << " Carbs:" << pair.second[2] << " Fats:" << pair.second[3] << "\n";
            totalCal += pair.second[0];
            totalPro += pair.second[1];
            totalCarb += pair.second[2];
            totalFat += pair.second[3];
        }
        cout << "------------------------------------\n";
        cout << "TOTAL -> Cal:" << totalCal << " Pro:" << totalPro << " Carbs:" << totalCarb << " Fats:" << totalFat << "\n";
    }

    void saveDailySummary() {
        ofstream out(currentUser + "_summary.txt");
        time_t now = time(0);
        char* dt = ctime(&now);
        string timeStr = dt;
        timeStr.erase(timeStr.find_last_not_of("\n\r") + 1);

        out << "Daily Summary for " << currentUser << ":\n";
        out << "Last Updated: " << timeStr << "\n";
        out << "-----------------------------\n";
        for (const auto& pair : dailyIntake) {
            out << pair.first << "," << pair.second[0] << "," << pair.second[1] << "," << pair.second[2] << "," << pair.second[3] << "\n";
        }
        out.close();
    }
};

// NEW: Implementation of the friend function
void printDetailedSummary(const Tracker& tracker) {
    int totalCal = 0;
    // The function can access private members directly!
    cout << "\n=== Detailed Friend Summary for: " << tracker.currentUser << " ===\n";
    if (tracker.dailyIntake.empty()) {
        cout << "No meals have been entered yet.\n";
        return;
    }

    // MODIFIED: This is the corrected loop that works with older compilers
    for (const auto& pair : tracker.dailyIntake) {
        // We manually get the meal and macros from the 'pair'
        const vector<int>& macros = pair.second;
        totalCal += macros[0]; // macros[0] is calories
    }
    
    cout << "Total calories consumed today: " << totalCal << " kcal.\n";
    cout << "This special report was generated by a friend function.\n";
}

// ===================================================================
// COUNSELLOR PORTAL CLASSES
// ===================================================================

// MODIFIED: Counsellor now inherits from Account
class Counsellor : public Account {
public:
    string counsellorName;

    Counsellor(string cname, string uname, string p) : Account(uname, p), counsellorName(cname) {}

    void displayDashboard() const override {
        cout << "\n--- Welcome to the Counsellor Dashboard, " << counsellorName << "! ---\n";
        cout << "Here you can create courses and view user progress.\n";
    }
};

class Course {
public:
    static void saveCourse(const string& cat, const string& desc, const string& cname) {
        ofstream out("courses.txt", ios::app);
        out << cat << "," << desc << "," << cname << "\n";
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
            counsellors.emplace(uname, Counsellor(cname, uname, pass));
        }
        in.close();
    }

    void registerCounsellor() {
        string cname, uname, pass;
        cout << "Enter Counsellor Name: "; getline(cin, cname);
        cout << "Enter Username: "; getline(cin, uname);
        cout << "Enter Password: "; getline(cin, pass);

        if (counsellors.count(uname)) {
            cout << "Username already exists.\n";
            return;
        }
        ofstream out("counsellors.txt", ios::app);
        out << cname << "," << uname << "," << pass << "\n";
        out.close();
        counsellors.emplace(uname, Counsellor(cname, uname, pass));
        cout << "Counsellor registered successfully.\n";
    }

    Counsellor* login() {
        string uname, pass;
        cout << "Enter Username: "; getline(cin, uname);
        cout << "Enter Password: "; getline(cin, pass);
        
        auto it = counsellors.find(uname);
        if (it != counsellors.end() && it->second.checkPassword() == pass) {
            cout << "\nLogin successful!\n";
            // Demonstrate Polymorphism with a Counsellor object
            it->second.displayDashboard();
            return &it->second;
        }
        cout << "\nInvalid username or password.\n";
        return nullptr;
    }

    void viewEnrolledUsers(const string& counsellorName) {
        ifstream in("enrollments.txt");
        if (!in) {
            cout << "No enrollment data found.\n";
            return;
        }

        map<string, vector<string>> enrolledUsersByCourse;
        set<string> validUsernames;
        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string courseCat, fileCounsellorName, enrolledUsername;
            getline(iss, courseCat, ',');
            getline(iss, fileCounsellorName, ',');
            getline(iss, enrolledUsername);
            if (fileCounsellorName == counsellorName) {
                enrolledUsersByCourse[courseCat].push_back(enrolledUsername);
                validUsernames.insert(enrolledUsername);
            }
        }
        in.close();

        if (enrolledUsersByCourse.empty()) {
            cout << "No users have enrolled in your courses yet.\n";
            return;
        }
        cout << "\n--- Users Enrolled in Your Courses ---\n";
        for (const auto& pair : enrolledUsersByCourse) {
            cout << "Course: " << pair.first << "\n";
            for (const auto& user : pair.second) {
                cout << "  - User: " << user << "\n";
            }
        }
        cout << "-------------------------------------\n";
        
        while (true) {
            cout << "\nEnter a username to view their summary (or type 'back' to return): ";
            string usernameToView;
            getline(cin, usernameToView);
            if (usernameToView == "back") break;
            
            if (validUsernames.find(usernameToView) == validUsernames.end()) {
                cout << "Error: User '" << usernameToView << "' is not enrolled in your courses.\n";
                continue;
            }

            string filename = usernameToView + "_summary.txt";
            ifstream summaryFile(filename);
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

// ===================================================================
// MAIN FUNCTION
// ===================================================================

void runUserPortal() {
    Tracker system;
    system.loadFoodDatabase();

    while (true) {
        cout << "\n--- User Portal Menu ---\n"
             << "1. Register\n"
             << "2. Login\n"
             << "3. Exit\n"
             << "Choose Option: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            system.registerUser();
        } else if (choice == 2) {
            if (system.loginUser()) {
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "\n--- Logged In Menu ---\n"
                         << "1. Enter Meal\n"
                         << "2. View Daily Summary\n"
                         << "3. View/Select Course\n"
                         << "4. Get Detailed Report (Friend Function Demo)\n"
                         << "5. Logout\n"
                         << "Choose: ";
                    int userChoice;
                    cin >> userChoice;
                    cin.ignore();

                    switch (userChoice) {
                        case 1: system.enterMeal(); break;
                        case 2: system.displayDailySummary(); break;
                        case 3: system.viewAndSelectCourse(); break;
                        case 4: printDetailedSummary(system); break; // Friend function call
                        case 5: loggedIn = false; break;
                        default: cout << "Invalid choice.\n";
                    }
                }
            }
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void runCounsellorPortal() {
    CounsellorPortal portal;
    while (true) {
        cout << "\n--- Counsellor Portal Menu ---\n"
             << "1. Register\n"
             << "2. Login\n"
             << "3. Exit\n"
             << "Choose: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            portal.registerCounsellor();
        } else if (choice == 2) {
            Counsellor* c = portal.login();
            if (c) {
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "\n--- Counsellor Dashboard (" << c->counsellorName << ") ---\n"
                         << "1. Create Course\n"
                         << "2. View Enrolled Users\n"
                         << "3. Logout\n"
                         << "Choose: ";
                    int cchoice;
                    cin >> cchoice;
                    cin.ignore();
                    if (cchoice == 1) {
                        string cat, desc;
                        cout << "Enter Course Category (e.g., Weight Loss): "; getline(cin, cat);
                        cout << "Enter Course Description: "; getline(cin, desc);
                        Course::saveCourse(cat, desc, c->counsellorName);
                        cout << "Course created successfully.\n";
                    } else if (cchoice == 2) {
                        portal.viewEnrolledUsers(c->counsellorName);
                    } else if (cchoice == 3) {
                        loggedIn = false;
                    } else {
                        cout << "Invalid choice.\n";
                    }
                }
            }
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}


int main() {
    while (true) {
        cout << "\n=======================================\n";
        cout << "   Main Menu: Welcome to Nutri-Fit   \n";
        cout << "=======================================\n";
        cout << "1. User Portal\n";
        cout << "2. Counsellor Portal\n";
        cout << "3. Exit Program\n";
        cout << "Choose Portal: ";

        int portalChoice;
        cin >> portalChoice;
        cin.ignore();

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (portalChoice) {
            case 1:
                runUserPortal();
                break;
            case 2:
                runCounsellorPortal();
                break;
            case 3:
                cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please select 1, 2, or 3.\n";
                break;
        }
    }

    return 0;
}