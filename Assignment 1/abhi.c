#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

#define DEBUG 1     // 1 => Turn ON debugging, 0 => Turn OFF debugging

void print_debug(const char * format, ...)
{
    va_list args;
    if (DEBUG)
    {
        va_start (args, format);
        vfprintf (stdout, format, args);
        va_end (args);
    }
}

//----------------------------- token types ------------------------------
#define KEYWORDS 	12
#define RESERVED 	38
#define VAR 		1
#define BEGIN 		2
#define END 		3
#define ASSIGN 		4
#define IF 		5
#define WHILE 		6
#define DO 		7
#define THEN 		8
#define PRINT 		9
#define INT 		10
#define REAL 		11
#define STRING 		12
#define PLUS 		13
#define MINUS 		14
#define UNDERSCORE 	15
#define DIV 		16
#define MULT 		17
#define EQUAL 		18
#define COLON 		19
#define COMMA 		20
#define SEMICOLON 	21
#define LBRAC 		22
#define RBRAC 		23
#define LPAREN 		24
#define RPAREN 		25
#define NOTEQUAL 	26
#define GREATER 	27
#define LESS 		28
#define LTEQ 		29
#define GTEQ 		30
#define LSHIFT 		31
#define RSHIFT 		32
#define DOT 		33
#define NUM 		34
#define INTID 		35
#define REALID 		36
#define ID 		37
#define ERROR 		38

//------------------- reserved words and token strings -----------------------

char *reserved[] =
	{	"",
		"VAR",
		"BEGIN",
		"END",
		"ASSIGN",
		"IF",
		"WHILE",
		"DO",
		"THEN",
		"PRINT",
		"INT",
		"REAL",
		"STRING",
		"+",
		"-",
		"_",
		"/",
		"*",
		"=",
		":",
		",",
		";",
		"[",
		"]",
		"(",
		")",
		"<>",
		">",
		"<",
		"<=",
		">=",
		"<<",
		">>",
		".",
		"NUM",
		"INTID",
		"REALID",
		"ID",
		"ERROR"
		};

int printReserved(int ttype)
{
	if (ttype <= RESERVED)
	{
		printf("%s\n",reserved[ttype]);
		return 1;
	}
	else
		return 0;
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input token
// This implementation does not allow tokens that are more
// than 100 characters long
#define MAX_TOKEN_LENGTH 100

char real[]={'R','E','A','L'};
char integer[]={'I','N','T'};

char token[MAX_TOKEN_LENGTH];     // token string
int  tokenLength;
int  line_no = 0;

//---------------------------------------------------------
// Global Variables associated with the input buffer
// This implementation does not allow lines that are more
// than 500 characters long
#define BUFFER_SIZE     500

int buffer[BUFFER_SIZE];
int start_pos   = 0;	// Signifies the start position in the buffer (#)
int next_pos    = 0;	// Signifies the next character to be read (^)
int maximal_pos = 0;	// Signifies the last character of the maximal 
                        // token found (*)

//---------------------------------------------------------
// Functions associated with the input buffer

void readNextLine()
{
	int  i;
	char c;
	start_pos   = -1; // start position is the position just
                      // before the beginning of the token string
	maximal_pos = -1; // maximal_position points to the end of the
                      // maximal_matched token string
	next_pos    = 0;  // next_pos points to the next input character

	i = 0;
	c = getchar();
	while (c != '\n' && !feof(stdin) && (i < BUFFER_SIZE))
	{
		buffer[i] = c;
		c = getchar();
		i++;
	}
	if (i >= BUFFER_SIZE)
	{
		printf("Maximum allowed line length exceeded\n");
		exit(1);
	}
	else
		buffer[i] = EOF;
		line_no++;
}

/* you can use the following function for Debugging purposes */
void printBuffer()
{
	print_debug("BUFFER: ");
	int i = 0;
	while (buffer[i] != EOF)
	{
		print_debug("%c", (char) buffer[i]);
		i++;
	}
	print_debug("\n");
	print_debug("START_POS   = %d\n", start_pos);
	print_debug("NEXT_POS    = %d\n", next_pos);
	print_debug("MAXIMAL_POS = %d\n", maximal_pos);
}

void skip_space_and_set_positions()
{
	while (isspace((char) buffer[next_pos]))
	{
		next_pos++;
	}
	maximal_pos = next_pos - 1;
	start_pos   = next_pos - 1;
}

// this function copies the maximal token string to the token array
// it assumes the token string is not empty
void copy_token_string_from_buffer()
{
	int i;
	tokenLength = 0;
	for (i = start_pos + 1; i <= maximal_pos; i++)
	{
		token[tokenLength] = (char) buffer[i];
		tokenLength++;
	}
	token[tokenLength] = '\0';
}

//----------------------------------------------------------

// This function checks if a string is a keyword. 
// The implementation is not the most efficient, but probably 
// the simplest!
int isKeyword(char *s)
{
	int i;

	for (i = 1; i <= KEYWORDS; i++)
		if (strcmp(reserved[i],s) == 0)
	   		return i;
	return FALSE;
}


	 
	 
	int chk_further()
	 {
	int a1,a2=0;
	int ttype=0;
	int ext =0;	 
	int k =0;
    int a,i,r,m,y= 0;
	 char temp[100];

	while (isalnum(buffer[next_pos]))
	{
		next_pos++;
	}
	maximal_pos = next_pos - 1;  
	
	copy_token_string_from_buffer();

	ext=0;
	 if(buffer[next_pos]=='.')
	 {
	     k = next_pos+1;
		 
	   
	    if(isdigit(buffer[k]))
		{     
		  if (buffer[k] != '0')
		while (isdigit(buffer[k]))
			k++;
		else 
		k++;
		  
		  if(isalpha(buffer[k]))
		  {   i=0;
		      y=k;	
		    
		    while(isalpha(buffer[k]))
			{
			temp[i]=buffer[k];
			i++;
			k++;
			
   			}
	            		temp[i]='\0'; 
			r=0;             	
			for(m=0; m<4;m++ )
			{
			
			if(temp[m]==real[m])
			r++;
			
			}
			a=0;
			 for(m=0; m<3;m++ )
			{
			if(temp[m]==integer[m])
			a++;
			
			}
			if(r == 4)
			{
			  next_pos=y+4;
			  maximal_pos=next_pos-1;
			 ttype=REALID;
			  copy_token_string_from_buffer();
			  a1=1;
			}
		     
			if(a==3)
		     {
		      next_pos=y+3;
			  maximal_pos=next_pos-1;
			 ttype=INTID;
			  copy_token_string_from_buffer();a2=1;
			  }
			if(a1==1 || a2==1)			
				{ext=1;}}
			  }
	
	
	 }
	 
	 if(ext == 0) {
	 ttype = isKeyword(token);
	 
	 if(ttype == 0)
	 {ttype = ID;}
	 }
	
	
return ttype; 

 }
	 
	 

// this function is called only if the next input character is a digit
int scan_number()
{
	int ttype;

	ttype = NUM;
	if (buffer[next_pos] != '0')
		while (isdigit(buffer[next_pos]))
			next_pos++;
	else // NUM is 0
		next_pos++;

	maximal_pos = next_pos - 1;

	copy_token_string_from_buffer();
	return ttype;
}

int getToken()
{
	char c;
	int ttype;

	skip_space_and_set_positions();

	c = buffer[next_pos];
	next_pos++;

	switch (c)
	{
		case '.': ttype = DOT; break;
		case '+': ttype = PLUS; break;
		case '-': ttype = MINUS; break;
		case '_': ttype = UNDERSCORE; break;
		case '/': ttype = DIV; break;
		case '*': ttype = MULT; break;
		case '=': ttype = EQUAL; break;
		case ':': ttype = COLON; break;
		case ',': ttype = COMMA; break;
		case ';': ttype = SEMICOLON; break;
		case '[': ttype = LBRAC; break;
		case ']': ttype = RBRAC; break;
		case '(': ttype = LPAREN; break;
		case ')': ttype = RPAREN; break;
		case '<':
		        if(buffer[next_pos]=='<')
				{ttype = LSHIFT;
				next_pos++;
				break;}
				else if(buffer[next_pos]=='=')
				{ttype = LTEQ;
				next_pos++;
				break;}
				else if(buffer[next_pos]=='>')
				{ttype = NOTEQUAL;
				next_pos++;
				break;}
				else 
				ttype = LESS;
				break;
		case '>':	
				if(buffer[next_pos]=='=')
				{ttype = GTEQ;
				next_pos++;
				break;}
				else if(buffer[next_pos]=='>')
				{ttype = RSHIFT;
				next_pos++;
				break;}
				else
				ttype = GREATER ;
				break;
		default :
			if (isdigit(c))
			{
				next_pos--;
				ttype = scan_number();
			}
			else if (isalpha(c)) 
			{
				// token is either a keyword or ID or INTID or REALID
				next_pos--;
				ttype = chk_further();
				/* 
				 * TODO: Write code to handle INTID and REALID
				 * It might be better to replace scan_id_keyword() 
				 * with a function that handles INTID and REALID 
				 * in addition to ID and keywords
				 */
			}
			else if (c == EOF)
			  ttype = EOF;
			else
			  ttype = ERROR;
			break;
	}	// End Switch
	return ttype;
}

int main()
{
	int ttype;

	while (!feof(stdin))
	{
		readNextLine();
		// printBuffer();
		while ((ttype = getToken()) != EOF)
		{
			/*  EOF indicates that we have reached the end of buffer */
			// printBuffer();
			printf("%d %d ", line_no, ttype);
			if ((ttype == NUM) || (ttype == ID) || 
				(ttype == INTID) || (ttype == REALID))
			{
				printf("%s \n", token);
			}
			else
			{
				printReserved(ttype);
			}
		}
	}
	return 0;
}

