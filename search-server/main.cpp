#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>
#include <numeric>
#include <deque>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
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
    Document() = default;
    Document(int id, double relevance, int rating)
            : id(id)
            , relevance(relevance)
            , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

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
    explicit SearchServer(const StringContainer& stop_words)
            : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
        if (!IsValidStopWords()) {
            throw invalid_argument("Denied characters in stop-words");
        }
    }

    explicit SearchServer(const string& stop_words_text)
            : SearchServer(
            SplitIntoWords(stop_words_text))  // Invoke delegating constructor from string container
    {
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        if (documents_.count(document_id) != 0 or document_id < 0) {
            throw invalid_argument("Denied document_id");
        }
        indexes.push_back(document_id);
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (const auto& str : words) {
            if (!IsValidWord(str)) {
                throw invalid_argument("Denied characters in input sentence");
            }
        }
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }

    int GetDocumentId(int index) {
        if (count(indexes.begin(), indexes.end(), index) == 0) {
            throw out_of_range("Such index is out of range");
        }
        return indexes.at(index);
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query,
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

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(
                raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating) {
                    return document_status == status;
                });
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return tuple<vector<string>, DocumentStatus>({matched_words, documents_.at(document_id).status});
    }

private:

    vector<int> indexes;

    bool IsValidStopWords() const {
        for (const auto& str : stop_words_) {
            if (!IsValidWord(str)) {
                return false;
            }
        }
        return true;
    }

    static bool IsValidWord(const string& word) {
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
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

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        if (!IsValidWord(text)) {
            throw invalid_argument("Denied characters in text");
        }
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            if (static_cast<int>(text.size()) == 1) {
                throw invalid_argument("there is minus without word");
            }
            else if (text[1] == '-') {
                throw invalid_argument("there is double minus");
            }
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
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
};
template <typename Iterator>
class IteratorRange {
    Iterator start;
    Iterator finish;
    int page_size;
public:
    explicit IteratorRange(Iterator begin_, Iterator end_, int size) : start(begin_), finish(end_), page_size(size) {}
    const Iterator begin() const {
        return start;
    }
    const Iterator end() const {
        return finish;
    }
    int size() const {
        return page_size;
    }
};

ostream &operator<<(ostream &os, const Document &document) {
    os << "{ "
       << "document_id = "s << document.id << ", "
       << "relevance = "s << document.relevance << ", "
       << "rating = "s << document.rating << " }";
    return os;
}

template<typename Iterator>
ostream &operator<<(ostream &os, const IteratorRange<Iterator> &result) {
    auto p = result.begin();
    for (int i = 0; i < result.size(); ++i) {
        os << *p;
        advance(p, 1);
    }
    return os;
}

template <typename Iterator>
class Paginator {
    vector<IteratorRange<Iterator>> result;
public:
    Paginator(Iterator begin_, Iterator end_, int page_size_) {
        int dist = distance(begin_, end_);
        int mode = dist / page_size_;
        for (int i = 0; i < dist; i += page_size_) {
            Iterator curEnd = begin_;
            if (i == mode * page_size_) {
                page_size_ = page_size_ - (dist % page_size_);
            }
            advance(curEnd, page_size_ - 1);
            result.push_back(IteratorRange<Iterator>{begin_, curEnd, page_size_});
            begin_ = curEnd;
            advance(begin_, 1);
        }

    }
    auto begin() const {
        return result.begin();
    }
    auto end() const {
        return result.end();
    }
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server) : server(search_server)  {
//        server = &search_server;
    }
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        auto result = server.FindTopDocuments(raw_query, document_predicate);
        AddRequest(result);
        return result;
    }
    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status) {
        auto result = server.FindTopDocuments(raw_query, status);
        AddRequest(result);
        return result;
    }
    vector<Document> AddFindRequest(const string& raw_query) {
        auto result = server.FindTopDocuments(raw_query);
        AddRequest(result);
        return result;
    }
    int GetNoResultRequests() const {
        return static_cast<int>(count_if(requests_.begin(), requests_.end(),[](const QueryResult& query) {
            return query.answers.empty();
        }));
    }
private:
    struct QueryResult {
        vector<Document> answers;
        int number_request = 0;
    };
    deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& server;
    void AddRequest(const vector<Document>& result) {
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
};

int main() {
    SearchServer search_server("and in at"s);
    RequestQueue request_queue(search_server);
    search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, {1, 1, 1});
    // 1439 запросов с нулевым результатом
    for (int i = 0; i < 1439; ++i) {
        request_queue.AddFindRequest("empty request"s);
    }
    // все еще 1439 запросов с нулевым результатом
    request_queue.AddFindRequest("curly dog"s);
    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    request_queue.AddFindRequest("big collar"s);
    // первый запрос удален, 1437 запросов с нулевым результатом
    request_queue.AddFindRequest("sparrow"s);
    cout << "Total empty requests: "s << request_queue.GetNoResultRequests() << endl;
    return 0;
}
