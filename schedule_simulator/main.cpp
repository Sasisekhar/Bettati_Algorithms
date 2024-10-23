#include "algorithmH.hpp"
#include "input_output.hpp"

int main(int argc, char** argv) {

    if(argc < 1) {
        std::cerr << "Invalid Argument" << std::endl;
        return 1;
    }
    
    auto input = read_tasks_from_csv(argv[1]);

    if(input.num_processors == -1){
        return -1;
    }


    
    auto output = algorithm_H(input.task_set, input.num_processors);

    if(output.first == false) {
        // std::cerr << "not schedulable" << std::endl;
        return -1;
    }

    write_schedule_to_csv(output.second, argv[2]);



    return 0;
}