#include <iostream>
#include <vector>
#include <string>
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

class LoginSystem
{
private:
    vector<User> user;

public:
    void registerUser(const string un, const string p)
    {
        for (const auto u : user)
        {
            if (u.getUsername() == un)
            {
                cout << "name alrady exist";
                return;
            }
        }

        user.push_back(User(un, p));
        cout << "User registerted";
    }

    bool login(const string un, const string pass)
    {
        for (const auto u : user)
        {
            if (u.getUsername() == un && u.checkpassword() == pass)
            {
                cout << "login successful";
                return true;
            }
        }
        cout << "invalid name and paswword";
        return false;
    }
};

int main()
{
    LoginSystem system;
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
            cout << "Enter user name";
            cin >> u;
            cout << "Enter user password";
            cin >> p;
            system.registerUser(u, p);
        }

        else if (a == 3)
        {
            cout << "Enter username: ";
            cin >> u;
            cout << "Enter password: ";
            cin >> p;
            if (system.login(u, p) == true)
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
        else if(a==5){
            return 0;
        }
    }
}