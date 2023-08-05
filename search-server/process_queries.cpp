#include "process_queries.h"


std::vector<std::vector<Document>>
ProcessQueries(const SearchServer &search_server, const vector<std::string> &queries) {
    std::vector<std::vector<Document>> result(queries.size());
    std::transform(execution::par, queries.begin(), queries.end(), result.begin(), [&search_server](const string& query) {
        return search_server.FindTopDocuments(query);
    });
    return result;
}

std::list<Document>
ProcessQueriesJoined(const SearchServer &search_server, const vector<std::string> &queries) {
    std::list<Document> result;
    for (const auto& tmp : ProcessQueries(search_server, queries)) {
        for (const auto& doc : tmp) {
            result.push_back(doc);
        }
    }
    return result;
}
