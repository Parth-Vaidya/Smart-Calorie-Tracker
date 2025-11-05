#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cctype>
using namespace std;

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

class User : public Account {
private:
    string currentUser;
public:
    // The constructor now calls the base class constructor
    User(string u, string p) : Account(u, p) {}
    User() : Account("", "") {}
    string getCurrentUser() const { return currentUser; }


    // MODIFIED: Providing the required implementation for the virtual function.
    // The 'override' keyword is good practice to ensure we are correctly overriding a base class function.
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

int main()
{
    User user;
    while (true)
    {
        cout << "\n===========================================================================================\n";
        cout << "                          Simple Fitness/Calorie Tracker                          ";
        cout << "\n===========================================================================================\n";
        cout << "   1.ChartOfNutrition | 2.CalculateNutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit  ";
        cout << "\n-------------------------------------------------------------------------------------------\n";

        int a;
        cout << "Choose Option:";
        cin >> a;
        if (cin.fail()) {
            cout << "Wrong input!!" << endl;
            return 0;
        }
        cin.ignore();

        if (a == 4) {
            user.registerUser();
        }
        else if (a == 3) {
            if (user.loginUser()) {
                Tracker system;
                system.loadFoodDatabase();
                system.setCurrentUser(user.getCurrentUser());
                cout << "\n=============================================\n";
                cout << "      Welcome choose operation to perform     ";
                cout << "\n---------------------------------------------\n";
                cout << " 1.EnterMeal 2.CalculateNutrition 3.PrePlanDaily ";
                cout << "\n---------------------------------------------\n";
                int x;
                cin >> x;
                cin.ignore();

                if (x == 1)
                    system.enterMeal();
            }
        }
        else if (a == 5)
            return 0;
    }
}
