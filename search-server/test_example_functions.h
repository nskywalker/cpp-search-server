#pragma once
#include "remove_duplicates.h"
#include "process_queries.h"

void AddDocument(SearchServer& searchServer, int document_id, const string& document, DocumentStatus status,
                 const vector<int>& ratings);

void print_indexes();

void remove_doc();

void getFreqWordsOnId();

void erase_duplicates();

void test_par_joined();