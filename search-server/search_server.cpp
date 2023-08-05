#include "search_server.h"

SearchServer::SearchServer(const string& stop_words_text)
        : SearchServer(
        SplitIntoWords(stop_words_text))  // Invoke delegating constructor from string container
{
}

void SearchServer::AddDocument(int document_id, const string& document, DocumentStatus status,
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
        document_to_word_freqs_[document_id][word] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
}

int SearchServer::GetDocumentId(int index) {
    if (count(indexes.begin(), indexes.end(), index) == 0) {
        throw out_of_range("Such index is out of range");
    }
    return indexes.at(index);
}

vector<Document> SearchServer::FindTopDocuments(const string& raw_query, DocumentStatus status) const {
    return FindTopDocuments(
            raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating) {
                return document_status == status;
            });
}

vector<Document> SearchServer::FindTopDocuments(const string& raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

tuple<vector<string>, DocumentStatus> SearchServer::MatchDocument(const string& raw_query, int document_id) const {
    return MatchDocument(execution::seq, raw_query, document_id);
}

tuple<vector<string>, DocumentStatus>
SearchServer::MatchDocument(const execution::sequenced_policy &seqOrParRem, const string &raw_query,
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
    return tuple<vector<string>, DocumentStatus>({matched_words, documents_.at(document_id).status});
}

tuple<vector<string>, DocumentStatus>
SearchServer::MatchDocument(const execution::parallel_policy &seqOrParRem, const string &raw_query,
                            int document_id) const {

}

bool SearchServer::IsValidStopWords() const {
    for (const auto& str : stop_words_) {
        if (!IsValidWord(str)) {
            return false;
        }
    }
    return true;
}

bool SearchServer::IsValidWord(const string& word) {
    return none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

bool SearchServer::IsStopWord(const string& word) const {
    return stop_words_.count(word) > 0;
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const vector<int>& ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(string text) const {
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

SearchServer::Query SearchServer::ParseQuery(const string& text) const {
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

SearchServer::Query SearchServer::ParseQuery(const execution::sequenced_policy& sequencedPolicy, const string& text) const {

}


double SearchServer::ComputeWordInverseDocumentFreq(const string& word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}

vector<int>::iterator SearchServer::begin() {
    return indexes.begin();
}


vector<int>::iterator SearchServer::end()  {
    return indexes.end();
}

const map<string, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static map<string, double> ret;
    if (!ret.empty()) {
        ret.clear();
    }

    for (const auto& [word, second_map] : word_to_document_freqs_) {
        if(second_map.find(document_id) != second_map.end()) {
            ret[word] = second_map.at(document_id);
        }
    }
    return ret;
}

void SearchServer::RemoveDocument(int document_id) {
    RemoveDocument(execution::seq, document_id);
}

void SearchServer::RemoveDocument(const execution::sequenced_policy &seqOrParRem, int document_id) {
    {
        auto it = documents_.find(document_id);
        if (it != documents_.end()) {
            documents_.erase(it);
        }
    }

    {
        auto it = find(indexes.begin(), indexes.end(), document_id);
        if (it != indexes.end()) {
            indexes.erase(it);
        }
    }

    {
        for (auto& [word, maps] : word_to_document_freqs_) {
            auto it = maps.find(document_id);
            if (it != maps.end()) {
                maps.erase(it);
            }
        }
    }

    {
        auto it = document_to_word_freqs_.find(document_id);
        if (it != document_to_word_freqs_.end()) {
            document_to_word_freqs_.erase(it);
        }
    }
}

void SearchServer::RemoveDocument(const execution::parallel_policy &seqOrParRem, int document_id) {
    {
        auto it = find( indexes.begin(), indexes.end(), document_id);
        if (it != indexes.end()) {
            indexes.erase(it);
        }
    }

    {
        auto it = find_if(seqOrParRem, documents_.begin(), documents_.end(), [document_id](const auto& doc){
            return doc.first == document_id;
        });
        if (it != documents_.end()) {
            documents_.erase(it);
        }
    }

    {
        if (document_to_word_freqs_.find(document_id) != document_to_word_freqs_.end()) {
            auto& mapa = document_to_word_freqs_.at(document_id);
            vector<const string*> strs(mapa.size(), nullptr);
            transform(seqOrParRem, mapa.begin(), mapa.end(), strs.begin(), [](const auto& docs) {
                return &docs.first;
            });
            for_each(seqOrParRem, strs.begin(), strs.end(), [this, document_id](const auto& str) {
                word_to_document_freqs_.at(*str).erase(document_id);
            });
            document_to_word_freqs_.erase(document_id);
        }
    }
}


