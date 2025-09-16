//Chirag Wattamwar
//202411029

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>

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
};

class Tracker
{
private:
    string currentUser;
    map<string, vector<int>> foodDatabase; // food -> {cal, protein, carbs, fats}
    map<string, vector<int>> dailyIntake;  // meal -> aggregated macros

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
        for (auto &x : foodDatabase)
        {
            out << x.first << "," << x.second[0] << " " << x.second[1] << " " << x.second[2] << " " << x.second[3] << "\n";
        }
        out.close();
    }

    void registerUser()
    {
        string un, p;
        cout << "-----Register New User-----\n";
        cout << "Enter Username: ";
        getline(cin, un);

        cout << "Enter Password:";
        getline(cin, p);
        string line = un + "," + p;

        ofstream out("users.txt", ios::app);
        if (!out)
        {
            cout << "File could not be opened\n";
            return;
        }
        out << "\n"
            << line;
        out.close();
        cout << "User registerted";
    }

    bool loginUser()
    {
        string u, p;
        ifstream in("users.txt");
        if (!in)
        {
            cout << "file could not open";
            return 1;
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
                    currentUser = u;
                    loadFoodDatabase();
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
        return false;
        in.close();
    }

    void enterMeal()
    {
        cout << "\nChoose Meal: 1.Breakfast 2.Lunch 3.Snacks 4.Dinner\n";
        int choice;
        cin >> choice;
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
                cout << "Protein: ";
                cin >> pro;
                cout << "Carbs: ";
                cin >> carb;
                cout << "Fats: ";
                cin >> fat;
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
    Tracker system;
    while (1)
    {
        cout << "\n===========================================================================================\n";
        cout << "                          Simple Fitness/Calorie Tracker                          ";
        cout << "\n===========================================================================================\n";
        cout << "                      1. LogIn | 2.Create Account | 3. Exit  ";
        cout << "\n-------------------------------------------------------------------------------------------\n";

        int a;
        cout << "Choose Option:";
        cin >> a;
        cin.ignore();

        if (a == 1)
        {
            if (system.loginUser() == true)
            {
                cout << "\n=============================================\n";
                cout << "     Welcome choose operation to perform     ";
                cout << "\n---------------------------------------------\n";
                cout << " 1.EnterMeal 2.CalculateNeutrition 3.PrePlanDaily";
                cout << "\n---------------------------------------------\n";
                int x = 0;
                cin >> x;
                cin.ignore();
                switch (x)
                {
                case 1:
                    system.enterMeal();
                    break;

                default:
                    break;
                }
            }
        }
        else if (a == 2)
        {
            system.registerUser();
        }

        else if (a == 3)
        {
            return 0;
        }
    }
}
