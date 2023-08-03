#include "converterJSON.hpp"

#include "nlohmann/json.hpp"

#include <iostream>
#include <vector>
#include <fstream>


ConverterJSON::ConverterJSON(){
    
    nlohmann::json config;
    std::ifstream configFile("..\\jsonFiles\\config.json");
    configFile >> config;
    configFile.close();
    for(int i = 0; i < config["files"].size(); i++){
        std::ifstream textFile(config["files"][i]);
        std::string text;
        while(!textFile.eof()){
            std::string buff;
            textFile >> buff;
            text += " " + buff;
        }
        textDocuments.push_back(text);
    }

    
    responseLimit = config["config"]["max_responses"];

    configFile.close();
    //sets up cofig file


    //
    
    nlohmann::json requestsJSON;
    std::ifstream requestsFile("..\\jsonFiles\\requests.json");
    requestsFile >> requestsJSON;
    for(int i = 0; i < requestsJSON["requests"].size(); i++){
        requests.push_back(requestsJSON["requests"][i]);
    }

    requestsFile.close();
    


}

std::vector<std::string> ConverterJSON::GetTextDocuments(){return textDocuments;}

int ConverterJSON::GetResponsesLimit(){return responseLimit;}

std::vector<std::string> ConverterJSON::GetRequests(){return requests;}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>answers){
    
    std::ofstream answersFile("..\\jsonFiles\\answers.json");
    nlohmann::ordered_json answersJSON;
    for(int i = 0; i < answers.size(); i++){
        std::string buffer = "request";
        buffer += '0' + (int)((i + 1)/100);
        buffer += '0' + (int)((i + 1)%100/10);
        buffer += '0' + (i + 1)%10;
        nlohmann::ordered_json request;
        request["result"] = (bool)(answers[i].size() != 0);
        if(request["result"]){
            nlohmann::ordered_json results;
            for(int j = 0; j < answers[i].size(); ++j){
                nlohmann::ordered_json pairJSON = {{"docid", answers[i][j].first}, {"rank", answers[i][j].second}};
                request["relevance"].push_back(pairJSON);
            }
            answersJSON["answers"][buffer]["relevance"].push_back(results);
        }
        answersJSON["answers"][buffer] = request;

    }
    answersFile << std::setw(4) << answersJSON << std::endl;
    answersFile.close();
}