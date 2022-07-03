#include <iostream>
#include <fstream> //for reading a .txt file
#include <string>
#include <sstream> //for createWordPool function
#include <vector>  //for createWordPool function
#include <cmath> // for square root calculation
#include <chrono> // measuring execution time
#include <algorithm> // for sort() function
using namespace std;

string retrieveFileContent(string);
string renderDocument(string, vector<string>&);
vector<string> strToWord(string);
vector<string> createWordPool(string);
void getOccurrencesVector(int*, vector<string>, vector<string>);
float getCosineSimilarity(int*, int*, int);
float getDocSimilarity(string, vector<string>, string, vector<string>&);
void getSentenceSimilarity(vector<string>&, vector<string>&);

int main() {
    auto start_time = std::chrono::high_resolution_clock::now(); //start the timer

    //Add the name of your .txt files to the list below and run the program.
    vector <string> documentsContainer = { "main_document.txt", "document1.txt", "document2.txt" };
    
    //Prepeare main_document.txt for initial use.
    vector<string> sentenceContainer_main;
    string doc1_rendered = renderDocument(retrieveFileContent("main_document.txt"), sentenceContainer_main);
    vector<string> doc1_storeWords = strToWord(doc1_rendered);

    //similarity rate variable
    float similarity_rate;

    cout << "WELCOME! C++ PROGRAM FOR PLAGIARISM DETECTION AND SIMILARITY CALCULATION." << endl << "***\n";

    /*
    cout << "...listing all the sentences in main_document.txt:" << "\n";
    for (int j = 0; j < sentenceContainer_main.size(); j++) {
        cout << j+1 << ") " << sentenceContainer_main[j] << endl;
    }

    cout << "##########################################################################" << endl;
    */

    for (int i = 1; i < documentsContainer.size(); i++) {
        vector<string> sentenceContainer;

        similarity_rate = getDocSimilarity(doc1_rendered, doc1_storeWords, documentsContainer[i], sentenceContainer);

        /*
        cout << "...listing all the sentences of " << documentsContainer[i] << " \n";

        for (int m = 0; m < sentenceContainer.size(); m++) {
            cout << m + 1 << ") " << sentenceContainer[m] << endl;
        }
        */
        cout << "\n";

        cout << "Similarity rate between " << documentsContainer[0]
             << " and " << documentsContainer[i] << " is:"
             << endl << "=>" << similarity_rate << "\n";

        getSentenceSimilarity(sentenceContainer_main, sentenceContainer);
    }

    auto end_time = std::chrono::high_resolution_clock::now(); //end the timer
    auto time = end_time - start_time;
    cout << "It took " << time / std::chrono::milliseconds(1) 
         << "ms for this program to complete execution.\n";
    return 0;
}

string retrieveFileContent(string doc_name) {
    fstream docObj(doc_name, ios::in);
    string str = "";
    string line = "";

    while (getline(docObj, line)) {
        str += line;
    }

    return str;
}

string renderDocument(string str, vector<string> &sentenceContainer)
{
    string word = "";
    string document = "";
    string temp_sentence = "";

    for (auto x : str)
    {
        //Add to the list below the punctiation marks that you want the program to be sensitive to.
        if (x == ' ' || x == '.' || x == '?' || x == '!' || x == ',' || x == ':' || x == ';')
        {
            document += word;
            temp_sentence += word;
            //Add to the list below the punctiation marks that ends a sentence.
            if(x != '.' && x != '?' && x != '!') {  
            document += ' ';
                
                if(temp_sentence != "") {
                    if( x != ',' && x != ':' && x != ';') {
                        temp_sentence += ' ';
                    }
                }
            }
            else {
                sentenceContainer.push_back(temp_sentence);
                temp_sentence = "";
            }
            word = "";
        }
        else {
            word = word + x;
        }
    }

    return document;
}

vector<string> strToWord(string str) {
    istringstream iss(str);
    vector<string> words;

    do {
        string subs;
        iss >> subs;
        words.push_back(subs);
    } while (iss);

    return words;
}

vector<string> createWordPool(string str) {
    vector<string> words;
    vector<string> temp;

    words = strToWord(str);
    temp.push_back(words[0]);

    for (int i = 1; i < words.size()-1; i++) {
        for (int j = 0; j < temp.size(); j++) {
            if (words[i] == temp[j]) {
                break;
            }
            else if (j == (temp.size()-1)) {
                temp.push_back(words[i]);
            }
        }
    }

    return temp;
}

void getOccurrencesVector(int* arr, vector<string> document, vector<string> words_template) {
    for (int i = 0; i < words_template.size(); i++) {
        for (int j = 0; j < document.size()-1; j++) {
            if (words_template[i] == document[j]) {
                arr[i] += 1;
            }
        }
    }
}

float getCosineSimilarity(int* main_document, int* document_1, int ARRAY_SIZE) {
    float dividend = 0;
    int squared1 = 0, squared2 = 0;

    for (int i = 0; i < ARRAY_SIZE; i++) {
        dividend += (main_document[i] * document_1[i]);
        squared1 += (main_document[i] * main_document[i]);
        squared2 += (document_1[i] * document_1[i]);
    }

    return dividend/sqrt(squared1*squared2);
}

float getDocSimilarity(string doc1_rendered, vector <string> doc1_storeWords, string doc2, vector<string> &doc2_sentenceContainer) {
    string doc2_rendered = renderDocument(retrieveFileContent(doc2), doc2_sentenceContainer);

    vector<string> wordsTemplate = createWordPool(doc1_rendered + " " + doc2_rendered);

    //break a document down into its words
    vector<string> doc2_storeWords = strToWord(doc2_rendered);

    //create dynamic arrays to store vectors for both documents
    int ARRAY_SIZE = wordsTemplate.size();
    int* doc1_arrVec = new int[ARRAY_SIZE];
    int* doc2_arrVec = new int[ARRAY_SIZE];

    //initialize array elements
    for (int i = 0; i < ARRAY_SIZE; i++) {
        doc1_arrVec[i] = 0;
        doc2_arrVec[i] = 0;
    }

    getOccurrencesVector(doc1_arrVec, doc1_storeWords, wordsTemplate);
    getOccurrencesVector(doc2_arrVec, doc2_storeWords, wordsTemplate);

    return getCosineSimilarity(doc2_arrVec, doc1_arrVec, ARRAY_SIZE);
}

void getSentenceSimilarity(vector<string> &main_sentenceContainer, vector<string> &doc2_sentenceContainer) {
    vector<string> wordsTemplate;
    vector<string> a_nthSentenceStoreWords;
    vector<string> d_nthSentenceStoreWords;
    float similarity_Rate;
    
    //Create a struct to store the sentences sorted in descending order.
    struct Row {
        float result{};
        vector<int> values{};
        Row() {};

        bool operator<(const Row& other)
        {
            return result > other.result;
        }
    };

    Row my_row;
    vector<Row> row_Container;
   
    //cout << "...listing similarity rate between all the possible combinations of sentences:" << endl;

    for (int i = 0; i < doc2_sentenceContainer.size(); i++) {
        for (int j = 0; j < main_sentenceContainer.size(); j++) {
            wordsTemplate = createWordPool(doc2_sentenceContainer[i] + " " + main_sentenceContainer[j]);

            //Create two arrays into which we will store the vectors
            //for the calculation of cosine similarity.
            int ARRAY_SIZE = wordsTemplate.size();
            int* a_nthSentenceVecArr = new int[ARRAY_SIZE];
            int* d_nthSentenceVecArr = new int[ARRAY_SIZE];

            //initialize array elements
            for (int m = 0; m < ARRAY_SIZE; m++) {
                a_nthSentenceVecArr[m] = 0;
                d_nthSentenceVecArr[m] = 0;
            }

            a_nthSentenceStoreWords = strToWord(main_sentenceContainer[j]);
            d_nthSentenceStoreWords = strToWord(doc2_sentenceContainer[i]);

            getOccurrencesVector(a_nthSentenceVecArr, a_nthSentenceStoreWords, wordsTemplate);
            getOccurrencesVector(d_nthSentenceVecArr, d_nthSentenceStoreWords, wordsTemplate);

            similarity_Rate = getCosineSimilarity(a_nthSentenceVecArr, d_nthSentenceVecArr, ARRAY_SIZE);

            my_row.result = similarity_Rate;
            my_row.values.push_back(j);
            my_row.values.push_back(i);
            row_Container.push_back(my_row);
            my_row.values.clear();

            /*
            cout << j+1 << "-a and " << i+1 << "-d is: "
                 << similarity_Rate << endl;
            */
        }
    }

    sort(row_Container.begin(), row_Container.end());
    
    cout << endl << "...listing the top 5 most similar sentences:";
    for (int i = 0; i < row_Container.size(); i++) {
        if(i < 5) {
            cout << "\n" << i + 1 << ")\n" << "Similarity rate between these two sentences:" << endl;
            if (row_Container[i].result == 1) {
                cout << "=>Direct plagiarism detected. Two sentences are the same." << endl;
            }
            else {
                cout << "=>" << row_Container[i].result << endl;
            }
            cout << "1st sentence: " << main_sentenceContainer[row_Container[i].values[0]] << endl
                 << "2nd sentence: " << doc2_sentenceContainer[row_Container[i].values[1]] << endl;
        }
        else {
            break;
        }
    }
    cout << "##########################################################################" << endl;
}