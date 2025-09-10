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


    Tracker(IDataManager* dm) : dataManager(dm), currentUser(nullptr) {
        users = dataManager->loadUsers();
        foodDatabase = dataManager->loadFoodDatabase();
    }


    //register login logout implemented by parth

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

    //---------------------------//-------------------------------//

    void add_FOOD_TO_LOG(){
        if (!currentUser)
        {   
            console.log("Error : User does not Exist")
            return 
        }

        cout<<"\n Select Meal Time "<< endl;
        cout << "1. Breakfast\n2. Lunch\n3. Dinner\n4. Snacks\n";
        int choice ; 
        string mealType;
        cin<<choice;
        switch (choice)
        {
        case 1:

            mealType = "Breakfast" ; 
            break;

        case 2:
            mealType = "Lunch";
            break;

        case 3:
            mealType = "Dinner";
            break;

        case 4: 
            mealType = "Snacks"; 
            break;


        
            default:
                cout<<"Invalid Choice . " <<endl;
                break;
        }

        vector<FoodItem> searchResults ; 

        cout << "\nEnter food name to search or add: ";

        string foodName ; 
        getline(cin , foodName);


        
        
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

//  core functionality



    return 0; 
}