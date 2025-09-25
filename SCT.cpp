#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cctype>
<<<<<<< HEAD
=======
#include <limits> // Required for input buffer clearing
#include <ctime>   // Required for getting the current time

>>>>>>> origin/AdityaMain
using namespace std;

class User
{
private:
    string username;
    string userpassword;

public:
    User(string u, string p)
    {
        username = u;
        userpassword = p;
    }

    string getUsername() const
    {
        return username;
    }

    string checkpassword() const
    {
        return userpassword;
    }
<<<<<<< HEAD

    // friend void showuser(const User &u);
};

// void showUser(const User &u){
//     cout<<"Username: "<< u.username<<"Password : "<< u.userpassword;
// }
=======
};

struct CourseInfo {
    string category;
    string counsellorName;
    string fullDetails;
};
>>>>>>> origin/AdityaMain

class Tracker
{
private:
    string currentUser;
    map<string, vector<int>> foodDatabase;
    map<string, vector<int>> dailyIntake;

public:
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
<<<<<<< HEAD
        for (auto &x : foodDatabase)
        {
            out << x.first << "," << x.second[0] << " " << x.second[1] << " " << x.second[2] << " " << x.second[3] << "\n";
        }
        out.close();
    }
=======
        for (map<string, vector<int>>::const_iterator it = foodDatabase.begin(); it != foodDatabase.end(); ++it)
        {
            out << it->first << "," << it->second[0] << " " << it->second[1] << " " << it->second[2] << " " << it->second[3] << "\n";
        }
        out.close();
    }

>>>>>>> origin/AdityaMain
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
<<<<<<< HEAD
        out << "\n"
            << line;
        out.close();
        cout << "User registerted";
=======
        out << "\n" << line;
        out.close();
        cout << "User registered";
>>>>>>> origin/AdityaMain
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

    void enterMeal()
    {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;

<<<<<<< HEAD
        if (choice < 1 || choice > 4)
        {
            cout << "Invalid input. Exiting...\n";
=======
        if (cin.fail() || choice < 1 || choice > 4)
        {
            cout << "Invalid input. Please choose a number from 1 to 4.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
>>>>>>> origin/AdityaMain
            return;
        }
        cin.ignore();
        string meal;
<<<<<<< HEAD
        if (choice == 1)
            meal = "Breakfast";
        else if (choice == 2)
            meal = "Lunch";
        else if (choice == 3)
            meal = "Snacks";
        else if (choice == 4)
            meal = "Dinner";
=======
        if (choice == 1) meal = "Breakfast";
        else if (choice == 2) meal = "Lunch";
        else if (choice == 3) meal = "Snacks";
        else if (choice == 4) meal = "Dinner";
>>>>>>> origin/AdityaMain

        cout << "Start adding food items for " << meal << ". Type 'done' when finished.\n";

        string food;
        while (true)
        {
            cout << "Food: ";
            getline(cin, food);

<<<<<<< HEAD
            if (food == "done")
                break;

            else
            {

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
=======
            if (food == "done") break;

            if (foodDatabase.find(food) == foodDatabase.end())
            {
                cout << "Food not found in database. Enter macros for " << food << ":\n";
                int cal, pro, carb, fat;
                cout << "Calories: "; cin >> cal;
                if (cin.fail()){ cout << "Not an int!!" << endl; return; }
                cout << "Protein: "; cin >> pro;
                if (cin.fail()){ cout << "Not an int!!" << endl; return; }
                cout << "Carbs: "; cin >> carb;
                if (cin.fail()){ cout << "Not an int!!" << endl; return; }
                cout << "Fats: "; cin >> fat;
                if (cin.fail()){ cout << "Not an int!!" << endl; return; }
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

        if (dailyIntake.count("Breakfast") && dailyIntake.count("Lunch") && dailyIntake.count("Snacks") && dailyIntake.count("Dinner")) {
             displayDailySummary();
        }
        saveDailySummary(); // Save after every meal entry
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
        if (courses.empty()) {
            cout << "No courses found in the file.\n";
            return;
        }
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << i + 1 << ". " << courses[i].fullDetails << "\n---------------------------------\n";
        }
        cout << "Enter the number of the course you want to select (or 0 to cancel): ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice > 0 && choice <= courses.size()) {
            const CourseInfo& selectedCourse = courses[choice - 1];
            ofstream out("enrollments.txt", ios::app);
            out << selectedCourse.category << "," << selectedCourse.counsellorName << "," << currentUser << "\n";
            out.close();
            cout << "\n*** You have successfully enrolled in the '" << selectedCourse.category << "' course by " << selectedCourse.counsellorName << "! ***\n";
        } else {
            cout << "\nInvalid selection or action cancelled.\n";
>>>>>>> origin/AdityaMain
        }
    }

    void displayDailySummary()
    {
<<<<<<< HEAD
        int totalCal = 0, totalPro = 0, totalCarb = 0, totalFat = 0;
        cout << "\n====== Daily Nutrition Summary ======\n";
        for (auto &x : dailyIntake)
        {
            cout << x.first << " -> Calories:" << x.second[0] << " Protein:" << x.second[1] << " Carbs:" << x.second[2] << " Fats:" << x.second[3] << "\n";
            totalCal += x.second[0];
            totalPro += x.second[1];
            totalCarb += x.second[2];
            totalFat += x.second[3];
=======
        time_t now = time(0);
        char* dt = ctime(&now);

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
>>>>>>> origin/AdityaMain
        }
        cout << "------------------------------------\n";
        cout << "TOTAL -> Calories:" << totalCal << " Protein:" << totalPro << " Carbs:" << totalCarb << " Fats:" << totalFat << "\n";
    }

<<<<<<< HEAD
    void saveDailySummary()
    {
        ofstream out(currentUser + "_summary.txt", ios::app);
        out << "Daily Summary for " << currentUser << ":\n";
        for (auto &x : dailyIntake)
        {
            out << x.first << "," << x.second[0] << "," << x.second[1] << "," << x.second[2] << "," << x.second[3] << "\n";
        }
        out << "-----------------------------\n";
=======
    // ** MODIFIED to store the current date and time in the file **
    void saveDailySummary()
    {
        ofstream out(currentUser + "_summary.txt");
        
        // Get the current time
        time_t now = time(0);
        char* dt = ctime(&now);

        // Convert to string and remove the newline character that ctime adds
        string timeStr = dt;
        timeStr.erase(timeStr.find_last_not_of("\n\r") + 1);

        out << "Daily Summary for " << currentUser << ":\n";
        // Write the timestamp to the file
        out << "Last Updated: " << timeStr << "\n";
        out << "-----------------------------\n";
        
        // Write the meal data
        for (map<string, vector<int>>::iterator it = dailyIntake.begin(); it != dailyIntake.end(); ++it)
        {
            out << it->first << "," << it->second[0] << "," << it->second[1] << "," << it->second[2] << "," << it->second[3] << "\n";
        }
>>>>>>> origin/AdityaMain
        out.close();
    }
};

int main()
{
    Tracker *system = new Tracker();
<<<<<<< HEAD
    while (true)
    {
        cout << "\n===========================================================================================\n";
        cout << "                             Simple Fitness/Calorie Tracker                                   ";
        cout << "\n===========================================================================================\n";
        cout << "   1.ChartOfNutrition | 2.CalculateNutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit  ";
=======
    system->loadFoodDatabase();

    while (true)
    {
        cout << "\n===========================================================================================\n";
        cout << "                                 Simple Fitness/Calorie Tracker                                  ";
        cout << "\n===========================================================================================\n";
        cout << "   1.ChartOfNutrition | 2.CalculateNutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit   ";
>>>>>>> origin/AdityaMain
        cout << "\n-------------------------------------------------------------------------------------------\n";

        int a;
        cout << "Choose Option:";
        cin >> a;
        if (cin.fail())
        {
            cout << "Wrong input!!" << endl;
<<<<<<< HEAD
=======
            delete system;
>>>>>>> origin/AdityaMain
            return 0;
        }
        cin.ignore();

<<<<<<< HEAD
        if (a == 1)
        {
            break;
        }
        else if (a == 2)
        {
            break;
        }
        else if (a == 4)
        {
            system->registerUser();
            break;
        }

=======
        if (a == 4)
        {
            system->registerUser();
        }
>>>>>>> origin/AdityaMain
        else if (a == 3)
        {
            if (system->loginUser() == true)
            {
<<<<<<< HEAD
                cout << "\n======================================================\n";
                cout << "|           Welcome choose operation to perfrom        |";
                cout << "\n------------------------------------------------------\n";
                cout << " 1.EnterMeal 2.CalculateNutrition 3.PrePlanDaily 4.Store";
                cout << "\n-------------------------------------------------------\n";
                int x = 0;
                cin >> x;
                cin.ignore();
                switch (x)
                {
                case 1:
                    system->enterMeal();
                    break;

                case 2:
                    break;

                case 3:
                    break;

                case 4:
                    cout << "\n======================================================\n";
                    cout << " |                   NutriFit Store                     |";
                    cout << "\n------------------------------------------------------\n";
                    cout << " 1.Explore Plan  2.Schedule Counsellor Meeting 3.-------";
                    cout << "\n-------------------------------------------------------\n";
                    int x = 0;
                    cin >> x;
                    cin.ignore();
                    // system->;
                    break;
=======
                bool loggedIn = true;
                while(loggedIn) {
                    cout << "\n=============================================\n";
                    cout << "           Welcome choose operation to perform   ";
                    cout << "\n---------------------------------------------\n";
                    cout << " 1.EnterMeal 2.Select Course 3.Logout ";
                    cout << "\n---------------------------------------------\n";
                    cout << "Choose Option: ";
                    
                    int x;
                    cin >> x;
                     if (cin.fail()) {
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
                        cout << "Logging out...\n";
                        loggedIn = false;
                        break;
                    default:
                        cout << "Invalid option. Please try again.\n";
                        break;
                    }
>>>>>>> origin/AdityaMain
                }
            }
        }
        else if (a == 5)
        {
            delete system;
            return 0;
        }
<<<<<<< HEAD
    }
    delete system;
=======
        else {
             cout << "This option is not yet implemented.\n";
        }
    }
    delete system;
    return 0;
>>>>>>> origin/AdityaMain
}