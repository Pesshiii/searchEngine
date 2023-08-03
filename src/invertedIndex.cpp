#include "invertedIndex.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include <thread>
#include <mutex>


std::mutex listAccess;


void getUniqWords(const std::string text, std::vector<std::string>* unicWords){
    std::stringstream s(text);
    while(!s.eof()){
        std::string buff;
        s >> buff;
        bool unic = true;
        for(auto sepWord: *unicWords){
            if(buff == sepWord) unic = false;
        }
        if(unic){
            listAccess.lock();
            unicWords->push_back(buff);
            listAccess.unlock();
        }
    }
}

void searchInText(const std::string word, std::string text, std::vector<Entry>* wordCount, int id){
    std::stringstream s(text);
    Entry counter = {(size_t) id, 0};
    while(!s.eof()){
        std::string buffer;
        s >> buffer;
        if(buffer == word){
            ++counter.count;
        }
    }
    if(counter.count > 0)
    {
        listAccess.lock();
        wordCount->push_back(counter);
        listAccess.unlock();
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs){
    
    std::vector<std::string> unicWords;
    std::vector<std::thread*> docThreads; 


    for(auto doc: input_docs){
        auto findSepWords = new std::thread(getUniqWords, doc, &unicWords);
        docThreads.push_back(findSepWords);
    }


    for(auto docThread: docThreads){
        if(docThread->joinable())docThread->join();
    }
    for(auto docThread: docThreads){
        delete docThread;
    }
    

    for(int i = 0; i < unicWords.size(); ++i){
        std::vector<std::thread*> wordThreads; 
        std::vector<Entry> wordCount;

        for(int j = 0; j < input_docs.size(); ++j){
            auto countWord = new std::thread(searchInText, unicWords[i], input_docs[j], &wordCount, j);
            wordThreads.push_back(countWord);
        }
        for(auto wordThread: wordThreads){
            if(wordThread->joinable()) wordThread->join();
        }
        for(auto wordThread: wordThreads){
            delete wordThread;
        }

        freq_dictionary.insert(std::pair(unicWords[i], wordCount));
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){return freq_dictionary[word];}