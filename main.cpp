#include "algorithmH.hpp"

int main() {
    // std::vector<tasks> task_set = {
    //     {
    //         .pid = 1,
    //         .total_processing_time = 10,
    //         .release_time = 1,
    //         .deadline = 10,
    //         .subtask_set = {
    //             {
    //                 .spid = 11,
    //                 .processing_time = 3,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 12,
    //                 .processing_time = 1,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 13,
    //                 .processing_time = 4,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 14,
    //                 .processing_time = 2,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //         }
    //     },
    //     {
    //         .pid = 2,
    //         .total_processing_time = 10,
    //         .release_time = 1,
    //         .deadline = 13,
    //         .subtask_set = {
    //             {
    //                 .spid = 21,
    //                 .processing_time = 3,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 22,
    //                 .processing_time = 1,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 23,
    //                 .processing_time = 4,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 24,
    //                 .processing_time = 2,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //         }
    //     },
    //     {
    //         .pid = 3,
    //         .total_processing_time = 10,
    //         .release_time = 5,
    //         .deadline = 30,
    //         .subtask_set = {
    //             {
    //                 .spid = 31,
    //                 .processing_time = 3,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 32,
    //                 .processing_time = 1,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 33,
    //                 .processing_time = 4,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 34,
    //                 .processing_time = 2,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //         }
    //     },
    //     {
    //         .pid = 4,
    //         .total_processing_time = 10,
    //         .release_time = 14,
    //         .deadline = 26,
    //         .subtask_set = {
    //             {
    //                 .spid = 41,
    //                 .processing_time = 3,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 42,
    //                 .processing_time = 1,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 43,
    //                 .processing_time = 4,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //             {
    //                 .spid = 44,
    //                 .processing_time = 2,
    //                 .effective_release_time = -1,
    //                 .effective_deadline = -1
    //             },
    //         }
    //     }
    // };

    std::vector<tasks> task_set = {
        {
            .pid = 1,
            .total_processing_time = 8,
            .release_time = 1,
            .deadline = 10,
            .subtask_set = {
                {
                    .spid = 11,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 12,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 13,
                    .processing_time = 3,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 14,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
            }
        },
        {
            .pid = 2,
            .total_processing_time = 9,
            .release_time = 1,
            .deadline = 16,
            .subtask_set = {
                {
                    .spid = 21,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 22,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 23,
                    .processing_time = 4,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 24,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
            }
        },
        {
            .pid = 3,
            .total_processing_time = 12,
            .release_time = 1,
            .deadline = 22,
            .subtask_set = {
                {
                    .spid = 31,
                    .processing_time = 3,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 32,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 33,
                    .processing_time = 3,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 34,
                    .processing_time = 4,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
            }
        },
        {
            .pid = 4,
            .total_processing_time = 11,
            .release_time = 14,
            .deadline = 28,
            .subtask_set = {
                {
                    .spid = 41,
                    .processing_time = 2,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 42,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 43,
                    .processing_time = 5,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 44,
                    .processing_time = 3,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
            }
        },
        {
            .pid = 5,
            .total_processing_time = 7,
            .release_time = 14,
            .deadline = 29,
            .subtask_set = {
                {
                    .spid = 51,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 52,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 53,
                    .processing_time = 4,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
                {
                    .spid = 54,
                    .processing_time = 1,
                    .effective_release_time = -1,
                    .effective_deadline = -1
                },
            }
        }
    };

    task_set = compute_effective_times(task_set);
    print_tasks_as_table(task_set);

    
    auto scheduled = algorithm_H(task_set, 4);

    if(scheduled.first) {
        std::cout << "Schedule after Algorithm H:" << std::endl;
        print_gantt_chart(scheduled.second);
    } else {
        std::cerr << "Not feasible" << std::endl;
    }

    auto inflated_schedule = scheduled.second;
    std::cout << "Schedule after Algorithm C:" << std::endl;
    print_gantt_chart(algorithm_C(inflated_schedule, 4, 4));

    return 0;
}