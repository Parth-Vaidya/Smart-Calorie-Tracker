#include <bits/stdc++.h>
using namespace std;

// declare classes 
class FoodItem;
class Meal;
class DailyLog;
class User;
class IDataManager;
class Tracker;

// food item with calories, all the macros and an option to add micros
class FoodItem {
private:
    string name;
    double calories;
    double protein; 
    double carbs;   
    double fats;  
    map<string, double> micronutrients;

public:
    // Constructor
    FoodItem(string n, double cal, double p, double c, double f)
    {
        name = n.empty() ? "Unknown" : n;
        calories = (cal >= 0) ? cal : 0;
        protein  = (p   >= 0) ? p : 0;
        carbs    = (c   >= 0) ? c : 0;
        fats     = (f   >= 0) ? f : 0; 
    }

    // 001. Encapsulation
    string getName() const { return name; }
    double getCalories() const { return calories; }
    double getProtein() const { return protein; }
    double getCarbs() const { return carbs; }
    double getFats() const { return fats; }
    const map<string, double>& getMicronutrients() const { return micronutrients; }

    void addMicro(const string& microName, double value) 
    {
        if (microName.empty())
        {
            cout << "Invalid micronutrient name." << endl;
            return;
        }
        if (value < 0)
        {
            cout << "Negative calories don't exist!" << endl;
            return;
        }
        micronutrients[microName] = value;
    }

    // display details
    void display() const 
    {
        cout << "    - " << left << setw(20) << name << " | Cal: " << setw(6) << calories << " | P: " << setw(5) << protein << "g" << " | C: " << setw(5) << carbs << "g" << " | F: " << setw(5) << fats << "g" << endl;
    }
};

class Meal {
private:
    string mealType;
    vector<FoodItem> foodItems;

public:
    // Constructor
    // 002. Aggregation
    Meal(string type) : mealType(type) {}

    void addFood(const FoodItem& food) 
    {
        foodItems.push_back(food);
    }

    double getTotalCalories() const 
    {
        double total = 0;
        for (const auto& item : foodItems) total += item.getCalories();
        return total;
    }

    double getTotalProtein() const 
    {
        double total = 0;
        for (const auto& item : foodItems) total += item.getProtein();
        return total;
    }

    double getTotalCarbs() const 
    {
        double total = 0;
        for (const auto& item : foodItems) total += item.getCarbs();
        return total;
    }
    
    double getTotalFats() const 
    {
        double total = 0;
        for (const auto& item : foodItems) total += item.getFats();
        return total;
    }

    void display() const 
    {
        if (foodItems.empty()) return;
        cout << "  --- " << mealType << " ---" << endl;
        for (const auto& item : foodItems) item.display();
    }
    
    const vector<FoodItem>& getFoodItems() const 
    {
        return foodItems;
    }
};

class DailyLog {
private:
    string date;
    map<string, Meal> meals;

public:
    
    // 003. Composition
    DailyLog() : date("") 
    {
        meals.emplace("Breakfast", Meal("Breakfast"));
        meals.emplace("Lunch", Meal("Lunch"));
        meals.emplace("Snacks", Meal("Snacks"));
        meals.emplace("Dinner", Meal("Dinner"));   
    }

    // Constructor
    DailyLog(string d) : date(d) 
    {
        meals.emplace("Breakfast", Meal("Breakfast"));
        meals.emplace("Lunch", Meal("Lunch"));
        meals.emplace("Snacks", Meal("Snacks"));
        meals.emplace("Dinner", Meal("Dinner"));
    }

    void addFoodToMeal(const string& mealType, const FoodItem& food) 
    {
        auto it = meals.find(mealType);
        if (it != meals.end()) it->second.addFood(food);
        else cout << "Meal type does not exist" << mealType << endl;
    }
    
    string getDate() const { return date; }
    const map<string, Meal>& getMeals() const { return meals; }

    void displaySummary() const 
    {
        double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFats = 0;
        
        cout << "\n===== Daily Summary for " << date << " =====\n" << endl;

        for (const auto& pair : meals) 
        {
            const Meal& meal = pair.second;
            meal.display();
            totalCalories += meal.getTotalCalories();
            totalProtein += meal.getTotalProtein();
            totalCarbs += meal.getTotalCarbs();
            totalFats += meal.getTotalFats();
        }

        cout << "\n----------------------------------------" << endl;
        cout << "Total Daily Intake:" << endl;
        cout << "  Calories: " << totalCalories << " kcal" << endl;
        cout << "  Protein:  " << totalProtein << " g" << endl;
        cout << "  Carbs:    " << totalCarbs << " g" << endl;
        cout << "  Fats:     " << totalFats << " g" << endl;
        cout << "========================================\n" << endl;
    }
};
