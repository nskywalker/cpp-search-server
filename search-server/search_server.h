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
#include <deque>
#include "document.h"
#include "string_processing.h"
#include "concurrent_map.h"
#include <mutex>
#include <future>

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

    explicit SearchServer(string_view stop_text);

    void AddDocument(int document_id, const string_view document, DocumentStatus status,
                     const vector<int>& ratings);

    int GetDocumentId(int index);

    template <typename DocumentPredicate, typename Policy>
    vector<Document> FindTopDocuments(Policy, string_view raw_query,
                                      DocumentPredicate document_predicate) const;

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(string_view raw_query,
                                      DocumentPredicate document_predicate) const;

    vector<Document> FindTopDocuments(string_view raw_query, DocumentStatus status) const;

    template <typename PolicyExec>
    vector<Document> FindTopDocuments(PolicyExec policyExec, string_view raw_query) const;

    template <typename PolicyExec>
    vector<Document> FindTopDocuments(PolicyExec policyExec, string_view raw_query, DocumentStatus status) const;

    vector<Document> FindTopDocuments(string_view raw_query) const;

    int GetDocumentCount() const;

    tuple<vector<string_view>, DocumentStatus> MatchDocument(string_view raw_query, int document_id) const;

    tuple<vector<string_view>, DocumentStatus>
    MatchDocument(const execution::sequenced_policy& seqOrParRem, const string_view raw_query, int document_id) const;

    tuple<vector<string_view>, DocumentStatus>
    MatchDocument(const execution::parallel_policy& seqOrParRem, string_view raw_query, int document_id) const;

    vector<int>::iterator begin();
    vector<int>::iterator end();

    const map<string_view, double> & GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);

    void RemoveDocument(const execution::parallel_policy& seqOrParRem, int document_id);

    void RemoveDocument(const execution::sequenced_policy& seqOrParRem, int document_id);

private:
    deque<string> storage;
    vector<int> indexes;
    bool IsValidStopWords() const;
    static bool IsValidWord(string_view word);
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    map<string_view , map<int, double>> word_to_document_freqs_;
    map<int, map<string_view, double>> document_to_word_freqs_;
    map<int, DocumentData> documents_;
    bool IsStopWord(string_view word) const;
    vector<string_view> SplitIntoWordsNoStop(std::string_view text) const;
    static int ComputeAverageRating(const vector<int>& ratings);

    struct QueryWord {
        string_view data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string_view text) const;

    struct Query {
        vector<string_view> plus_words;
        vector<string_view> minus_words;
    };

    const set<string, less<>> stop_words_;

    Query ParseQuery(string_view text) const;

    Query ParseQuery(bool isErasedDuplicates, string_view text) const;

    // Existence required
    double ComputeWordInverseDocumentFreq(const string_view word) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(execution::sequenced_policy, const Query &query, DocumentPredicate document_predicate) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(execution::parallel_policy, const Query &query, DocumentPredicate document_predicate) const;
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    if (!IsValidStopWords()) {
        throw invalid_argument("Denied characters in stop-words");
    }
}

template <typename DocumentPredicate, typename Policy>
vector<Document> SearchServer::FindTopDocuments(Policy policy, string_view raw_query,
                                                DocumentPredicate document_predicate) const {
    const double bias = 1e-6;
    const Query query = ParseQuery(raw_query);
    auto matched_documents = FindAllDocuments(policy, query, document_predicate);

    sort(policy, matched_documents.begin(), matched_documents.end(),
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
vector<Document> SearchServer::FindAllDocuments(execution::sequenced_policy, const Query &query,
                                                DocumentPredicate document_predicate) const {
    map<int, double> document_to_relevance;
    for (const auto& word : query.plus_words) {
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

    for (const auto& word : query.minus_words) {
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

template <typename DocumentPredicate>
vector<Document> SearchServer::FindAllDocuments(execution::parallel_policy, const Query &query, DocumentPredicate document_predicate) const {
    ConcurrentMap<int, double> document_to_relevance(std::thread::hardware_concurrency());
    const auto& plus_words = query.plus_words;
    for_each(execution::par, plus_words.begin(), plus_words.end(), [&](const auto& word) {
        if (word_to_document_freqs_.count(word) != 0) {
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id].ref_to_value += term_freq * inverse_document_freq;
                }
            }
        }
    });
    auto doc_res = document_to_relevance.BuildOrdinaryMap();
    const auto& minus_words = query.minus_words;
    std::mutex m;
    for_each(execution::par, minus_words.begin(), minus_words.end(), [&](const auto& word) {
        if (word_to_document_freqs_.count(word) != 0) {
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                lock_guard guard(m);
                doc_res.erase(document_id);
            }
        }
    });
    vector<Document> matched_documents;
    matched_documents.reserve(doc_res.size());
    for_each(execution::par, doc_res.begin(), doc_res.end(), [&](const std::pair<int, double>& pair) {
        matched_documents.emplace_back(pair.first, pair.second, documents_.at(pair.first).rating);
    });
    return matched_documents;
}

template <typename PolicyExec>
vector<Document> SearchServer::FindTopDocuments(PolicyExec policyExec, string_view raw_query) const {
    return FindTopDocuments(policyExec, raw_query, DocumentStatus::ACTUAL);
}

template<typename PolicyExec>
vector<Document>
SearchServer::FindTopDocuments(PolicyExec policyExec, string_view raw_query, DocumentStatus status) const {
    return FindTopDocuments(policyExec,
                            raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status,
                                                [[maybe_unused]] int rating) {
                return document_status == status;
            });
}

template <typename DocumentPredicate>
vector<Document> SearchServer::FindTopDocuments(string_view raw_query,
                                  DocumentPredicate document_predicate) const {
    return FindTopDocuments(execution::seq, raw_query, document_predicate);
}