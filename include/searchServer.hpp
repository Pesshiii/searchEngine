#pragma once

#include "invertedIndex.hpp" 

#include <iostream>
#include <vector>


struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
 public:

    /**
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };


    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

 private:
    InvertedIndex _index;
};

std::vector<std::string> sortByFreq(std::vector<std::string> words, std::vector<size_t> freq){
    std::vector<std::string> sortedWords;

    for(int i = 0; i < freq.size(); ++i){
        size_t minVal;
        for(int j = 0; j < freq.size(); ++j){
            if(freq[j] != -1){
                minVal = freq[j];
                break;
            }
        }
        int minId = 0;
        for(int j = 0; j < freq.size(); ++j){
            if(freq[j] != -1 && minVal >= freq[j]){
                minId = j;
                minVal = freq[j];
            }
        }
        freq[minId] = -1;
        sortedWords.push_back(words[minId]);
    }

    return sortedWords;
}

std::vector<RelativeIndex> sortByRank(std::vector<RelativeIndex> indexes){
    std::vector<RelativeIndex> sortedIndexes;

    for(int i = 0; i < indexes.size(); ++i){
        float maxVal = 0;
        int maxId = 0;
        for(int j = 0; j < indexes.size(); ++j){
            if(maxVal < indexes[j].rank){
                maxVal = indexes[j].rank;
                maxId = j;

            }
        }
        sortedIndexes.push_back(indexes[maxId]);
        indexes[maxId].rank = -1;
    }

    return sortedIndexes;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input){
    std::vector<std::vector<RelativeIndex>> relIndexes;
    for(auto request: queries_input){
        std::vector<std::string> uniqWords;
        std::stringstream s(request);
        while (!s.eof())
        {
            std::string buffer;
            s >> buffer;
            bool unique = true;
            for(auto word: uniqWords){
                if(buffer == word){
                    unique = false;
                    break;
                }
            }
            if(unique) uniqWords.push_back(buffer);
        }


        std::vector<size_t> frequensy;

        for(auto word: uniqWords){
            size_t totalCount = 0;
            for(auto locCount: _index.GetWordCount(word)){
                totalCount += locCount.count;
            }
            frequensy.push_back(totalCount);
        }

        
        uniqWords = sortByFreq(uniqWords, frequensy);


        float maxIndex = 0;

        std::vector<RelativeIndex> absIndexes;
        for(auto wordEntry: _index.GetWordCount(uniqWords[0])){
            RelativeIndex docAbsIndex{wordEntry.doc_id, (float)wordEntry.count};
            for(auto otherWord: uniqWords){
                if(otherWord != uniqWords[0]){
                    for(auto otherWordEntry: _index.GetWordCount(otherWord)){
                        if(otherWordEntry.doc_id == wordEntry.doc_id){
                            docAbsIndex.rank += otherWordEntry.count;
                        }
                    }
                }
            }
            absIndexes.push_back(docAbsIndex);
            if(docAbsIndex.rank > maxIndex) maxIndex = docAbsIndex.rank;
        }
        

        relIndexes.push_back(sortByRank(absIndexes));

        for(int i = 0; i < absIndexes.size(); i++){
            relIndexes.back()[i].rank /= maxIndex;
        }

    }


    return relIndexes;
}


