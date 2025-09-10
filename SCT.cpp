#include<iostream>
#include<map>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <limits>
#include <algorithm>
#include<vector>
#include<ctime> 

using namespace std;

class IDataManager {
    // Interface for data handling
};

class User{
    // User data
};

class FoodItem{
    // Food data
};

class Tracker {
private:
    IDataManager *dataManager;
    map<string, User> users;
    User * currentUser;
    vector<FoodItem> foodDatabase;

public: 

    // this is a helper function should be shifted to the private section
    string getDate() {
        time_t t = time(0);
        tm* now = localtime(&t);

        stringstream ss;
        ss << (now->tm_year + 1900) << '-'
           << setfill('0') <<setw(2) << (now->tm_mon + 1) << '-'
           << setfill('0') <<setw(2) << now->tm_mday;
        return ss.str();
    }

    string toLower(string s){
        transform(s.begin() ,s.end() , s.begin() ,:: tolower);
        return s;

    }


    
};

int main() {
//time implementation

    Tracker t;
    string date = t.getDate();
    cout << date;


// string to lower implementation

    string s = "AadfdfbhhJHHJLHL gdsfgsdgsd";

    string lower = t.toLower(s);
    cout<<lower;

//

    return 0; 
}