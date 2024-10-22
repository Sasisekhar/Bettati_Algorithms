#ifndef ALGORITHM_H_HPP__
#define ALGORITHM_H_HPP__

#include "algorithmA.hpp"
#include "algorithmC.hpp"

std::pair<bool, std::vector<process_schedule>> algorithm_H( const std::vector<tasks>& __task_set, 
                                                            int num_processors,
                                                            const std::pair<bool, int> force_bottleneck = {false, 0}) {
    std::cout << "\nperforming Algorithm H" << std::endl;

    std::vector<process_schedule> final_schedule;
    auto _task_set = __task_set;

    std::cout << "computing rij, dij" << std::endl;
    _task_set = compute_effective_times(_task_set);
    std::cout << "given task set" << std::endl;
    print_tasks_as_table(_task_set);

    auto task_set = _task_set;



    std::cout << "determine Tmax for each processor:" << std::endl;
    // Step 2: Determine Tmax,j for each processor
    std::vector<int> Tmax_j(num_processors, 0);  // Tmax for each processor (initialize with 0)
    for (int processor = 0; processor < num_processors; ++processor) {
        for (const auto& task : task_set) {
            const auto& subtask = task.subtask_set[processor];
            Tmax_j[processor] = std::max(Tmax_j[processor], subtask.processing_time);  // Find the max processing time for each processor
        }
    }

    for(int i = 0; i < Tmax_j.size(); i++) {
        std::cout << "P" << i << ": " << Tmax_j[i] << std::endl;
    }

    std::cout << "inflating all subtasks per Tmax" << std::endl;
    // Step 3: Inflate all subtasks on each processor
    for (int processor = 0; processor < num_processors; ++processor) {
        for (auto& task : task_set) {
            auto& subtask = task.subtask_set[processor];
            if (subtask.processing_time < Tmax_j[processor]) {
                // Inflate by adding idle time
                int idle_time = Tmax_j[processor] - subtask.processing_time;

                subtask.processing_time = Tmax_j[processor];  // Make processing time equal to Tmax_j
                // subtask.effective_deadline -= idle_time;  // Not sure if I should adjust deadline to account for idle time
            }
        }
    }
    std::cout << "inflated task set:" << std::endl;
    print_tasks_as_table(task_set);

    std::cout << "scheduling inflated tasks" << std::endl;
    // Step 4: Schedule the inflated task set using Algorithm A
    auto [feasible, inflated_schedule] = algorithm_A(task_set, num_processors, _task_set, force_bottleneck);
    if (!feasible) {
        std::cout << "no feasible schedule exists after inflating the subtasks.\n";
        return std::make_pair(false, final_schedule);
    }
    std::cout << "inflated schedule:" << std::endl;
    print_gantt_chart(inflated_schedule);

    std::cout << "Compacting the schedule:" << std::endl;
    auto compacted_schedule = algorithm_C(inflated_schedule);
    print_gantt_chart(compacted_schedule);

    return std::make_pair(true, compacted_schedule);
}



#endif