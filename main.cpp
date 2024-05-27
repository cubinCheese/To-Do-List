#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Class - Abstract interface for our to do list
class TodoListInterface{
    public:
        // each function is a pure virtual function --> thereby making it abstract
        virtual void add_task(const string& description) = 0; // add tasks to list
        virtual void remove_task(int index) = 0;              // remove tasks from list
        virtual void display_tasks() const = 0;               // display tasks from list
        virtual void mark_finish(int index) = 0;              // mark a task as finished
        virtual void save_file(const string& filename) = 0;   // save to file
        virtual void load_file(const string& filename) = 0;   // load from file

        // destructor
        virtual ~TodoListInterface() {}
};

// Class - actual implementation of todo list abstract functions
// terminal based
class TerminalTodoList : public TodoListInterface {
    private:
        vector<string> tasks;    // vector of "tasks" with string descriptions
        vector<bool> completed;  // vector of mirroring tasks marking finished or not

    public:
        void add_task(const string& description) override {
            // access tasks vector and populate with new entry
            tasks.push_back(description);
            completed.push_back(false);
        }
        
        void remove_task(int index) override {

            // ensure index is within bounds
            if (index >= 1 && index <= tasks.size()) {
                auto indexToRemove = tasks.begin() + index - 1;   // define iterator index-position for removal
                auto indexToComplete = completed.begin() + index - 1;
                tasks.erase(indexToRemove);
                completed.erase(indexToComplete);
            }
        }
        
        void display_tasks() const override {
            
            cout << "---------| To Do List |---------" << endl;

            // iterate through tasks and cout to terminal
            for (int i = 0; i < tasks.size(); i++) {
                cout << i + 1 << ". " << tasks[i] << (completed[i] ? "      [X] " : "      [ ]") << endl;
            }

            cout << "________________________________" << endl;
            
        }
        
        void mark_finish(int index) override {
            if (index >= 1 && index <= tasks.size()) {
                completed[index - 1] = true;
            }
            
        }

        void save_file(const string& filename) override {
            ofstream file(filename);
            if (file.is_open()) {
                for (size_t i = 0; i < tasks.size(); ++i) {
                    file << completed[i] << " " << tasks[i] << endl;
                }
                file.close();
                cout << "Tasks saved to " << filename << endl;
            } else {
                cerr << "Unable to open file: " << filename << endl;
            }
        }

        void load_file(const string& filename) override {
            ifstream file(filename);
            if (file.is_open()) {

                // clean existing cache of vectors
                tasks.clear();
                completed.clear();

                string line;
                while (getline(file, line)) {
                    bool status = line[0] - '0';
                    string task = line.substr(2);
                    tasks.push_back(task);
                    completed.push_back(status);
                }
                file.close();
                cout << "Tasks loaded from " << filename << endl;
            } else {
                cerr << "Unable to open file: " << filename << endl;
            }
        }

};

// User interface functions for terminal based-interaction
class TerminalUI {
    public:
        static void displayMenu() {
            cout << "\nTodo List Menu\n";
            cout << "1. Add Task\n";
            cout << "2. Display Tasks\n";
            cout << "3. Mark Task as Completed\n";
            cout << "4. Delete Task\n";
            cout << "5. Save Tasks to File\n";
            cout << "6. Exit\n\n";
        }
        
        // user selects action from display menu
        static int getUserChoice() {
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;
            return choice;
        }

        // retrieve user input of task description
        static string getUserInput(const string &prompt) {
            string input;
            cout << prompt;
            cin.ignore();
            getline(cin, input);
            return input;
        }

        // retrieve task no. to be viewed
        static int getIndexFromUser(const string& prompt) {
            int index;
            cout << prompt;
            cin >> index;
            return index;
        }
};

int main() {
    TerminalTodoList todoList;
    todoList.load_file("tasks.txt");  // open new empty file

    int choice;
    do {
        TerminalUI::displayMenu();
        choice = TerminalUI::getUserChoice();

        switch (choice) {
            case 1: {
                string description = TerminalUI::getUserInput("Enter task description: ");
                todoList.add_task(description);
                break;
            }
            case 2: {
                todoList.display_tasks();
                break;
            }
            case 3: {
                int index = TerminalUI::getIndexFromUser("Enter task index to mark as complete: ");
                todoList.mark_finish(index);
                break;
            }
            case 4: {
                int index = TerminalUI::getIndexFromUser("Enter task index to delete: ");
                todoList.remove_task(index);
                break;
            }
            case 5: {
                todoList.save_file("tasks.txt");
                break;
            }
            case 6: {
                cout << "Exiting..." << endl;
                break;
            }
            default:
                cout << "Invalid Choice. Please try again.";
        }
    } while (choice != 6);

    return 0;
}