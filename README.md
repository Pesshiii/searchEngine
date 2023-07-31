# Search Engine
Personal googlesearch for your pc
## Description

This piece of software is used to find sets of words in specified .txt files using inverted index system.

## What do you use search engine for?
It basicly works like a google or yandex but for your own   `.txt`  files. 

So you use __search engine__ if you have some phrase or  search request to find fast. 

# How to use

- To use this code you first need to type in paths for text files and limit on the number of the answers in `config.json` file.

```json
{
    "config": {
        "name": "SearchEngine",
        "version": "0.1",
        "max_responses": 5
    },
    "files": [
        "../resources/file001.txt",
        "../resources/file002.txt",
        "../resources/file003.txt",
        "../resources/file004.txt"
        ]
    }
```
- Then you can use `request.json` file to specify what different sets of words are you looking for.

```json
{
    "requests": [
        "is the capital",
        "moscow is the capital of russia",
        "the",
        "lisboa is the capital of portugal",
        "shvabra"
    ]
    }
    
```
- Finaly run the code(after building the project type in comand line, when you are in project folder, `cd build` then `.\search_engine.exe` and you will get your ranked answers in `answers.json` file.

```json
{
    "answers": {
        "request001": {
            "result": true,
            "relevance": [
                {
                    "docid": 0,
                    "rank": 1.0
                },
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 0.75
                }
            ]
        },
...
        "request005": {
            "result": false
        }
    }
}
```

# About the project
- Files for configuration, searching and results are `.json` type, so this project includes library from [nlohmann](https://github.com/nlohmann/json)

- Tests are done by [google-test](https://github.com/google/googletest).

- InvertedIndex class maps all the unic words in files using `<thread>` library

## Inverted index sistem is implemented in this project:

**Inverted index** is an index data structure storing a mapping from content, such as words or numbers, to its locations in a document or a set of documents. In simple words, it is a hashmap-like data structure that directs you from a word to a document.

Code ranges results based on their relevance, using this equasion:


![equasion](https://latex.codecogs.com/svg.image?&space;R=\frac{R_{abs}}{R_{{abs}{max}}})



![equasion](https://latex.codecogs.com/svg.image?&space;R) - is our relevance

![equasion](https://latex.codecogs.com/svg.image?&space;R_{abs}) - all appearences of given word in different files
![equasion](https://latex.codecogs.com/svg.image?&space;R_{{abs}_{max}}) - max\; of ![equasion](https://latex.codecogs.com/svg.image?&space;R_{abs}) in given set

implemented here:

```cpp


        float maxIndex = 0;

        std::vector<RelativeIndex> absIndexes;
        for(auto wordEntry: _index.GetWordCount(uniqWords[0])){
            RelativeIndex docAbsIndex{wordEntry.doc_id, (float)wordEntry.count};
            for(auto otherWord: uniqWords){
                if(otherWord != uniqWords[0]){
                    for(auto otherWordEntry: _index.GetWordCount(otherWord)){
                        if(otherWordEntry.doc_id == wordEntry.doc_id){
                            docAbsIndex.rank += otherWordEntry.count;            // counts all the enries from the given set of uniq words
                        }
                    }
                }
            }
            absIndexes.push_back(docAbsIndex);
            if(docAbsIndex.rank > maxIndex) maxIndex = docAbsIndex.rank;        // finds max quantity of entries
        }
        

        relIndexes.push_back(sortByRank(absIndexes));

        for(int i = 0; i < absIndexes.size(); i++){
            relIndexes.back()[i].rank /= maxIndex;                                // devides every value by max for this request
        }

    }


    return relIndexes;
  }
```
