#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>


using namespace std;
const int MAX_RESULT_DOCUMENT_COUNT = 5;
string ReadLine()
{
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber()
{
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string &text)
{
    vector<string> words;
    string word;
    for (const char c : text)
    {
        if (c == ' ')
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
        {
            word += c;
        }
    }
    if (!word.empty())
    {
        words.push_back(word);
    }

    return words;
}

set<string> ParseStopWords(const string &text)
{
    set<string> stop_words;
    for (const string &word : SplitIntoWords(text))
    {
        stop_words.insert(word);
    }
    return stop_words;
}
struct Document
{
    int id;
    double relevance;
};


vector<string> SplitIntoWordsNoStop(const string &text, const set<string> &stop_words)
{
    vector<string> words;
    for (const string &word : SplitIntoWords(text))
    {
        if (stop_words.count(word) == 0)
        {
            words.push_back(word);
        }
    }
    return words;
}


class SearchServer
{
public:
    void AddDocument(int document_id, const string &document)
    {
        const vector<string> words = SplitIntoWordsNoStop(document);
        double tf = 1.0/words.size();
        for(string word: words)
        {
            word_to_documents_freqs_[word][document_id] += tf;
        }
        ++document_count_;
    }

    void SetStopWords(string stop_words_joined)
    {
        for (string str : SplitIntoWordsNoStop(stop_words_joined))
        {
            stop_words_.insert((str));
        }
    }

    vector<Document> FindTopDocuments(const string &raw_query) const
    {
        Query query_words = ParseQuery(raw_query);
        vector<Document> matched_documents = FindAllDocuments(query_words);
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document &frst, const Document &scnd) -> bool
             {
                 return frst.relevance > scnd.relevance;
             });
        if ((int)matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT)
        {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    struct Query
    {
        set<string> plus_word;
        set<string> minus_word;
    };

    int document_count_ = 0;
    map<string, map<int, double>> word_to_documents_freqs_;
    set<string> stop_words_;
    int MAX_RESULT_DOCUMENT_COUNT = 5;

    vector<string> SplitIntoWordsNoStop(const string &text) const
    {
        vector<string> words;
        for (const string &word : SplitIntoWords(text))
        {
            if (stop_words_.count(word) == 0)
            {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ModyfyWord(set<string>& query_words) const
    {
        Query processed_query;
        for (string word : query_words)
        {
            if (word[0] == '-') {processed_query.minus_word.insert(word.substr(1));}
            else {processed_query.plus_word.insert(word);}
        }

        return processed_query;
    }

    Query ParseQuery(const string& text) const
    {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text))
        {
            query_words.insert(word);
        }

        return ModyfyWord(query_words);
    }

    double CalcIDF(const double& word_in_doc_count) const
    {
    return log(document_count_/word_in_doc_count);
    }

    vector<Document> FindAllDocuments(const Query& query_words) const
    {
        vector<Document> matched_documents;
        map<int, double> document_to_relevance;

        for(const string& plusw: query_words.plus_word)
        {
            if (word_to_documents_freqs_.count(plusw))
            {
                double idf = CalcIDF(static_cast<double>(word_to_documents_freqs_.at(plusw).size()));
                for (auto& [doc,freqs]: word_to_documents_freqs_.at(plusw))
                {
                    document_to_relevance[doc] += freqs * idf;
                }
            }
        }
        for(const string& minusw: query_words.minus_word){
            if(word_to_documents_freqs_.count(minusw))
            {
                for (auto& [doc,freqs]: word_to_documents_freqs_.at(minusw))
                {
                document_to_relevance.erase(doc);
                }
            }
        }
        for(auto& [d,r]: document_to_relevance){
            matched_documents.push_back({d,r});
        }

        return matched_documents;
    }
};

SearchServer CreateSearchServer()
{
    SearchServer myserver;
    const string stop_words_joined = ReadLine();
    myserver.SetStopWords(stop_words_joined);
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id)
    {
        myserver.AddDocument(document_id, ReadLine());
    }

    return myserver;
}

int main()
{

    const SearchServer serch_server = CreateSearchServer();

    const string query = ReadLine();

    for (auto [document_id, relevance] : serch_server.FindTopDocuments(query))
    {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s << endl;
    }
}
