#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
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
    if (cmd == "exit") {
        std::cout << "Exiting the shell..." << std::endl;
        exit(0);
    } else if (cmd == "ls") {
        system("ls");
    } else {
        std::cout << "Command not found: " << cmd << std::endl;
    }
}

// Signal handler for Ctrl+C (SIGINT)
void signalHandler(int signum) {
    std::cout << "\nExiting the shell..." << std::endl;
    exit(signum);
}

// Function to get the username, either from a file or by asking the user
std::string getUsername() {
    std::string username;
    std::string dataDir = "./data";
    std::string usernameFile = dataDir + "/username.txt";
    
    // Check if the directory exists
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directory(dataDir);
    }

    // Check if the username file exists
    if (std::filesystem::exists(usernameFile)) {
        // Read the username from the file
        std::ifstream file(usernameFile);
        std::getline(file, username);
        file.close();
    } else {
        // Ask the user for their username if the file doesn't exist
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        
        // Save the username to the file
        std::ofstream file(usernameFile);
        file << username << std::endl;
        file.close();
    }
    return username;
}

int main() {
    // Set background to white and text to black (ANSI escape codes)
    std::cout << "\033[48;5;15m\033[38;5;0m";  // White background, black text
    
    // Register signal handler for Ctrl+C
    signal(SIGINT, signalHandler);

    // Get the username (either from file or ask the user)
    std::string username = getUsername();
    std::string hostname = getHostname();

    std::string input;
    while (true) {
        // Display the prompt as user@host-$
        std::cout << username << "@" << hostname << "-$ ";
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