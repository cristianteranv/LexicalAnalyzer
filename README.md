# LexicalAnalyzer
Lexical analyzer that recognizes tokens for a made up programming language.
Reads from a file if provided as a command line argument or else from standard input. 
Recognizes	PRINT, LET, IF, LOOP, BEGIN, END, ID, INT, STR, PLUS, MINUS, STAR, SLASH, BANG, LPAREN, RPAREN, SC, ERR, DONE as tokens.
ERR is returned if a syntax error is recognized. DONE is returned if all of the input has been read with no errors.
If the command line argument -v is given, it will print the tokens and the line they are in immediately as they are recognized
If the command line argument -consts is given, the program will print a list of all the string and integer literals recognized.
If the command line argument -ids is given, the program will print a list of all the identifiers that were recognized.
