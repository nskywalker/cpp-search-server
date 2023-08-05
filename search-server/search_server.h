#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <tuple>
#include <map>
#include <numeric>
#include <cmath>
#include <execution>
#include "document.h"
#include "string_processing.h"

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words);

    explicit SearchServer(const string& stop_words_text);

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings);

    int GetDocumentId(int index);

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query,
                                      DocumentPredicate document_predicate) const;

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const;

    vector<Document> FindTopDocuments(const string& raw_query) const;

    int GetDocumentCount() const;

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const;

    tuple<vector<string>, DocumentStatus> MatchDocument(const execution::sequenced_policy& seqOrParRem, const string& raw_query, int document_id) const;

    tuple<vector<string>, DocumentStatus> MatchDocument(const execution::parallel_policy& seqOrParRem, const string& raw_query, int document_id) const;

    vector<int>::iterator begin();
    vector<int>::iterator end();

    const map<string, double>& GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);

    void RemoveDocument(const execution::parallel_policy& seqOrParRem, int document_id);

    void RemoveDocument(const execution::sequenced_policy& seqOrParRem, int document_id);

private:
    vector<int> indexes;
    bool IsValidStopWords() const;
    static bool IsValidWord(const string& word);
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, map<string, double>> document_to_word_freqs_;
    map<int, DocumentData> documents_;
    bool IsStopWord(const string& word) const;
    vector<string> SplitIntoWordsNoStop(const string& text) const;

    static int ComputeAverageRating(const vector<int>& ratings);

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const;

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const;

    Query ParseQuery(const execution::sequenced_policy& sequencedPolicy, const string& text) const;

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const;
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    if (!IsValidStopWords()) {
        throw invalid_argument("Denied characters in stop-words");
    }
}

template <typename DocumentPredicate>
vector<Document> SearchServer::FindTopDocuments(const string& raw_query,
                                  DocumentPredicate document_predicate) const {
    const double bias = 1e-6;
    const Query query = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(),
         [bias](const Document& lhs, const Document& rhs) {
             if (abs(lhs.relevance - rhs.relevance) < bias) {
                 return lhs.rating > rhs.rating;
             }
             return lhs.relevance > rhs.relevance;
         });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}

template <typename DocumentPredicate>
vector<Document> SearchServer::FindAllDocuments(const Query& query,
                                  DocumentPredicate document_predicate) const {
    map<int, double> document_to_relevance;
    for (const string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
            const auto& document_data = documents_.at(document_id);
            if (document_predicate(document_id, document_data.status, document_data.rating)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }
    }

    for (const string& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
            document_to_relevance.erase(document_id);
        }
    }

    vector<Document> matched_documents;
    for (const auto [document_id, relevance] : document_to_relevance) {
        matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}