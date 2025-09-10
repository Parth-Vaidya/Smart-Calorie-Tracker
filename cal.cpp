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