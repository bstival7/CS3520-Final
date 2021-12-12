// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include <fstream>
#include "its.hpp"

using namespace std;

// All users currently registered
vector<User*> users;
vector<Project*> projects;
User * current;

// Import users from users.txt
void import_data() {
  ifstream u("users.txt");
  string user;
  while (getline(u, user)) {
    users.push_back(new User(user));
  }
  u.close();
}

// Login script
void run_login() {
  bool exit = false;
  char choice;
  //User current_user;
  //Prompts the user to login or register, exits after successful login
  while (!exit) {
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cin >> choice;
    switch (choice) {
      case '1': {
        string username;
        cout << "Enter your username to login: ";
        cin >> username;
        for (User *u : users) {
          if (u->getUsername() == username) {
            current = u;
            exit = true;
            cout << "Logged in succesfully." << endl;
            break;
          } 
        }
        cout << "Username "  << "\"" << username << "\"" <<  " does not exist. Try again." << endl;
        break;
      } case '2': {
        string username;
        cout << "Enter a username to register: ";
        cin >> username;
        for (User *u : users) {
          if (u->getUsername() == username) {
            cout << "Username " << "\"" << username << "\"" << " already exists. Try again." << endl;
            break;
          } 
        }
        users.push_back(new User(username));
        cout << "Successfully registered " << "\"" << username << "\"" << endl;
        exit = true;
        break;
      } default: {
        cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Open project script
void run_open_project() {
  
}

// Open project as leader script
void open_as_leader() {
  
}

// Open project as member
void open_as_member() {
  
}

// Menu script

// Main method
int main() {
  // import_users();
  run_login();

}