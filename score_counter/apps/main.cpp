#include <iostream>
#include <score_counter/core.hpp>

int main() {
    std::cout << "\n////////////////////////////\nScore Counter Is Now Running\n////////////////////////////\n" << std::endl;
    
    int core_return_state = run_core_logic();
    if(core_return_state){
        std::cout << "\nProgram End Failure" << std::endl;
        return 1;
    }
    std::cout << "\nProgram End Success" << std::endl;
    return 0;
}