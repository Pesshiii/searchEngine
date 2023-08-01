#include "searchServer.hpp"

#include "gtest/gtest.h"

//
#include "invertedIndexTest.hpp"
#include "searchServerTest.hpp"


using namespace std;

int main(int argc, char *argv[]){
    auto converter = new ConverterJSON;
    auto inverter = new InvertedIndex;

    inverter->UpdateDocumentBase(converter->GetTextDocuments());

    auto server = new SearchServer(*inverter);
    
    std::vector<std::vector<std::pair<int, float>>> answers;
    for(auto responses: server->search(converter->GetRequests())){
        answers.emplace_back();
        for(int i = 0; i < converter->GetResponsesLimit(); ++i){
            if(i < responses.size()){
                answers.back().emplace_back((int)responses[i].doc_id, responses[i].rank);
            }
        }
    }
    converter->putAnswers(answers);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
