#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string name;
    vector<string> prerequisites;
    bool error;
    Course() {
        error = false;
    }
};

// forward declarations
void displayCourse(Course course);
void displayPrerequisites(Course course);

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Course Display class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class CourseDisplay {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void deleteNode(Node* node);
    void inOrder(Node* node);
public:
    CourseDisplay();
    virtual ~CourseDisplay();
    void Insert(Course course);
    Course Search(string courseNumber);
    void PrintCourses();
    bool isEmpty();
};

/**
 * Default constructor
 */
CourseDisplay::CourseDisplay() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
CourseDisplay::~CourseDisplay() {
    // recurse from root deleting every node
    deleteNode(root);
}

void CourseDisplay::deleteNode(Node* node){
    if(node!=nullptr){
        // delete node in the left tree
        deleteNode(node->left);
        // delete node in the right tree
        deleteNode(node->right);
        // delete current node
        delete node;
    }
}

/**
 * Insert a Course
 */
void CourseDisplay::Insert(Course course) {
    // if root equarl to null ptr
    if(root == nullptr){
      // root is equal to new node course
        root = new Node(course);
    }else{
      // add Node root and course
        addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course CourseDisplay::Search(string courseNumber) {
    // set current node equal to root
    Node* curr = root;
    // keep looping downwards until bottom reached or matching courseNumber found
    while(curr != nullptr){
        // if match found, return current course
        if(curr->course.courseNumber == courseNumber){
            return curr->course;
        }
        // if course is smaller than current node then traverse left
        if(courseNumber < curr->course.courseNumber){
            curr = curr->left;
        }
        // else larger so traverse right
        else{
            curr = curr->right;
        }
    }

    Course course;
    return course;
}

// Print Course List
void CourseDisplay::PrintCourses(){
    inOrder(root);
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void CourseDisplay::addNode(Node* node, Course course) {
    if (node->course.courseNumber == course.courseNumber) {
        return;
    }
    // if node is larger then add to left
    if(node->course.courseNumber > course.courseNumber){
        // if no left node
        if(node->left == nullptr){
            // this node becomes left
            node->left = new Node(course);
        }else{
        // else recurse down the left node
            addNode(node->left, course);
        }
    }else{
        // if no right node
        if(node->right == nullptr){
            // this node becomes right
            node->right = new Node(course);
        }else{
            // recurse down the right node
            addNode(node->right, course);
        }
    }
}

void CourseDisplay::inOrder(Node* node){
    //if node is not equal to null ptr
    if(node != nullptr){
        //inOrder left
        inOrder(node->left);
        //output course number and course name
        displayCourse(node->course);
        //inOrder right
        inOrder(node->right);
    }
}
bool CourseDisplay::isEmpty(){
    return root == nullptr;
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ", " << course.name << endl;
}
// Display the course prerequisites to the console (std::out)
void displayPrerequisites(Course course){
    cout << "Prerequisites: ";
    if(course.prerequisites.empty()){
        cout << "None" << endl;
        return;
    }
    for(int i=0; i<course.prerequisites.size(); ++i){
        if(i > 0){
            cout << ", ";
        }
        cout << course.prerequisites[i];
    }
    cout<<endl;
}

/**
 * Load a CSV file containing course into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the course read
 */
void loadCourses(string csvPath, CourseDisplay* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    ifstream file(csvPath);

    if (!file.is_open()) {
        cout << "Error: Cannot open file!" << endl;
        return;
    }

    string line;
    while(getline(file, line)){
        stringstream courseInfo(line);
        Course course;

        // First field: course number
        getline(courseInfo, course.courseNumber, ',');
        // Second field: course name
        getline(courseInfo, course.name, ',');

        // Check for missing course number or name
        if (course.courseNumber.empty() || course.name.empty()) {
            continue;
        }
        // Remaining fields: prerequisites
        string prerequisites;
        while (getline(courseInfo, prerequisites, ',')) {
            if (!prerequisites.empty()) {
                course.prerequisites.push_back(prerequisites);
            }
        }
        // Insert course in the node
        bst->Insert(course);
    }
    cout << "Load Success!" << endl;
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseNumber;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        break;
    default:
        csvPath = "ABCU_Input.csv";
    }

    // Define a timer variable
    clock_t ticks;

    CourseDisplay* bst;
    bst = new CourseDisplay();
    Course course;

    int choice = 0;
    
    while (choice != 9) {
        cout << "Welcome to the course planner" << endl;
        cout << "  1. Load Data Structure. " << endl;
        cout << "  2. Print Course List. " << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            loadCourses(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            if(bst->isEmpty()){
                cout << "Load Data Structure First." << endl;
                break;
            }
            cout << "Here is a sample schedule: " << endl;
            bst->PrintCourses();
            break;

        case 3:
            if(bst->isEmpty()){
                cout << "Load Data Structure First." << endl;
                break;
            }
            ticks = clock();
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            course = bst->Search(courseNumber);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!course.courseNumber.empty()) {
                displayCourse(course);
                displayPrerequisites(course);
            } else {
            	cout << "Course Number " << courseNumber << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 9:
            break;

        default:
            cout << choice << " is not a valid option."<<endl;
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;
    delete bst;

	return 0;
}
