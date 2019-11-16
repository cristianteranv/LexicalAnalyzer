#include <iostream>
#include "lex.h"
#include <string>
#include <ctype.h>
using namespace std;


bool isKeyword(string word)
{
	if (!word.compare("print") || !word.compare("let") || !word.compare("if") || !word.compare("loop") || !word.compare("begin") || !word.compare("end"))
	{
		return true;
	}
	else
		return false;
}

bool isOperator(char c)
{
	if (c == '(' || c == '*' || c == '/' || c == '+' || c == '-' || c == ')' || c == ';' || c == '!')
		return true;
	else
		return false;
}

bool isDelimiter(char c)
{
	if (isOperator(c) || c == ',' || c == '>' || c == '<' || c == '=' || c == '[' || c == ']' || c == '{' || c == '}' || isspace(c))
		return true;
	else
		return false;
}


Lex getNextToken(istream& in, int& linenumber)
{
	enum lexstate {SBEGIN, READSTR, READINT, READIDENT, BLOCKCOMMENT, LINECOMMENT, READY};
	Token token = ERR;
	char ch;
	lexstate state = SBEGIN;
	string lexeme;
	
	while (in.get(ch))
	{
		switch (state)
		{
			case SBEGIN:
				if (in.eof())
				{
					Lex tkn(DONE, "", linenumber);
					return tkn;
				}
				else if (ch == '\n')
				{
					linenumber++;
					continue;
				}
				else if (isalpha(ch))
				{
					state = READIDENT;
					lexeme = ch;
				}
				else if (isdigit(ch))
				{
					state = READINT;
					lexeme = ch;
				}
				else if (isDelimiter(ch))
				{
					if (isOperator(ch))
					{
						lexeme = ch;
						if (ch == '(')
						{
							Lex tkn(LPAREN, lexeme, linenumber);
							return tkn;
						}
						else if (ch == ')')
						{
							Lex tkn(RPAREN, lexeme, linenumber);
							return tkn;
						}
						else if (ch == '*')
						{
							Lex tkn(STAR, lexeme, linenumber);
							return tkn;
						}
						else if (ch == '/')
						{
							if (in.peek() == '/')
							{
								state = LINECOMMENT;
								continue;
							}
							/*else if (in.peek() == '*')
							{
								state = BLOCKCOMMENT;
								continue;
							}*/
							else
							{
								Lex tkn(SLASH, lexeme, linenumber);
								return tkn;
							}
						}
						else if (ch == '+')
						{
							Lex tkn(PLUS, lexeme, linenumber);
							return tkn;
						}
						else if (ch == '-')
						{
							Lex tkn(MINUS, lexeme, linenumber);
							return tkn;
						}
						else if (ch == ';')
						{
							Lex tkn(SC, lexeme, linenumber);
							return tkn;
						}
						else if (ch == '!')
						{
							Lex tkn(BANG, lexeme, linenumber);
							return tkn;
						}
					}
					else if (isspace(ch))
					{
						continue;
					}
				}
				else if (ch == '"')
				{
					lexeme = "";
					state = READSTR;
				}
				else if (ch == '.')
				{
					Lex tkn(ERR, ".", linenumber);
					return tkn;
				}
				break;

			case READSTR:
				token = STR;
				if (ch == '"')
				{
					state = READY;
				}
				else if (ch == '\\')
				{
					if (in.peek() == 'n')
					{
						in.get(ch);
						lexeme += '\n';
						continue;
					}
					else if (in.peek() == 't')
					{
						in.get(ch);
						lexeme += '\t';
					}
					else
					{
						in.get(ch);
						lexeme += ch;
					}
				}
				else
				{
					lexeme += ch;
				}
				break;

			case READINT:
				token = INT;
				if (isalpha(ch))
				{
					lexeme += ch;
					Lex error(ERR, lexeme, linenumber);
					return error;
				}
				else if (isDelimiter(ch))
				{
					in.putback(ch);
					state = READY;
				}
				else if (ch == '.')
				{
					Lex tkn(ERR, ".", linenumber);
					return tkn;
				}
				else if (isdigit(ch))
				{
					lexeme += ch;
				}
				break;

			case READIDENT:
				token = ID;
				if (isDelimiter(ch) || ch == '.')
				{
					in.putback(ch);
					state = READY;
					continue;
				}
				lexeme += ch;
				break;

			case LINECOMMENT:
				if (ch == '\n')
				{
					state = SBEGIN;
					linenumber++;
				}
				break;

			case BLOCKCOMMENT:
				if (ch == '\n')
				{
					linenumber++;
				}
				else if (ch == '*')
				{
					if (in.peek() == '/')
					{
						in.get(ch);
						state = SBEGIN;
					}
					else
						continue;
				}
				break;

			case READY:
				in.putback(ch);
				if (isKeyword(lexeme))
				{
					if (!lexeme.compare("print"))
					{
						Lex tkn(PRINT, lexeme, linenumber);
						return tkn;
					}
					else if (!lexeme.compare("let"))
					{
						Lex tkn(LET, lexeme, linenumber);
						return tkn;
					}
					else if (!lexeme.compare("if"))
					{
						Lex tkn(IF, lexeme, linenumber);
						return tkn;
					}
					else if (!lexeme.compare("loop"))
					{
						Lex tkn(LOOP, lexeme, linenumber);
						return tkn;
					}
					else if (!lexeme.compare("begin"))
					{
						Lex tkn(BEGIN, lexeme, linenumber);
						return tkn;
					}
					else if (!lexeme.compare("end"))
					{
						Lex tkn(END, lexeme, linenumber);
						return tkn;
					}
				}
				else
				{
					Lex tkn(token, lexeme, linenumber);
					return tkn;
				}
		}
	}
	if (in.eof())
	{
		Lex tkn(DONE, "", linenumber);
		return tkn;
	}
	else
	{
		Lex tkn(ERR, "Weird i/o error.", linenumber);
		return tkn;
	}
}