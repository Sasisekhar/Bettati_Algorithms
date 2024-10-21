#ifndef ALGORITHM_H_HPP__
#define ALGORITHM_H_HPP__

#include "algorithmA.hpp"
#include "algorithmC.hpp"

std::pair<bool, std::vector<schedule>> algorithm_H(std::vector<tasks> _task_set, int num_processors) {
    std::cout << "\nPerforming Algorithm H per the PhD thesis of Dr. Bettati" << std::endl;
    std::vector<schedule> final_schedule;
    auto task_set = _task_set;

    // Step 2: Determine Tmax,j for each processor
    std::vector<int> Tmax_j(num_processors, 0);  // Tmax for each processor
    for (int processor = 0; processor < num_processors; ++processor) {
        for (const auto& task : task_set) {
            const auto& subtask = task.subtask_set[processor];
            Tmax_j[processor] = std::max(Tmax_j[processor], subtask.processing_time);  // Find the max processing time for each processor
        }
    }

    // Step 3: Inflate all subtasks on each processor
    for (int processor = 0; processor < num_processors; ++processor) {
        for (auto& task : task_set) {
            auto& subtask = task.subtask_set[processor];
            if (subtask.processing_time < Tmax_j[processor]) {
                // Inflate by adding idle time
                int idle_time = Tmax_j[processor] - subtask.processing_time;

                subtask.processing_time = Tmax_j[processor];  // Make processing time equal to Tmax_j
                subtask.effective_deadline -= idle_time;  // Adjust deadline to account for idle time
            }
        }
    }

    // Step 4: Schedule the inflated task set using Algorithm A
    auto [feasible, inflated_schedule] = algorithm_A(task_set, num_processors);
    if (!feasible) {
        std::cout << "No feasible schedule exists after inflating the subtasks.\n";
        return std::make_pair(false, final_schedule);
    }

    return std::make_pair(true, inflated_schedule);
}



#endif