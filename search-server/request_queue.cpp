#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) : server(search_server)  {
    
    }

    std::vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status) {
        auto result = server.FindTopDocuments(raw_query, status);
        AddRequest(result);
        return result;
    }
    std::vector<Document> RequestQueue::AddFindRequest(const string& raw_query) {
        auto result = server.FindTopDocuments(raw_query);
        AddRequest(result);
        return result;
    }
    int RequestQueue::GetNoResultRequests() const {
        return static_cast<int>(std::count_if(requests_.begin(), requests_.end(),[](const QueryResult& query) {
            return query.answers.empty();
        }));
    }

void RequestQueue::AddRequest(const vector<Document>& result) {
        if (requests_.empty()) {
            requests_.push_back({result, 1});
        }
        else if (static_cast<int>(requests_.size()) == min_in_day_) {
            requests_.pop_front();
            requests_.push_back({result, 1});
        }
        else {
            int number_request = requests_.back().number_request;
            requests_.push_back({result, ++number_request});
        }
    }