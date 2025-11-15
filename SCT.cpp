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
// #include counsellor.cpp
using namespace std;

// Base class for all account types
class Account
{
protected:
    string username;
    string password;

public:
    Account(string u, string p) : username(u), password(p) {}

    string getUsername() const
    {
        return username;
    }

    string checkPassword() const
    {
        return password;
    }

    // Pure virtual function makes this an abstract class
    virtual void displayDashboard() const = 0;

    virtual ~Account() {}
};

class User : public Account
{
private:
    string currentUser;

public:
    User(string u, string p) : Account(u, p) {}
    User() : Account("", "") {}
    string getCurrentUser() const { return currentUser; }

    void displayDashboard() const override
    {
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

    bool loginUser(string name)
    {
        string u, p;
        u=name;
        ifstream in("users.txt");
        if (!in)
        {
            cout << "Error: could not open users.txt\n";
            return false;
        }
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
                cin.ignore();
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

// Counsellor class, inherits from Account
class Counsellor : public Account
{
public:
    string counsellorName;

    Counsellor(string cname, string uname, string p) : Account(uname, p), counsellorName(cname) {}

    void displayDashboard() const override
    {
        cout << "\n--- Welcome to the Counsellor Dashboard, " << counsellorName << "! ---\n";
        cout << "Here you can create courses and view user progress.\n";
    }

    // Static function to register a new counsellor
    static void registerCounsellor()
    {
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
        while (getline(in, line))
        {
            istringstream iss(line);
            string fileCname, fileUname;
            getline(iss, fileCname, ',');
            getline(iss, fileUname, ',');
            if (fileUname == uname)
            {
                exists = true;
                break;
            }
        }
        in.close();

        if (exists)
        {
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
    static Counsellor *loginCounsellor()
    {
        string uname, pass;
        cout << "-------Counsellor LogIn-------\n";
        cout << "Enter Username: ";
        getline(cin, uname);
        cout << "Enter Password: ";
        getline(cin, pass);

        ifstream in("counsellors.txt");
        if (!in)
        {
            cout << "Error: could not open counsellors.txt\n";
            return nullptr;
        }

        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string fileCname, fileUname, filePass;
            getline(iss, fileCname, ',');
            getline(iss, fileUname, ',');
            getline(iss, filePass, ',');

            if (fileUname == uname && filePass == pass)
            {
                cout << "LogIn Successful\n";
                return new Counsellor(fileCname, fileUname, filePass);
            }
        }
        cout << "Username or Password does not match.\n";
        in.close();
        return nullptr;
    }
};

class Tracker
{
private:
    string currentUser;
    map<string, vector<int>> foodDatabase;
    map<string, vector<int>> dailyIntake;
    struct CourseInfo
    {
        string category;
        string counsellorName;
        string fullDetails;
    };

public:
Tracker(string name){
    currentUser=name;
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
        for (map<string, vector<int>>::const_iterator it = foodDatabase.begin(); it != foodDatabase.end(); ++it)
        {
            out << it->first << "," << it->second[0] << " " << it->second[1] << " " << it->second[2] << " " << it->second[3] << "\n";
        }
        out.close();
    }

    void enterMeal()
    {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 4)
        {
            cout << "Invalid input. Please choose a number from 1 to 4.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                    return;
                }
                cout << "Protein: ";
                cin >> pro;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    return;
                }
                cout << "Carbs: ";
                cin >> carb;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
                    return;
                }
                cout << "Fats: ";
                cin >> fat;
                if (cin.fail())
                {
                    cout << "Not an int!!" << endl;
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
        cout << meal << " entries completed.\n";

        if (dailyIntake.count("Breakfast") && dailyIntake.count("Lunch") && dailyIntake.count("Snacks") && dailyIntake.count("Dinner"))
        {
            displayDailySummary();
            saveDailySummary(); // Save after dinner meal entry
        }
        // saveDailySummary(); // Save after every meal entry
    }

    void viewAndSelectCourse()
    {
        ifstream in("courses.txt");
        if (!in)
        {
            cout << "\nSorry, no courses are available at the moment.\n";
            return;
        }
        vector<CourseInfo> courses;
        string line;
        cout << "\n--- Available Fitness Courses ---\n";
        while (getline(in, line))
        {
            istringstream iss(line);
            string cat, cal_str, f_str, p_str, c_str, desc, cname;
            getline(iss, cat, ',');
            getline(iss, cal_str, ',');
            getline(iss, f_str, ',');
            getline(iss, p_str, ',');
            getline(iss, c_str, ',');
            getline(iss, desc, ',');
            getline(iss, cname);
            stringstream courseDetails;
            courseDetails << "Category: " << cat << "\n"
                          << "   Description: " << desc << "\n"
                          << "   Created by: " << cname;
            courses.push_back({cat, cname, courseDetails.str()});
        }
        in.close();
        if (courses.empty())
        {
            cout << "No courses found in the file.\n";
            return;
        }
        for (size_t i = 0; i < courses.size(); ++i)
        {
            cout << i + 1 << ". " << courses[i].fullDetails << "\n---------------------------------\n";
        }
        cout << "Enter the number of the course you want to select (or 0 to cancel): ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice > 0 && choice <= courses.size())
        {
            const CourseInfo &selectedCourse = courses[choice - 1];
            ofstream out("enrollments.txt", ios::app);
            out << selectedCourse.category << "," << selectedCourse.counsellorName << "," << currentUser << "\n";
            out.close();
            cout << "\n*** You have successfully enrolled in the '" << selectedCourse.category << "' course by " << selectedCourse.counsellorName << "! ***\n";
        }
        else
        {
            cout << "\nInvalid selection or action cancelled.\n";
        }
    }

    void displayDailySummary()
    {
        time_t now = time(0);
        char *dt = ctime(&now);

        int totalCal = 0, totalPro = 0, totalCarb = 0, totalFat = 0;
        cout << "\n====== Daily Nutrition Summary ======\n";
        cout << "Report generated on: " << dt;
        cout << "------------------------------------\n";

        for (map<string, vector<int>>::iterator it = dailyIntake.begin(); it != dailyIntake.end(); ++it)
        {
            cout << it->first << " -> Calories:" << it->second[0] << " Protein:" << it->second[1] << " Carbs:" << it->second[2] << " Fats:" << it->second[3] << "\n";
            totalCal += it->second[0];
            totalPro += it->second[1];
            totalCarb += it->second[2];
            totalFat += it->second[3];
        }
        cout << "------------------------------------\n";
        cout << "TOTAL -> Calories:" << totalCal << " Protein:" << totalPro << " Carbs:" << totalCarb << " Fats:" << totalFat << "\n";

        // Compare with planned data (if available)
        compareWithPlan(totalCal, totalPro, totalCarb, totalFat);
    }

    struct NutritionPlan
    {
        int calories;
        int protein;
        int carbs;
        int fats;
    };

    queue<NutritionPlan> nutritionQueue; // stores future daily plans

    void prePlanNutrition()
    {
        int days;
        cout << "\nEnter how many days you want to plan nutrition for: ";
        cin >> days;

        if (cin.fail() || days <= 0)
        {
            cout << "Invalid number of days.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        for (int i = 1; i <= days; i++)
        {
            NutritionPlan plan;
            cout << "\n--- Enter plan for Day " << i << " ---\n";
            cout << "Calories: ";
            cin >> plan.calories;
            cout << "Protein: ";
            cin >> plan.protein;
            cout << "Carbs: ";
            cin >> plan.carbs;
            cout << "Fats: ";
            cin >> plan.fats;

            nutritionQueue.push(plan);
        }

        cout << "\nNutrition plans added successfully for " << days << " day(s)!\n";
    }

    void compareWithPlan(int totalCal, int totalPro, int totalCarb, int totalFat)
    {
        if (nutritionQueue.empty())
        {
            cout << "\nNo planned nutrition data available for comparison.\n";
            return;
        }

        NutritionPlan todayPlan = nutritionQueue.front();

        cout << "\n====== Comparison with Planned Nutrition ======\n";
        cout << "Planned -> Calories: " << todayPlan.calories
             << " | Protein: " << todayPlan.protein
             << " | Carbs: " << todayPlan.carbs
             << " | Fats: " << todayPlan.fats << "\n";

        cout << "Actual  -> Calories: " << totalCal
             << " | Protein: " << totalPro
             << " | Carbs: " << totalCarb
             << " | Fats: " << totalFat << "\n";

        bool exceeded = false;
        if (totalCal > todayPlan.calories)
        {
            cout << "Limit exceeded in Calories!\n";
            exceeded = true;
        }
        if (totalPro > todayPlan.protein)
        {
            cout << " Limit exceeded in Protein!\n";
            exceeded = true;
        }
        if (totalCarb > todayPlan.carbs)
        {
            cout << "Limit exceeded in Carbs!\n";
            exceeded = true;
        }
        if (totalFat > todayPlan.fats)
        {
            cout << " Limit exceeded in Fats!\n";
            exceeded = true;
        }
        if (!exceeded)
            cout << "All nutrients are within planned limits!\n";

        nutritionQueue.pop(); // remove plan for the completed day
    }

    void saveDailySummary()
    {
        ofstream out(currentUser + "_summary.txt", ios::app);

        // Get the current time
        time_t now = time(0);
        char *dt = ctime(&now);

        // Convert to string and remove the newline character that ctime adds
        string timeStr = dt;
        timeStr.erase(timeStr.find_last_not_of("\n\r") + 1);

        out << "Daily Summary for " << currentUser << ":\n";

        out << "Last Updated: " << timeStr << "\n";
        out << "-----------------------------\n";

        for (map<string, vector<int>>::iterator it = dailyIntake.begin(); it != dailyIntake.end(); ++it)
        {
            out << it->first << "," << it->second[0] << "," << it->second[1] << "," << it->second[2] << "," << it->second[3] << "\n";
        }
        out.close();
    }
};

class CounsellorFeatures
{
public:void createCourse(const string &counsellorName)
{
    string name, price, calories, protein, carbs, desc;

    cout << "Enter Course Name: ";
    getline(cin, name);

    cout << "Enter Course Price (INR): ";
    getline(cin, price);

    cout << "Enter Daily Calories: ";
    getline(cin, calories);

    cout << "Enter Daily Protein (g): ";
    getline(cin, protein);

    cout << "Enter Daily Carbs (g): ";
    getline(cin, carbs);

    cout << "Enter Course Description: ";
    getline(cin, desc);

    ofstream out("courses.txt", ios::app);

    // name, price, calories, protein, carbs, description, counsellor
    out << name << ","
        << price << ","
        << calories << ","
        << protein << ","
        << carbs << ","
        << desc << ","
        << counsellorName << "\n";

    out.close();

    cout << "Course created successfully.\n";
}


    void viewMyCourses(const string &counsellorName)
    {
        cout << "\n--- Courses Created By " << counsellorName << " ---\n";
        ifstream in("courses.txt");
        if (!in)
        {
            cout << "Could not open courses.txt file.\n";
            return;
        }

        string line;
        bool foundCourses = false;
        while (getline(in, line))
        {
            istringstream iss(line);
            string cat, desc, fileCounsellorName;

            getline(iss, cat, ',');
            getline(iss, desc, ',');
            getline(iss, fileCounsellorName, ',');

            if (fileCounsellorName == counsellorName)
            {
                cout << "Category: " << cat << "\n";
                cout << "  Description: " << desc << "\n";
                cout << "---------------------------------\n";
                foundCourses = true;
            }
        }
        in.close();

        if (!foundCourses)
        {
            cout << "You have not created any courses yet.\n";
        }
    }

    // Shows which users are in which courses
    void viewEnrolledUsers(const string &counsellorName)
    {

        // Load this counsellor's course descriptions
        map<string, string> courseDetailsMap;
        ifstream coursesFile("courses.txt");
        if (coursesFile)
        {
            string line;
            while (getline(coursesFile, line))
            {
                istringstream iss(line);
                string cat, desc, fileCounsellorName;
                getline(iss, cat, ',');
                getline(iss, desc, ',');
                getline(iss, fileCounsellorName, ',');
                if (fileCounsellorName == counsellorName)
                {
                    courseDetailsMap[cat] = desc;
                }
            }
        }
        coursesFile.close();

        ifstream in("enrollments.txt");
        if (!in)
        {
            cout << "No enrollment data found.\n";
            return;
        }

        map<string, vector<string>> enrolledUsersByCourse;
        set<string> validUsernames;
        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string courseCat, fileCounsellorName, enrolledUsername;
            getline(iss, courseCat, ',');
            getline(iss, fileCounsellorName, ',');
            getline(iss, enrolledUsername);
            if (fileCounsellorName == counsellorName)
            {
                enrolledUsersByCourse[courseCat].push_back(enrolledUsername);
                validUsernames.insert(enrolledUsername);
            }
        }
        in.close();

        if (enrolledUsersByCourse.empty())
        {
            cout << "No users have enrolled in your courses yet.\n";
            return;
        }
        cout << "\n--- Users Enrolled in Your Courses ---\n";
        for (const auto &pair : enrolledUsersByCourse)
        {
            string courseCategory = pair.first;
            string courseDesc = courseDetailsMap.count(courseCategory) ? courseDetailsMap[courseCategory] : "N/A";

            cout << "Course: " << courseCategory << " (" << courseDesc << ")\n";
            for (const auto &user : pair.second)
            {
                cout << "  - User: " << user << "\n";
            }
        }
        cout << "-------------------------------------\n";

        while (true)
        {
            cout << "\nEnter a username to view their summary (or type 'back' to return): ";
            string usernameToView;
            getline(cin, usernameToView);
            if (usernameToView == "back")
                break;

            if (validUsernames.find(usernameToView) == validUsernames.end())
            {
                cout << "Error: User '" << usernameToView << "' is not enrolled in your courses.\n";
                continue;
            }

            string filename = usernameToView + "_summary.txt";
            ifstream summaryFile(filename);
            if (summaryFile.is_open())
            {
                cout << "\n--- Displaying Summary for " << usernameToView << " ---\n";
                string summaryLine;
                while (getline(summaryFile, summaryLine))
                {
                    cout << summaryLine << endl;
                }
                cout << "--- End of Summary ---\n";
                summaryFile.close();
            }
            else
            {
                cout << "Could not find a summary file for user '" << usernameToView << "'.\n";
            }
        }
    }
};

class FunctionwithoutLogin
{
private:
    struct FoodItem
    {
        string name;
        int calories;
        int protein;
        int carbs;
        int fats;
    };
    vector<FoodItem> foodData;
    size_t currentIndex = 0;

public:
    void loadDatabase(const string &foodDB)
    {
        if (foodDB.empty())
            return;
        ifstream file("foodDB.txt");
        if (!file)
        {
            cerr << "Error: Cannot open " << foodDB << endl;
            return;
        }
        foodData.clear();
        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string name, rest;
            getline(ss, name, ','); // name before comma
            getline(ss, rest);      // rest after comma

            FoodItem item;
            item.name = name;
            stringstream values(rest);
            values >> item.calories >> item.protein >> item.carbs >> item.fats;

            foodData.push_back(item);
        }
        file.close();
    }

    void showNext10()
    {

        loadDatabase("foodDB.txt");
        if (foodData.empty())
        {
            cout << "Database is empty. Please load it first.\n";
            return;
        }

        size_t total = foodData.size();
        size_t start = currentIndex;
        size_t end = min(currentIndex + 10, total);

        cout << "\n-------------------------------------------------------------\n";
        cout << left << setw(18) << "Name"
             << setw(10) << "Calories"
             << setw(10) << "Protein"
             << setw(10) << "Carbs"
             << setw(10) << "Fats" << endl;
        cout << "-------------------------------------------------------------\n";

        for (size_t i = start; i < end; ++i)
        {
            cout << left << setw(18) << foodData[i].name
                 << setw(10) << foodData[i].calories
                 << setw(10) << foodData[i].protein
                 << setw(10) << foodData[i].carbs
                 << setw(10) << foodData[i].fats << endl;
        }

        cout << "-------------------------------------------------------------\n";
        cout << "Showing " << (start + 1) << "-" << end << " of " << total << endl;

        currentIndex = end;

        if (currentIndex >= total)
        {
            cout << "End of database reached. Resetting...\n";
            currentIndex = 0;
        }
        else
        {
            cout << "Call showNext10() again to view the next 10 entries.\n";
        }
    }

    void searchFoodFromFile()
    {
        ifstream file("foodDB.txt");
        if (!file)
        {
            cout << "Error: Could not open foodDB.txt\n";
            return;
        }

        string query;
        cout << "\nEnter food name to search: ";
        // cin.ignore();
        getline(cin, query);

        // Convert search term to lowercase for case-insensitive matching
        transform(query.begin(), query.end(), query.begin(), ::tolower);

        string line;
        bool found = false;

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            // Split line into name and nutrient part
            size_t commaPos = line.find(',');
            if (commaPos == string::npos)
                continue;

            string name = line.substr(0, commaPos);
            string nutrients = line.substr(commaPos + 1);

            // Convert to lowercase for case-insensitive comparison
            string nameLower = name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            if (nameLower == query)
            {
                FoodItem item;
                item.name = name;

                stringstream ss(nutrients);
                ss >> item.calories >> item.protein >> item.carbs >> item.fats;

                cout << "\n Found in database:\n";
                cout << "-------------------------------------------------------------\n";
                cout << left << setw(18) << "Name"
                     << setw(10) << "Calories"
                     << setw(10) << "Protein"
                     << setw(10) << "Carbs"
                     << setw(10) << "Fats" << endl;
                cout << "-------------------------------------------------------------\n";
                cout << left << setw(18) << item.name
                     << setw(10) << item.calories
                     << setw(10) << item.protein
                     << setw(10) << item.carbs
                     << setw(10) << item.fats << endl;
                cout << "-------------------------------------------------------------\n";
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "'" << query << "' not found in database.\n";
        }

        file.close();
    }
};

int main()
{

    User user;
    FunctionwithoutLogin *nologinfunc = new FunctionwithoutLogin();

    while (true)
    {
        cout << "\n=========================================================================================\n";
        cout << "                                      N U T R I F I T                                      \n";
        cout << "                     where you can calculate, track, and maintain your Meal             \n";
        cout << "===========================================================================================\n";
        cout << "     1. Chart Of Nutrition  |  2. Calculate Nutrition For Your Food  |  3. Log In          \n";
        cout << "     4. Create Account      |  5. Exit                                                     \n";
        cout << "-------------------------------------------------------------------------------------------\n";

        int a;
        cout << "Choose Option:";
        cin >> a;
        if (cin.fail())
        {
            cout << "Wrong input!!" << endl;
            return 0;
        }
        cin.ignore();

        if (a == 1)
        {
            string filename = "foodDB.txt";
            char choice;

            do
            {
                nologinfunc->showNext10();
                cout << "\nShow next 10? (y/n): ";
                cin >> choice;
            } while (choice == 'y' || choice == 'Y');

            cout << "\nThank you for visiting\n";
        }

        else if (a == 2)
        {
            nologinfunc->searchFoodFromFile();
            break;
        }

        else if (a == 3)
        {
            delete nologinfunc;

            // Ask for account type
            cout << "Login as: 1.User 2.Counsellor\n";
            int loginChoice;
            cin >> loginChoice;
            string name;
            cin.ignore();
            if (loginChoice == 1)
            {
                // User Login Logic
                cout << "-------User LogIn-------\n";
                cout << "Enter User name:";
                cin>>name;
                if (user.loginUser(name) == true)
                {
                    Tracker *system = new Tracker(name);
                    system->loadFoodDatabase();
                    bool loggedIn = true;
                    while (loggedIn)
                    {
                        cout << "\n==================================================================\n";
                        cout << "                          NutriFit Tracker                           ";
                        cout << "\n------------------------------------------------------------------\n";
                        cout << " 1.EnterMeal  |  2.Select Course  |   3.PrePlanner   |     4.Logout ";
                        cout << "\n-------------------------------------------------------------------\n";
                        cout << "Choose Option: ";

                        int x;
                        cin >> x;
                        if (cin.fail())
                        {
                            cout << "Wrong input!! Please enter a number.\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }

                        switch (x)
                        {
                        case 1:
                            system->enterMeal();
                            break;
                        case 2:
                            system->viewAndSelectCourse();
                            break;
                        case 3:
                            system->prePlanNutrition();
                            break;
                        case 4:
                            cout << "Logging out...\n";
                            loggedIn = false;
                            delete system;
                            break;
                        default:
                            cout << "Invalid option. Please try again.\n";
                            break;
                        }
                    }
                }
            }
            else if (loginChoice == 2)
            {
                // Counsellor Login Logic

                Counsellor *loggedInCounsellor = Counsellor::loginCounsellor();
                if (loggedInCounsellor != nullptr)
                {

                    loggedInCounsellor->displayDashboard();
                    CounsellorFeatures features;
                    bool counsellorLoggedIn = true;

                    while (counsellorLoggedIn)
                    {
                        cout << "\n--- Counsellor Menu ---\n";
                        cout << "1. Create Course\n";
                        cout << "2. View My Courses\n";
                        cout << "3. View Enrolled Users\n";
                        cout << "4. Logout\n";
                        cout << "Choose: ";
                        int cChoice;
                        cin >> cChoice;
                        cin.ignore();

                        switch (cChoice)
                        {
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
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
        else if (a == 4)
        {
            // Ask for account type
            cout << "Register as: 1.User 2.Counsellor\n";
            int regChoice;
            cin >> regChoice;
            cin.ignore();
            if (regChoice == 1)
            {
                user.registerUser();
            }
            else if (regChoice == 2)
            {
                Counsellor::registerCounsellor();
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
        else if (a == 5)
        {
            delete nologinfunc;
            return 0;
        }
        else
        {
            cout << "This option is not yet implemented.\n";
        }
    }
    return 0;
}