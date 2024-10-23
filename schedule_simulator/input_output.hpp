#ifndef INPUT_OUTPUT_HPP__
#define INPUT_OUTPUT_HPP__

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "data_structures.hpp"

// Define a helper function to split strings
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read tasks from a CSV file and populate the vector of tasks
struct input_format {
    std::vector<tasks> task_set;
    int num_processors;
};

input_format read_tasks_from_csv(const std::string& filename) {
    std::vector<tasks> task_set;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return {task_set, -1};  // Exit with a non-zero return value to indicate failure
    }

    std::string line;
    int pid = 1;  // Task ID
    int num_processors = 0;

    while (std::getline(file, line)) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() < 2) {
            std::cerr << "Invalid CSV format!" << std::endl;
            continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(tokens[0][0]))) {
            continue;
        }

        num_processors = tokens.size() - 2;

        // Read release time and deadline
        int release_time = std::stoi(tokens[0]);
        int deadline = std::stoi(tokens[1]);

        // Read processing times for each processor
        std::vector<subtasks> subtasks;
        int total_processing_time = 0;
        for (int i = 0; i < tokens.size() - 2; ++i) {
            int processing_time = std::stoi(tokens[2 + i]);
            total_processing_time += processing_time;

            // Subtask structure (effective release time and deadline are set to -1 for now)
            subtasks.push_back({
                .spid = pid * 10 + (i + 1),
                .processing_time = processing_time,
                .effective_release_time = -1,
                .effective_deadline = -1
            });
        }

        // Create a task structure and add it to the task set
        task_set.push_back({
            .pid = pid,
            .total_processing_time = total_processing_time,
            .release_time = release_time,
            .deadline = deadline,
            .subtask_set = subtasks
        });

        pid++;
    }

    return  {
                .task_set = task_set, 
                .num_processors = num_processors
            };
}

void write_schedule_to_csv(const std::vector<process_schedule>& schedule, const std::string& output_file) {
    std::ofstream file(output_file);
    
    // Write the header
    file << "Processor, Task ID, Start Time, Completion Time, Deadline\n";
    
    // Write the schedule
    for (const auto& s : schedule) {
        file << s.processor_id << ", T" << s.task.spid << ", " 
             << s.start_time << ", " << s.start_time + s.task_duration << ", " << s.task.effective_deadline << "\n";
    }

    file.close();
    std::cout << "Schedule saved to " << output_file << std::endl;
}

#endif