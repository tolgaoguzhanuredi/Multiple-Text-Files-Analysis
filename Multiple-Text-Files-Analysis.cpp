#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

class Frequent_Words
{
public:
    Frequent_Words();
    ~Frequent_Words();
    int findTop(string& word);
    void getStopWords();
    void insertWords(string someWord);
    string makeItSpace(string someWord);
    bool checkBodyStart(string someWord);
    bool checkBodyEnd(string someWord);
    void insert(const string& input);
    bool stopWordsChecker(string someWord);
    bool numChecker(string someWord);
    string switchCapitals(string someWord);
    int getCount() { return count; };
    void setCount(int newCount) { count = newCount; };
    int getMAX() { return MAX; };
    void setMAX(int newMAX) { MAX = newMAX; };
    bool activated = false;
private:
    int MAX = 1;
    string* words = new string[1];
    int* instances = new int[1];
    int count = 0;
    string stopwords[571];
    clock_t begin, end;
};

Frequent_Words::Frequent_Words()
{
    begin = clock();
    getStopWords();
}

Frequent_Words::~Frequent_Words()
{
    end = clock();
    double elapsed = (end - begin) / (double)CLOCKS_PER_SEC;
    cout << "Total Elapsed Time:" << elapsed << " seconds" << endl;
}

void Frequent_Words::insert(const string& input)
{
    for (int i = 0; i < getCount(); i++)
        if (input == words[i])
        {
            instances[i]++;
            return;
        }
    if (getCount() == getMAX())
    {
        string* temp = new string[getMAX() + 1];
        int* temp2 = new int[getMAX() + 1];
        for (int i = 0; i < getMAX(); i++)
        {
            temp[i] = words[i];
            temp2[i] = instances[i];
        }
        delete[] words;
        delete[] instances;
        int m = getMAX();
        setMAX(m + 1);
        words = temp;
        instances = temp2;
    }
    words[getCount()] = input;
    instances[getCount()] = 1;
    int c = getCount();
    setCount(c + 1);
}

int Frequent_Words::findTop(string& word)
{
    int topCount = instances[0];
    int topIndex = 0;
    for (int i = 0; i < getCount(); i++)
        if (instances[i] > topCount)
        {
            topCount = instances[i];
            topIndex = i;
        }
    instances[topIndex] = 0;
    word = words[topIndex];
    return topCount;
}

bool Frequent_Words::stopWordsChecker(string someWord)
{
    for (int i = 0; i < 570; i++)
    {
        if (someWord == stopwords[i]) return true;
    }
    return false;
}

bool Frequent_Words::numChecker(string someWord)
{
    if (someWord[0] >= '0' && someWord[0] <= '9') return true;
    else return false;
}

string Frequent_Words::switchCapitals(string someWord)
{
    for (int i = 0; i < someWord.size(); i++)
    {
        if (someWord[i] >= 'A' && someWord[i] <= 'Z') someWord[i] += 32;
    }
    return someWord;
}

void Frequent_Words::insertWords(string someWord)
{
    someWord = switchCapitals(someWord);
    if (stopWordsChecker(someWord) == false && numChecker(someWord) == false && someWord.size() > 1)
    {
        insert(someWord);
    }
}

string Frequent_Words::makeItSpace(string someWord)
{
    int pos, division_count = 0;
    for (int i = 0; i < someWord.length(); ++i)
    {
        if (someWord[i] == '-' || someWord[i] == '/' || someWord[i] == '.' || someWord[i] == ',' || someWord[i] == '!' || someWord[i] == '?')
        {
            division_count++;
        }
    }
    while (division_count > 0)
    {
        for (int i = 0; i < someWord.length(); ++i)
        {
            if (someWord[i] == '-' || someWord[i] == '/' || someWord[i] == '.' || someWord[i] == ',' || someWord[i] == '!' || someWord[i] == '?')
            {
                someWord[i] = ' ';
                pos = i;
                break;
            }
        }
        string newWord = someWord.substr(0, pos);
        someWord = someWord.substr(pos + 1);
        insertWords(newWord);
        division_count--;
    }
    return someWord;
}

bool Frequent_Words::checkBodyStart(string someWord)
{
    string body_start = "<BODY>";
    if (body_start.size() > someWord.size()) return false;
    for (int i = 0; i < someWord.size(); i++)
    {
        int j = 0;
        if (someWord[i] == body_start[j])
        {
            int k = i;
            while (someWord[i] == body_start[j] && j < body_start.size())
            {
                j++;
                i++;
            }
            if (j == body_start.size())
            {
                string newWord = someWord.substr(17);
                insertWords(newWord);
                return true;
            }
            else i = k;
        }
    }
    return false;
}

bool Frequent_Words::checkBodyEnd(string someWord)
{
    string body_end = "</BODY>";
    if (body_end.size() > someWord.size()) return false;
    for (int i = 0; i < someWord.size(); i++)
    {
        int j = 0;
        if (someWord[i] == body_end[j])
        {
            int k = i;
            while (someWord[i] == body_end[j] && j < body_end.size())
            {
                j++;
                i++;
            }
            if (j == body_end.size()) return true;
            else i = k;
        }
    }
    return false;
}

void Frequent_Words::getStopWords()
{
    ifstream file;
    file.open("stopwords.txt"); ////You should adress your stopwords location//
    for (int i = 0; i < 570; i++)
        file >> stopwords[i];
}

int main()
{
    Frequent_Words reuters;
    string word;
    fstream data;
    string file_name = "texts\\reut2-"; //You should adress your input location//
    string file_type = ".sgm"; //You should choose your folder type//
    for (int i = 0; i < 22; i++)
    {
        string i_string = to_string(i);
        string combined = file_name + i_string + file_type;
        data.open(combined); 
        while (data >> word)
        {
            if (reuters.checkBodyStart(word))
            {
                reuters.activated = true;
                continue;
            }
            if (reuters.checkBodyEnd(word)) reuters.activated = false;
            if (reuters.activated)
            {
                reuters.insertWords(reuters.makeItSpace(word));
            }
        }
        data.close();
    }
    for (int i = 0; i < 10; i++)
        cout << reuters.findTop(word) << " " << word << endl;
    return 0;
}