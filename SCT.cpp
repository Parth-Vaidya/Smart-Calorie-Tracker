#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

class FoodItem;
class Meal;
class Dailylog;
class User;
class Tracker;

class FoodItem
{
private:
    string name;
    int calories;
    int protein;
    int carbs;
    int fats;

public:
    FoodItem(string n, int cal, int p, int c, int f)
        : name(n), calories(c), protein(p), carbs(c), fats(f) {}

    string getname() const { return name; }
    int getcalories() const { return calories; }
    int getprotein() const { return protein; }
    int getcarbs() const { return carbs; }
    int getfats() const { return fats; }

    void display() const
    {
        cout << left << setw(10) << name << "| Cal: " << calories << setw(5) << "| Prot: " << protein << setw(5) << " | Carb: " << setw(5) << carbs << " | Fat: " << setw(5) << fats;
    }
};

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

// class LoginSystem
// {
// private:
//     vector<User> user;

// public:
//     void registerUser(const string un, const string p)
//     {
//         for (const auto u : user)
//         {
//             if (u.getUsername() == un)
//             {
//                 cout << "name alrady exist";
//                 return;
//             }
//         }

//         user.push_back(User(un, p));
//         cout << "User registerted";
//     }

//     bool login(const string un, const string pass)
//     {
//         for (const auto u : user)
//         {
//             if (u.getUsername() == un && u.checkpassword() == pass)
//             {
//                 cout << "login successful";
//                 return true;
//             }
//         }
//         cout << "invalid name and paswword";
//         return false;
//     }
// };

class Tracker
{
    private:
    vector<User> user;

    public:

    void registerUser()
    {
        string un, p;
        cout<<"-----Register New User-----\n";
        cout<<"Enter Username: ";
        cin>>un;
        for (const auto u : user)
        {
            if (u.getUsername() == un)
            {
                cout << "name alrady exist";
                return;
            }
        }
        cout<<"Enter Password:";
        cin>>p;
        user.push_back(User(un, p));
        cout << "User registerted";
    }

    bool loginUser(){
        string u,p;
        int a=8;
        // while(a<=10){
        cout<<"-------User LogIn-------\n";
        cout<<"Enter User name:";
        cin>>u;
        cout<<"Enter Password:";
        cin>>p;
        for(const auto a:user){
            if(a.getUsername()==u && a.checkpassword()==p){
                cout<<"Login successfull";
                // int s=11;
                // a=s;
            }
            else{
                cout<<"Invalid Credentials!\ntry again";
                // a++;
                return false;
            }
        }
    // }   
    }
};

int main()
{
    Tracker system;
    while (1)
    {
        cout << "\n=====================================================================================\n";
        cout << "                          Simple Fitness/Calorie Tracker                          ";
        cout << "\n=====================================================================================\n";
        cout << "   1.ChartOfNeutrition | 2.CalculateNeutrition4YourFood | 3.LogIn | 4.CreateAccount | 5.Exit  ";
        cout << "\n-------------------------------------------------------------------------------------\n";

        int a;
        cin >> a;

        string u, p;
        if (a == 1)
        {
        }

        else if (a == 4)
        {
            system.registerUser();
            // cout << "Enter user name";
            // cin >> u;
            // cout << "Enter user password";
            // cin >> p;
            // system.registerUser(u, p);
        }

        else if (a == 3)
        {
            // cout << "Enter username: ";
            // cin >> u;
            // cout << "Enter password: ";
            // cin >> p;
            if (system.loginUser() == true)
            {
                cout << "\n=====================================================================================\n";
                cout << u << " choose function you want to with";
                cout << "\n-------------------------------------------------------------------------------------\n";

                int x = 0;
                switch (x)
                {
                case 0:

                    break;

                default:
                    break;
                }
            }
        }
        else if (a == 5)
        {
            return 0;
        }
    }
}