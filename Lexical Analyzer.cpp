#include "lex.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
using namespace std;

extern Lex getNextToken(istream& in, int& linenumber);

bool cmdValidate(int argc, char* argv[], bool& cmdV, bool& cmdConst, bool& cmdIds)
{
	if (argc > 1)
	{
		int i = 1;
		while (argv[i][0] == '-')
		{
			if (strcmp(argv[i], "-v") == 0)
			{
				cmdV = true;
			}
			else if (strcmp(argv[i], "-consts") == 0)
			{
				cmdConst = true;
			}
			else if (strcmp(argv[i], "-ids") == 0)
			{
				cmdIds = true;
			}
			else
			{
				cerr << "UNRECOGNIZED FLAG " << argv[i] << endl;
				return false;
			}
			if (i == argc - 1)
			{
				break;
			}
			i++;
		}
		if (i + 1 == argc) //checking the file is in fact the last argument (only file)
		{
			return true;
		}
		else //if two file names
		{
			cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
			return false;
		}
	}
	else
	{
		return true;
	}
}

int main(int argc, char* argv[])
{
	bool cmdV = false,
		cmdConst = false,
		cmdIds = false;
	string tokens[19] = { "PRINT", "LET", "IF", "LOOP", "BEGIN", "END", "ID", "INT", "STR", "PLUS", "MINUS", "STAR", "SLASH", "BANG", "LPAREN", "RPAREN","SC", "ERR", "DONE" };

	if (cmdValidate(argc, argv, cmdV, cmdConst, cmdIds) == 0)
	{
		return -1;
	}
	istream* inn = &cin;
	ifstream fin;
	if (argv[argc - 1][0] != '-')
	{
		fin.open(argv[argc - 1]);
		if (!fin.is_open())
		{
			cerr << "CANNOT OPEN " << argv[argc - 1] << endl;
			return -1;
		}
		else
		{
			inn = &fin;
		}
	}

	int linenum = 0;
	Lex t = getNextToken(*inn, linenum);
	Token tokk = t.GetToken();
	int count = 1;
	int idCount = 0;
	map<string, int> identifiers;
	map<string, int> strConsts;
	map<string, int> intConsts;
	while (tokk != ERR && tokk != DONE)
	{
		if (cmdV)
		{
			cout << tokens[tokk];
			if (tokk == ID || tokk == INT || tokk == STR || tokk == ERR)
			{
				cout << "(" << t.GetLexeme() << ")";
			}
			cout << endl;
		}
		if (cmdIds)
		{
			if (tokk == ID)
			{
				idCount++;
				identifiers[t.GetLexeme()] = linenum;
			}
		}
		if (cmdConst)
		{
			if (tokk == STR)
			{
				strConsts[t.GetLexeme()] = linenum;
			}
			if (tokk == INT)
			{
				intConsts[t.GetLexeme()] = linenum;
			}
		}
		t = getNextToken(*inn, linenum);
		tokk = t.GetToken();
		count++;
	}
	if (tokk == ERR)
	{
		cout << "Error on line " << linenum + 1 << " (" << t.GetLexeme() << ")" << endl;
		return -1;
	}
	if (tokk == DONE)
	{
		count--;
	}
	if (cmdConst)
	{
		map<string, int>::iterator it = strConsts.begin();
		if (it != strConsts.end())
		{
			cout << "STRINGS:" << endl;
			while (it != strConsts.end())
			{
				cout << it->first << endl;
				it++;
			}
		}
		map<string, int>::iterator itt = intConsts.begin();
		if (itt != intConsts.end())
		{
			cout << "INTEGERS:" << endl;
			while (itt != intConsts.end())
			{
				cout << itt->first << endl;
				itt++;
			}
		}
	}
	if (cmdIds)
	{
		if (idCount > 0)
		{
			cout << "IDENTIFIERS: ";
			map<string, int>::iterator it = identifiers.begin();
			int i = 0;
			while (i < identifiers.size() - 1)
			{
				cout << it->first << ", ";
				i++;
				it++;
			}
			cout << it->first;
			cout << endl;
		}
	}
	//summary
	cout << "Lines: " << linenum << endl;
	if (linenum != 0)
	{
		cout << "Tokens: " << count << endl;
	}


}