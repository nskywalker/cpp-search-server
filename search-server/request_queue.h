#pragma once
#include <vector>
#include <deque>
#include "search_server.h"

using namespace std;

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        auto result = server.FindTopDocuments(raw_query, document_predicate);
        AddRequest(result);
        return result;
    }
    std::vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        vector<Document> answers;
        int number_request = 0;
    };
    deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& server;
    void AddRequest(const vector<Document>& result);
};