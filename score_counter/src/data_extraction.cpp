#include <iostream>
#include <string>
#include <score_counter/data_extraction.hpp>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace data_extraction{
    static json jsonData;
    static int matchSelectionNum = 345;
    static const json* selectedMatch = nullptr;

    //you must call extract_match_data before any other functions can work
    int extract_match_data(){
        auto response = cpr::Get(
            cpr::Url{
                "https://site.api.espn.com/apis/site/v2/sports/soccer/fifa.world/scoreboard"
            }
        );

        if (response.status_code != 200) {
            std::cerr << "Request failed: " << response.status_code << '\n';
            return 1;
        }

        jsonData = json::parse(response.text);

        if(matchSelectionNum != 345){
            set_match(matchSelectionNum);
        }

        return 0;
    }
    int get_listed_matches(){
        int i = 1;
        bool matchFound = false;
        for (const auto& event : jsonData["events"]) {
            matchFound = true;
            const auto& competitors =
                event["competitions"][0]["competitors"];

            std::string team1 = competitors[0]["team"]["displayName"];
            std::string team2 = competitors[1]["team"]["displayName"];

            std::string printLine = std::to_string(i) + ": " + team1 + " vs " + team2;
            if(event["status"]["type"]["state"] == "in"){
                printLine += " | LIVE";
            }
            std::cout << printLine << std::endl;
            i++;
        }
        if(!matchFound){
            std::cout << "Match not found.\n";
            return 1;
        }
        else return 0;
    }
    int set_match(int matchNumber){
        selectedMatch = &jsonData["events"][matchNumber-1];
        if((*selectedMatch).is_null()){
            //case of an invalid input
            return 1;
        }
        matchSelectionNum = matchNumber;
        return 0;
    }
    std::string get_score(int type){
        const auto& competitors = (*selectedMatch)["competitions"][0]["competitors"];
        std::string score1 = competitors[0]["score"];
        std::string score2 = competitors[1]["score"];
        std::string scoreText;
        if(type == 1){
            scoreText = score1;
        }
        else if (type == 2){
            scoreText = score2;
        }
        else{
            std::string team1 = competitors[0]["team"]["displayName"];
            std::string team2 = competitors[1]["team"]["displayName"];
            scoreText = team1 + " " + score1 + " - " + score2 + " " + team2;
        }

        return scoreText;
    }
    std::string get_event_info(){
        const auto& competitors = (*selectedMatch)["competitions"][0]["competitors"];
        std::string team1= competitors[0]["team"]["displayName"];
        std::string team2 = competitors[1]["team"]["displayName"];
        std::string city = (*selectedMatch)["competitions"][0]["venue"]["address"]["city"];

        std::string eventText = team1 + " vs " + team2 + " in " + city;
        return eventText;
    }
    std::string get_time_info(){
        std::string clock = (*selectedMatch)["status"]["displayClock"];
        std::string statusDescription = (*selectedMatch)["status"]["type"]["description"];
        bool statusCompleted = (*selectedMatch)["status"]["type"]["completed"];
        if(statusCompleted == true){
            return "Final";
        }
        else if(statusDescription == "Half Time"){
            return "Half Time";
        }
        else if((*selectedMatch)["status"]["type"]["name"] == "STATUS_SCHEDULED"){
            return "Scheduled";
        }
        else{
            return "Minute " + clock;
        }
    }
}