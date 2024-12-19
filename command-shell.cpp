#include <iostream>   // For standard input/output stream
#include <sstream>    // For parsing and tokenizing input
#include <vector>     // For dynamic arrays (command arguments)
#include <string>     // For easier string manipulation
#include <cstring>    // For C-style string functions (system calls)
#include <unistd.h>   // For system calls like fork() and execvp()
#include <sys/wait.h> // For waiting on child processes
#include <fcntl.h>    // For file descriptor management (redirection)
#include <cstdlib>    // For standard utilities like exit codes
#include <limits>     // For handling input edge cases

using namespace std;


class CustomShell {
private:
    // Maximum number of arguments supported
    static const int MAX_ARGS = 64;
    
    // Method to split input into tokens
    vector<string> tokenize(const string& input) {
        vector<string> tokens;
        istringstream iss(input);
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    // Execute single command
    int executeCommand(const vector<string>& tokens) {
        char* args[MAX_ARGS];
        for (size_t i = 0; i < tokens.size(); ++i) {
            args[i] = const_cast<char*>(tokens[i].c_str());
        }
        args[tokens.size()] = NULL;
        
        pid_t pid = fork();
        
        if (pid == -1) {
            cerr << "Fork failed!" << endl;
            return -1;
        } else if (pid == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                cerr << "Command not found: " << args[0] << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            }
        }
        return 0;
    }
    
    // Execute pipeline of commands
    int executePipeline(const vector<vector<string>>& commands) {
        int numCommands = commands.size();
        int pipes[2][2];
        
        for (int i = 0; i < numCommands; ++i) {
            // Create pipe for this command (except for last command)
            if (i < numCommands - 1) {
                if (pipe(pipes[i % 2]) == -1) {
                    cerr << "Pipe creation failed!" << endl;
                    return -1;
                }
            }
            
            pid_t pid = fork();
            
            if (pid == -1) {
                cerr << "Fork failed!" << endl;
                return -1;
            } else if (pid == 0) {
                // Child process
                
                // Input redirection for non-first commands
                if (i > 0) {
                    if (dup2(pipes[(i-1) % 2][0], STDIN_FILENO) == -1) {
                        cerr << "Input redirection failed!" << endl;
                        exit(EXIT_FAILURE);
                    }
                    close(pipes[(i-1) % 2][0]);
                    close(pipes[(i-1) % 2][1]);
                }
                
                // Output redirection for non-last commands
                if (i < numCommands - 1) {
                    if (dup2(pipes[i % 2][1], STDOUT_FILENO) == -1) {
                        cerr << "Output redirection failed!" << endl;
                        exit(EXIT_FAILURE);
                    }
                    close(pipes[i % 2][0]);
                    close(pipes[i % 2][1]);
                }
                
                // Prepare arguments
                char* args[MAX_ARGS];
                for (size_t j = 0; j < commands[i].size(); ++j) {
                    args[j] = const_cast<char*>(commands[i][j].c_str());
                }
                args[commands[i].size()] = NULL;
                
                // Execute command
                if (execvp(args[0], args) == -1) {
                    cerr << "Command not found: " << args[0] << endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                // Parent process
                
                // Close pipe file descriptors
                if (i > 0) {
                    close(pipes[(i-1) % 2][0]);
                    close(pipes[(i-1) % 2][1]);
                }
            }
        }
        
        // Wait for all child processes
        for (int i = 0; i < numCommands; ++i) {
            wait(NULL);
        }
        
        return 0;
    }
    
public:
    void run() {
        string input;
        
        while (true) {
            cout << "CustomShell> ";
            getline(cin, input);
            
            // Trim leading and trailing whitespaces
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);
            
            if (input.empty()) continue;
            
            // Check for termination command
            if (input == "quit") {
                cout << "Exiting shell..." << endl;
                break;
            }
            
            // Check for pipeline
            size_t pipePos = input.find('|');
            
            if (pipePos == string::npos) {
                // Single command execution
                vector<string> tokens = tokenize(input);
                executeCommand(tokens);
            } else {
                // Pipeline execution
                vector<vector<string>> pipelineCommands;
                
                // Split commands by pipe
                istringstream iss(input);
                string command;
                while (getline(iss, command, '|')) {
                    // Trim whitespaces
                    command.erase(0, command.find_first_not_of(" \t"));
                    command.erase(command.find_last_not_of(" \t") + 1);
                    
                    pipelineCommands.push_back(tokenize(command));
                }
                
                executePipeline(pipelineCommands);
            }
        }
    }
};
