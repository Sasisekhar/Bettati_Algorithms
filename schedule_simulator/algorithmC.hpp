#ifndef ALGORITHM_C_HPP__
#define ALGORITHM_C_HPP__

#include "algorithmA.hpp"

std::vector<process_schedule> algorithm_C(const std::vector<process_schedule>& _schedule) {
    // Create a map to group schedules by processor ID
    std::map<int, std::vector<process_schedule>> schedule_map; //schedule_map[Processor][task]
    std::vector<process_schedule> final_schedule;

    std::map<int, std::vector<int>> _r;

    // Populate the map with schedules grouped by processor
    for (const auto& sched : _schedule) {
        schedule_map[sched.processor_id].push_back(sched);
    }

    for (auto& [processor_id, schedules] : schedule_map) {
        std::sort(schedules.begin(), schedules.end(), [](const process_schedule& a, const process_schedule& b) {
            return a.start_time < b.start_time;
        });
    }

    for (const auto& [processor_id, schedule] : schedule_map) {
        for(const auto& task : schedule) {
            _r[processor_id].push_back(task.task.effective_release_time);
        }
    }

    schedule_map[0][0].start_time = std::max(schedule_map[0][0].start_time, _r[0][0]);
    for (auto& [processor_id, schedule] : schedule_map) {
        if(processor_id > 0) {
            schedule_map[processor_id][0].start_time = schedule_map[processor_id - 1][0].start_time + schedule_map[processor_id - 1][0].task.processing_time;
        }

        schedule_map[processor_id][0].task_duration = schedule_map[processor_id][0].task.processing_time;
    }

    for (auto& [processor_id, schedule] : schedule_map) {
        if(processor_id < schedule_map.size() - 1) {
            for(int i = 1; i < schedule.size(); i++) {
                schedule_map[processor_id][i].start_time = std::max(schedule_map[processor_id][i - 1].start_time + schedule_map[processor_id][i - 1].task.processing_time,
                                                                    _r[processor_id][i]);                                    
                _r[processor_id + 1][i] = schedule_map[processor_id][i].start_time + schedule_map[processor_id][i].task.processing_time;
                schedule_map[processor_id][i].task_duration = schedule_map[processor_id][i].task.processing_time;
            }
        } else {
            for(int i = 1; i < schedule.size(); i++) {
                schedule_map[processor_id][i].start_time = std::max(schedule_map[processor_id][i - 1].start_time + schedule_map[processor_id][i - 1].task.processing_time,
                                                                    _r[processor_id][i]);
                schedule_map[processor_id][i].task_duration = schedule_map[processor_id][i].task.processing_time;
            }
        }
    }

    for (const auto& [processor_id, schedule] : schedule_map) {
        for(const auto& task : schedule) {
            final_schedule.push_back(task);
        }
    }

    return final_schedule;
}

#endif