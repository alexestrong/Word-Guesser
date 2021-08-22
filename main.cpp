/*
 * File:          proj1.cpp
 * Project:       CMSC 202 Project 1, Spring 2021
 * Author:        Alex Strong
 * Date:          2/25/21
 * Section:       52
 * E-mail:        astrong3@umbc.edu
 *
 * Description:   This program runs a simple word guessing game
 *                in which the user has to guess a random word
 *                pulled from a .txt file.  It is like hangman
 *                and after your 7th failed attempt at guessing
 *                a letter, you lose the game. Otherwise you win
 *                the game by guessing all the correct letters
 */

#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <fstream>
using namespace std;


const int MAX_NUMBER_OF_WORDS = 10000;  //Max words that can be imported from .txt file
const int MAX_GUESSES = 100;  //Max character guesses that can be stored


//NAME: findSize
//PRE-CON: Game starts and there is a text file of words
//POST-CON: Will return the size of words in the text file
int findSize();
//NAME: loadWords
//PRE-CON: Assumes that the the amount of words are correctly found in the text file.
//POST-CON: Will sift through words and return a random word chosen from the text file.
string loadWords(int totalNumberOfWords);
//NAME: displayPuzzle
//PRE-CON: Assumes that a random word is found.
//POST-CON: Will return how many letters are left blank from the puzzle.
int displayPuzzle(string randomWord, char guessedArray[]);
//NAME: guessLetter
//PRE-CON: Assumes that a user inputs a single lowercase character.
//POST-CON: Will return 1 if the guess was a fail. Will return 0 if the guess was correct.
int guessLetter(char guessedArray[], char badGuessArray[], string randomWord);
//NAME: gameReset
//PRE-CON: Assumes that the game was completed and the user would like to play the game again
//POST-CON: Will reset variables back to their original state in order to play the game again
int gameReset(char guessedArray[]);



int main() {
    string randomWord;
    char guessedArray[MAX_GUESSES] = {};    //Holds char array of ALL guesses
    char badGuessArray[MAX_GUESSES] = {};   //Holds char array of bad guesses
    int failedGuesses = 0;                  //Compares to placeholder to take note of when value increases
    int previousFailedGuesses = 0;          //Placeholder of previous "failedGuesses" value
    int totalNumberOfWords;
    int wordsRemaining = 1;                 //Spaces left open that have not been guessed yet "_"
    char gameContinue = 'y';

    cout << "Welcome to the UMBC Word Guess game." << endl;

    totalNumberOfWords = findSize();

    do{
        randomWord = loadWords(totalNumberOfWords);
        cout << "I am thinking of a word that is " << randomWord.size() << " letters long." << endl;


        //Main loop of game
        while (failedGuesses != 7)
        {
            wordsRemaining = displayPuzzle(randomWord, guessedArray);

            if (wordsRemaining != 0)
                previousFailedGuesses += guessLetter(guessedArray, badGuessArray, randomWord);

            if (failedGuesses != previousFailedGuesses)
            {
                cout << "You have " << previousFailedGuesses << "/7 failed guesses" << endl;
                failedGuesses = previousFailedGuesses;
            }

            if (wordsRemaining == 0)
                failedGuesses = 7;
        }


        if (wordsRemaining == 0)
            cout << "Congrats you won!" << endl;
        else
            cout << "Sorry, you lost :(" << endl << "The correct word was: " << randomWord << endl;

        cout << "Would you like to play again? (type y/n)." << endl;
        cin >> gameContinue;
        if (gameContinue == 'y' || gameContinue == 'Y')
        {
            failedGuesses = gameReset(guessedArray);
            previousFailedGuesses = 0;
        }

    }while(gameContinue == 'y' || gameContinue == 'Y');

    cout << "Thank you for playing the UMBC Word Guess game!" << endl;
    return 0;
}



//Will find the size of the word list
int findSize()
{
    int wordCounter = 0;      //Counts how many words are pulled from the .txt file
    string countingVariable;  //Placeholder variable for each word

    ifstream myWords ("words.txt");
    if(myWords.is_open())
    {
        while(myWords >> countingVariable)
            wordCounter++;

        cout << "Your file was loaded!" << endl;
        cout << "Loaded " << wordCounter << " words from file." << endl;
        myWords.close();
    }
    else
        cout << "Unable to open file." << endl;

    return wordCounter;
}


//Puts the words into an array and then randomly selects a word within the array
string loadWords(int totalNumberOfWords)
{
    int currentNumber = 0;                         //Placeholder to keep track of index of array
    int wordCounter = 0;                           //Keeps count of how many words are being imported
    string currentWord;                            //Placeholder for the current word during traversal of .txt file
    string wordArray[MAX_NUMBER_OF_WORDS] = {};    //String Array containing all of the words in the .txt file
    int randomNumber;                              //Random number between 0 & number of words pulled from .txt file

    srand(time(NULL));
    randomNumber = rand() % totalNumberOfWords;

    ifstream myWords ("words.txt");
    if(myWords.is_open())
    {
        while(myWords >> currentWord)
        {
            wordArray[currentNumber] = currentWord;
            wordCounter++;
            currentNumber++;
        }
        myWords.close();
    }else
        cout << "Unable to open file." << endl;

    return wordArray[randomNumber];
}


//Displays puzzle based on whether the letters have been properly guessed or still hidden
int displayPuzzle(string randomWord, char guessedArray[])
{
    int wordLength = 0;
    char currentLetter;
    int foundIndex = 0;         //Stores index of matching char.  Essentially used as a boolean
    char foundLetter;           //The matching letter that will be displayed as unhidden
    int remainingCounter = 0;   //Stores and returns value of remaining chars left to guess

    wordLength = randomWord.length();

    //Double "for" loop to compare each char of the randomWord to all chars in the guessArray
    for(int i=0; i < wordLength; i++)
    {
        for(int j=0; j < MAX_GUESSES; j++)
        {
            currentLetter = randomWord[i];
            if(currentLetter == guessedArray[j])
            {
                foundLetter = currentLetter;
                foundIndex = i+1;
            }
        }
        //If match found, it will print out that char as it's given letter
        if(foundIndex)
            cout << foundLetter;
        //If match not found, it will print out the hidden "_" character
        else
            {
            cout << "_";
            remainingCounter++;
            }

        foundIndex = 0;
    }

    cout << endl;
    if (remainingCounter != 0)
        cout << remainingCounter << " letters remain." << endl;

    return remainingCounter;
}


//Asks user to guess a character and function determines if guess is good, repeated, or incorrect.
int guessLetter(char guessedArray[], char badGuessArray[], string randomWord)
{
    char guessedLetter;       //The character that the user guesses
    int t = 0;                //Placeholder for a counter
    int openSpot = 0;         //Utilizes counting variable "t" to find the next open spot in the guessArray
    bool goodGuess = false;   //When true, the guess is valid. When false, you lose a guess
    bool sameGuess = false;   //Determines if the guess was already guessed. If true, you lose a guess

    //INPUT VALIDATION
    cout << "What letter would you like to guess?" << endl;
    cin >> guessedLetter;
    while(guessedLetter < 97 || guessedLetter > 122)
    {
        cout << "Please enter a lowercase character" << endl;
        cin.clear();
        cin.ignore(256, '\n');
        cin >> guessedLetter;
    }

    //Finding next open spot in array of guesses
    while(guessedArray[t])
        t++;
    openSpot = t;


    //Scans through characters in the random word to see if the guessed letter is in the word
    for(char l : randomWord)
    {
        if(l == guessedLetter)
            goodGuess = true;
    }
    //Scans through guess array to see if letter was already guessed
    //If already guessed then this is no longer a good guess and doesn't count
    for(int r = 0; r < MAX_GUESSES; r++)
    {
        if(guessedLetter == guessedArray[r])
        {
            sameGuess = true;
            goodGuess = false;
        }
    }

    //After determining good guess or not, the guess is now placed in the next open spot on the guess array
    guessedArray[openSpot] = guessedLetter;

    if(goodGuess)
        //Returns 0, as this number is being added (+=) onto failed guesses
        return 0;
    else
        {
        if(sameGuess)
            cout << "You ALREADY guessed this..." << endl;
        else
            cout << "'" << guessedLetter << "'" << " was not in the word!" << endl;
        //Returns 1 (counts as a failed guess), as this number is being added (+=) onto failed guesses
        return 1;
        }
}


//Resets guessedArray by setting all open slots back to null characters
int gameReset(char guessedArray[])
{
    for (int i = 0; i < MAX_GUESSES; i++)
        guessedArray[i] = '\0';
    //Returns 0 to set the failed guesses back to 0
    return 0;
}
