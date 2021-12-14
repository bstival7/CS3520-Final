// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

Project * loaded_project; // loaded project (may not be opened by user)

// All users currently registered
vector<User*> users;

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
  ifstream input("data/proj.txt");
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

// Save list of users registered
void save_users() {
  ofstream o("data/users.txt");
  for (User * u : users) {
    o << u->getUsername() << endl;
  }
  o.close();
}

// Save current project
void save_project(Project * p) {
  stringstream ss;

  map<User*, Role> roles = p->getUsers();
  vector<Issue*> backlog = p->getToDo();
  vector<Sprint*> sprints = p->getSprints();
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


