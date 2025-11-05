#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cctype>
#include <set> 
#include <limits> 
#include <ctime>

using namespace std;

// Base class for all account types
class Account {
protected: 
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

    // Pure virtual function makes this an abstract class
    virtual void displayDashboard() const = 0;

    virtual ~Account() {}
};

class User : public Account {
private:
    string currentUser;
public:
    User(string u, string p) : Account(u, p) {}
    User() : Account("", "") {}
    string getCurrentUser() const { return currentUser; }


    void displayDashboard() const override {
        cout << "\n--- Welcome to the User Dashboard, " << getUsername() << "! ---\n";
        cout << "Here you can track meals and enroll in courses.\n";
    }
    void registerUser()
    {
        bool exists = false;
        string un, p;
        cout << "-----Register New User-----\n";
        cout << "Enter Username: ";
        getline(cin, un);
        ifstream in("users.txt");
        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string firstWord;
            getline(iss, firstWord, ',');
            if (firstWord == un)
            {
                exists = true;
                break;
            }
        }
        in.close();
        if (exists)
        {
            cout << "Username already exists! Please choose another.\n";
            return;
        }

        cout << "Enter Password:";
        getline(cin, p);
        line = un + "," + p;

        ofstream out("users.txt", ios::app);
        if (!out)
        {
            cout << "File could not be opened\n";
            return;
        }
        out << line << "\n";
        out.close();
        cout << "User registered";
    }

    bool loginUser()
    {
        string u, p;
        ifstream in("users.txt");
        if (!in)
        {
            cout << "Error: could not open users.txt\n";
            return false;
        }

        cout << "-------User LogIn-------\n";
        cout << "Enter User name:";
        getline(cin, u);
        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string f, s;
            getline(iss, f, ',');
            getline(iss, s, ',');

            if (f == u)
            {
                cout << "Enter Password:";
                getline(cin, p);

                if (s == p)
                {
                    cout << "LogIn Successful\n";
                    currentUser = f;
                    return true;
                }
                else
                {
                    cout << "Passwords Does not match\n";
                    return false;
                }
            }
        }
        cout << "Username does not exist\n";
        in.close();
        return false;
    }
};

class Tracker
{
private:
    string currentUser;
    map<string, vector<int>> foodDatabase;
    map<string, vector<int>> dailyIntake;

public:
    void setCurrentUser(const string& user) {
        currentUser = user;
    }

    void loadFoodDatabase()
    {
        ifstream in("foodDB.txt");
        if (!in)
            return;

        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string food;
            int cal, pro, carb, fat;
            getline(iss, food, ',');
            iss >> cal >> pro >> carb >> fat;
            foodDatabase[food] = {cal, pro, carb, fat};
        }
        in.close();
    }

    void saveFoodDatabase()
    {
        ofstream out("foodDB.txt");
        for (auto &x : foodDatabase)
        {
            out << x.first << "," << x.second[0] << " " << x.second[1] << " " << x.second[2] << " " << x.second[3] << "\n";
        }
        out.close();
    }

    void enterMeal()
    {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > 4)
        {
            cout << "Invalid input. Exiting...\n";
            return;
        }
        cin.ignore();
        string meal;
        if (choice == 1)
            meal = "Breakfast";
        else if (choice == 2)
            meal = "Lunch";
        else if (choice == 3)
            meal = "Snacks";
        else if (choice == 4)
            meal = "Dinner";

        cout << "Start adding food items for " << meal << ". Type 'done' when finished.\n";

        string food;
        while (true)
        {
            cout << "Food: ";
            getline(cin, food);

            if (food == "done")
                break;

            if (foodDatabase.find(food) == foodDatabase.end())
            {
                cout << "Food not found in database. Enter macros for " << food << ":\n";
                int cal, pro, carb, fat;
                cout << "Calories: ";
                cin >> cal;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                cout << "Protein: ";
                cin >> pro;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                cout << "Carbs: ";
                cin >> carb;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                cout << "Fats: ";
                cin >> fat;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                cin.ignore();
                foodDatabase[food] = {cal, pro, carb, fat};
                saveFoodDatabase();
            }

            vector<int> macros = foodDatabase[food];
            if (dailyIntake.find(meal) == dailyIntake.end())
                dailyIntake[meal] = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++)
                dailyIntake[meal][i] += macros[i];

            cout << food << " added successfully!\n";
        }

        cout << meal << " completed.\n";

        if (meal == "Breakfast")
            saveDailySummary();
        if (meal == "Lunch")
            saveDailySummary();
        if (meal == "Snacks")
            saveDailySummary();
        if (meal == "Dinner")
        {
            displayDailySummary();
            saveDailySummary();
        }
    }

    void displayDailySummary()
    {
        int totalCal = 0, totalPro = 0, totalCarb = 0, totalFat = 0;
        cout << "\n====== Daily Nutrition Summary ======\n";
        for (auto &x : dailyIntake)
        {
            cout << x.first << " -> Calories:" << x.second[0] << " Protein:" << x.second[1] << " Carbs:" << x.second[2] << " Fats:" << x.second[3] << "\n";
            totalCal += x.second[0];
            totalPro += x.second[1];
            totalCarb += x.second[2];
            totalFat += x.second[3];
        }
        cout << "------------------------------------\n";
        cout << "TOTAL -> Calories:" << totalCal << " Protein:" << totalPro << " Carbs:" << totalCarb << " Fats:" << totalFat << "\n";
    }

    void saveDailySummary()
    {
        ofstream out(currentUser + "_summary.txt", ios::app);
        out << "Daily Summary for " << currentUser << ":\n";
        for (auto &x : dailyIntake)
        {
            out << x.first << "," << x.second[0] << "," << x.second[1] << "," << x.second[2] << "," << x.second[3] << "\n";
        }
        out << "-----------------------------\n";
        out.close();
    }
};

// Counsellor class, inherits from Account
class Counsellor : public Account {
public:
    string counsellorName;

    Counsellor(string cname, string uname, string p) : Account(uname, p), counsellorName(cname) {}

    void displayDashboard() const override {
        cout << "\n--- Welcome to the Counsellor Dashboard, " << counsellorName << "! ---\n";
        cout << "Here you can create courses and view user progress.\n";
    }

    // Static function to register a new counsellor
    static void registerCounsellor() {
        string cname, uname, pass;
        cout << "-----Register New Counsellor-----\n";
        cout << "Enter Your Full Name: "; 
        getline(cin, cname);
        cout << "Enter Username: "; 
        getline(cin, uname);

        // Check if username exists
        ifstream in("counsellors.txt");
        string line;
        bool exists = false;
        while (getline(in, line)) {
            istringstream iss(line);
            string fileCname, fileUname;
            getline(iss, fileCname, ',');
            getline(iss, fileUname, ',');
            if (fileUname == uname) {
                exists = true;
                break;
            }
        }
        in.close();

        if (exists) {
            cout << "Username already exists. Please try another.\n";
            return;
        }

        cout << "Enter Password: "; 
        getline(cin, pass);

        ofstream out("counsellors.txt", ios::app);
        out << cname << "," << uname << "," << pass << "\n";
        out.close();
        cout << "Counsellor registered successfully.\n";
    }

    // Static function to log in a counsellor
    static Counsellor* loginCounsellor() {
        string uname, pass;
        cout << "-------Counsellor LogIn-------\n";
        cout << "Enter Username: "; 
        getline(cin, uname);
        cout << "Enter Password: "; 
        getline(cin, pass);
        
        ifstream in("counsellors.txt");
        if (!in) {
            cout << "Error: could not open counsellors.txt\n";
            return nullptr;
        }

        string line;
        while (getline(in, line)) {
            istringstream iss(line);
            string fileCname, fileUname, filePass;
            getline(iss, fileCname, ',');
            getline(iss, fileUname, ',');
            getline(iss, filePass, ',');
            
            if (fileUname == uname && filePass == pass) {
                cout << "LogIn Successful\n";
                return new Counsellor(fileCname, fileUname, filePass);
            }
        }
        cout << "Username or Password does not match.\n";
        in.close();
        return nullptr;
    }
};

// Holds the logic for counsellor features
class CounsellorFeatures {
public:
    void createCourse(const string& counsellorName) {
        string cat, desc;
        cout << "Enter Course Category (e.g., Weight Loss): "; 
        getline(cin, cat);
        cout << "Enter Course Description: "; 
        getline(cin, desc);

        ofstream out("courses.txt", ios::app);
        out << cat << "," << desc << "," << counsellorName << "\n";
        out.close();
        cout << "Course created successfully.\n";
    }

    void viewMyCourses(const string& counsellorName) {
        cout << "\n--- Courses Created By " << counsellorName << " ---\n";
        ifstream in("courses.txt");
        if (!in) {
            cout << "Could not open courses.txt file.\n";
            return;
        }

        string line;
        bool foundCourses = false;
        while (getline(in, line)) {
            istringstream iss(line);
            string cat, desc, fileCounsellorName;
            
            getline(iss, cat, ',');
            getline(iss, desc, ',');
            getline(iss, fileCounsellorName, ',');

            if (fileCounsellorName == counsellorName) {
                cout << "Category: " << cat << "\n";
                cout << "  Description: " << desc << "\n";
                cout << "---------------------------------\n";
                foundCourses = true;
            }
        }
        in.close();

        if (!foundCourses) {
            cout << "You have not created any courses yet.\n";
        }
    }

    // Shows which users are in which courses
    void viewEnrolledUsers(const string& counsellorName) {
        
        // Load this counsellor's course descriptions
        map<string, string> courseDetailsMap;
        ifstream coursesFile("courses.txt");
        if (coursesFile) {
            string line;
            while (getline(coursesFile, line)) {
                istringstream iss(line);
                string cat, desc, fileCounsellorName;
                getline(iss, cat, ',');
                getline(iss, desc, ',');
                getline(iss, fileCounsellorName, ',');
                if (fileCounsellorName == counsellorName) {
                    courseDetailsMap[cat] = desc;
                }
            }
        }
        coursesFile.close();


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
            string courseCategory = pair.first;
            string courseDesc = courseDetailsMap.count(courseCategory) ? courseDetailsMap[courseCategory] : "N/A";

            cout << "Course: " << courseCategory << " (" << courseDesc << ")\n";
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




int main()
{
    User user; // For user portal logic
    
    Counsellor* loggedInCounsellor = nullptr;
    
    while (true)
    {
        cout << "\n===========================================================================================\n";
        cout << "                            Simple Fitness/Calorie Tracker                                 ";
        cout << "\n===========================================================================================\n";
        cout << "   1.ChartOfNutrition | 2.CalculateNutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit    ";
        cout << "\n-------------------------------------------------------------------------------------------\n";

        int a;
        cout << "Choose Option:";
        cin >> a;
        if (cin.fail()) {
            cout << "Wrong input!!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (a == 4) {
            // Ask for account type
            cout << "Register as: 1.User 2.Counsellor\n";
            int regChoice;
            cin >> regChoice;
            cin.ignore();
            if (regChoice == 1) {
                user.registerUser();
            } else if (regChoice == 2) {
                Counsellor::registerCounsellor();
            } else {
                cout << "Invalid choice.\n";
            }
        }
        else if (a == 3) {
            // Ask for account type
            cout << "Login as: 1.User 2.Counsellor\n";
            int loginChoice;
            cin >> loginChoice;
            cin.ignore();

            if (loginChoice == 1) {
                // User Login Logic
                if (user.loginUser()) {
                    Tracker system;
                    system.loadFoodDatabase();
                    system.setCurrentUser(user.getCurrentUser());
                    cout << "\n=============================================\n";
                    cout << "        Welcome choose operation to perform    ";
                    cout << "\n---------------------------------------------\n";
                    cout << " 1.EnterMeal 2.CalculateNutrition 3.PrePlanDaily ";
                    cout << "\n---------------------------------------------\n";
                    int x;
                    cin >> x;
                    cin.ignore();

                    if (x == 1)
                        system.enterMeal();
                    // ... (add other user menu options here)
                }
            } else if (loginChoice == 2) {
                // Counsellor Login Logic
                
                loggedInCounsellor = Counsellor::loginCounsellor();
                
                if (loggedInCounsellor != nullptr) {
                    
                    loggedInCounsellor->displayDashboard();
                    CounsellorFeatures features; 
                    bool counsellorLoggedIn = true;

                    while (counsellorLoggedIn) {
                            cout << "\n--- Counsellor Menu ---\n";
                            cout << "1. Create Course\n";
                            cout << "2. View My Courses\n";
                            cout << "3. View Enrolled Users\n";
                            cout << "4. Logout\n";
                            cout << "Choose: ";
                            int cChoice;
                            cin >> cChoice;
                            cin.ignore();

                            switch (cChoice) {
                                case 1:
                                    features.createCourse(loggedInCounsellor->counsellorName);
                                    break;
                                case 2:
                                    features.viewMyCourses(loggedInCounsellor->counsellorName);
                                    break;
                                case 3:
                                    features.viewEnrolledUsers(loggedInCounsellor->counsellorName);
                                    break;
                                case 4:
                                    counsellorLoggedIn = false;
                                    break;
                                default:
                                    cout << "Invalid choice.\n";
                            }
                        }
                    
                    delete loggedInCounsellor; // Clean up the object
                    loggedInCounsellor = nullptr;
                }
            } else {
                cout << "Invalid choice.\n";
            }
        }
        else if (a == 5)
            return 0;
    }
}