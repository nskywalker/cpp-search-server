#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (const auto& word : words) {
            word_to_document_freqs_[word][document_id] += (1./words.size());
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
    void setDocCnt(int docCnt) {document_count_=docCnt;}
    
    double CountIDF(const double& freq_word_in_all_docs) const {
        return log(document_count_/freq_word_in_all_docs);
    }

private:
    int document_count_ = 0;
    
    map <string, map<int, double>> word_to_document_freqs_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    pair<string, bool> FindMinusWord(const string& word) const {
        pair<string, bool> result;
        if (word[0] != '-') {
            result = {word, false};
        }
        else {
            result = {word.substr(1), true};
        }
        return result;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query_words;
        for (string& word : SplitIntoWordsNoStop(text)) {
            pair<string, bool> findMinWord = FindMinusWord(word);
            if(!findMinWord.second) {
                query_words.plus_words.insert(findMinWord.first);
            }
            else {
                query_words.minus_words.insert(findMinWord.first);
            }
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {
        vector<Document> matched_documents;
        map<int, double> document_to_relevance;
        for (const auto& word : query_words.plus_words) {
            if (word_to_document_freqs_.count(word)) {
                double freq_word_in_all_docs = static_cast<double>(word_to_document_freqs_.at(word).size());
                double IDF = CountIDF(freq_word_in_all_docs);
                for (auto& [id, TF] : word_to_document_freqs_.at(word)) {
                    document_to_relevance[id]+=(IDF*TF);
                }
            }
        }
        
        for (const auto& word : query_words.minus_words) {
            if (word_to_document_freqs_.count(word)) {
                for (const auto& [id, IDF] : word_to_document_freqs_.at(word)) {
                    document_to_relevance.erase(id);
                }
            }
        }
        
        for (const auto& [id, relevance] : document_to_relevance)
            matched_documents.push_back({id, relevance});
        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    search_server.setDocCnt(document_count);
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
