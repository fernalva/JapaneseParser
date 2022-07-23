#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
using namespace std;

//tables by Fernando and Blake
enum states {q0, q0q1, q0qy, qy, qsa, qt, qs, qc}; //different possible states we have

//tokenTypes and tokenName are in same order to easily switch between the two using array indexes
//if this is changed program will probably break

enum tokenTypes {EOFM, ERROR, PERIOD, WORD1, WORD2, VERB, VERBNEG,
	VERBPAST, VERBPASTNEG, IS, WAS, SUBJECT, OBJECT, DESTINATION, PRONOUN, CONNECTOR};

string tokenName[20] = {"EOFM", "ERROR", "PERIOD", "WORD1", "WORD2", "VERB", "VERBNEG",
"VERBPAST", "VERBPASTNEG", "IS", "WAS", "SUBJECT", "OBJECT", "DESTINATION", "PRONOUN", "CONNECTOR"};

//all the reserved words so we dont need a seperate file for them
string reservedWords[20] = {"masu", "masen", "mashita", "masendeshita", "desu", "deshita", "o", "wa",
	"ni", "watashi", "anata", "kare", "kanojo", "sore", "mata", "soshite", "shikashi", "dakara", "eofm"};

//prototypes
bool isWord(string&);
bool isPeriod(string&);
tokenTypes scanner(string&);

/*
	Driver for the scanner, opens the file input by user then
	uses getline to scan through it, taking the line and
	tokenizing it using a stringstream, the tokens are then
	processed through the scanner and the type is printed
*/
//By Abdullah and Blake
int main()
{
	tokenTypes ourTokenType; //Used to print the token type from the tokenName array
	string currentWord;     //Current word we are applying the dfa to
	string fileName;        //read in from cin
	string line;            //The current line of the file we are reading
	ifstream fin;           //to read file

	cout << "Enter the input file name: ";  //prompt
	cin >> fileName;

	fin.open(fileName.c_str());

	//Couldnt open file
	if (!fin)
	{
		cout << "Could not open file: " << fileName;
		return -1;
	}

	while (currentWord != "eofm") //until eofm is read in
	{
		string sentence[10];    //tokenized version of line, put here to flush the array each loop

		getline(fin, line); //gets a line from a fin

		stringstream ssin(line); //turn the line into a string stream
		int i = 0;
		while (ssin.good() && i < 10)
		{ //tokenize the line from getline
			ssin >> sentence[i];
			++i;
		}
		for (int j = 0; j < i; j++) //goes for how many words we input into sentence[]
		{
			currentWord = sentence[j]; //get the word

			if (currentWord != "") //make sure its not empty
			{
				ourTokenType = scanner(currentWord);//call scanner to get what type of token it is

				if (ourTokenType == ERROR) //check for error
				{
					cout << "Lexical error: " << currentWord << " is not a valid token" << "\n\n";
				}
				else //Print word and its type
				{
					cout << currentWord << " is type " << tokenName[ourTokenType] << "\n\n";
				}

				//break out of for loop
				if (ourTokenType == EOFM)
				{
					break;
				}
			}

		}
	}

	//go until eofm
	cout << "End of file is encountered.";

	fin.close(); //close opened files
	return 0;
}

/*
	DFA for words, follows the DFA made byour group (with a couple of missing transfers)
	to properly assess if a word should be accepted or not

	returns true if it is a word accepted by the scanner
*/
//By Fernando and Blake
bool isWord(string& word)
{
	states currentState = q0; //starting state
	int charPos = 0; //keep track of where we are in the string

	while (word[charPos] != '\0')
	{

		//start with final states
		
		//transitions out of q0
		if (currentState == q0 && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}
		else if (currentState == q0 && (word[charPos] == 'b' || word[charPos] == 'g' || word[charPos] == 'h' ||
			word[charPos] == 'k' || word[charPos] == 'n' || word[charPos] == 'm' || word[charPos] == 'p' || word[charPos] == 'r'))
		{
			currentState = qy;
		}
		else if (currentState == q0 && (word[charPos] == 'd' || word[charPos] == 'w' || word[charPos] == 'z'
			|| word[charPos] == 'y' || word[charPos] == 'j'))
		{
			currentState = qsa;
		}
		else if (currentState == q0 && word[charPos] == 't')
		{
			currentState = qt;
		}

		else if (currentState == q0 && word[charPos] == 's')
		{
			currentState = qs;

		}
		else if (currentState == q0 && word[charPos] == 'c')
		{
			currentState = qc;
		}

		//transitions out of q0q1
		else if (currentState == q0q1 && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			//loop back into q0q1 so dont update state
		}
		else if (currentState == q0q1 && (word[charPos] == 'b' || word[charPos] == 'm' || word[charPos] == 'k' ||
			word[charPos] == 'h' || word[charPos] == 'p' || word[charPos] == 'r' || word[charPos] == 'g'))
		{
			currentState = qy;
		}
		else if (currentState == q0q1 && (word[charPos] == 'd' || word[charPos] == 'w' || word[charPos] == 'z' ||
			word[charPos] == 'y' || word[charPos] == 'j'))
		{
			currentState = qsa;
		}
		else if (currentState == q0q1 && word[charPos] == 't')
		{
			currentState = qt;
		}
		else if (currentState == q0q1 && word[charPos] == 'n')
		{
			currentState = q0qy;
		}
		else if (currentState == q0q1 && word[charPos] == 's')
		{
			currentState = qs;
		}
		else if (currentState == q0q1 && word[charPos] == 'c')
		{
			currentState = qc;
		}

		//transitions out of q0qy
		else if (currentState == q0qy && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}
		else if (currentState == q0qy && (word[charPos] == 'b' || word[charPos] == 'm' || word[charPos] == 'k' ||
			word[charPos] == 'n' || word[charPos] == 'h' || word[charPos] == 'p' || word[charPos] == 'r' || word[charPos] == 'g'))
		{
			currentState = qy;
		}
		else if (currentState == q0qy && (word[charPos] == 'd' || word[charPos] == 'w' || word[charPos] == 'z' ||
			word[charPos] == 'y' || word[charPos] == 'j'))
		{
			currentState = qsa;
		}
		else if (currentState == q0qy && word[charPos] == 't')
		{
			currentState = qt;
		}
		else if (currentState == q0qy && word[charPos] == 's')
		{
			currentState = qs;
		}
		else if (currentState == q0qy && word[charPos] == 'c')
		{
			currentState = qc;
		}
		//end of final states


		//scraps of whats left

		//stuff to get into q0q1
		else if (currentState == qy && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}
		else if (currentState == qsa && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}
		else if (currentState == qt && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}
		else if (currentState == qs && (word[charPos] == 'a' || word[charPos] == 'e' || word[charPos] == 'i' ||
			word[charPos] == 'o' || word[charPos] == 'u' || word[charPos] == 'I' || word[charPos] == 'E'))
		{
			currentState = q0q1;
		}

		//stuff to get into qsa
		else if (currentState == qy && word[charPos] == 'y')
		{
			currentState = qsa;
		}
		else if (currentState == qs && word[charPos] == 'h')
		{
			currentState = qsa;
		}
		else if (currentState == qt && word[charPos] == 's')
		{
			currentState = qsa;
		}
		else if (currentState == qc && word[charPos] == 'h')
		{
			currentState = qsa;
		}
		//if none of the above then theres an invalid char or state
		else
		{
			return false;
		}
		//continue in string
		charPos++;
	}

	//check if were in a final state
	if (currentState == q0 || currentState == q0q1 || currentState == q0qy)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//Returns if its a period or not
//By Fernando and Blake
bool isPeriod(string& word)
{
	return (word == ".");
}

/*
* Uses the DFAs to determine if we should accept a word and
* if we do, what type of word it is
*/
//By Abdullah and Blake
tokenTypes scanner(string& word)
{
	states currentState = q0;
	int tokenNumber = -1; //invalid state at the start

	if (word == "eofm") //if we reached the end, return it
	{
		return EOFM;
	}
	else if (isPeriod(word)) //get the easy case out of the way first
	{
		return PERIOD; //return its a period
	}
	else if (isWord(word)) //check if its a word
	{
		for (int i = 0; i < 16; i++)
		{
			if (word == reservedWords[i])
			{
				tokenNumber = i; //set tokenNumber to the index of reserved words
			}
		}

		if (tokenNumber == -1) //word wasnt in reservedWords so we return its a word
		{
			if (word.back() == 'I' || word.back() == 'E') //Check if the last char is an I or E
			{
				return WORD2;
			}
			else
			{
				return WORD1;
			}
		}
		else //its either a reserved word or an error at this point
		{
			switch (tokenNumber) //switch to the correct tokenType
			{
			case 0:
				return VERB;
				break;

			case 1:
				return VERBNEG;
				break;

			case 2:
				return VERBPAST;
				break;

			case 3:
				return VERBPASTNEG;
				break;

			case 4:
				return IS;
				break;

			case 5:
				return WAS;
				break;

			case 6:
				return OBJECT;
				break;

			case 7:
				return SUBJECT;
				break;

			case 8:
				return DESTINATION;
				break;

			case 9: case 10: case 11: case 12: case 13:
				return PRONOUN;
				break;

			case 14: case 15: case 16: case 17:
				return CONNECTOR;
				break;

			default:
				return ERROR; //if it was a word but none of the types we found an error
				break;
			}
		}
	}

	return ERROR; //couldn't go through isWord() so its of type error
}