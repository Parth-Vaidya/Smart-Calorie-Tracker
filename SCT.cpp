#include <bits/stdc++.h>
using namespace std;

class Account
{
protected:
    string username;
    string password;

public:
    // constructor
    Account(string u, string p)
    {
        username = u;
        password = p;
    }

    string getun() const {return username;}
    string passwchecker() const {return password;}

    // PureVirtualFunction
    virtual void dashdisplay() const = 0;

    // destructor
    virtual ~Account() {}
};

// Inheritance: user from Account
class User : public Account
{
private:
    string liveuser;

public:

    User(string u, string p) : Account(u, p)
    {
        username = u;
        password = p;
    }
    
    User() : Account("", "") {}

    string getliveuser() const {return liveuser;}

    void dashdisplay() const override
    {
        cout << "\n--- Welcome to the User Dashboard, " << getun() << "! ---\n";
        cout << "Here you can track meals and enroll in courses.\n";
    }
    void userreg()
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
            string temp;
            getline(iss, temp, ',');
            if (temp == un)
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

        // save as username,password to the users.txt
        line = un + "," + p;
        ofstream out("users.txt", ios::app); // points at last of line to append

        if (!out)
        {
            cout << "File could not be opened\n";
            return;
        }
        out << line << "\n";
        out.close();
        cout << "User registered";
    }

    bool userlogin()
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
                    liveuser = f;
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

// Inheritance: counsellor from Account
class Counsellor : public Account
{
public:
    string counsn;

    // constructor
    Counsellor(string cname, string uname, string p) : Account(uname, p) {counsn = cname;}

    void dashdisplay() const override
    {
        cout << "\n--- Welcome to the Counsellor Dashboard, " << counsn << "! ---\n";
        cout << "Here you can create courses and view user progress.\n";
    }

    // function to register a new counsellor
    static void counsreg()
    {
        string cname, uname, pass;
        cout << "-----Register New Counsellor-----\n";
        cout << "Enter Your Full Name: ";
        getline(cin, cname);
        cout << "Enter Username: ";
        getline(cin, uname);

        ifstream in("counsellors.txt");
        string line;
        bool exists = false;
        while (getline(in, line))
        {
            istringstream iss(line);
            string counsnamefile, usernfile;
            getline(iss, counsnamefile, ',');
            getline(iss, usernfile, ',');
            if (usernfile == uname)
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

    // function to log in a counsellor
    static Counsellor *counslogin()
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
            cout << "Could not open counsellors.txt\n";
            return nullptr;
        }

        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string counsnamefile, usernfile, passfile;
            getline(iss, counsnamefile, ',');
            getline(iss, usernfile, ',');
            getline(iss, passfile, ',');

            if (usernfile == uname && passfile == pass)
            {
                cout << "LogIn Successful\n";
                return new Counsellor(counsnamefile, usernfile, passfile);
            }
        }
        cout << "Username or Password does not match.\n";
        in.close();
        return nullptr;
    }
};

// Tracker class contains all the functions for calorie tracking and storing logs
class Tracker
{
private:
    string liveuser;
    map<string, vector<int>> fooddb;
    map<string, vector<int>> eatsinday;
    struct CourseInfo
    {
        string category;
        string counsn;
        string detailss;
    };

public:
    void loadfdb()
    {
        ifstream in("foodDB.txt");
        if (!in) return;
        string line;

        while (getline(in, line))
        {
            istringstream iss(line);
            string food;
            int cal, pro, carb, fat;
            getline(iss, food, ',');
            iss >> cal >> pro >> carb >> fat;
            fooddb[food] = {cal, pro, carb, fat};
        }
        in.close();
    }

    void savefdb()
    {
        ofstream out("foodDB.txt");

        // iterate on maps
        for (map<string, vector<int>>::const_iterator it = fooddb.begin(); it != fooddb.end(); it++) out << it->first << "," << it->second[0] << " " << it->second[1] << " " << it->second[2] << " " << it->second[3] << "\n";
        out.close();
    }

    void whatmeal()
    {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > 4)
        {
            cout << "Invalid input. Please choose a number from 1 to 4.\n";
            cin.clear();
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
        while (true)
        {
            cout << "Food: ";
            getline(cin, food);

            if (food == "done") break;
            
            // add new foods into database
            if (fooddb.find(food) == fooddb.end())
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
                fooddb[food] = {cal, pro, carb, fat};
                savefdb();
            }

            vector<int> macros = fooddb[food];

            if (eatsinday.find(meal) == eatsinday.end()) eatsinday[meal] = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++) eatsinday[meal][i] += macros[i];

            cout << food << " added successfully!\n";
        }
        cout << meal << " entries completed.\n";

        if (eatsinday.count("Breakfast") && eatsinday.count("Lunch") && eatsinday.count("Snacks") && eatsinday.count("Dinner")) displayDailySummary();
        
        // save after every entered meal
        savedaysummary();
    }

    void viewAndSelectCourse()
    {
        ifstream in("courses.txt");
        if (!in)
        {
            cout << "\nSorry, no courses are available.\n";
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
            courseDetails << "Category: " << cat << "\n" << "Description: " << desc << "\n" << "Created by: " << cname;
            courses.push_back({cat, cname, courseDetails.str()});
        }
        in.close();
        if (courses.empty())
        {
            cout << "No courses found in the file.\n";
            return;
        }
        for (size_t i = 0; i < courses.size(); ++i) cout << i + 1 << ". " << courses[i].detailss << "\n---------------------------------\n";
        cout << "Enter the number of the course you want to select (or 0 to cancel): ";
        int choice;
        cin >> choice;

        // error handling
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice > 0 && choice <= courses.size())
        {
            const CourseInfo &selectedCourse = courses[choice - 1];
            ofstream out("enrollments.txt", ios::app);
            out << selectedCourse.category << "," << selectedCourse.counsn << "," << liveuser << "\n";
            out.close();
            cout << "\n*** You have successfully enrolled in the " << selectedCourse.category << " course by " << selectedCourse.counsn << "! ***\n";
        }
        else cout << "\nInvalid selection.\n";
    }

    void displayDailySummary()
    {
        time_t now = time(0);
        char *dt = ctime(&now);

        int totalCal = 0, totalPro = 0, totalCarb = 0, totalFat = 0;
        cout << "\n====== Daily Nutrition Summary ======\n";
        cout << "Report generated on: " << dt;
        cout << "------------------------------------\n";

        for (map<string, vector<int>>::iterator it = eatsinday.begin(); it != eatsinday.end(); ++it)
        {
            cout << it->first << " -> Calories:" << it->second[0] << " Protein:" << it->second[1] << " Carbs:" << it->second[2] << " Fats:" << it->second[3] << "\n";
            totalCal += it->second[0];
            totalPro += it->second[1];
            totalCarb += it->second[2];
            totalFat += it->second[3];
        }
        cout << "------------------------------------\n";
        cout << "TOTAL -> Calories:" << totalCal << " Protein:" << totalPro << " Carbs:" << totalCarb << " Fats:" << totalFat << "\n";

        // compare with planned data
        compareWithPlan(totalCal, totalPro, totalCarb, totalFat);
    }

    struct NutritionPlan
    {
        int calories;
        int protein;
        int carbs;
        int fats;
    };

    queue<NutritionPlan> nutritionQueue;

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
            cout << "\nNo planned nutrition data available.\n";
            return;
        }

        NutritionPlan todayPlan = nutritionQueue.front();

        cout << "\n----- Comparison with Planned Nutrition -----n";
        cout << "Planned -> Calories: " << todayPlan.calories
             << " | Protein: " << todayPlan.protein
             << " | Carbs: " << todayPlan.carbs
             << " | Fats: " << todayPlan.fats << "\n";

        cout << "Actual -> Calories: " << totalCal
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
        if (!exceeded) cout << "All nutrients are within planned limits!\n";

        // remove plan for the completed day
        nutritionQueue.pop();
    }

    void savedaysummary()
    {
        ofstream out(liveuser + "_summary.txt", ios::app);

        time_t now = time(0);
        char *dt = ctime(&now);
        string timeStr = dt;
        timeStr.erase(timeStr.find_last_not_of("\n\r") + 1);

        out << "Daily Summary for " << liveuser << ":\n";

        out << "Last Updated: " << timeStr << "\n";
        out << "-----------------------------\n";

        for (map<string, vector<int>>::iterator it = eatsinday.begin(); it != eatsinday.end(); it++) out << it->first << "," << it->second[0] << "," << it->second[1] << "," << it->second[2] << "," << it->second[3] << "\n";
        out.close();
    }
};

// features that counsellor can use in his dashboard
class CounsellorFeatures
{
public:
    void coursecreate(const string &counsn)
    {
        string cat, desc;
        cout << "Enter Course Category (e.g., Weight Loss): ";
        getline(cin, cat);
        cout << "Enter Course Description: ";
        getline(cin, desc);

        ofstream out("courses.txt", ios::app);
        out << cat << "," << desc << "," << counsn << "\n";
        out.close();
        cout << "Course created successfully.\n";
    }

    void courseview(const string &counsn)
    {
        cout << "\n--- Courses Created By " << counsn << " ---\n";
        ifstream in("courses.txt");
        if (!in)
        {
            cout << "Could not open courses.txt file.\n";
            return;
        }

        string line;
        bool iscourse = false;
        while (getline(in, line))
        {
            istringstream iss(line);
            string cat, desc, filecounsn;

            getline(iss, cat, ',');
            getline(iss, desc, ',');
            getline(iss, filecounsn, ',');

            if (filecounsn == counsn)
            {
                cout << "Category: " << cat << "\n";
                cout << "  Description: " << desc << "\n";
                cout << "---------------------------------\n";
                iscourse = true;
            }
        }
        in.close();

        if (!iscourse) cout << "You have not created any courses yet.\n";
    }

    // which users are in which courses
    void usersenrolled(const string &counsn)
    {
        map<string, string> courseDetailsMap;
        ifstream coursesFile("courses.txt");
        if (coursesFile)
        {
            string line;
            while (getline(coursesFile, line))
            {
                istringstream iss(line);
                string cat, desc, filecounsn;
                getline(iss, cat, ',');
                getline(iss, desc, ',');
                getline(iss, filecounsn, ',');
                if (filecounsn == counsn) courseDetailsMap[cat] = desc;
            }
        }
        coursesFile.close();

        ifstream in("enrollments.txt");
        if (!in)
        {
            cout << "No enrollment data found.\n";
            return;
        }

        map<string, vector<string>> usersbycourse;
        set<string> enrolledun;
        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string courseCat, filecounsn, enrolledUsername;
            getline(iss, courseCat, ',');
            getline(iss, filecounsn, ',');
            getline(iss, enrolledUsername);
            if (filecounsn == counsn)
            {
                usersbycourse[courseCat].push_back(enrolledUsername);
                enrolledun.insert(enrolledUsername);
            }
        }
        in.close();

        if (usersbycourse.empty())
        {
            cout << "No users have enrolled in your courses yet.\n";
            return;
        }

        cout << "\n--- Users Enrolled in Your Courses ---\n";
        for (const auto &pair : usersbycourse)
        {
            string categoryofcourse = pair.first;
            string descofcourse = courseDetailsMap.count(categoryofcourse) ? courseDetailsMap[categoryofcourse] : "N/A";

            cout << "Course: " << categoryofcourse << " (" << descofcourse << ")\n";
            for (int i = 0; i < pair.second.size(); i++) cout << "  - User: " << pair.second[i] << "\n"; 
        }
        cout << "--------------------------------------\n";

        while (true)
        {
            cout << "\nEnter a username to view their summary (or type 'back' to return): ";
            string untofind;
            getline(cin, untofind);
            if (untofind == "back") break;

            if (enrolledun.find(untofind) == enrolledun.end())
            {
                cout << "Error: User '" << untofind << "' is not enrolled in your courses.\n";
                continue;
            }

            string filename = untofind + "_summary.txt";
            ifstream summaries(filename);
            if (summaries.is_open())
            {
                cout << "\n--- Displaying Summary for " << untofind << " ---\n";
                string summaryline;
                while (getline(summaries, summaryline)) cout << summaryline << endl;
                cout << "--- End of Summary ---\n";
                summaries.close();
            }
            else cout << "Could not find a summary file for user '" << untofind << "'.\n";
        }
    }
};

class extrasfornologin
{
private:
    struct fooditem
    {
        string name;
        int calories;
        int protein;
        int carbs;
        int fats;
    };
    vector<fooditem> fooddetails;
    size_t curri = 0;

public:
    void loadfooddb(const string &foodDB)
    {
        if (foodDB.empty()) return;
        ifstream file("foodDB.txt");
        if (!file)
        {
            cout << "Error: Cannot open " << foodDB << endl;
            return;
        }
        fooddetails.clear();
        string line;
        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string name, rest;
            getline(ss, name, ',');
            getline(ss, rest);

            fooditem item;
            item.name = name;
            stringstream values(rest);
            values >> item.calories >> item.protein >> item.carbs >> item.fats;

            fooddetails.push_back(item);
        }
        file.close();
    }

    void next10()
    {

        loadfooddb("foodDB.txt");
        if (fooddetails.empty())
        {
            cout << "Database is empty. Please load it first.\n";
            return;
        }
        size_t total = fooddetails.size();
        size_t start = curri;
        size_t end = min(curri + 10, total);

        cout << "\n---------------------------------------------------------\n";
        cout << left << setw(18) << "Name" << setw(10) << "Calories" << setw(10) << "Protein" << setw(10) << "Carbs" << setw(10) << "Fats" << endl;
        cout << "----------------------------------------------------------\n";

        for (size_t i = start; i < end; ++i) cout << left << setw(18) << fooddetails[i].name << setw(10) << fooddetails[i].calories << setw(10) << fooddetails[i].protein << setw(10) << fooddetails[i].carbs << setw(10) << fooddetails[i].fats << endl;

        cout << "----------------------------------------------------------\n";
        cout << "Showing " << (start + 1) << "-" << end << " of " << total << endl;

        curri = end;

        if (curri >= total)
        {
            cout << "End of database reached. Resetting...\n";
            curri = 0;
        }
        else cout << "Call next10() again to view the next 10 entries.\n";
    }

    void searchfoodinfile()
    {
        ifstream file("foodDB.txt");
        if (!file)
        {
            cout << "Error: Could not open foodDB.txt\n";
            return;
        }

        string query;
        cout << "\nEnter food name to search: ";
        getline(cin, query);

        transform(query.begin(), query.end(), query.begin(), ::tolower);

        string line;
        bool found = false;

        while (getline(file, line))
        {
            if (line.empty()) continue;

            // split line into name and nutrient part
            size_t commaPos = line.find(',');
            if (commaPos == string::npos) continue;

            string name = line.substr(0, commaPos);
            string nutrients = line.substr(commaPos + 1);

            string nameLower = name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

            if (nameLower == query)
            {
                fooditem item;
                item.name = name;

                stringstream ss(nutrients);
                ss >> item.calories >> item.protein >> item.carbs >> item.fats;

                cout << "\n Found in database:\n";
                cout << "----------------------------------------------------------\n";
                cout << left << setw(18) << "Name" << setw(10) << "Calories" << setw(10) << "Protein" << setw(10) << "Carbs" << setw(10) << "Fats" << endl;
                cout << "----------------------------------------------------------\n";
                cout << left << setw(18) << item.name << setw(10) << item.calories << setw(10) << item.protein << setw(10) << item.carbs << setw(10) << item.fats << endl;
                cout << "----------------------------------------------------------\n";
                found = true;
                break;
            }
        }

        if (!found) cout << "'" << query << "' not found in database.\n";
        file.close();
    }
};

// the main function
int main()
{

    User user;
    extrasfornologin *nologinfunc = new extrasfornologin();
    Tracker *system = new Tracker();

    while (true)
    {
        cout << "\n===========================================================================================\n";
        cout << "                                      N U T R I F I T                                      \n";
        cout << "                     where you can calculate, track, and maintain your Meal             \n";
        cout << "============================================================================================\n";
        cout << "     1. Chart Of Nutrition  |  2. Calculate Nutrition For Your Food  |  3. Log In          \n";
        cout << "     4. Create Account       |  5. Exit                                                     \n";
        cout << "--------------------------------------------------------------------------------------------\n";

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
                nologinfunc->next10();
                cout << "\nShow next 10? (y/n): ";
                cin >> choice;
            } 
            while (choice == 'y' || choice == 'Y');

            cout << "\nThank you for visiting\n";
        }

        else if (a == 2)
        {
            nologinfunc->searchfoodinfile();
            break;
        }

        else if (a == 3)
        {
            delete nologinfunc;
            system->loadfdb();
            cout << "Login as: 1.User 2.Counsellor\n";
            int loginc;
            cin >> loginc;

            cin.ignore();
            if (loginc == 1)
            {
                if (user.userlogin() == true)
                {
                    bool loggedIn = true;
                    while (loggedIn)
                    {
                        cout << "\n================================================\n";
                        cout << "                    NutriFit Store                ";
                        cout << "\n------------------------------------------------\n";
                        cout << " 1.MealOfDay 2.SelectCourse 3.PrePlanner 4.Logout  ";
                        cout << "\n------------------------------------------------\n";
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
                            system->whatmeal();
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
                            break;
                        default:
                            cout << "Invalid option. Please try again.\n";
                            break;
                        }
                    }
                }
            }
            else if (loginc == 2)
            {
                Counsellor *counsloginned = Counsellor::counslogin();
                if (counsloginned != nullptr)
                {
                    counsloginned->dashdisplay();
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
                                features.coursecreate(counsloginned->counsn);
                                break;
                            case 2:
                                features.courseview(counsloginned->counsn);
                                break;
                            case 3:
                                features.usersenrolled(counsloginned->counsn);
                                break;
                            case 4:
                                counsellorLoggedIn = false;
                                break;
                            default:
                                cout << "Invalid choice.\n";
                        }
                    }

                    delete counsloginned;
                    counsloginned = nullptr;
                }
            }
            else cout << "Invalid choice.\n";
        }
        else if (a == 4)
        {
            cout << "Register as: 1.User 2.Counsellor\n";
            int regChoice;
            cin >> regChoice;
            cin.ignore();
            if (regChoice == 1) user.userreg();
            else if (regChoice == 2) Counsellor::counsreg();
            else cout << "Invalid choice.\n";
        }
        else if (a == 5)
        {
            delete nologinfunc;
            delete system;
            return 0;
        }
        else cout << "Invalid Choice\n";
    }
    delete nologinfunc;
    delete system;
}
