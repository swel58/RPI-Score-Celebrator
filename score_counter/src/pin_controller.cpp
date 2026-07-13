#include <iostream>
#include <thread>
#include <chrono>
#include <score_counter/pin_controller.hpp>
#include <pigpio.h>
#include <score_counter/pitches.hpp>
#include <cmath>

#define PI_BUZZER_PIN 18 // GPIO 4 supports the hardware clock (GPCLK0)

namespace pin_controller{
    constexpr int  SERVO_1 = 26;
    constexpr int SERVO_2 = 13;
    static double globalPitchMultiplier = 1.0f;

    void gpio_initialize(){
        gpioInitialise();
        gpioServo(SERVO_1, 2500);
        gpioServo(SERVO_2, 2500);
    }
    void gpio_terminate(){
        gpioInitialise();
        rpi_tone(PI_BUZZER_PIN, 0);
        gpioServo(SERVO_1, 2500);
        gpioServo(SERVO_2, 2500);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpioTerminate();
        
    }
    void rpi_tone(unsigned pin, unsigned frequency) {
        if (frequency == 0) {
            gpioHardwarePWM(pin, 0, 0); // Shut off the signal
        } else {
            // 500000 represents a 50% duty cycle square wave (500,000 out of 1,000,000)
            gpioHardwarePWM(pin, frequency, 500000); 
        }
    }
    void play_note(unsigned frequency, int holdLength, int totalLength, unsigned pin){
        if(pin == 800) pin = PI_BUZZER_PIN;
        if(totalLength == 0) totalLength = holdLength;
        int silenceLength = totalLength - holdLength;
        rpi_tone(pin, frequency*globalPitchMultiplier);
        std::this_thread::sleep_for(std::chrono::milliseconds(holdLength));
        rpi_tone(pin, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(silenceLength));

        //globalPitchMultiplier += 0.01f;

    }
    void goal(int team){
        pin_controller::gpio_initialize();
        std::cout << "Team " << team << " 🏃🏃🏃🏃🏃🏃⚽⚽⚽⚽⚽⚽⚽⚽⚽⚽⚽GOAL⚽⚽⚽⚽⚽⚽⚽⚽⚽⚽⚽🏃🏃🏃🏃🏃🏃 " << std::endl;
        int controlServo;
        if(team == 1) controlServo = SERVO_1;
        else if (team == 2) controlServo = SERVO_2;
        globalPitchMultiplier = 1.0f;
        for(int i = 0; i < 3; i++){
            gpioServo(controlServo, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            gpioServo(controlServo, 2000);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        if(team == 1) play_cucaracha(150);
        else play_cucaracha(300);
        for(int i = 0; i < 3; i++){
            gpioServo(controlServo, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            gpioServo(controlServo, 2000);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        gpioServo(controlServo, 2500);
        pin_controller::gpio_terminate();
    }
    void game_end(int team){
        pin_controller::gpio_initialize();
        if(team == 3){
            goal(1);
            goal(2);
        } else goal(team);
        gpioServo(SERVO_1, 2500);
        gpioServo(SERVO_2, 2500);
        pin_controller::gpio_terminate();
    }
    static int x = 0;
    static int slowAngle = 1000;
    static int slowDir = 1;
    static int slowSpeed = 15;
    static int maxAngle = 2000;
    static int minAngle = 1200;
    void back_and_forth(){
        slowAngle += slowSpeed * slowDir;
        //slowSpeed = (int)(47.5 * sin((2.0 * M_PI * x) / 100.0) + 52.5);
        if(slowAngle > maxAngle){
            slowDir = -1;
            slowAngle = maxAngle;
        }
        else if (slowAngle < minAngle){
            slowDir = 1;
            slowAngle = minAngle;
        }
        //gpioServo(SERVO_1, slowAngle);
        //gpioServo(SERVO_2, slowAngle);
        //gpioServo(SERVO_1, 2500);
        //gpioServo(SERVO_2, 2500);
        x++;
    }
    //songs More can be added
    void play_lamb(int noteLength){
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_C4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength*2);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_C4, noteLength, noteLength*2);
    }
    void play_cucaracha(int noteLength){
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_G4, noteLength, noteLength*3);
        play_note(NOTE_B4, noteLength, noteLength*2);

        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_G4, noteLength, noteLength*3);
        play_note(NOTE_B4, noteLength, noteLength*5);

        play_note(NOTE_G4, noteLength, noteLength*2);
        play_note(NOTE_G4, noteLength, noteLength);
        play_note(NOTE_FS4, noteLength, noteLength);
        play_note(NOTE_FS4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_E4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength*5);

        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_FS4, noteLength, noteLength*3);
        play_note(NOTE_A4, noteLength, noteLength*2);

        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_D4, noteLength, noteLength);
        play_note(NOTE_FS4, noteLength, noteLength*3);
        play_note(NOTE_A4, noteLength, noteLength*5);

        play_note(NOTE_D5, noteLength, noteLength*2);
        play_note(NOTE_E5, noteLength, noteLength);
        play_note(NOTE_D5, noteLength, noteLength);
        play_note(NOTE_C5, noteLength, noteLength);
        play_note(NOTE_B4, noteLength, noteLength);
        play_note(NOTE_A4, noteLength, noteLength);
        play_note(NOTE_G4, noteLength, noteLength*5);

    }
    void game_of_thrones(int noteLength){
        for(int i = 0; i < 5; i++){
            play_note(NOTE_A4, noteLength, noteLength*2);
            play_note(NOTE_E4, noteLength, noteLength*2);
            play_note(NOTE_F4, noteLength, noteLength);
            play_note(NOTE_G4, noteLength, noteLength);
        }
    }
    void play_asian(int noteLength){
        play_note(NOTE_DS5, noteLength, noteLength);
        play_note(NOTE_DS5, noteLength, noteLength);
        play_note(NOTE_DS5, noteLength, noteLength);
        play_note(NOTE_DS5, noteLength, noteLength);
        play_note(NOTE_CS5, noteLength, noteLength*2);
        play_note(NOTE_CS5, noteLength, noteLength*2);
        play_note(NOTE_AS4, noteLength, noteLength*2);
        play_note(NOTE_AS4, noteLength, noteLength*2);
        play_note(NOTE_CS5, noteLength, noteLength*2);
    }
}