#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>      //Failure exits
#include <array>
#include <vector>
using namespace std;

static const int NUM_QUESTIONS = 20;

/*
 Reads NUM_QUESTIONS answers from the given file into `answers`.
 Exits program with error if the file can't be opened or doesn’t have enough valid answers.
 */
void getAnswers(const char* filename, array<char, NUM_QUESTIONS>& answers) 
{
    ifstream fin(filename);
    if (!fin.is_open()) 
    {
        cerr << "Error: could not open file '" << filename << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUM_QUESTIONS; ++i) 
    {
        char ch;
        if (!(fin >> ch) || (ch != 'A' && ch != 'B' && ch != 'C' && ch != 'D')) 
        {
            cerr << "Error: file '" << filename
                << "' is missing/invalid answer at line " << (i + 1) << endl;
            exit(EXIT_FAILURE);
        }
        answers[i] = ch;
    }
    fin.close();
}

/*
 Compares key[] vs student[], and records:
 - indices of missed questions in missedQs,
 - correct vs student answers per missed question in missedAnswerPairs.
 Returns count of incorrect answers.
 */
int gradeExam(const array<char, NUM_QUESTIONS>& key,
    const array<char, NUM_QUESTIONS>& student,
    vector<int>& missedQs,
    vector<array<char, 2>>& missedAnswerPairs)
{
    for (int i = 0; i < NUM_QUESTIONS; ++i) 
    {
        if (student[i] != key[i]) 
        {
            missedQs.push_back(i + 1);
            missedAnswerPairs.push_back({ key[i], student[i] });
        }
    }
    return static_cast<int>(missedQs.size());
}

/*
 Prints the exam report:
 - Number missed,
 - Table of missed question #, correct answer, student's answer,
 - Percentage score,
 - Pass/fail message (pass if >= 70%).
 */
void writeReport(const vector<int>& missedQs,
    const vector<array<char, 2>>& missedAnswerPairs,
    int numMissed)
{
    cout << fixed << setprecision(2);
    cout << "\nExam Report Detail\n";
    cout << "Number questions missed: " << numMissed << endl;

    if (numMissed > 0) {
        cout << "Missed questions and correct answers:\n";
        cout << "Question  Correct  Yours\n";
        for (size_t i = 0; i < missedQs.size(); ++i) {
            cout << setw(8) << missedQs[i] << setw(9) << missedAnswerPairs[i][0] << setw(9) << missedAnswerPairs[i][1] << endl;
        }
    }

    int numCorrect = NUM_QUESTIONS - numMissed;
    double pctCorrect = (static_cast<double>(numCorrect) / NUM_QUESTIONS) * 100.0;
    cout << "Test Score: " << pctCorrect << "%\n";

    if (pctCorrect >= 70.0) 
    {
        cout << "You passed the exam\n";
    }
    else 
    {
        cout << "You failed the exam\n";
    }
}

int main(int argc, char* argv[])
{
    const char* keyFile = "CorrectAnswers.txt";
    const char* studentFile = "StudentAnswers.txt";

    //For different files usage
    if (argc == 3) 
    {
        keyFile = argv[1];
        studentFile = argv[2];
    }

    array<char, NUM_QUESTIONS> keyAnswers;
    array<char, NUM_QUESTIONS> studentAnswers;

    getAnswers(keyFile, keyAnswers);
    getAnswers(studentFile, studentAnswers);

    vector<int> missedQuestions;
    vector<array<char, 2>> missedPairs;

    int numMissed = gradeExam(keyAnswers, studentAnswers, missedQuestions, missedPairs);
    writeReport(missedQuestions, missedPairs, numMissed);

    return EXIT_SUCCESS;
}
