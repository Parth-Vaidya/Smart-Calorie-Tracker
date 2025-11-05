#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
<<<<<<< Updated upstream

=======
#include <cctype>
#include <limits>
#include <ctime>
#include <queue>
// #include counsellor.cpp
>>>>>>> Stashed changes
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
        string line = un + ","+ p;

        ofstream out("users.txt", ios::app);
        if (!out)
        {
            cout << "File could not be opened\n";
            return;
        }
        out << "\n"<< line;
        out.close();
        cout << "User registerted";
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
        
        if (choice % 1 != 0) 
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
                if(cal % 1 != 0)
                {
                    cout << "Not an int!!" << endl;
                    return;
                }
                cout << "Protein: ";
                cin >> pro;
                if(pro % 1 != 0)
                {
                    cout << "Not an int!!" << endl;
                    return;
                }
                cout << "Carbs: ";
                cin >> carb;
                if(carb % 1 != 0)
                {
                    cout << "Not an int!!" << endl;
                    return;
                }
                cout << "Fats: ";
                cin >> fat;
                if(fat % 1 != 0)
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

        if (meal == "Dinner")
        {
            displayDailySummary();
<<<<<<< Updated upstream
            saveDailySummary();
=======
        }
        saveDailySummary(); // Save after every meal entry
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
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    }

        void saveDailySummary()
=======

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
>>>>>>> Stashed changes
    {
        ofstream out(currentUser + "_summary.txt", ios::app);
        out << "Daily Summary for " << currentUser << ":\n";
<<<<<<< Updated upstream
        for (auto &x : dailyIntake)
=======

        out << "Last Updated: " << timeStr << "\n";
        out << "-----------------------------\n";

        for (map<string, vector<int>>::iterator it = dailyIntake.begin(); it != dailyIntake.end(); ++it)
>>>>>>> Stashed changes
        {
            out << x.first << "," << x.second[0] << "," << x.second[1] << "," << x.second[2] << "," << x.second[3] << "\n";
        }
        out << "-----------------------------\n";
        out.close();
    }
};

<<<<<<< Updated upstream
int main()
{
    Tracker system;
      while (1)
=======
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

    FunctionwithoutLogin *nologinfunc = new FunctionwithoutLogin();
    Tracker *system = new Tracker();

    while (true)
>>>>>>> Stashed changes
    {
        cout << "\n===========================================================================================\n";
        cout << "                          Simple Fitness/Calorie Tracker                          ";
        cout << "\n===========================================================================================\n";
        cout << "   1.ChartOfNeutrition | 2.CalculateNeutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit  ";
        cout << "\n-------------------------------------------------------------------------------------------\n";

        int a;
        cout<<"Choose Option:";
        cin >> a;
<<<<<<< Updated upstream
=======
        if (cin.fail())
        {
            cout << "Wrong input!!" << endl;
            return 0;
        }
>>>>>>> Stashed changes
        cin.ignore();

        if (a == 1)
        {
<<<<<<< Updated upstream

        }

        else if (a == 4)
        {
            system.registerUser();
=======
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
>>>>>>> Stashed changes
        }

        else if (a == 3)
        {
<<<<<<< Updated upstream
             while (1)
    {
            if (system.loginUser() == true)
=======
            delete nologinfunc;
            system->loadFoodDatabase();
            if (system->loginUser() == true)
>>>>>>> Stashed changes
            {
                cout << "\n=============================================\n";
                cout <<"     Welcome choose operation to perfrom     ";
                cout << "\n---------------------------------------------\n";
                cout<<" 1.EnterMeal 2.CalculateNeutrition 3.PrePlanDaily";
                cout << "\n---------------------------------------------\n";
                int x = 0;
                cin>>x;
                switch (x)
                {
<<<<<<< Updated upstream
                case 1:
                    system.enterMeal();
                    break;
=======
                    cout << "\n================================================\n";
                    cout << "                    NutriFit Store                ";
                    cout << "\n------------------------------------------------\n";
                    cout << " 1.EnterMeal 2.Select Course 3.PrePlanner 4.Logout  ";
                    cout << "\n------------------------------------------------\n";
                    cout << "Choose Option: ";
>>>>>>> Stashed changes

                default:
                    break;
                }
            }
        }
<<<<<<< Updated upstream
        }
        else if (a == 5)
        {
=======
        else if (a == 4)
        {
            system->registerUser();
        }
        else if (a == 5)
        {
            delete nologinfunc;
            delete system;
>>>>>>> Stashed changes
            return 0;
        }
    }
<<<<<<< Updated upstream
=======
    delete nologinfunc;
    delete system;
    return 0;
>>>>>>> Stashed changes
}