#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <score_counter/core.hpp>
#include <score_counter/data_extraction.hpp>
#include <score_counter/pin_controller.hpp>


constexpr int inputIntakeFrequency = 1000;
//how often we should poll for data. Currently set to every 30 seconds to avoid too many get requests
constexpr int pollingFrequency = 30;

std::atomic<bool> running(true);

void listen_for_quit() {
    char ch;
    std::cin >> ch;
    running = false;
}

int run_core_logic()
{
    //pin_controller::goal(1);

    data_load_prompt();

    data_extraction::get_listed_matches();
    if(match_selection_prompt()){
        pin_controller::gpio_terminate();
        return 0;
    } //else continue. A match has been selected


    // Start the input listener thread
    std::thread input_thread(listen_for_quit);

    int loop_count = 0; // Initialize a counter before the loop
    int previousScore1 = std::stoi(data_extraction::get_score(1));
    int previousScore2 = std::stoi(data_extraction::get_score(2));

    while(running){    //repeated display of info loop
        if (loop_count % pollingFrequency == 0) {
            if(data_extraction::extract_match_data()){
                //connection failure case. Maybe instead give a prompt to make request again.
                input_thread.join();
                pin_controller::gpio_terminate();
                return 1;
            }
            std::string eventInfo = data_extraction::get_event_info();
            std::cout << std::endl << eventInfo << std::endl;

            std::string scoreText = data_extraction::get_score();
            int currentScore1 = std::stoi(data_extraction::get_score(1));
            int currentScore2 = std::stoi(data_extraction::get_score(2));
            if(currentScore1 > previousScore1){
                pin_controller::goal(1);
            }
            if(currentScore2 > previousScore2){
                pin_controller::goal(2);
            }
            previousScore1 = currentScore1;
            previousScore2 = currentScore2;
            std::string timeInfo = data_extraction::get_time_info();
            std::cout << scoreText << " | " << timeInfo << std::endl;
            std::cout << std::endl << "Enter any key then enter to quit.\n" << std::endl;
            if(timeInfo == "Final"){ //end program if match ends. It will play a goal for whichever team wins, or both if it is a tie.
                int winner;
                if(currentScore1 > currentScore2){
                    winner = 1;
                }
                else if(currentScore2 > currentScore1){
                    winner = 2;
                }
                else winner = 3;
                pin_controller::game_end(winner);
                input_thread.join();
                pin_controller::gpio_terminate();
                return 0;

            }

        }
        else{
            //Print dots to show that program is still running while waiting for another request.
            pin_controller::back_and_forth();
            if(loop_count % pollingFrequency == 1){
                std::cout << "....." << std::endl;
            }
            if(loop_count % pollingFrequency == pollingFrequency*1/5){
                std::cout << "...." << std::endl;
            }
            if(loop_count % pollingFrequency == pollingFrequency*2/5){
                std::cout << "..." << std::endl;
            }
            if(loop_count % pollingFrequency == pollingFrequency*3/5){
                std::cout << ".." << std::endl;
            }
            if(loop_count % pollingFrequency == pollingFrequency*4/5){
                std::cout << "." << std::endl;
            }
        }
        loop_count++;
        // delay
        std::this_thread::sleep_for(std::chrono::milliseconds(inputIntakeFrequency));
    }
    // Join the thread before exiting
    input_thread.join();
    pin_controller::gpio_terminate();
    return 0;
}

int data_load_prompt(){
    std::cout << "Loading today's world cup matches..." << std::endl;
    //Get Request. Json gets parsed
    if(data_extraction::extract_match_data()){
        //connection failure case. Maybe instead give a prompt to make request again.
        return 1;
    }
    return 0;
}

int match_selection_prompt(){
    std::cout << "Enter match number or 'q' to exit: ";
    char input;
    
    //check first for 'q'. Then check if its a number. Then check if its an in range number.
    while(true){
        if (!(std::cin >> input)) {
            // Handle catastrophic stream failure (like Ctrl+D / EOF)
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // 2. Clear any leftover characters in the current line (like extra letters or trailing spaces)
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(input == 'q') return 1;
        int inputNum = input - '0';
        if(inputNum >= 0 && inputNum <= 9 && !data_extraction::set_match(inputNum)){
            //success
            return 0;
        }
        else{
            std::cout << "\nInvalid input\nPlease enter a valid match number: ";
        }
    }
}

