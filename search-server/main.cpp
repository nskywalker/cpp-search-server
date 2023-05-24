#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "search_server.h"

const double bias = 1e-6;

using namespace std;

/*
struct Document {
    int id;
    double relevance;
    int rating;
};
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, [](int document_id, DocumentStatus status, int rating) {return status == DocumentStatus::ACTUAL;});
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus document_status) const {
        return FindTopDocuments(raw_query, [document_status](int document_id, DocumentStatus status, int rating) {return status == document_status;});
    }

    template<class Comparator>
    vector<Document> FindTopDocuments(const string& raw_query, Comparator comparator) const  {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, comparator);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < bias) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });

        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
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
        return {matched_words, documents_.at(document_id).status};
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
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
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
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

    template<class Comparator>
    vector<Document> FindAllDocuments(const Query& query, Comparator comparator) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                if (comparator(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
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
}; */

/*
   void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename T>
void RunTestImpl(T t, const string& func) {
    if (!t) {
        cerr << boolalpha;
        cerr << func << t << " != " << true << endl;
        abort();
    }
    cerr << func << " OK" << endl;
}

#define RUN_TEST(func)  RunTestImpl(func, #func)
*/

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
}


//проверка Добавления документов
void TestAddDocument() {
    SearchServer server;
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
    const auto found_docs = server.FindTopDocuments("in"s);
    ASSERT(found_docs.size() == 1);
    const Document& doc0 = found_docs[0];
    ASSERT(doc0.id == doc_id);
}

//Проверка исключения минус-слов
void TestExcludeMinusWords() {
    SearchServer server;

    const int doc_id0 = 0;
    const string content0 = "cat in the city norwegian"s;
    const vector<int> ratings0 = {1, 2, 3};
    server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 1, 3};
    server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);


    const int doc_id2 = 2;
    const string content2 = "cat is for a walk"s;
    const vector<int> ratings2 = {1, 5, 3};
    server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);


    const auto found_docs = server.FindTopDocuments("cat -norwegian");
    ASSERT(found_docs.size() == 1);
}

//Проверка матчинга слова
void TestMatchSearchServer() {
    SearchServer server;

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 1, 3};
    server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);

    const auto result = server.MatchDocument("I missed the cat where that's my problem", doc_id1);
    const vector<string> answer {"cat", "the"};
    ASSERT(get<0>(result) == answer);
    ASSERT(get<1>(result) == DocumentStatus::ACTUAL);
}

//Вычисление рейтинга документа
void TestCalcRating() {
    SearchServer server;

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 2, 3};
    server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);

    const auto found_docs = server.FindTopDocuments("cat");
    const auto& doc = found_docs[0];
    ASSERT(doc.rating == 3.0);
}

//Проверка сортировки рейтинга по релевантности
void TestSortRelevance() {
    SearchServer server;

    const int doc_id0 = 0;
    const string content0 = "cat in the city norwegian"s;
    const vector<int> ratings0 = {1, 2, 3};
    server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 1, 7};
    server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);


    const int doc_id2 = 2;
    const string content2 = "cat is for a walk"s;
    const vector<int> ratings2 = {1, 5, 3};
    server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);


    const auto found_docs = server.FindTopDocuments("cat");
    ASSERT(found_docs.size() == 3);
    for (int i = 1; i < static_cast<int>(found_docs.size()); i++) {
        const auto& lhs = found_docs[i-1];
        const auto& rhs = found_docs[i];
        //PrintDocument(lhs);
        //PrintDocument(rhs);
        ASSERT(lhs.relevance > rhs.relevance or (abs(lhs.relevance - rhs.relevance) < bias and lhs.rating > rhs.rating));
    }
}

//Фильтрация результатов поиска с использованием предиката, задаваемого пользователем
void TestPredicateSearch() {
    SearchServer server;

    const int doc_id0 = 0;
    const string content0 = "cat in the city norwegian"s;
    const vector<int> ratings0 = {1, 2, 3};
    server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 1, 3};
    server.AddDocument(doc_id1, content1, DocumentStatus::REMOVED, ratings1);


    const int doc_id2 = 2;
    const string content2 = "cat is for a walk"s;
    const vector<int> ratings2 = {1, 5, 3};
    server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);

    {
        const auto found_docs = server.FindTopDocuments("cat", [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL;});
        ASSERT(found_docs.size() == 2);
    }

    {
        const auto found_docs = server.FindTopDocuments("cat", [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0;});
        for (const auto doc : found_docs) {
            ASSERT(doc.id % 2 == 0);
        }
    }

}

//Поиск документов, имеющих заданный статус
void TestStatusSearch() {
    SearchServer server;

    const int doc_id0 = 0;
    const string content0 = "cat in the city norwegian"s;
    const vector<int> ratings0 = {1, 2, 3};
    server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse inder the tree"s;
    const vector<int> ratings1 = {4, 1, 3};
    server.AddDocument(doc_id1, content1, DocumentStatus::REMOVED, ratings1);


    const int doc_id2 = 2;
    const string content2 = "cat is for a walk"s;
    const vector<int> ratings2 = {1, 5, 3};
    server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);


    const auto found_docs = server.FindTopDocuments("norwegian cat",
                                                    [](int document_id, DocumentStatus status, int rating) {
                                                        return status == DocumentStatus::ACTUAL;
                                                    });
    ASSERT_EQUAL(found_docs.size(), 2);


    const auto found_docs1 = server.FindTopDocuments("cat", [](int document_id, DocumentStatus status, int rating) {
        return status == DocumentStatus::REMOVED;
    });
    ASSERT_EQUAL(found_docs1.size(), 1);

}

//Корректное вычисление релевантности найденных документов
void TestCorrectCalcRelevance() {
    SearchServer search_server;
    search_server.SetStopWords("is the for a under in"s);

    const int doc_id0 = 0;
    const string content0 = "cat in the city norwegian"s;
    const vector<int> ratings0 = {1, 2, 3};
    search_server.AddDocument(doc_id0, content0, DocumentStatus::ACTUAL, ratings0);

    const int doc_id1 = 1;
    const string content1 = "norwegian cat eats the mouse under the tree"s;
    const vector<int> ratings1 = {4, 1, 3};
    search_server.AddDocument(doc_id1, content1, DocumentStatus::ACTUAL, ratings1);


    const int doc_id2 = 2;
    const string content2 = "cat is for a walk"s;
    const vector<int> ratings2 = {1, 5, 3};
    search_server.AddDocument(doc_id2, content2, DocumentStatus::ACTUAL, ratings2);

    const auto found_doc = search_server.FindTopDocuments("norwegian cat"s);

    ASSERT(found_doc[0].relevance == (1.0 / 3 * log(3.0 / 3) + 1.0 / 3 * log(3.0 / 2)));
    ASSERT(found_doc[1].relevance == (1.0 / 5 * log(3.0 / 2) + 1.0 / 5 * log(3.0 / 3)));
    ASSERT(found_doc[2].relevance == (1.0 / 2 * log(3.0 / 3)));
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    
    RUN_TEST(TestAddDocument);
    RUN_TEST(TestCalcRating);
    RUN_TEST(TestExcludeMinusWords);
    RUN_TEST(TestSortRelevance);
    RUN_TEST(TestMatchSearchServer);
    RUN_TEST(TestPredicateSearch);
    RUN_TEST(TestStatusSearch);
    RUN_TEST(TestCorrectCalcRelevance);
    
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}
