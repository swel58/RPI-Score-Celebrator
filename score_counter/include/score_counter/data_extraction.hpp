#pragma once

namespace data_extraction{
    int extract_match_data();
    int get_listed_matches();
    int set_match(int matchNumber);
    std::string get_score(int type = 3);
    std::string get_event_info();
    std::string get_time_info();
}