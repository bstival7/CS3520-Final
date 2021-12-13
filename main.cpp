// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "its.hpp"
#include <cstdlib>

using namespace std;

// All users currently registered
vector<User*> users;
User * current_user; // current user
Project * loaded_project; // loaded project (may not be opened by user)
Project * p; // current project
Issue * ci; //current issue

// Get user pointer from username string
User * getUser(string username) {
  for (User * u : users) {
    if (username == u->getUsername()) {
      return u;
    }
  }
  return NULL;
}

void load_project() {
  //variables used for project constructor
  string name;
  map<User*, Role> roles;
  vector<Issue*> to_do_list;
  vector<Sprint*> sprint_list;
  vector<Issue*> work_done_list;
  vector<Issue*> all_issues;
  int nid;
  
  //other variables
  string line;
  map<int, Issue*> issues;
  
  //get users1
  ifstream input("data/" + to_string(i) + ".txt");
  //get project name
  getline(input, line);
  if (line != "P%") {
    throw 2; // corrupted file
  }
  getline(input, line);
  name = line;

  //get user data
  getline(input, line);
  if (line != "U%") {
    throw 2; // corrupted file
  }
  while (true) {
    getline(input, line);
    if (line == "I%") {
      break;
    }

    string username = line;
    User * user = getUser(username);
    getline(input, line);
    Role role = static_cast<Role>(stoi(line));
    
    roles[user] = role;
  }
  
  //get issues
  while (true) {
    getline(input, line);
    if (line == "S%") {
      break;
    }
    
    int id = stoi(line);
    
    // other issue fields
    IssueType type;
    string da;
    int ttc;
    int pri;
    string desc;
    bool comp; 
    User * rep;
    User * ass;
    
    getline(input, line);
    type = static_cast<IssueType>(stoi(line));
    getline(input, line);
    da = line;
    getline(input, line);
    ttc = stoi(line);
    getline(input, line);
    pri = stoi(line);
    getline(input, line);
    desc = line;
    getline(input, line);
    comp = stoi(line);
    getline(input, line);
    rep = getUser(line);
    getline(input, line);
    ass = getUser(line);
    
    Issue * is = new Issue(id, type, da, ttc, pri, desc, comp, rep, ass);
    all_issues.push_back(is);
    issues[id] = is;
  }

  //get sprints
  while (true) {
    getline(input, line);
    if (line == "TD%") {
      break;
    }
    vector<Issue*> issue_list;
    int weeks;
    int current_week;
    bool finished;

    //list of issues associated with a sprint
    while (true) {
      getline(input, line);
      if (line == "W%") {
        break;
      }
      int id = stoi(line);
      issue_list.push_back(issues[id]);
    }
    getline(input, line);
    weeks = stoi(line);
    getline(input, line);
    current_week = stoi(line);
    getline(input, line);
    finished = stoi(line);
    Sprint* temp = new Sprint(issue_list,weeks,current_week,finished);
    sprint_list.push_back(temp);
  }

  //get to_do
  while (true) {
    getline(input, line);
    if (line == "WD%") {
      break;
    }
    int id = stoi(line);
    to_do_list.push_back(issues[id]);
  }

  while (true) {
    getline(input, line);
    if (line == "NID%") {
      break;
    }
    int id = stoi(line);
    work_done_list.push_back(issues[id]);
  }
  getline(input, line);
  nid = stoi(line);

  Project* pr = new Project(name, roles, to_do_list, work_done_list, sprint_list, all_issues, nid);
  loaded_project = pr;
}

// Import users from users.txt and projects from projects.txt
void import_data() {
  ifstream u("data/users.txt");
  string user;
  while (getline(u, user)) {
    users.push_back(new User(user));
  }
  u.close();
  load_project();
}

// Save list of users registered
void save_users() {
  ofstream o("data/users.txt");
  for (User * u : users) {
    o << u->getUsername() << endl;
  }
  o.close();
}

// Save current project
void save_project() {
  stringstream ss;

  map<User*, Role> roles = p->getUsers();
  vector<Issue*> backlog = p->getToDo();
  vector<Sprint*> sprints = p->getInProgress();
  vector<Issue*> workdone = p->getWorkDone();

  // compile every issue in the project into one map of issue id -> issue object
  map<int, Issue*> issues;

  // get issues from backlog
  for (Issue* i : backlog) {
    if (issues.count(i->getID()) == 0) {
      issues[i->getID()] = i;
    }
  }

  // get issues from sprint
  for (Sprint* s : sprints) {
    for (Issue* i : s->getIssues()) {
      if (issues.count(i->getID()) == 0) {
        issues[i->getID()] = i;
      }
    }
  }

  // get issues from work done
  for (Issue* i : workdone) {
    if (issues.count(i->getID()) == 0) {
      issues[i->getID()] = i;
    }
  }

  ss << "P%" << endl;
  ss << p->getName() << endl;
  ss << "U%" << endl;
  for (auto u : roles) {
    ss << u.first->getUsername() << endl;
    ss << u.second << endl;
  }
  ss << "I%" << endl;
  for (auto p : issues) {
    int id = p.first;
    Issue * i = p.second;
    ss << id << endl;
    ss << i->getType() << endl;
    ss << i->getDateAdded() << endl;
    ss << i->getTimeToComplete() << endl;
    ss << i->getPriority() << endl;
    ss << i->getDescription() << endl;
    ss << i->isComplete() << endl;
    ss << i->getReporter()->getUsername() << endl;
    ss << i->getAssignee()->getUsername() << endl;
  }
  ss << "S%" << endl;
  for (Sprint * s : sprints) {
    for (Issue* i : s->getIssues()) {
      ss << i->getID() << endl;
    }
    ss << "W%" << endl;
    ss << s->getWeeks() << endl;
    ss << s->getCurrentWeek() << endl;
    ss << s->isComplete() << endl;
  }
  ss << "TD%" << endl;
  for (Issue * i : backlog) {
    ss << i->getID() << endl;
  }
  ss << "WD%" << endl;
  for (Issue * i :workdone) {
    ss << i->getID() << endl;
  }
  ss << "NID%" << endl << p->getNID();

  ofstream o("data/proj.txt");
  o << ss.rdbuf();
  o.close();
}

void print_issues(vector<Issue*> iss) {
  for (int i = 0; i < iss.size(); i++) {
    cout << i+1 << ". Issue " << iss[i]->getID();
    if (iss[i]->isComplete()) {
      cout << "[Resolved]" << endl;
    } else {
      cout << "[Unresolved]" << endl;
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
        exit(0);
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
  for (auto p : roles) {
    cout << role_to_str(p.second) << ": ";
    cout << p.first->getUsername();
    cout << endl;
  }
}

// Script to modify an issue
void run_modify_issue(Issue * i) {
  char choice;
  /*
  IssueType type; // the type of issue
  string date_added; // the date the issue was created
  int time_to_complete; // estimated time of completion
  int priority; // (1-3) priority of the issue to be resolved
  string desc; // description of the issue (one line)
  bool complete; // is the issue resolved?
  User * reporter; // who is working on this issue?
  User * assignee; // who assigned this issue?
  */
  cout << "Select aspect to modify: " << endl;
  cout << "1. Type" << endl;
  cout << "2. Date Added" << endl;
  cout << "3. Priority" << endl;
  cout << "4. Description" << endl;
  cout << "5. Completion" << endl;
  cout << "6. Reporter" << endl;
  cout << "7. Days to Complete" << endl;
  cout << "8. Days to Complete" << endl;
}

// Script to add an issue
void run_add_issue() {
  int typenum;
  string da;
  int ttc;
  int priority;
  string desc;
  User* reporter;

  cout << "Issue types: " << endl;
  cout << "1. User Story" << endl;
  cout << "2. Task" << endl; 
  cout << "3. Build" << endl; 
  cout << "4. Test" << endl; 
  cout << "5. Debug" << endl; 
  cout << "6. Documentation" << endl; 
  cout << "Enter issue type number: ";
  cin >> typenum;
  cout << "Enter date added: " << endl;
  cin >> da;
  cout << "Enter estimated number of days to resolve: " << endl;
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
  cout << "Enter description: " << endl;
  getline(cin, desc);
  while (true) {
    string username;
    cout << "Enter user to assign issue to: " << endl;
    cin >> username;
    User u* = getUser(username);
    if (u == NULL) {
      cout << "User does not exist. Try again."
    }
    if (p->getUsers().count())
  }
  
  
  
}

// Open project as leader script
void run_open_as_leader() {
  cout << "1. View/update collaborators..." << endl;
  cout << "2. View/update backlog..." << endl;
  cout << "3. View/update sprints..." << endl;
  cout << "4. View/update project info..." << endl;
  cout << "5. Delete project..." << endl;
}

// print project name and collaborators
void print_proj_info() {
  cout << "Project name: " << p->getName() << endl;
  cout << "Collaborators: " << endl;
  print_collaborators();
}

// Open project as member
void run_open_as_member() {
  bool quit = false;
  int issc,cc;
  Issue* ci; // current_issue
  vector<Issue*> backlog = p->getToDo();
  while (!quit) {
    char choice;
    cout << "1. View collaborators..." << endl;
    cout << "2. View backlog..." << endl;
    cout << "3. View sprints..." << endl;
    cout << "4. View project info..." << endl;
    cout << "5. View my assigned issues..." << endl;
    cin >> choice;
    switch (choice) {
      case '1': {
        print_collaborators();
        break;
      } case '2': {
        print_issues(backlog);
        cout << "Select Issue: ";
        cin >> issc;
        issc--;
        backlog[issc]->print();
        cout << "1. Modify Issue" << endl;
        cout << "2. Return" << endl;
        cin >> cc;
        if (cc == 1)
        run_modify_issue(backlog[issc]);
        else
        break;
      } case '3': {
        break;
      } case '4': {
        break;
      } case '5': {
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
  char choice;
  bool quit = false;
  string p_name;
  while (!quit) {
    cout << "1. Open project..." << endl; //show existing projects associated with the user
    cout << "2. Create project..." << endl; //create a new project. prompt the user for information and automatically set them as the project lead
    cin >> choice;
    switch (choice) {
      case '1' : {
        cout << "Projects: \n";
        map<User*, Role> roles = loaded_project->getUsers();
        if (roles.count(current_user) == 1) {
          cout << 1 << ". " << loaded_project->getName() << " [" << role_to_str(roles[current_user]) << "]" << endl;
        } else {
          cout << "You have no projects. Create one or log into an account with one." << endl;
          break;
        }
        if (roles[current_user] == Role::LEAD) {
          run_open_as_leader();
        } else {
          run_open_as_member();
        }
        break;
      } case '2' : { 
        cout << "Enter a name for the project (one word): ";
        cin >> p_name;
        Project * temp = new Project(p_name, current_user);
        p = temp;
        cout << "Project " << "\"" << p_name << "\" successfully created." << endl;
        save_project();
        quit = true;
        break;
      } case 'Q' : { 
        exit(0);
      } default: {
        cout << "\"" << choice << "\"" << " is an invalid option. Try again." << endl;
        break;
      }
    }
  }
}

// Simulate 60 seconds -> 1 week, increment week of current project
void simulate_time() {
  int timer = 0;
  struct timespec tim = {
    0,
    1000000
  }; // Each execution of while(1) is approximately 1mS
  struct timespec tim_ret;

  while (1) {
    timer++;
    if (timer == 60000) {
      cout << "Week passed." << endl;
      p->step_one_week();
      timer = 0;
    }
  }
}

// Main method
int main() {
  import_data();
  run_login();
  run_open_project();
  
  // Free memory
  for (auto u : users) {
    delete u;
  }
  for (auto p : projects) {
    delete p;
  }
}