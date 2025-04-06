#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
using namespace std;


class Word {
public:
    string doc;
    vector<int> position;
    Word(const string& docName, int pos) : doc(docName) {
        position.push_back(pos);
    }
    void addPosition(int pos) {
        position.push_back(pos);
    }
};


class MapSearch {

private:
    unordered_map<string, vector<Word>> wordIndex;

public:
    void crawlDocument(const string& fileName);
    void searchWord(const string& word);
    void searchTwoWords(const string& word1, const string& word2);
    void searchMultipleWords(const vector<string>& words);
    void searchPhrase(const string& phrase);
    void searchWithExclusion(const string& word1, const string& word2);
};

void MapSearch::crawlDocument(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << fileName << endl;
        return;
    }
    string word;
    int position = 1;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (wordIndex.find(word) == wordIndex.end()) {
            wordIndex[word] = { Word(fileName, position) };
        }
        else {
            auto& wordInstances = wordIndex[word];
            auto it = find_if(wordInstances.begin(), wordInstances.end(),
                [&](const Word& instance) { return instance.doc == fileName; });

            if (it == wordInstances.end()) {
                wordInstances.push_back(Word(fileName, position));
            }
            else {
                it->addPosition(position);
            }
        }
        position++;
    }
    file.close();
}

void MapSearch::searchWord(const string& word) {
    cout << "Searching for single word: " << word << endl;
    auto it = wordIndex.find(word);
    if (it != wordIndex.end()) {
        cout << "Found in documents: ";
        for (const auto& instance : it->second) {
            cout << instance.doc << " ";
        }
        cout << endl;
    }
    else {
        cout << "Not found in any document." << endl;
    }
}

void MapSearch::searchTwoWords(const string& word1, const string& word2) {
    cout << "Searching for two words: " << word1 << " and " << word2 << endl;

    auto it1 = wordIndex.find(word1);
    auto it2 = wordIndex.find(word2);

    if (it1 != wordIndex.end() && it2 != wordIndex.end()) {
        cout << "Found in common documents: ";
        vector<string> commonDocuments;
        set_intersection(
            it1->second.begin(), it1->second.end(),
            it2->second.begin(), it2->second.end(),
            back_inserter(commonDocuments),
            [](const Word& a, const Word& b) {
                return a.doc < b.doc;
            }
        );

        for (const auto& instance : commonDocuments) {
            cout << instance << " ";
        }
        cout << endl;
    }
    else {
        cout << "One or both words not found in any document." << endl;
    }
}

void MapSearch::searchMultipleWords(const vector<string>& words) {
    cout << "Searching for multiple words: ";
    for (const auto& word : words) {
        cout << word << " ";
    }
    cout << endl;
    vector<vector<string>> documents(words.size());
    for (size_t i = 0; i < words.size(); ++i) {
        auto it = wordIndex.find(words[i]);
        if (it != wordIndex.end()) {
            for (const auto& instance : it->second) {
                documents[i].push_back(instance.doc);
            }
        }
    }
    vector<string> commonDocuments;
    for (size_t i = 1; i < documents.size(); ++i) {
        set_intersection(
            documents[0].begin(), documents[0].end(),
            documents[i].begin(), documents[i].end(),
            back_inserter(commonDocuments)
        );
    }
    if (!commonDocuments.empty()) {
        cout << "Found in common documents: ";
        for (const auto& doc : commonDocuments) {
            cout << doc << " ";
        }
        cout << endl;
    }
    else {
        cout << "No common documents found." << endl;
    }
}

void MapSearch::searchPhrase(const string& phrase) {
    cout << "Searching for phrase: " << phrase << endl;
    istringstream iss(phrase);
    vector<string> words;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));
    for (const auto& word : words) {
        auto it = wordIndex.find(word);
        if (it != wordIndex.end()) {
            cout << "Found in documents for word '" << word << "': ";
            for (const auto& instance : it->second) {
                cout << instance.doc << " ";
            }
            cout << endl;
        }
        else {
            cout << "Word '" << word << "' not found in any document." << endl;
        }
    }
}

void MapSearch::searchWithExclusion(const string& word1, const string& word2) {
    cout << "Searching with exclusion: " << word1 << " - " << word2 << endl;

    auto it1 = wordIndex.find(word1);
    auto it2 = wordIndex.find(word2);

    if (it1 != wordIndex.end() && it2 != wordIndex.end()) {
        cout << "Found in documents where " << word1 << " is present but " << word2 << " is not: ";
        vector<string> excludedDocuments;
        set_difference(
            it1->second.begin(), it1->second.end(),
            it2->second.begin(), it2->second.end(),
            back_inserter(excludedDocuments),
            [](const Word& a, const Word& b) {
                return a.doc < b.doc;
            }
        );

        for (const auto& instance : excludedDocuments) {
            cout << instance << " ";
        }
        cout << endl;
    }
    else {
        cout << "One or both words not found in any document." << endl;
    }
}

class HashMapSearch {
private:
    map<string, vector<Word>> wordIndex;

public:
    void crawlDocument(const string& fileName);
    void searchWord(const string& word);
    void searchTwoWords(const string& word1, const string& word2);
    void searchMultipleWords(const vector<string>& words);
    void searchPhrase(const string& phrase);
    void searchWithExclusion(const string& word1, const string& word2);
};

void HashMapSearch::crawlDocument(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << fileName << endl;
        return;
    }
    string word;
    int position = 1;
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (wordIndex.find(word) == wordIndex.end()) {
            wordIndex[word] = { Word(fileName, position) };
        }
        else {
            auto& wordInstances = wordIndex[word];
            auto it = find_if(wordInstances.begin(), wordInstances.end(),
                [&](const Word& instance) { return instance.doc == fileName; });

            if (it == wordInstances.end()) {
                wordInstances.push_back(Word(fileName, position));
            }
            else {
                it->addPosition(position);
            }
        }

        position++;
    }
    file.close();
}
void MapSearch::searchWord(const string& word) {
    cout << "Searching for single word: " << word << endl;
    auto it = wordIndex.find(word);
    if (it != wordIndex.end()) {
        cout << "Found in documents: ";
        for (const auto& instance : it->second) {
            cout << instance.doc << " ";
        }
        cout << endl;
    }
    else {
        cout << "Not found in any document." << endl;
    }
}

void MapSearch::searchTwoWords(const string& word1, const string& word2) {
    cout << "Searching for two words: " << word1 << " and " << word2 << endl;

    auto it1 = wordIndex.find(word1);
    auto it2 = wordIndex.find(word2);

    if (it1 != wordIndex.end() && it2 != wordIndex.end()) {
        cout << "Found in common documents: ";
        vector<string> commonDocuments;
        set_intersection(
            it1->second.begin(), it1->second.end(),
            it2->second.begin(), it2->second.end(),
            back_inserter(commonDocuments),
            [](const Word& a, const Word& b) {
                return a.doc < b.doc;
            }
        );

        for (const auto& instance : commonDocuments) {
            cout << instance << " ";
        }
        cout << endl;
    }
    else {
        cout << "One or both words not found in any document." << endl;
    }
}

void MapSearch::searchMultipleWords(const vector<string>& words) {
    cout << "Searching for multiple words: ";
    for (const auto& word : words) {
        cout << word << " ";
    }
    cout << endl;
    vector<vector<string>> documents(words.size());
    for (size_t i = 0; i < words.size(); ++i) {
        auto it = wordIndex.find(words[i]);
        if (it != wordIndex.end()) {
            for (const auto& instance : it->second) {
                documents[i].push_back(instance.doc);
            }
        }
    }
    vector<string> commonDocuments;
    for (size_t i = 1; i < documents.size(); ++i) {
        set_intersection(
            documents[0].begin(), documents[0].end(),
            documents[i].begin(), documents[i].end(),
            back_inserter(commonDocuments)
        );
    }
    if (!commonDocuments.empty()) {
        cout << "Found in common documents: ";
        for (const auto& doc : commonDocuments) {
            cout << doc << " ";
        }
        cout << endl;
    }
    else {
        cout << "No common documents found." << endl;
    }
}

void MapSearch::searchPhrase(const string& phrase) {
    cout << "Searching for phrase: " << phrase << endl;
    istringstream iss(phrase);
    vector<string> words;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));
    for (const auto& word : words) {
        auto it = wordIndex.find(word);
        if (it != wordIndex.end()) {
            cout << "Found in documents for word '" << word << "': ";
            for (const auto& instance : it->second) {
                cout << instance.doc << " ";
            }
            cout << endl;
        }
        else {
            cout << "Word '" << word << "' not found in any document." << endl;
        }
    }
}

void MapSearch::searchWithExclusion(const string& word1, const string& word2) {
    cout << "Searching with exclusion: " << word1 << " - " << word2 << endl;

    auto it1 = wordIndex.find(word1);
    auto it2 = wordIndex.find(word2);

    if (it1 != wordIndex.end() && it2 != wordIndex.end()) {
        cout << "Found in documents where " << word1 << " is present but " << word2 << " is not: ";
        vector<string> excludedDocuments;
        set_difference(
            it1->second.begin(), it1->second.end(),
            it2->second.begin(), it2->second.end(),
            back_inserter(excludedDocuments),
            [](const Word& a, const Word& b) {
                return a.doc < b.doc;
            }
        );

        for (const auto& instance : excludedDocuments) {
            cout << instance << " ";
        }
        cout << endl;
    }
    else {
        cout << "One or both words not found in any document." << endl;
    }
}



int main() {
    MapSearch mapSearchEngine;
    HashMapSearch hashMapSearchEngine;
    int numberOfFiles;
    cout << "Enter the number of files to search: ";
    cin >> numberOfFiles;
    for (int i = 1; i <= numberOfFiles; ++i) {
        string file = "C:\\Users\\Admin\\OneDrive\\Desktop\\review_text\\review_text\\review_" + to_string(i) + ".txt";
        mapSearchEngine.crawlDocument(file);
        hashMapSearchEngine.crawlDocument(file);
    }
    while (true) {
        cout << "\nSEARCH ENGINE\n";
        cout << "1. Search Word\n";
        cout << "2. Search 2 Words with +\n";
        cout << "3. Search Multiple words (space separated)\n";
        cout << "4. Search phrase/sentence\n";
        cout << "5. Search Exclusion/Subtraction\n";
        cout << "6. Subtraction with sentence as well as multiple words\n";
        cout << "7. Exit\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 7) {
            break;
        }
        switch (choice) {
        case 1: {
            string query;
            cout << "Enter the word to search: ";
            cin >> query;
            mapSearchEngine.searchWord(query);
            hashMapSearchEngine.searchWord(query);
            break;
        }
        case 2: {
            string query1, query2;
            cout << "Enter the 2 words to search (separated by space): ";
            cin >> query1 >> query2;
            mapSearchEngine.searchTwoWords(query1, query2);
            hashMapSearchEngine.searchTwoWords(query1, query2);
            break;
        }
        case 3: {
            int numWords;
            cout << "Enter the number of words to search: ";
            cin >> numWords;
            vector<string> words(numWords);
            cout << "Enter the words to search (space separated): ";
            for (auto& word : words) {
                cin >> word;
            }
            mapSearchEngine.searchMultipleWords(words);
            hashMapSearchEngine.searchMultipleWords(words);
            break;
        }
        case 4: {
            string query;
            cout << "Enter the sentence to search: ";
            cin.ignore(INT_MAX, '\n');
            getline(cin, query);
            mapSearchEngine.searchPhrase(query);
            hashMapSearchEngine.searchPhrase(query);
            break;
        }

        case 5: {
            string query1, query2;
            cout << "Enter the 2 words for exclusion (separated by space): ";
            cin >> query1 >> query2;
            mapSearchEngine.searchWithExclusion(query1, query2);
            hashMapSearchEngine.searchWithExclusion(query1, query2);
            break;
        }
        case 6: {
            int numWords;
            cout << "Enter the number of words for subtraction: ";
            cin >> numWords;
            vector<string> words(numWords);
            cout << "Enter the words for subtraction (space separated): ";
            for (auto& word : words) {
                cin >> word;
            }
            string excludeWord;
            cout << "Enter the word to exclude: ";
            cin >> excludeWord;
          /*  mapSearchEngine.searchWithExclusionAndSentence(words, excludeWord);
            hashMapSearchEngine.searchWithExclusionAndSentence(words, excludeWord);*/
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
