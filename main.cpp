// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include "its.hpp"

using namespace std;

// All users currently registered
vector<User> users;

int main() {
  bool exit = false;
  int choice;
  //User current_user;
  //Prompts the user to login or register, exits after successful login
  while (!exit) {
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cin >> choice;
    switch (choice) {
      case 1:
      //current_user.login();
      exit = true;
      break;
      case 2:
      //current_user.register();
      break;
    }
  }
}