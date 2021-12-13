// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "its.hpp"

using namespace std;

// All users currently registered
vector<User*> users;
vector<Project*> projects;
User * current_user; // current user
Project * p; // current project

int project_number; // project number in files
int num_projects;

// Get user pointer from username string
User * getUser(string username) {
  for (User * u : users) {
    if (username == u->getUsername()) {
      return u;
    }
  }
  return NULL;
}

void update_num_projects() {
  ofstream np("data/num_projects.txt");
  np << num_projects;
  np.close();
}

void load_projects() {
  //find the number of projects
  ifstream np("data/num_projects.txt");
  string num;
  getline(np, num);
  num_projects = stoi(num);
  np.close();

  //variables used for project constructor
  string name;
  map<User*, Role> roles;
  vector<Issue*> to_do_list;
  vector<Sprint*> sprint_list;
  vector<Issue*> work_done_list;
  int nid;
  
  //other variables
  string line;
  map<int, Issue*> issues;
  
  //get users1
  for (int i = 0; i < num_projects; i++) {
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

    Project* pr = new Project(name, roles, to_do_list, work_done_list, sprint_list, nid);
    projects.push_back(pr);
  }
}

// Import users from users.txt and projects from projects.txt
void import_data() {
  ifstream u("data/users.txt");
  string user;
  while (getline(u, user)) {
    users.push_back(new User(user));
  }
  u.close();
  load_projects();
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

  ofstream o("data/" + to_string(project_number) + ".txt");
  o << ss.rdbuf();
  o.close();
}


// Login script
void run_login() {
  bool exit = false;
  char choice;
  //User current_user;
  //Prompts the user to login or register, exits after successful login
  while (!exit) {
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
            exit = true;
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
          exit = true;
        }
        break;
      } case 'Q' : {
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
  bool exit = false;
  while (!exit) {
    cout << "1. Open project..." << endl; //show existing projects associated with the user
    cout << "2. Create project..." << endl; //create a new project. prompt the user for information and automatically set them as the project lead
    cin >> choice;
    switch (choice) {
      case '1' : {
        cout << "Projects: ";
        for (auto x : projects) {
          if (x->getUsers().count(current_user) == 1) {
            cout << x->getName() << ", ";
          }
        }
        cout << endl;
        break;
      } case '2' : { 
        string p_name;
        cout << "Enter a name for the project (one word): ";
        cin >> p_name;
        Project * temp = new Project(p_name, current_user);
        projects.push_back(temp);
        p = temp;
        cout << "Project " << "\"" << p_name << "\" successfully created." << endl;
        project_number = num_projects;
        num_projects++;
        save_project();
        update_num_projects();
        exit = true;
        break;
      } case 'Q' : { 
        return;
      }
    }
  }
}

// Open project as leader script
void open_as_leader() {
  cout << "1. View/update collaborators..." << endl;
  cout << "2. View/update backlog..." << endl;
  cout << "3. View/update sprints..." << endl;
  cout << "4. View project info..." << endl;
  cout << "5. Delete project..." << endl;
}

// Open project as member
void open_as_member() {
  cout << "1. View collaborators..." << endl;
  cout << "2. View/update backlog..." << endl;
  cout << "3. View sprints..." << endl;
  cout << "4. View project info..." << endl;
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