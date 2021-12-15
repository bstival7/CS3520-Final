// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "its.hpp"
#include "proj_io.hpp"
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

User * current_user; // current user
Project * p; // current project
bool stop = false;

// print list of issues
void print_issues(vector<Issue*> iss) {
  int num;
  for (int i = 0; i < iss.size(); i++) {
    num = i+1;
    cout << num << ". Issue " << iss[i]->getID();
    if (iss[i]->isComplete()) {
      cout << " [Resolved]" << endl;
    } else {
      cout << " [Unresolved]" << endl;
    }
  }
}

// Login script
void run_login() {
  bool quit = false;
  char choice;
  //User current_user;
  //Prompts the user to login or register, exits after successful login
  while (!quit) {
    cout << "1. Login..." << endl;
    cout << "2. Register..." << endl;
    cout << "Q. Quit..." << endl;
    cin >> choice;
    switch (choice) {
      case '1': {
        string username;
        cout << "Enter your username to login: ";
        cin >> username;
        for (User *u : users) {
          if (u->getUsername() == username) {
            current_user = u;
            quit = true;
            cout << "Logged in succesfully." << endl;
            return;
          } 
        }
        cout << "Username "  << "\"" << username << "\"" <<  " does not exist. Try again." << endl;
        break;
      } case '2': {
        string username;
        cout << "Enter a username to register: ";
        cin >> username;
        bool exists = false;
        for (User *u : users) {
          if (u->getUsername() == username) {
            cout << "Username " << "\"" << username << "\"" << " already exists. Try again." << endl;
            exists = true;
            break;
          }
        }
        if (!exists) {
          users.push_back(new User(username));
          save_users();
          cout << "Successfully registered " << "\"" << username << "\"" << endl;
          cout << "Now logged in as user " << "\"" << username << "\"" << endl;
          quit = true;
        }
        break;
      } case 'Q' : {
        stop = true;
        return;
      } default: {
        cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Print collaborators of project
void print_collaborators() {
  map<User*, Role> roles = p->getUsers();
  for (auto x : roles) {
    cout << role_to_str(x.second) << ": ";
    cout << x.first->getUsername();
    cout << endl;
  }
}

// Print and provide options to modify collaborators
void view_update_collaborators() {
  map<User*, Role> roles = p->getUsers();
  vector<User*> us;
  string role, username;
  while (true) {
    int i = 1;
    for (auto const& x : roles) {
      role = role_to_str(x.second);
      username = x.first->getUsername();
      cout << i 
      << ". Edit role of [" << role 
      << "]: " << username << endl;
      us.push_back(x.first);
      i++;
    }

    cout << i << ". " << "Add collaborator..." << endl;
    i++;
    cout << i << ". " << "Remove collaborator..." << endl;
    i++;
    cout << i << ". Return" << endl;
    cout << "Enter an option number: ";
    int option;
    cin >> option;
    if (option >= 1 && option <= us.size()) {
      int rolenum;
      cout << "Roles: " << endl;
      cout << "1. LEAD" << endl;
      cout << "2. MEMBER" << endl;
      cout << "Enter role for new collaborator: ";
      cin >> rolenum;
      rolenum--;
      Role role = static_cast<Role>(rolenum);
      int selection = option-1;
      p->updateRole(us[selection], role);
      break;
    } else if (option == i-2) { // add collaborator
      string username;
      cout << "Enter username of user to add: ";
      cin >> username;
      User * u = getUser(username);
      if (u == NULL) {
        cout << "Given username does not exist. Try again." << endl;
        continue;
      }
      int rolenum;
      cout << "Roles: " << endl;
      cout << "1. LEAD" << endl;
      cout << "2. MEMBER" << endl;
      cout << "Enter role for new collaborator: ";
      cin >> rolenum;
      rolenum--;
      Role role = static_cast<Role>(rolenum);
      p->add_collaborator(u, role);
      save_project(p);
      break;
    } else if (option == i-1) { // remove collaborator
      string username;
      cout << "Enter username of user to remove: ";
      cin >> username;
      User * u = getUser(username);
      if (u == NULL) {
        cout << "Given username does not exist. Try again." << endl;
        continue;
      }
      p->remove_collaborator(u);
      break;
    } else if (option == i) {
      save_project(p);
      return;
    } else {
      cout << "Incorrect output." << endl;
    }
  }
}

// Script to modify an issue
void run_modify_issue(Issue * i) {
  char choice;
  string user_input;
  cout << "-------------------------" << endl;
  cout << "Select aspect to modify: " << endl;
  cout << "1. Type" << endl;
  cout << "2. Date Added" << endl;
  cout << "3. Days to Complete" << endl;
  cout << "4. Priority" << endl;
  cout << "5. Description" << endl;
  cout << "6. Completion" << endl;
  cout << "7. Reporter" << endl;
  cout << "8. Assignee" << endl;
  cout << "Q. Quit" << endl;
  cin >> choice;
  switch (choice) {
    case '1' : {
      cout << "--------" << endl;
      cout << "0. US" << endl;
      cout << "1. TASK" << endl;
      cout << "2. BUILD" << endl;
      cout << "3. TEST" << endl;
      cout << "4. DEBUG" << endl;
      cout << "5. DOC" << endl;
      cout << "--------" << endl;
      cout << "Changing to...";
      cin >> user_input;
      int buffer = stoi(user_input);
      if (buffer >= 0 && buffer <= 5) {
        i -> setType(static_cast<IssueType>(buffer));
      } else {
        cout << "Invalid input." << endl;
      }
      break;
    }
    case '2' : {
      cout << "Changing date to...";
      cin >> user_input;
      cout << endl;
      i -> setDate(user_input);
      break;
    }
    case '3' : {
      cout << "Changing days to complete to...";
      cin>>user_input;
      cout << endl;
      i -> setTimeToComplete(stoi(user_input));
      break;
    }
    case '4' : {
      cout << "Priorities: High, Medium, Low" << endl;
      cout << "Changing priority to...";
      cin >> user_input;
      cout << endl;
      if (user_input == "High") {
        i->setPriority(3);
      } else if (user_input == "Medium") {
        i->setPriority(2);
      } else if (user_input == "Low") {
        i->setPriority(1);
      } else {
        cout << "Invalid input." << endl;
      }
      break;
    }
    case '5' : {
      cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); 
      cout << "Change description to...";
      getline(cin, user_input);
      cout << endl;
      i ->setDescription(user_input);
      break;
    }
    case '6' : {
      cout << "1. Resolved/2. Unresolved";
      cout << "Changing completion to...";
      cin >> user_input;
      cout << endl;
      if (user_input == "1") {
        i->setCompletion(true);
      } else if (user_input == "2") {
        i->setCompletion(false);
      } else {
        cout << "Invalid input." << endl;
      }
      break;
    }
    case '7' : {
      cout << "Assign this issue to... ";
      cin >> user_input;
      cout << endl;
      bool user_exists = false;
      User * u;
      for (auto x : users) {
        if (x -> getUsername() == user_input) {
          user_exists = true;
          u = x;
        }
      }
      if (user_exists) {
        i -> setReporter(u);
      } else {
        cout << "Username is invalid." << endl;
      }
      break;
    }
    case '8' : {
      cout << "Changing assigner to... ";
      cin >> user_input;
      cout << endl;
      bool user_exists = false;
      User * u;
      for (auto x : users) {
        if (x -> getUsername() == user_input) {
          user_exists = true;
          u = x;
        }
      }
      if (user_exists) {
        i -> setAssignee(u);
      } else {
        cout << "Username is invalid." << endl;
      }
      break;
    }
    case 'Q' : {
      save_project(p);
      return;
    }
  }
}

// Script to add an issue
// Assigns to current user automatically if the given boolean is true
void run_add_issue(bool self_assign) {
  int typenum;
  IssueType type;
  string da;
  int ttc;
  int priority;
  string desc;
  User* reporter;
  cout << "------------------------------------" << endl;
  while (true) {
    cout << "Issue types: " << endl;
    cout << "1. User Story" << endl;
    cout << "2. Task" << endl; 
    cout << "3. Build" << endl; 
    cout << "4. Test" << endl; 
    cout << "5. Debug" << endl; 
    cout << "6. Documentation" << endl; 
    cout << "Enter issue type number: ";
    cin >> typenum;
    if (typenum < 1 || typenum > 6) {
      cout << "Invalid option, try again." << endl;
    } else {
      type = static_cast<IssueType>(typenum-1);
      break;
    }
  }
 
  cout << "Enter date added: ";
  cin >> da;
  cout << "Enter estimated number of days to resolve: ";
  cin >> ttc;
  while (true) {
    cout << "Enter priority: " << endl;
    cout << "1. Low" << endl;
    cout << "2. Medium" << endl; 
    cout << "3. High" << endl; 
    cin >> priority;
    if (priority < 1 || priority > 3) {
      cout << "Not a valid option. Try again" << endl;
    } else {
      break;
    }
  }
  cin.ignore(numeric_limits<std::streamsize>::max(), '\n'); 
  cout << "Enter description: ";
  getline(cin, desc);
  if (!self_assign) {
    while (true) {
      int usernum;
      cout << "Assign to whom? " << endl;
      vector<User*> valid_users;
      for (auto x : p->getUsers()) {
        valid_users.push_back(x.first);
      }
      for (int i = 1; i <= valid_users.size(); i++) {
        cout << i << ". " + valid_users.at(i-1)->getUsername() << endl;
      }
      cout << "Enter number: ";
      cin >> usernum;
      if (usernum < 1 || usernum > valid_users.size()) {
        cout << "Invalid option. Try again." << endl;
      } else {
        reporter = valid_users.at(usernum-1);
        break;
      }
    }
  } else {
    reporter = current_user;
  }
  
  //IssueType t, string date, int work_days, int prio, string description, User * assignedTo, User * assignedBy
  Issue* newissue = new Issue(type, da, ttc, priority, desc, reporter, current_user);
  p->add_issue(newissue);
}

// View and update backlog
void view_update_backlog() {
  int int_choice,cc,cthree,num;
  vector<Issue*> backlog = p->getToDo();
  cout << "------------------------------------" << endl;
  cout << "1. View/Update Backlog" << endl;
  cout << "2. Add Issue" << endl;
  cout << "3. Add backlog to current sprint" << endl;
  cin >> cthree;
  if (cthree == 1) {
    if (backlog.size() > 0) {
      cout << "------------------------------------" << endl;
      print_issues(backlog);
      cout << "Select issue: ";
      cin >> cc;
      num=cc-1;
      backlog[num]->print();
      cout << "1. Modify" << endl;
      cout << "2. Return" << endl;
      cin >> int_choice;
      if (int_choice == 1) {
        run_modify_issue(backlog[num]);
      } else if (int_choice == 2) {
        save_project(p);
        return;
      } else {
        cout << "Invalid option." << endl;
      }
    } else {
      cout << "No issues currently in backlog." << endl;
    }
  } else if (cthree == 2) {
    run_add_issue(false);
  } else if (cthree == 3) {
    p->add_backlog_to_sprint();
  } else {
    cout << "Invalid option." << endl;
  }
}

// Print the backlog of issues
void view_backlog() {
  int num,cc,cthree;
  vector<Issue*> backlog = p->getToDo();
  while (true) {
    cout << "------------------------------------" << endl;
    print_issues(backlog);
    cout << "Select issue: ";
    cin>>cc;
    num=cc--;
    backlog[num]->print();
    cout << "1. Return to backlog." << endl;
    cout << "2. Return to menu." << endl;
    cin >> cthree;
    if (cthree == 2) {
      return;
    }
  }
}

// View/update my assigned issues script
void view_my_assigned_issues() {
  int int_choice,cc;
  vector<Issue*> all_issues = p->getAssignedIssues(current_user);
  if (all_issues.empty()) {
    cout << "No assigned issues." << endl;
    return;
  } else {
    print_issues(all_issues);
    cout << "Select issue to see more info: ";
    cin >> int_choice;
    int_choice--;
    all_issues[int_choice]->print();
    cout << "1. Modify Issue" << endl;
    cout << "2. Return" << endl;
    cin >> cc;
    if (cc == 2) {
      save_project(p);
      return;
    } else if (cc == 1) {
      run_modify_issue(all_issues[int_choice]);
    } else {
      cout << "Invalid Input." << endl;
    }
  }
}

// print project name
void print_proj_name() {
  cout << "Project name: " << p->getName() << endl;
}

//Prints a sprint in a formatted manner
bool run_print_sprints() {
  vector<Sprint*> sprints = p->getSprints();
  int num = 0;
  if (sprints.size() > 0) {
    for (int i = 0; i < sprints.size(); i++) {
      num = i+1;
      cout << num << ". Sprint " << num << ": ";
      if (sprints[i]->isComplete()) {
        cout << "[FINISHED]" << endl;
      } else {
        cout << "[IN PROGRESS]" << endl;
      }
    }
    return true;
  } else {
    cout << "No sprints are assigned to this project." << endl;
    return false;
  }
}

//Prints sprints
void view_sprints() {
  int int_choice;
  char c;
  vector<Sprint*> sp = p->getSprints();
  while (true) {
    cout << "------------------------------------" << endl;
    if (!run_print_sprints()) {
      return;
    }
    cout << "Select Sprint: ";
    cin>>int_choice;
    int_choice--;
    sp[int_choice]->print();
    cout << "1. Select Another Sprint" << endl;
    cout << "2. Return" << endl;
    cin >> c;
    if (c == '2') {
      save_project(p);
      return;
    }
  }
}

//modifies an existing sprint
void modify_sprints() {
  while (true) {
    char choice,c2;
    int int_choice;
    vector<Sprint*> spv = p->getSprints();
    Sprint* sp; 
    cout << "------------------------------------" << endl;
    if (spv.size() == 0) {
      cout << "No sprints assigned to this project." << endl;
      return;
    } else {
      run_print_sprints();
    }
    cout << "Select Sprint: ";
    cin>>int_choice;
    int_choice--;
    sp = spv[int_choice];
    sp->print();
    cout << "1. Modify sprint" << endl;
    cout << "2. Return" << endl;
    cin >> choice;
    if (choice == '1') {
      cout << "-----------------------" << endl;
      cout << "1. Modify Issues" << endl;
      cout << "2. Modify Length" << endl;
      cout << "3. Modify Current Week" << endl;
      cout << "4. Modify Status" << endl;
      cout << "5. Return" << endl;
      cin >> c2;
      switch (c2) {
        case '1': {
          vector<Issue*> iss = sp->getIssues();
          if (!iss.empty()) {
            print_issues(iss);
            cout << "Select an issue: ";
            cin >> int_choice;
            int_choice--;
            run_modify_issue(iss[int_choice]);
            break;
          } else {
            cout << "No issues to modify!" << endl;
            return;
          }
        } case '2': {
          cout << "Changing sprint length to... ";
          cin >> int_choice;
          cout << endl;
          sp->setWeeks(int_choice);
          break;
        } case '3': {
          cout << "Changing current week to... ";
          cin >> int_choice;
          cout << endl;
          sp->setCurrentWeek(int_choice);
          break;
        } case '4': {
          cout << "Changing status to...[1. COMPLETE/2. IN PROGRESS] ";
          cin >> int_choice;
          if (int_choice == 1) {
            sp->setCompletion(true);
          } else if (int_choice == 2) {
            sp->setCompletion(false);
          } else {
            cout << "Invalid input." << endl;
          }
          break;
        } case '5': {
          return;
        } default: {
          cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
          break;
        }
      } 
    } else if (choice =='2') {
      save_project(p);
      return;
    } else {
      cout << "Invalid option." << endl;
    }
  }
}
//creates a new sprint and adds it to the project
void add_sprint() {
  int choice;
  string ch;
  Sprint* spr = new Sprint();
  cout << "------------------------------------------" << endl;
  cout << "How long will this sprint last in weeks? ";
  cin>>choice;
  spr->setWeeks(choice);
  p->add_sprint(spr);
  cout << "New sprint created. Assign issues to it through the backlog." << endl;
}
// view/update sprints
void view_update_sprints() {
  int choice;
  while (true) {
    cout << "1. Modify Sprints" << endl;
    cout << "2. Add New Sprint" << endl;
    cout << "3. Return" << endl;
    cin >> choice;
    switch (choice) {
      case 1 : {
        modify_sprints();
        break;
      } case 2 : {
        add_sprint();
        break;
      } case 3 : {
        save_project(p);
        return;
      } default: {
        cout << "Invalid selection." << endl;
        break;
      }
    }
  }
  
}


// Simulate 60 seconds -> 1 week, increment week of current project
void sprint_mode() {
  int timer = 0;
  struct timespec tim = {
    0,
    1000000
  }; // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;

  while (1) {
    if (!p->current_sprint_exists()) {
      cout << "There are currently no sprints assigned to this project." << endl;
      return;
    }
    timer++;
    if (timer % 5000 == 0) {
      bool quit = false;
      while (!quit) {
        vector<Issue*> sprint_issues;
        if (p->getRole(current_user) == Role::LEAD) {
          sprint_issues = p->getAllUnfinishedIssuesInCurrentSprint();
        } else {
          sprint_issues = p->getUnfinishedAssignedIssuesInCurrentSprint(current_user);
        }
        int num = sprint_issues.size()+1;
        print_issues(sprint_issues);
        int option;
        cout << num << ". " << "No issues completed - continue sprint" << endl;
        cout << "SPRINT PAUSED!"; 
        while (true) {
          cout << " Mark which issue complete? Enter a number: " << endl;
          cin >> option;
          if (option < 1 || option > sprint_issues.size()+1) {
            cout << "Invalid option. Please enter a valid option: " << endl;
            break;
          } else if (option == num) {
            quit = true;
            break;
          } else {
            sprint_issues[option-1]->setCompletion(true);
            cout << "Issue ID " << sprint_issues[option-1]->getID() << " marked as complete." << endl;
            int opt;
            cout << "Any other issues completed? (1-yes - select again, 2-no - continue with sprint): " << endl;
            cin >> opt;
            if (opt == 1) {
              continue;
            } else {
              quit = true;
              break;
            }
          }
        }
      }
      cout << "Continuing sprint..." << endl;
    }
    if (timer == 10000) {
      cout << "Week passed." << endl;
      p->step_one_week();
      return;
    }
    nanosleep(&tim, &tim_ret);
  }
}

// Open project as leader script
void run_open_as_leader() {
  bool quit = false;
  while (!quit) {
    cout << "------------------------------------" << endl;
    cout << "1. View/update collaborators..." << endl;
    cout << "2. View/update backlog..." << endl;
    cout << "3. View/update sprints..." << endl;
    cout << "4. View/update project name..." << endl;
    cout << "5. View/update my assigned issues..." << endl;
    cout << "6. Begin Sprints!" << endl;
    cout << "7. Delete project..." << endl;
    cout << "Q. Quit" << endl;
    cout << "Enter an option: " << endl;
    char option;
    cin >> option;
    switch (option) {
      case '1': {
        view_update_collaborators();
        break;
      } case '2': {
        view_update_backlog();
        break;
      } case '3': {
        view_update_sprints();
        break;
      } case '4': {
        print_proj_name();
        cout << "Update project name? (y/n): ";
        char option2;
        cin >> option2;
        if (option2 == 'y' || option2 == 'Y') {
          string newname;
          cout << "Enter new name: ";
          cin >> newname;
          p->setName(newname);
        }
        break;
      } case '5': {
        view_my_assigned_issues();
        break;
      } case '6': {
        sprint_mode();
        break;
      } case '7': {
        string input;
        cout << "Are you sure you want to delete the project? This cannot be undone: type \"CONTINUE\" (case sensitive) to continue, \"Q\" to go back" << endl;
        cin >> input;
        if (input == "CONTINUE") {
          remove("data/proj.txt");
          p = NULL;
          loaded_project = NULL;
          cout << "Project deleted. Going back to login menu." << endl;
          return;
        } else if (input == "Q") {
          break;
        } else {
          cout << "Input does not match \"CONTINUE\"." << endl;
        }
        break;
      } case 'Q': {
        stop = true;
        return;
      } default: {
        cout << "\"" << option << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Open project as member
void run_open_as_member() {
  bool quit = false;
  int int_choice,cc,cthree;
  while (!quit) {
    char choice;
    cout << "-----------------------------" << endl;
    cout << "1. View collaborators..." << endl;
    cout << "2. View sprints..." << endl;
    cout << "3. View project name..." << endl;
    cout << "4. View/update my assigned issues..." << endl;
    cout << "5. Add issue..." << endl;
    cout << "6. Begin Sprints!" << endl;
    cout << "Q. Quit" << endl;
    cin >> choice;
    switch (choice) {
      case '1': {
        print_collaborators();
        break;
      } case '2': {
        view_sprints();
        break;
      } case '3': {
        print_proj_name();
        break;
      } case '4': {
        view_my_assigned_issues();
        break;
      } case '5': {
        run_add_issue(true);
        break;
      } case '6': {
        sprint_mode();
        break;
      } case 'Q': {
        stop = true;
        return;
      } default: {
        cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Open project script
void run_open_project() {
  char choice;
  bool quit = false;
  string p_name;
  while (!quit) {
    cout << "1. Open project..." << endl; //show existing projects associated with the user
    cout << "2. Create project..." << endl; //create a new project. prompt the user for information and automatically set them as the project lead
    cout << "Q. Quit..." << endl;
    cin >> choice;
    switch (choice) {
      case '1' : {
        if (loaded_project == NULL) {
          cout << "No project exists. Create a new one." << endl;
          break;
        }
        map<User*, Role> roles = loaded_project->getUsers();
        if (roles.count(current_user) == 1) {
          cout << "Opening project...\n";
          p = loaded_project;
          p->setCurrentUser(current_user);
        } else {
          cout << "You have no projects. Create one or log into an account with one." << endl;
          break;
        }
        if (roles[current_user] == Role::LEAD) {
          run_open_as_leader();
          return;
        } else {
          run_open_as_member();
          quit = true;
          return;
        }
        break;
      } case '2' : { 
        cout << "Enter a name for the project (one word): ";
        cin >> p_name;
        Project * temp = new Project(p_name, current_user);
        loaded_project = temp;
        p = temp;
        cout << "Project " << "\"" << p_name << "\" successfully created." << endl;
        save_project(p);
        quit = true;
        break;
      } case 'Q' : { 
        stop = true;
        return;
      } default: {
        cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Main method
int main() {
  import_data();
  
  while (!stop) {
    run_login();
    if (stop) {
      break;
    }
    run_open_project();
  }
  
  // Free memory
  for (auto u : users) {
    delete u;
  }
  delete p;
  return 0;
}