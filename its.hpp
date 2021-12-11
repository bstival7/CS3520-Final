#pragma once

// Andrew Li & Bernardo Stival
// 11 December 2021
// C++ Final Project - Issue tracking system

#include <vector>
#include <string>
#include <map>

using namespace std;

// Enumerate types of issues
enum class IssueType {
  US, 
  TASK, 
  BUILD, 
  TEST, 
  DEBUG,
  DOC
};

// Enumerate user roles in project
enum class Role {
  LEAD,
  MEMBER
}

// Represent an issue containing data about the issue
class Issue {
  private:
    IssueType type;
    string date_added;
    int priority; // 1-3
    string desc;
    bool complete;
  public:
    //Constructor & Destructor
    //Getters & Setters
    //Overload >> to print issue
};

// Represent a sprint, containing a list of issues and a time frame
class Sprint {
  private:
    vector<Issue> issues;
    int weeks; // time frame
    int current_week;
  public:
    //Constructor & Destructor
    //Getters & Setters
    //Overload >> to print sprint
    //finished
};

// Represent a project 
class Project {
  private:
    User * current;
    map<User, Role> users;
    vector<Issue> to_do;
    vector<Issue> work_done;
    vector<Sprint> in_progress;
  public:
    //Constructor & Destructor
    //Getters & Setters
    
    //Add Member
    //Add Lead
    //Add Issues
    //Add Sprints
    //Remove Member
    //Remove Lead
    //Remove Issues
    //Remove Sprints
    //Update Issues
    //Update Sprints
    //Promote Member
    //Demote Lead
};

class User {
  private:
    string username;
    vector<Project> projects;
  public:
    //Constructor & Destructor
    //Getters & Setters

    // login
    // register
    // Create Project
    // Overload < for map
    friend bool operator<(User& lhs, User& rhs) {
      return lhs.username < rhs.username;
    }
};
