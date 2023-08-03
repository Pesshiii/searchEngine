#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <mutex>



struct Entry {
    size_t doc_id, count = 1;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
        count == other.count);
    }

};


class InvertedIndex{

 public:
    InvertedIndex() = default;
    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    /**
    * Метод определяет количество вхождений слова word в загруженной базе документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word);
 private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
 
};

void getUniqWords(std::string text, std::vector<std::string>* unicWords);
void searchInText(std::string word, std::string text, std::vector<Entry>* wordCount, int id);

void getUniqWords(const std::string text, std::vector<std::string>* unicWords);
void searchInText(const std::string word, std::string text, std::vector<Entry>* wordCount, int id);
