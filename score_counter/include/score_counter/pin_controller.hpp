#pragma once

namespace pin_controller{
    void gpio_initialize();
    void gpio_terminate();
    void rpi_tone(unsigned pin, unsigned frequency);
    void play_note(unsigned frequency, int holdLength, int totalLength = 0, unsigned pin = 800);
    void goal(int team);
    void game_end(int team);
    void back_and_forth();

    void play_lamb(int noteLength);
    void play_cucaracha(int noteLength);
    void game_of_thrones(int noteLength);
    void play_asian(int noteLength);
}