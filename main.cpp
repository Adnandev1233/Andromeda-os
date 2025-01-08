#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <csignal>
#include <filesystem>

// Function to get the system hostname
std::string getHostname() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    return std::string(hostname);
}

// Function to handle command execution
void executeCommand(const std::string &cmd) {
    // Handle mkdir (create directory)
    if (cmd.substr(0, 6) == "mkdir ") {
        std::string dir = cmd.substr(6);
        if (std::filesystem::create_directory(dir)) {
            std::cout << "Directory created: " << dir << std::endl;
        } else {
            std::cout << "Failed to create directory: " << dir << std::endl;
        }
    }
    // Handle ls (list directory)
    else if (cmd == "ls") {
        system("ls");
    }
    // Handle touch (create an empty file)
    else if (cmd.substr(0, 5) == "touch ") {
        std::string filename = cmd.substr(6);
        std::ofstream file(filename);
        if (file) {
            std::cout << "File created: " << filename << std::endl;
        } else {
            std::cout << "Failed to create file: " << filename << std::endl;
        }
    }
    // Handle exit command
    else if (cmd == "exit") {
        std::cout << "Exiting the shell..." << std::endl;
        exit(0);
    }
    else {
        std::cout << "Command not found: " << cmd << std::endl;
    }
}

// Signal handler for Ctrl+C (SIGINT)
void signalHandler(int signum) {
    std::cout << "\nExiting the shell..." << std::endl;
    exit(signum);
}

// Function to get a custom directory display (e.g., short directory name like "androm")
std::string getCustomDirectory() {
    char currentDir[1024];
    getcwd(currentDir, sizeof(currentDir));

    // Example logic to shorten the directory path (you can customize this logic)
    std::string dir = std::string(currentDir);
    size_t found = dir.find_last_of("/\\");
    if (found != std::string::npos) {
        return dir.substr(found + 1);  // Get the last part of the directory path
    }
    return dir;  // Return the full directory name if no special directory name is found
}

// Function to display ASCII boot splash
void displayBootSplash() {
    std::string splash = R"(
                                         @@@@                                   
                                        @@ @@(                                  
                                      @@&   @@                                  
                                     @@     @@%                                 
                                   @@@@@&/   @@                                 
                                 @@@       /@@@@@                               
                               @@@                                              
    )";
    std::cout << splash << std::endl;
}

int main() {
    // Set background to white and text to black (ANSI escape codes)
    std::cout << "\033[48;5;15m\033[38;5;0m";  // White background, black text

    // Display the ASCII boot splash
    displayBootSplash();

    // Register signal handler for Ctrl+C
    signal(SIGINT, signalHandler);

    std::string hostname = getHostname();
    std::string input;

    while (true) {
        // Get the custom directory (shortened display, like "androm")
        std::string customDir = getCustomDirectory();
        
        // Display the prompt as user@host-directory-$
        std::cout << "user@" << hostname << "-" << customDir << "-$ ";
        std::getline(std::cin, input);

        // Remove leading and trailing spaces
        size_t start = input.find_first_not_of(' ');
        size_t end = input.find_last_not_of(' ');
        input = input.substr(start, end - start + 1);

        if (!input.empty()) {
            executeCommand(input);
        }
    }

    // Reset color (optional, for better appearance when the program exits)
    std::cout << "\033[0m";
    return 0;
}