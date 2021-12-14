// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <bits/stdc++.h>
#include <map>

using namespace std;

// Enumerate types of issues
enum class IssueType {
  US = 0, 
  TASK = 1, 
  BUILD = 2, 
  TEST = 3, 
  DEBUG = 4,
  DOC = 5
};

string type_to_str(IssueType t) {
  if (t == IssueType::US) {
    return "User Story";
  } else if (t == IssueType::TASK) {
    return "Task";
  } else if (t == IssueType::BUILD) {
    return "Build";
  } else if (t == IssueType::TEST) {
    return "Test";
  } else if (t == IssueType::DEBUG) {
    return "Debug";
  } else {
    return "Documentation";
  }
}

// print IssueType number
ostream& operator<< (ostream& os, const IssueType& it)
{
   os << static_cast<underlying_type<IssueType>::type>(it);
   return os;
}

// Enumerate user roles in project
enum class Role {
  LEAD = 0,
  MEMBER = 1
};

string role_to_str(Role r) {
  if (r == Role::LEAD) {
    return "LEAD";
  } else {
    return "MEMBER";
  }
}

// print Role number
ostream& operator<< (ostream& os, const Role& r)
{
   os << static_cast<underlying_type<Role>::type>(r);
   return os;
}

// Represent a user
class User {
  private:
    string username; //unique username
    // vector<Project> projects; //list of projects assigned to the user
  public:
    // Constructors
    User() {}
    User(string u) {
      username = u;
    }
    // Destructor
    ~User() {}
    
    //Getters & Setters
    const string getUsername() {
      return username;
    }

    // Create Project
    // Overload < for map
    friend bool operator<(User& lhs, User& rhs) {
      return lhs.username < rhs.username;
    }

    friend bool operator==(User& lhs, User& rhs) {
      return lhs.username == rhs.username;
    }
};

// Represent an issue containing data about the issue
class Issue {
  private:
    int id; // id number
    IssueType type; // the type of issue
    string date_added; // the date the issue was created
    int time_to_complete; // estimated time of completion
    int priority; // (1-3) priority of the issue to be resolved
    string desc; // description of the issue (one line)
    bool complete; // is the issue resolved?
    User * reporter; // who is working on this issue?
    User * assignee; // who assigned this issue?
  public:
    // Constructor & Destructor
    Issue() {}

    // Constructor without ID
    Issue(IssueType t, string date, int work_days, int prio, string description, User * assignedTo, User * assignedBy) {
      time_to_complete = work_days;
      type = t;
      date_added = date;
      priority = prio;
      desc = description;
      reporter = assignedTo;
      assignee = assignedBy;
      complete = false;
    }

    // Constructor with all fields
    Issue(int i, IssueType t, string date, int work_days, int prio, string description, bool comp, User * assignedTo, User * assignedBy) {
      id = i;
      time_to_complete = work_days;
      type = t;
      date_added = date;
      priority = prio;
      desc = description;
      reporter = assignedTo;
      assignee = assignedBy;
      complete = comp;
    }
    
    ~Issue() {}
      
    //Getters & Setters
    const IssueType getType() {
      return type;
    }
    const string getDateAdded() {
      return date_added;
    }
    const int getPriority() {
      return priority;
    }
    const string getDescription() {
      return desc;
    }
    const int getID() {
      return id;
    }
    User* getReporter() {
      return reporter;
    }
    User* getAssignee() {
      return assignee;
    }
    const bool isComplete() {
      return complete;
    }
    const int getTimeToComplete() {
      return time_to_complete;
    }
    void setDescription(string d) {
      desc = d;
    }
    void setType(IssueType t) {
      type = t;
    }
    void setDate(string date) {
      date_added = date;
    }
    void setPriority(int i) {
      priority = i;
    }
    void setID(int i) {
      id = i;
    }
    void setCompletion(bool b) {
      complete = b;
    }
    void setReporter(User* u) {
      reporter = u;
    }
    void setAssignee(User* u) {
      assignee = u;
    }
    void setTimeToComplete(int i) {
      time_to_complete = i;
    }
    
    void print() {
      //convert priority to text
      string prio;
      if (priority == 1)
      prio = "Low";
      else if (priority == 2)
      prio = "Medium";
      else
      prio = "High";
      //convert completion to text
      string comp;
      if (complete)
      comp = "Yes";
      else
      comp = "No";
      
      //print info
      cout << "ISSUE " << id << endl;
      cout << "type: " << type_to_str(type) << endl;
      cout << "Date Added: " << date_added << endl;
      cout << "Est. Days to Fix: " << time_to_complete << endl;
      cout << "Priority: " << prio << endl;
      cout << "Description: " << desc << endl;
      cout << "Complete? " << comp << endl;
      cout << "Assigned By " << assignee->getUsername();
      cout << " To " << reporter->getUsername();
      cout << endl;
    }
    
    // compare by priority
    friend bool operator>(const Issue& lhs, const Issue& rhs) {
      return lhs.priority > rhs.priority;
    }

    // compare for map
    friend bool operator<(const Issue& lhs, const Issue& rhs) {
      return lhs.id < rhs.id;
    }
};

// Represent a sprint, containing a list of issues and a time frame
class Sprint {
  private:
    vector<Issue*> issues; // all issues assigned to the sprint
    int weeks; // time frame, number of weeks
    int current_week; // current week in the sprint
    bool finished; // is the sprint complete?
  public:
    //Constructor & Destructor
    Sprint() {}
    
    // Constructor that creates new sprint for use in project
    Sprint(vector<Issue*> i, int w) {
      issues = i;
      weeks = w;
      current_week = 0;
      finished = false;
    }

    // Constructor that creates sprint with all fields specified
    Sprint(vector<Issue*> i, int w, int cw, bool f) {
      issues = i;
      weeks = w;
      current_week = cw;
      finished = f;
    }
    
    ~Sprint() {
      for (Issue* i : issues) {
        delete i;
      }
    }

    //Getters & Setters
    const vector<Issue*>& getIssues() {
      return issues;
    }
    const int getWeeks() {
      return weeks;
    }
    const int getCurrentWeek() {
      return current_week;
    }
    const bool isComplete() {
      return finished;
    }
    void setIssues(vector<Issue*> i) {
      issues = i;
      sort(issues.begin(),issues.end(), greater<>());
    }
    void setWeeks(int w) {
      weeks = w;
    }

    void setCurrentWeek(int i) {
      current_week = i;
      if (current_week >= weeks) {
        finished = true;
      } else {
        finished = false;
      }
    }

    // Increments the current week the sprint is on; updates finished field if the current week exceeds the number of weeks allotted
    void increment_week() {
      current_week++;
      if (current_week == weeks) {
        finished = true;
      }
    }
    void setCompletion(bool b) {
      finished = b;
    }
    void addIssue(Issue* i) {
      for (Issue * is : issues) {
        if (i->getID() == is->getID()) {
          return;
        }
      }
      issues.push_back(i);
      sort(issues.begin(), issues.end(), greater<>());
    }
    void removeIssue(Issue * i) {
      auto pos = find(issues.begin(), issues.end(), i);
      if (pos != issues.end()) {
        issues.erase(pos);
      }
    }
    void print() {
      cout << "Sprint" << endl;
      cout << "Weeks: " << weeks << endl;
      cout << "Current Week: " << current_week << endl;
      cout << "Finished? " << finished << endl;
      cout << "Issues: ";
      for (auto x : issues) {
        cout << "[Issue " << x->getID() << ", ";
        if (x->isComplete()) {
          cout << "Resolved], ";
        } else {
          cout << "Unresolved], ";
        }
      }
      cout << endl;
    }
};

// Represent a project 
class Project {
  private:
    string name; // name of project
    User * current_user; // current user working on the project
    Sprint * current_sprint; // current sprint
    map<User*, Role> roles; // users and user roles
    vector<Issue*> to_do; // to-do list of issues, sorted by highest priority first
    vector<Issue*> work_done; // list of issues that are finished
    vector<Issue*> all_issues; // list of all issues in the project
    vector<Sprint*> sprints; // list of sprints in order from earliest creation time
    int next_issue_id; // next issue id
    int num_leads = 0; // number of leaders
  public:
    // Constructor & Destructor
    Project() {}
    Project(string project_name, User* u) {
      name = project_name;
      current_user = u;
      next_issue_id = 0;
      roles[u] = Role::LEAD;
      num_leads++;
    }
    
    Project(string project_name, map<User*, Role> users, vector<Issue*> backlog, vector<Issue*> completed, vector<Sprint*> s, vector<Issue*> allissues, int nid) {
      name = project_name;
      roles = users;
      to_do = backlog;
      work_done = completed;
      sprints = s;
      next_issue_id = nid;
      all_issues = allissues;
      sort(to_do.begin(), to_do.end(), greater<>());
      //Counts the amount of leads in the project
      for (auto const& x : roles) {
        if (x.second == Role::LEAD) {
          num_leads++;
        }  
      }
      for (Sprint * sp : sprints) {
        if (!sp->isComplete()) {
          current_sprint = sp;
          break;
        }
      }
    }
    ~Project() {
      for (Issue* i : to_do) {
        delete i;
      }
      for (Issue* i : work_done) {
        delete i;
      }
      for (Sprint* s : sprints) {
        delete s;
      }
    }
    
    // Getters & Setters
    const string getName() {
      return name;
    }
    const User* getCurrentUser() {
      return current_user;
    }
    const vector<Issue*> getToDo() {
      return to_do;
    }
    const vector<Issue*> getWorkDone() {
      return work_done;
    }
    const vector<Issue*> getAllIssues() {
      return all_issues;
    }
    const map<User*, Role> getUsers() {
      return roles;
    }
    const vector<Sprint*> getSprints() {
      return sprints;
    }
    const int getNID() {
      return next_issue_id;
    }
    void setName(string pname) {
      name = pname;
    }
    void setCurrentUser(User* u) {
      current_user = u;
    }
    void setUsers(map<User*, Role> u) {
      roles = u;
    }

    // (L) Remove collaborator, return whether remove was successful
    bool remove_collaborator(User* u) {
      if (current_user == u) {
        cout << "Cennot remove yourself." << endl;
        return false;
      }
      //Only allows leads to remove users
      if (roles[current_user] != Role::LEAD) {
        cout << "Insufficient Permissions." << endl;
        return false;
      } else if (roles.count(u) == 0) {
        cout << "User does not exist in this project" << endl;
        return false;
      } else if (num_leads == 1 && roles[u] == Role::LEAD) {
        cout << "Cannot remove only lead." << endl;
        return false;
      }
      // if this is a leader
      if (roles[u] == Role::LEAD) {
        num_leads--;
      }
      roles.erase(u);
      cout << "Removed collaborator " << u->getUsername() << "." << endl;
      return true;
    }
    
    // (L) Add collaborator, return whether add was successful
    bool add_collaborator(User * u, Role role) {
      if (roles[current_user] != Role::LEAD) { // insufficient permissions
        cout << "Insufficient Permissions" << endl;
        return false;
      }
      if (roles.count(u) == 1) { // collaborator already exists
        cout << "Collaborator with that username already exists." << endl;
        return false;
      }
      roles[u] = role; // add collaborator
      if (role == Role::LEAD) {
        num_leads++;
      }
      cout << "Added collaborator " << u->getUsername() << "." << endl;
      return true;
    }

    // Add issue to backlog, return whether issue add was successful
    bool add_issue(Issue * i) {
      i->setID(next_issue_id);
      to_do.push_back(i);
      sort(to_do.begin(), to_do.end(), greater<>());
      all_issues.push_back(i);
      next_issue_id++;
      return true;
    }

    // Remove issue from backlog
    bool remove_issue(Issue * i) {
      bool erased = false; // have we erased anything?

      // erase from backlog
      for (int c = 0; c < to_do.size(); c++) {
        if (to_do.at(c)->getID() == i->getID()) {
          to_do.erase(to_do.begin()+c);
          erased = true;
          break;
        }
      }

      // erase from sprints
      for (Sprint * s : sprints) {
        vector<Issue*> si = s->getIssues();
        for (int c = 0; c < si.size(); c++) {
          if (i->getID() == si[c]->getID()) {
            si.erase(si.begin()+c);
            erased = true;
            break;
          }
        }
      }

      // erase from work done 
      for (int c = 0; c < work_done.size(); c++) {
        if (work_done.at(c)->getID() == i->getID()) {
          work_done.erase(work_done.begin()+c);
          erased = true;
          break;
        }
      }
      
      // erase from all issues
      for (int c = 0; c < all_issues.size(); c++) {
        if (all_issues.at(c)->getID() == i->getID()) {
          all_issues.erase(all_issues.begin()+c);
          break;
        }
      }

      return erased;
    }

    // Does a current sprint exist?
    bool current_sprint_exists() {
      return current_sprint != NULL;
    }

    // Add all backlog issues to current sprint
    bool add_backlog_to_sprint() {
      if(current_sprint == NULL) {
        cout << "No current sprint." << endl;
        return false;
      } else {
        for (Issue* i : to_do) {
          i->setDescription(i->getDescription() + "// Old id: " + to_string(i->getID()));
          i->setID(next_issue_id);
          next_issue_id++;
          current_sprint->addIssue(i);
        }
        to_do.clear();
        cout << "Backlog added to current sprint." << endl;
        return true;
      }
    }

    // return the list of issues assigned to this user
    vector<Issue*> getAssignedIssues(User * u) {
      vector<Issue*> v;
      if (roles[u] == Role::LEAD) {
        return all_issues;
      } else {
        for (Issue* x : all_issues) {
          if (x->getReporter() == u) {
            v.push_back(x);
          }
        }
      return v; 
      }
    }

    // return the list of unfinished issues  in the current sprint assigned to this user
    vector<Issue*> getUnfinishedAssignedIssuesInCurrentSprint(User * u) {
      vector<Issue*> v;
      for (Issue* x : current_sprint->getIssues()) {
        if (x->getReporter() == u && !x->isComplete()) {
          v.push_back(x);
        }
      } 
      return v;
    }

    // return the list of unfinished issues in the current sprint
    vector<Issue*> getAllUnfinishedIssuesInCurrentSprint() {
      vector<Issue*> v;
      for (Issue* x : current_sprint->getIssues()) {
        if (!x->isComplete()) {
          v.push_back(x);
        }
      } 
      return v;
    }

    // (L) Add sprint, update current sprint if necessary
    bool add_sprint(Sprint * s) {
      if (current_sprint == NULL && !s->isComplete()) {
        current_sprint = s;
      }
      sprints.push_back(s);
      return true;
    }

    // Increment the week, handle sprint issue transfer if a sprint finishes
    void step_one_week() {
      if (current_sprint == NULL) {
        return;
      }
      current_sprint->increment_week();
      // if current sprint is finished
      if (current_sprint->isComplete()) {
        vector<Issue*> current_issues = current_sprint->getIssues();
        vector<Issue*> unfinished;
        vector<Issue*> finished;
        // add unfinished issues from current finished sprint and add to backlog
        for (Issue* i : current_issues) {
          if (!i->isComplete()) {
            unfinished.push_back(i);
            i->setTimeToComplete(i->getTimeToComplete()-7);
          } else {
            finished.push_back(i);
            i->setTimeToComplete(i->getTimeToComplete()-7);
          }
        }
        // add unfinished issues to backlog
        to_do.insert(to_do.end(), unfinished.begin(), unfinished.end());
        work_done.insert(work_done.end(), finished.begin(), finished.end());
        // update current sprint
        for (Sprint * s : sprints) {
          if (!s->isComplete()) {
            current_sprint = s;
            return;
          }
        }
        // if there is no other current sprint
        current_sprint = NULL;
        sort(to_do.begin(), to_do.end(), greater<>());
      }
    }

    // Get the role of a user
    Role getRole(User * u) {
      if (roles.count(u) == 0) {
        throw 1; // user does not exist, should never get to this point
      } else {
        return roles[u];
      }
    }

    // (L) Update Role
    bool updateRole(User* u, Role role) {
      if (roles[current_user] != Role::LEAD) {
        cout << "Insufficient Permissions." << endl;
        return false;
      } else if (roles.count(u) == 0) {
        cout << "Given user does not exist in this project" << endl;
        return false;
      } else if (role == Role::MEMBER && roles[u] == Role::LEAD && num_leads == 1) {
        // if we are trying to demote the only leader
        cout << "Cannot demote the only project leader." << endl;
        return false;
      } else {
        if (role == Role::MEMBER && roles[u] == Role::LEAD) {
          num_leads--;
        } else if (role == Role::LEAD && roles[u] == Role::MEMBER) {
          num_leads++;
        }
        roles[u] = role;
        cout << "Role update successful." << endl;
        return true;
      } 
    }
};