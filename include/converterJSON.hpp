#pragma once

#include <iostream>
#include <vector>



/**
* Класс для работы с JSON-файлами
*/

class ConverterJSON {
 private:
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;
    int responseLimit;
 public:
    ConverterJSON();
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных
    * в config.json
    */
    std::vector<std::string> GetTextDocuments();
    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return
    */
    int GetResponsesLimit();
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>answers);
};
