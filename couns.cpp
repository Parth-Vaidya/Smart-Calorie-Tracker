#include <bits/stdc++.h>
using namespace std;

class Counsellor
{
    string counsellorName;
    string username;
    string password;

public:
    Counsellor(string cname = "default", string uname = "user", string pass = "123") : counsellorName(cname), username(uname), password(pass) {}
    string getCounsellorName() const { return counsellorName; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }

    virtual void displayProfile() const
    {
        cout << "Counsellor Name: " << counsellorName << "\nUsername: " << username << "\n";
    }
    friend class CounsellorPortal;
};

class Course
{
    string category;
    int targetCalories;
    int fats, protein, carbs;
    string description;
    string counsellorName;

public:
    Course(string cat, int cal, int f, int p, int c, string desc, string cname) : category(cat), targetCalories(cal), fats(f), protein(p), carbs(c), description(desc), counsellorName(cname) {}

    void saveCourse()
    {
        ofstream out("courses.txt", ios::app);
        out << category << "," << targetCalories << "," << fats << "," << protein << "," << carbs << "," << description << "," << counsellorName << "\n";
        out.close();
    }

    static void viewCourses()
    {
        ifstream in("courses.txt");
        if (!in)
        {
            cout << "No courses available.\n";
            return;
        }
        string line;
        cout << "\n--- Available Courses ---\n";
        while (getline(in, line))
        {
            istringstream iss(line);
            string cat, cal, f, p, c, desc, cname;
            getline(iss, cat, ',');
            getline(iss, cal, ',');
            getline(iss, f, ',');
            getline(iss, p, ',');
            getline(iss, c, ',');
            getline(iss, desc, ',');
            getline(iss, cname);
            cout << "Category: " << cat << "\nCalories: " << cal << "\nFats: " << f << " Protein: " << p << " Carbs: " << c << "\nDescription: " << desc << "\nCounsellor: " << cname << "\n----------------------\n";
        }
    }
};

class CounsellorPortal
{
    map<string, Counsellor> counsellors;

public:
    void loadCredentials()
    {
        ifstream in("counsellors.txt");
        if (!in)
            return;
        string line;
        while (getline(in, line))
        {
            istringstream iss(line);
            string cname, uname, pass;
            getline(iss, cname, ',');
            getline(iss, uname, ',');
            getline(iss, pass);
            counsellors[uname] = Counsellor(cname, uname, pass);
        }
    }

    void saveCredentials()
    {
        ofstream out("counsellors.txt");
        for (auto &c : counsellors)
        {
            out << c.second.getCounsellorName() << "," << c.second.getUsername() << "," << c.second.getPassword() << "\n";
        }
    }

    // Overloaded auth functions
    void auth(string cname, string uname, string pass)
    {
        if (counsellors.find(uname) != counsellors.end())
        {
            cout << "Username already exists.\n";
            return;
        }
        counsellors[uname] = Counsellor(cname, uname, pass);
        saveCredentials();
        cout << "Counsellor registered successfully.\n";
    }

    Counsellor *auth(string uname, string pass)
    {
        auto it = counsellors.find(uname);
        if (it == counsellors.end())
        {
            cout << "No such user.\n";
            return nullptr;
        }
        if (it->second.getPassword() == pass)
        {
            cout << "Login successful. Welcome " << it->second.getCounsellorName() << "!\n";
            return &it->second;
        }
        else
        {
            cout << "Password incorrect.\n";
            return nullptr;
        }
    }
};

int main()
{
    CounsellorPortal portal;
    portal.loadCredentials();

    while (true)
    {
        cout << "\n--- Counsellor Menu ---\n";
        cout << "1. Register\n2. Login\n3. Exit\nChoose: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            string cname, uname, pass;
            cout << "Enter Counsellor Name: ";
            getline(cin, cname);
            cout << "Enter Username: ";
            getline(cin, uname);
            cout << "Enter Password: ";
            getline(cin, pass);
            portal.auth(cname, uname, pass);
        }
        else if (choice == 2)
        {
            string uname, pass;
            cout << "Enter Username: ";
            getline(cin, uname);
            cout << "Enter Password: ";
            getline(cin, pass);
            Counsellor *c = portal.auth(uname, pass);
            if (c)
            {
                cout << "\n--- Profile ---\n";
                c->displayProfile();
                cout << "----------------\n";

                while (true)
                {
                    cout << "\n--- Course Menu ---\n";
                    cout << "1. Create Course\n2. View Courses\n3. Logout\nChoose: ";
                    int cchoice;
                    cin >> cchoice;
                    cin.ignore();

                    if (cchoice == 1)
                    {
                        int cal, f, p, carb;
                        string cat, desc;

                        int catChoice;
                        cout << "Enter Course Category (1 = Weight Loss, 2 = Weight Gain): ";
                        if (!(cin >> catChoice))
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            throw runtime_error("Invalid input! Please enter a number (1 or 2).");
                        }
                        if (catChoice == 1)
                            cat = "Weight Loss";
                        else if (catChoice == 2)
                            cat = "Weight Gain";
                        else
                            throw runtime_error("Invalid choice! Enter 1 or 2 only.");

                        cout << "Enter Target Calories: ";
                        cin >> cal;
                        cout << "Enter Target Fats: ";
                        cin >> f;
                        cout << "Enter Target Protein: ";
                        cin >> p;
                        cout << "Enter Target Carbs: ";
                        cin >> carb;
                        cin.ignore();

                        cout << "Enter Course Description: ";
                        getline(cin, desc);

                        Course newCourse(cat, cal, f, p, carb, desc, c->getCounsellorName());
                        newCourse.saveCourse();
                        cout << "Course created successfully.\n";
                    }
                    else if (cchoice == 2)
                    {
                        Course::viewCourses();
                    }
                    else break;
                }
            }
        }
        else break;
    }
    return 0;
}
