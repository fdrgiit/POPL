/*----------------------------------------------------------------------------
Note: the code in this file is not to be shared with anyone or posted online.
(c) Rida Bazzi, 2013
----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "syntax.h"

#define TRUE 1
#define FALSE 0

//----------------------------- token types ------------------------------
#define KEYWORDS 14
#define RESERVED 38
#define VAR 1
#define BEGIN 2
#define END 3
#define ASSIGN 4 
#define IF 5
#define WHILE 6 
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define BOOLEAN 13
#define TYPE 14
#define PLUS 15
#define MINUS 16
#define DIV 17
#define MULT 18
#define EQUAL 19
#define COLON 20 
#define COMMA 21
#define SEMICOLON 22
#define LBRAC 23
#define RBRAC 24
#define LPAREN 25
#define RPAREN 26
#define NOTEQUAL 27
#define GREATER 28
#define LESS 29
#define LTEQ 30
#define GTEQ 31
#define DOT 32
#define ID 33
#define NUM 10
#define REALNUM 11
#define ERROR 36
#define LBRACE 37
#define RBRACE 38
#define NOOP 39

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
		"print", 
		"INT",
		"REAL",
		"STRING",
		"BOOLEAN",
		"TYPE",
		"+",
		"-", 
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
		".",
		"ID",
		"NUM",
		"REALNUM",		
		"ERROR",
		"{",
		"}"
		};

int printToken(int ttype)
{
   if (ttype <= RESERVED)
   {   printf("%s\n",reserved[ttype]);
       return 1;
   } else
       return 0; 
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH];      // token string
int  ttype;                        // token type
int  activeToken = FALSE;                  
int  tokenLength;


int line_no = 1;

//----------------------------------------------------------
void skipSpace()
{
   char c;

   c = getchar(); 
   line_no += (c == '\n');
   while (!feof(stdin) && isspace(c))
   {    c = getchar(); 
        line_no += (c == '\n');
   }

   // return character to input buffer if eof is not reached
   if (!feof(stdin)) 
        ungetc(c,stdin);
}

int isKeyword(char *s)
{
     int i;

     for (i = 1; i <= KEYWORDS; i++)
	if (strcmp(reserved[i],s) == 0)
	   return i;
     return FALSE;
}

// ungetToken() simply sets a flag so that when getToken() is called
// the old ttype is returned and the old token is not overwritten 
// NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
// AT LEAST ONE CALL TO getToken()
// CALLING TWO ungetToken() WILL NOT UNGET TWO TOKENS  
void ungetToken()
{
    activeToken = TRUE;
}


int scan_number()
{
	char c;
	
	c = getchar();
	if (isdigit(c))
	{	// First collect leading digits before dot
		// 0 is a nNUM by itself
		if (c == '0')	      
		{	token[tokenLength] = c;
			tokenLength++;
			token[tokenLength] = '\0';
		} else
		{	while (isdigit(c))
			{	token[tokenLength] = c;
				tokenLength++;;
				c = getchar();
			}
			ungetc(c,stdin);
			token[tokenLength] = '\0';
		}

		// Check if leading digits are integer part of a REALNUM
		c = getchar();
		if (c == '.')
		{	c = getchar();
			if (isdigit(c))
			{	token[tokenLength] = '.';
				tokenLength++;
				while (isdigit(c))
				{	token[tokenLength] = c;
					tokenLength++;
					c = getchar();
				}
				token[tokenLength] = '\0';
				if (!feof(stdin)) 
					ungetc(c,stdin);
				return REALNUM;
			} else
			{	ungetc(c, stdin);    // note that ungetc returns characters on a stack, so we first
				c = '.';             // return the second character and set c to '.' and return c again
				ungetc(c,stdin);				                                 
				return  NUM;         
                        }
		} else
		{	ungetc(c, stdin);
			return NUM;
		}
	} else
		return ERROR;   
}


int scan_id_or_keyword()
{
	int ttype;
	char c;

	c = getchar();
	if (isalpha(c))
	{	while (isalnum(c))
		{	token[tokenLength] = c;
			tokenLength++;
			c = getchar();
		}
		if (!feof(stdin)) 
			ungetc(c,stdin); 
             
		token[tokenLength] = '\0';		                
		ttype = isKeyword(token); 
		if (ttype == 0) 
			ttype = ID;
		return ttype;
	} else
		return ERROR;
}                            
   

int getToken()
{	char c;
 
       if (activeToken)
       { activeToken = FALSE;
         return ttype;
       }   // we do not need an else because the function returns in the body 
           // of the if
   
       skipSpace();   
       tokenLength = 0;
       c = getchar();
       switch (c)
       {   case '.': return DOT;
           case '+': return PLUS;
           case '-': return MINUS;
           case '/': return DIV;
           case '*': return MULT;
           case '=': return EQUAL;
           case ':': return COLON;
           case ',': return COMMA;
           case ';': return SEMICOLON;
           case '[': return LBRAC;
           case ']': return RBRAC;
           case '(': return LPAREN;
           case ')': return RPAREN;
           case '{': return LBRACE;
           case '}': return RBRACE;
           case '<':
               c = getchar();
               if (c == '=')
                   return LTEQ;
               else if (c == '>')
                   return NOTEQUAL;
               else
               {
                   ungetc(c,stdin);
                   return LESS;
               }
           case '>': 
               c = getchar();
               if (c == '=')
                   return GTEQ;
               else
               {
                   ungetc(c, stdin);
                   return GREATER;
               }
           default :
               if (isdigit(c))
               {	ungetc(c,stdin);
               		return scan_number();
               }
               else if (isalpha(c))
               {	ungetc(c,stdin);
               		return scan_id_or_keyword();
               }
               else if (c == EOF)
               		return EOF;
               else
               		return ERROR;
	}
}



/*----------------------------------------------------------------------------
SYNTAX ANALYSIS SECTION
----------------------------------------------------------------------------*/
#define PRIMARY 0
#define EXPR 1

/*-------------------------------UTILITIE---------------------------*/
void syntax_error(char* NT, int line_no)
{
	printf("Syntax error while parsing %s line %d\n", NT, line_no);
}

/*--------------------------------------------------------------------
  PRINTING PARSE TREE
---------------------------------------------------------------------*/
void print_parse_tree(struct programNode* program)
{	
	print_decl(program->decl); 
	print_body(program->body);
}

void print_decl(struct declNode* dec)
{
	if (dec->type_decl_section != NULL)
	{	
		print_type_decl_section(dec->type_decl_section);
	}
	if (dec->var_decl_section != NULL)
	{	
		print_var_decl_section(dec->var_decl_section);
	}
}

void print_body(struct bodyNode* body)
{
	printf("{\n");
	print_stmt_list(body->stmt_list); 
	printf("}\n");
	
}

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection)
{
	printf("VAR\n");
	if (varDeclSection->var_decl_list != NULL)
		print_var_decl_list(varDeclSection->var_decl_list);
}

void print_var_decl_list(struct var_decl_listNode* varDeclList)
{
	print_var_decl(varDeclList->var_decl);
	if (varDeclList->var_decl_list != NULL)
		print_var_decl_list(varDeclList->var_decl_list);	
}

void print_var_decl(struct var_declNode* varDecl)
{	
	print_id_list(varDecl->id_list);
	printf(": ");
	print_type_name(varDecl->type_name);
	printf(";\n");
}

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection)
{
	printf("TYPE\n");
	if (typeDeclSection->type_decl_list != NULL)
		print_type_decl_list(typeDeclSection->type_decl_list);
}

void print_type_decl_list(struct type_decl_listNode* typeDeclList)
{
	print_type_decl(typeDeclList->type_decl);
	if (typeDeclList->type_decl_list != NULL)
		print_type_decl_list(typeDeclList->type_decl_list);	
}

void print_type_decl(struct type_declNode* typeDecl)
{	
	print_id_list(typeDecl->id_list);
	printf(": ");
	print_type_name(typeDecl->type_name);
	printf(";\n");
}

void print_type_name(struct type_nameNode* typeName)
{
	if (typeName->type != ID)
		printf("%s ", reserved[typeName->type]);
	else
		printf("%s ", typeName->id);
}

void print_id_list(struct id_listNode* idList)
{
	printf("%s ",idList->id);
	if (idList->id_list != NULL)
	{	printf(", ");
		print_id_list(idList->id_list);
	}
}

void print_stmt_list(struct stmt_listNode* stmt_list)
{
	print_stmt(stmt_list->stmt);
	if (stmt_list->stmt_list != NULL)
		print_stmt_list(stmt_list->stmt_list);

}

void print_assign_stmt(struct assign_stmtNode* assign_stmt)
{
	printf("%s ", assign_stmt->id);
	printf("= ");
	print_expression_prefix(assign_stmt->expr);
	printf("; \n");
}

void print_while_stmt(struct while_stmtNode* while_stmt) // While statement identification function.
{
	printf("WHILE ");
	struct conditionNode* conder;
	conder = while_stmt->condition;
	struct primaryNode* primary;
	primary=conder->left_operand;

if (primary->tag == ID)
              {	printf("%s ", primary->id);}
	else if (primary->tag == NUM)
		{printf("%d ", primary->ival);}
	else if (primary->tag == REALNUM)
		{printf("%.4f ", primary->fval);}


	if(conder->relop == 27)
		printf("!= ");
	else if(conder->relop == 28)
		printf("> ");
	else if(conder->relop == 29)
		printf("< ");
	else if(conder->relop == 30)
		printf("<= ");
	else if(conder->relop == 31)
		printf(">= ");
	
	if(conder->relop >= 27 && conder->relop <= 31)
	{
	primary=conder->right_operand;
	if (primary->tag == ID)
		printf("%s ", primary->id);
	else if (primary->tag == NUM)
		printf("%d ", primary->ival);
	else if (primary->tag == REALNUM)
		printf("%.4f ", primary->fval);
	}		

	printf("\n");
	print_body(while_stmt->body);
}

void print_stmt(struct stmtNode* stmt)
{
	if (stmt->stmtType == ASSIGN)
		print_assign_stmt(stmt->assign_stmt);
	
	if(stmt->stmtType == WHILE) //Checking other than assignment statement
		print_while_stmt(stmt->while_stmt);
}

void print_expression_prefix(struct exprNode* expr)
{
	if (expr->tag == EXPR)
	{
		printf("%s ", reserved[expr->operator]);

		print_expression_prefix(expr->leftOperand);
		print_expression_prefix(expr->rightOperand);
	} else
	if (expr->tag == PRIMARY)
	{
		if (expr->primary->tag == ID)
			printf("%s ", expr->primary->id);
		else
		if (expr->primary->tag == NUM)
			printf("%d ", expr->primary->ival);
		else
		if (expr->primary->tag == REALNUM)
			printf("%.4f ", expr->primary->fval);
	}
}


/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/
struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct declNode* make_declNode()
{
	return (struct declNode*) malloc(sizeof(struct declNode));
}

struct type_decl_sectionNode* make_type_decl_sectionNode()
{
	return (struct type_decl_sectionNode*) malloc(sizeof(struct type_decl_sectionNode));
}

struct var_decl_sectionNode* make_var_decl_sectionNode()
{
	return (struct var_decl_sectionNode*) malloc(sizeof(struct var_decl_sectionNode));
}

struct var_declNode* make_var_declNode()
{
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}

struct type_declNode* make_type_declNode()
{
	return (struct type_declNode*) malloc(sizeof(struct type_declNode));
}

struct type_decl_listNode* make_type_decl_listNode()
{
	return (struct type_decl_listNode*) malloc(sizeof(struct type_decl_listNode));
}

struct var_decl_listNode* make_var_decl_listNode()
{
	return (struct var_decl_listNode*) malloc(sizeof(struct var_decl_listNode));
}

struct id_listNode* make_id_listNode()
{
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}

struct type_nameNode* make_type_nameNode()
{
	return (struct type_nameNode*) malloc(sizeof(struct type_nameNode));
}

struct bodyNode* make_bodyNode()
{
	return (struct bodyNode*) malloc(sizeof(struct bodyNode));
}

struct stmt_listNode* make_stmt_listNode()
{
	return (struct stmt_listNode*) malloc(sizeof(struct stmt_listNode));
}

struct stmtNode* make_stmtNode()
{
	return (struct stmtNode*) malloc(sizeof(struct stmtNode));
}

struct while_stmtNode* make_while_stmtNode()
{
	return (struct while_stmtNode*) malloc(sizeof(struct while_stmtNode));
}

struct conditionNode* make_conditionNode() 
{
	return (struct conditionNode*) malloc(sizeof(struct conditionNode));
}

struct assign_stmtNode* make_assign_stmtNode()
{
	return (struct assign_stmtNode*) malloc(sizeof(struct assign_stmtNode));
}

struct exprNode* make_exprNode()
{
	return (struct exprNode*) malloc(sizeof(struct exprNode));
}

struct primaryNode* make_primaryNode()
{
	return (struct primaryNode*) malloc(sizeof(struct primaryNode));
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  PARSING AND BUILDING PARSE TREE
---------------------------------------------------------------------*/
struct primaryNode* primary()
{
	struct primaryNode* primar;
	
	ttype = getToken(); // Determining wether the type is ID, NUM or REALNUM
	if (ttype == ID)
	{   
		primar = make_primaryNode();
		primar->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(primar->id, token);
		primar->tag = ID;
	}
	else if (ttype == NUM)
		{
		primar = make_primaryNode();
		primar->ival = atoi(token);
		primar->tag = NUM;
		}
	else if (ttype == REALNUM)
		{
		primar = make_primaryNode();
		primar->fval = atof(token);
		primar->tag = REALNUM;
		}
	else
	{
		syntax_error("primary. INT, REAL, or ID expected", line_no);
		exit(0);
	}
}

struct conditionNode* condition()
{
	struct conditionNode* cond;
	ttype = getToken();

	if (ttype == ID || ttype == NUM || ttype == REALNUM) //Determining the type of operations applied on the 
	{							// determined type(NUM or ID)
		ungetToken();
		cond = make_conditionNode();
		cond->left_operand = primary();

		ttype = getToken();
		if(ttype == LBRACE && cond->left_operand->tag == ID)
		{
			if(cond->left_operand->tag == ID) //Checking for comparison order
			{
				ungetToken();
				cond->relop = 0;
				cond->right_operand = NULL;
			}
			else
			{
				printf("%s", fault[3]); // Type Mismatch between the inputs.
				exit(0);				
			}
		}
		else if(ttype >= 27 && ttype <= 32 ) //Comparsion between types defined
		{
			cond->relop = ttype;
			ttype = getToken();
			if (ttype == ID || ttype == NUM || ttype == REALNUM)
			{	
				ungetToken();
				cond->right_operand = primary();
			}
			else
			{
				syntax_error("condition. right_operand expected", line_no);
				exit(0);
			}
		}

		else
		{
			syntax_error("condition. relop or while_stmt. body. LBRACE expected", line_no);
			exit(0);		
		}
	}
	else
	{
		syntax_error("condition. ID or primary expected", line_no);
		exit(0);
	}	
	return cond; 
}

struct exprNode* factor()
{
	struct exprNode* facto;
	
	ttype = getToken();
	
	if (ttype == LPAREN)
	{	facto = expr();
		ttype = getToken();
		if (ttype == RPAREN)
			return facto;
		else 
		{	syntax_error("factor. RPAREN expected", line_no);
			exit(0);
		}
	} else
	if (ttype == NUM)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->operator = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = NUM;
		facto->primary->ival = atoi(token);
		return facto;
	} else
	if (ttype == REALNUM)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->operator = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = REALNUM;
		facto->primary->fval = atof(token);
		return facto;
	} else
	if (ttype == ID)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->operator = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = ID;
		facto->primary->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(facto->primary->id,token);
		return facto;
	} else
	{	syntax_error("factor. NUM, REALNUM, or ID, expected", line_no);
		exit(0);
	}
}

struct exprNode*term()
{
	struct exprNode* ter;
	struct exprNode* f;

	
	ttype = getToken();
	if ((ttype == ID)|(ttype == LPAREN)|(ttype == NUM)|(ttype == REALNUM))
	{	ungetToken();
		f = factor();
		ttype = getToken();
		if ((ttype == MULT) | (ttype == DIV))
		{	ter = make_exprNode(); 
			ter->operator = ttype; 
			ter->leftOperand = f;
			ter->rightOperand = term();
			ter->tag = EXPR;
			return ter;

		} else	
		if ((ttype == SEMICOLON)|(ttype == PLUS)|(ttype == MINUS)|(ttype == RPAREN))
		{	ungetToken();
			return f;
		} else
		{	syntax_error("term. MULT or DIV expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("term. ID, LPAREN, NUM, or REALNUM expected", line_no);
		exit(0);
	}
}


struct exprNode* expr()
{
	struct exprNode* exp;
	struct exprNode* t;
	
	ttype = getToken();
	if ((ttype == ID)|(ttype == LPAREN)|(ttype == NUM)|(ttype == REALNUM))
	{	ungetToken();
		t = term();
		ttype = getToken();
		if ((ttype == PLUS) | (ttype == MINUS))
		{	exp = make_exprNode();
			exp->operator = ttype; 
			exp->leftOperand = t;
			exp->rightOperand = expr();			
			exp->tag = EXPR;
			return exp;
		} else	
		if ((ttype == SEMICOLON)|(ttype == MULT)|(ttype==DIV)|(ttype == RPAREN))
		{	ungetToken();
			return t;
		} else
		{	syntax_error("expr. PLUS, MINUS, or SEMICOLON expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("expr. ID, LPAREN, NUM, or REALNUM expected", line_no);
		exit(0);
	}
}

struct assign_stmtNode* assign_stmt()
{	struct assign_stmtNode* assignStmt;
	
	int identype, current, i, pointfwd, j;

	ttype = getToken();
	if (ttype == ID) 
	{	assignStmt = make_assign_stmtNode();
		assignStmt->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(assignStmt->id,token);
		ttype = getToken();
		if (ttype == EQUAL)
		{
			assignStmt->expr = expr();
			
			identype = opSymbol(assignStmt->id);
			pointfwd = 9 + optypevar; //After Print 
			
			if(identype == 0) //No Identifiers present
			{
				optypevar++;
				pointfwd++;  
				incrTypeVar(pointfwd, assignStmt->id);
			}

			current = markexp(assignStmt->expr);

			if(identype == 0 && current != 0) //Expressions exists in the program
			{
				for (j = 0; j < 100 && varLogMatrix[j].argint > 0; j++)
					{
						if(varLogMatrix[j].sortCode == pointfwd)
						 {varLogMatrix[j].sortCode = current;}
					}
			}
			
			else if(identype == 0 && current == 0) // No ID or expression present in the program
			{
				for(i = 0; varLogMatrix[i].sortCode != 0; i++)
				{
					if(varLogMatrix[i].sortCode > pointfwd)
						varLogMatrix[i].sortCode = pointfwd;
				}
			}
			else if(identype <= 13 && current <= 13) 
			{
				if(identype != current)
				{
					printf("%s", fault[3]);
					exit(0);					
				}
			}
			else if(current == 0 && identype != 0) // ID is present
			{
				for(i = 0; varLogMatrix[i].sortCode != 0; i++)
				{
					if(varLogMatrix[i].sortCode > pointfwd)
						varLogMatrix[i].sortCode = identype;
				}
			}
			else if(identype > 13 && current > 13)
			{
				if(identype <= current)
					tUpdate(identype, current);
				else
					tUpdate(current, identype);
			}

			else if(identype > 13 && current <= 13)
			{
				tUpdate(current, identype);
			}
			else if(identype <= 13 && current > 13)
			{
				tUpdate(identype, current);
			}
			return assignStmt;
		} else
		{	syntax_error("assign_stmt. EQUAL expected", line_no);
			exit(0);
		}
	} else
	{
		syntax_error("assign_stmt. ID expected", line_no);
		exit(0);
	}
}

struct while_stmtNode* while_stmt()
{
	
	int unique, point, var =1, i;

	struct while_stmtNode* whileStmt;
	ttype = getToken();
	
	if (ttype == WHILE) // Determing While construct in the loop
	{	
		whileStmt = make_while_stmtNode(); //Parsing
		ttype = getToken();

		if (ttype == ID || ttype == NUM || ttype == REALNUM)
		{
			ungetToken();
			whileStmt->condition = condition(); 
			
			if(whileStmt->condition->relop == 0) // specified condition
			{
				unique = opSymbol(whileStmt->condition->left_operand->id);
				point = whileStmt->condition->left_operand->tag;
				
				if(point == ID) // Checking for ID
				{
					if(unique > BOOLEAN)
					{
						for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
						{
							if(varLogMatrix[i].sortCode == unique)
							 {varLogMatrix[i].sortCode = BOOLEAN;}
						}
					}
					else if(unique == BOOLEAN)
					{
						var--; //Checking for Boolean type under While Condition execution. 
					}
					else if(unique == 0) // Checking for ID type
					{
						incrTypeVar(BOOLEAN, whileStmt->condition->left_operand->id);
					}
					else
					{
						printf("%s", fault[3]); // Type mismatch in the while statement
						exit(0);						
					}
				}
				else
				{
					printf("%s", fault[3]); // Type mismatch in the while declaration
					exit(0);
				}
			}
			else
				conditionMat(whileStmt->condition->left_operand, whileStmt->condition->right_operand); // No condition

			ttype = getToken();
			if (ttype == LBRACE) //Checking for Braces in given condition
			{
				ungetToken();
				whileStmt->body = body();
			}
			else
			{
				syntax_error("while_stmt. body. LBRACE expected", line_no);
				exit(0);
			}
		}
		else
		{
			syntax_error("while_stmt. ID or primary expected", line_no); // No ID, NUM, etc in while
			exit(0);
		}
	} 
	else
	{
		syntax_error("while_stmt. WHILE expected", line_no);
		exit(0);
	}	
	return whileStmt;
	
}

struct stmtNode* stmt()
{
	struct stmtNode* stm;

	ttype = getToken();
	stm = make_stmtNode();

	if (ttype == ID) // assign_stmt
	{	ungetToken();
		stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		ttype = getToken();
		if (ttype == SEMICOLON)
		{	return stm;
		}
		else
		{	syntax_error("stmt. SEMICOLON expected", line_no);
			exit(0);
		}
	} else
	if (ttype == WHILE) // while_stmt
	{	ungetToken();
		stm->while_stmt = while_stmt();
		stm->stmtType = WHILE;
		return stm; 
	} else // syntax error
	{
		syntax_error("stmt. ID or WHILE expected", line_no);
		exit(0);
	}	
}

struct stmt_listNode* stmt_list()
{
	struct stmt_listNode* stmtList;

	ttype = getToken();

	if ((ttype == ID)|(ttype == WHILE))
	{	ungetToken();
		stmtList = make_stmt_listNode();
		stmtList->stmt = stmt();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			stmtList->stmt_list = stmt_list();
			return stmtList;
		} 
		else if(ttype == WHILE) //Checking for While Instance
		{
			ungetToken();
			stmtList->stmt_list = stmt_list();
			return stmtList;
		}
		else	// If the next token is not in FOLLOW(stmt_list), 
			// let the caller handle it. 
		{	ungetToken();
			return stmtList;
		}
	} else
	{
		syntax_error("stmt_list. ID or WHILE expected", line_no);
		exit(0);
	}
	
}

struct bodyNode* body()
{	struct bodyNode* bod;

	ttype = getToken();
	if (ttype == LBRACE)
	{	bod = make_bodyNode();
		bod->stmt_list = stmt_list();
		ttype = getToken();
		if (ttype == RBRACE)
			return bod;
		else
		{	syntax_error("body. RBRACE expected", line_no);
			exit(0); 
		}
	} else
	{	syntax_error("body. LBRACE expected", line_no);
		exit(0); 
	}
}

struct type_nameNode* type_name()
{
	int symbol = 0; 
	struct type_nameNode* tName;
	tName = make_type_nameNode();

	ttype = getToken();
	if ((ttype == ID)||(ttype == INT)||(ttype==REAL)||(ttype == STRING)||(ttype==BOOLEAN))
	{	tName->type = ttype;
		if (ttype == ID)
		{	tName->id = (char *) malloc(tokenLength+1);
			strcpy(tName->id,token);	
		}

		numvarT = 0;
		nameCheck = 1;
		symbol = reiterate(ttype, token);
		if(symbol == 3)
		{
			optypevar++;
			numvarT = 1;
			count1 = 0;
			moveStoreList(9 + optypevar);
			count2 = 0;
			count3 = 1;
			count1 = 1;
			incrMat(9 + optypevar, token);
		}
		else if(symbol > 9)
		{
			count1 = 0;
			moveStoreList(symbol);
		}
		else
		{
			printf("%s", fault[symbol]);
			exit(0);
		} 
		return tName;
	} else
	{	syntax_error("type_name. type name expected", line_no);
		exit(0);
	}
}

struct id_listNode* id_list()
{
	struct id_listNode* idList;
	idList = make_id_listNode();
	ttype = getToken();
	if (ttype == ID)
	{	
		idList->id = (char*) malloc(tokenLength+1);
		strcpy(idList->id, token);
		incrStoreMat(token);
		ttype = getToken();
		if (ttype == COMMA)
		{
			idList->id_list = id_list();
			return idList;
			
		} else
		if (ttype == COLON)
		{	ungetToken();
			idList->id_list = NULL;
			return idList;
		} else
		{	syntax_error("id_list. COMMA or COLON expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("id_list. ID expected", line_no);
		exit(0);
	}
}

struct type_declNode* type_decl()
{	
	struct type_declNode* typeDecl;
	typeDecl = make_type_declNode();
	ttype = getToken();
	int j;
	if (ttype == ID)
	{	
		ungetToken();
		count2 = 0;
		count3 = 1;
		count1 = 0;

		for (j = 0; j < 20; j++)
		{
			storeList[j].stored = 0;
		}	

		typeDecl->id_list = id_list();
		ttype = getToken();
		if (ttype == COLON)
		{	typeDecl->type_name = type_name();
			ttype = getToken();
			if (ttype == SEMICOLON)
			{	return typeDecl;
			}
			else
			{	syntax_error("type_decl. SEMICOLON expected", line_no);
				exit(0);
			}
		} 
	} else
	{
		syntax_error("type_decl. ID expected", line_no);
	}
}

struct var_declNode* var_decl()
{	
	int j;
	struct var_declNode* varDecl;
	varDecl = make_var_declNode();
	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		count2 = 1;
		count3 = 0;
		count1 = 0;
		for (j = 0; j < 20; j++)
		{
			storeList[j].stored = 0;
		}
		varDecl->id_list = id_list();
		ttype = getToken();
		if (ttype == COLON)
		{	varDecl->type_name = type_name();
			ttype = getToken();
			if (ttype == SEMICOLON)
			{	return varDecl;
			}
			else
			{	syntax_error("var_decl. SEMICOLON expected", line_no);
				exit(0);
			}
		} 
	}
}	

struct var_decl_listNode* var_decl_list()
{
	struct var_decl_listNode* varDeclList;
	varDeclList = make_var_decl_listNode();

	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		varDeclList->var_decl = var_decl();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			varDeclList->var_decl_list = var_decl_list();
			return varDeclList;
		}  else	
		{	ungetToken();
			return varDeclList;
		} 
	} else
	{	syntax_error("var_decl_list. ID expected", line_no);
		exit(0);
	} 
}

struct type_decl_listNode* type_decl_list()
{
	struct type_decl_listNode* typeDeclList;
	typeDeclList = make_type_decl_listNode();

	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		typeDeclList->type_decl = type_decl();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			typeDeclList->type_decl_list = type_decl_list();
			return typeDeclList;
		}  else	
		{	ungetToken();
			return typeDeclList;
		} 
	} else
	{	syntax_error("type_decl_list. ID expected", line_no);
		exit(0);
	} 
}

struct var_decl_sectionNode* var_decl_section()
{
	struct var_decl_sectionNode *varDeclSection;
	varDeclSection = make_var_decl_sectionNode();

	ttype = getToken();
	if (ttype == VAR)
	{	// no need to ungetToken() 
		varDeclSection->var_decl_list = var_decl_list();  
		return varDeclSection;
	} else
	{	syntax_error("var_decl_section. VAR expected", line_no);
		exit(0);
	}
}

struct type_decl_sectionNode* type_decl_section()
{
	struct type_decl_sectionNode *typeDeclSection;
	typeDeclSection = make_type_decl_sectionNode();

	ttype = getToken();
	if (ttype == TYPE)
	{
		typeDeclSection->type_decl_list = type_decl_list();
		return typeDeclSection;            
	} else
	{	syntax_error("type_decl_section. TYPE expected", line_no);
		exit(0);
	}
}

struct declNode* decl()
{	
	struct declNode* dec;
	dec = make_declNode();

	ttype = getToken();
	if (ttype == TYPE)
	{	ungetToken();
		dec->type_decl_section = type_decl_section();
		ttype = getToken();
		if (ttype == VAR)
    		{	// type_decl_list is epsilon
			// or type_decl already parsed and the 
			// next token is checked
			ungetToken();
       			dec->var_decl_section = var_decl_section();
    		} else
		{	ungetToken();
			dec->var_decl_section = NULL;
		}
		return dec;
	} else
	{
		dec->type_decl_section = NULL; 
	    	if (ttype == VAR)
    		{	// type_decl_list is epsilon
			// or type_decl already parsed and the 
			// next token is checked
			ungetToken(); 
       			dec->var_decl_section = var_decl_section();
			return dec;
    		} else
		{	if (ttype == LBRACE)
			{	ungetToken();	
				dec->var_decl_section = NULL;
				return dec;
			} else
			{	syntax_error("decl. LBRACE expected", line_no);
				exit(0);		// stop after first syntax error
			}
		}
	}
}

struct programNode* program()
{	struct programNode* prog;
	int i = 0;
	prog = make_programNode();
	ttype = getToken();
	if ((ttype == TYPE) || (ttype == VAR) || (ttype == LBRACE))
	{	ungetToken();  
		prog->decl = decl();
		count1 = 1;
		for(i=0; i < 20; i++)
		{
			storeList[i].id = NULL;
		}
		storeList = NULL;

		prog->body = body();
		return prog;
	} else
	{	syntax_error("program. TYPE or VAR or LBRACE expected", line_no);
		exit(0);		// stop after first syntax error
	}
}

void matrixComp()
{
	int i, j;	
	varLogMatrix = (struct varLogType*)malloc(sizeof(struct varLogType)*100);
	for(i=0; i < 100; i++)
	{
		varLogMatrix[i].flLimit = (struct varLog*)malloc(sizeof(struct varLog)*20);
		for (j = 0; j < 20; j++)
			varLogMatrix[i].flLimit[j].id = (char*)malloc(sizeof(char)*100);
		varLogMatrix[i].sortCode = 0;
		varLogMatrix[i].argint = 0;
	}

	storeList = (struct matrixStore*)malloc(sizeof(struct matrixStore)*20);
	for(i=0; i < 20; i++)
	{
		storeList[i].id = (char*)malloc(sizeof(char)*MAX_TOKEN_LENGTH);
		storeList[i].stored = 0;
	}	
}

void incrStoreMat(char* matrix)
{
	int i;
	for (i = 0; i < 20; i++)
	{
		if(storeList[i].stored == 0)
		{
			strcpy(storeList[i].id, matrix);
			storeList[i].stored = 1; 
			return;
		}
	}
	exit(0);
}

void incrMat(int type, char* typeName)
{
	int i, next_pos, max_pos;
	for(i = 0; i < 100; i++)
	{
		if(varLogMatrix[i].sortCode == type)
		{
			next_pos = i;
		}
	}
	max_pos= varLogMatrix[next_pos].argint;
	strcpy(varLogMatrix[next_pos].flLimit[max_pos].id, typeName);
	varLogMatrix[next_pos].argint++;
	varLogMatrix[next_pos].flLimit[max_pos].fl = '@';
	varLogMatrix[next_pos].flLimit[max_pos].implicit = count1;
	return;
}
int reiterate(int type, char* name) 
{  
	int i, j, var;
	for (i = 0; i < 100; i++)
	{
		if(type > 9 && type < 14 && nameCheck == 1)
		{
			return type;
		}

		for (j = 0; j < varLogMatrix[i].argint; j++)
		{
			if(strcmp(name, varLogMatrix[i].flLimit[j].id) == 0)
			{
				if(count3 == 1 && count2 == 0 && nameCheck == 0 && varLogMatrix[i].flLimit[j].fl == '@') 
				{
					var = 0;					
					return var;
				}
				else if(count2 == 1 && count3 == 0 && nameCheck == 0 && varLogMatrix[i].flLimit[j].fl == '@')
				{
					var = 1;					
					return var;
				}
				else if(count2 == 1 && count3 == 0 && nameCheck == 0 && varLogMatrix[i].flLimit[j].fl == '$') 
				{
					var = 2;					
					return var;
				}
				else if(nameCheck == 1 && varLogMatrix[i].flLimit[j].fl == '@') 
				{	
					return varLogMatrix[i].sortCode;
				}
				else
				{
					printf("%d, %s", type, name);
					printf("\ntflag=%d, vflag=%d, isTypename=%d, fl=%c", count3, count2 ,nameCheck , varLogMatrix[i].flLimit[j].fl);
				}
			}
		}
	}
	var = 3;	
	return var;
}


void moveStoreList(int type)
{
	int i, iterate, set_pos;
	nameCheck = 0; 
	for(i = 0; i < 100; i++)
	{
		if(varLogMatrix[i].argint == 0)
		{
			set_pos = i;
			break;
		}
	}
	for(i = 0; storeList[i].stored == 1; i++)
	{
		iterate = reiterate(type, storeList[i].id);
		if(iterate != 3)
		{
			printf("%s", fault[iterate]);
			exit(0);
		}
		else
		{
			strcpy(varLogMatrix[set_pos].flLimit[i].id, storeList[i].id);
			varLogMatrix[set_pos].flLimit[i].implicit = count1;
			if(count3 == 1 && count2 == 0)
			{
				varLogMatrix[set_pos].flLimit[i].fl = '@'; //Type
			}
			else if(count2 == 1 && count3 == 0)
			{
				varLogMatrix[set_pos].flLimit[i].fl = '$'; //Variable
			}
			varLogMatrix[set_pos].argint++;
		} 
		varLogMatrix[set_pos].sortCode = type;
	}
}
 
int opSymbol(char* name)
{
	int i, j;
	for (i = 0; i < 100; i++)
	{
		for(j = 0; j < 20; j++)
		{
			if(strcmp(varLogMatrix[i].flLimit[j].id, name) == 0 && varLogMatrix[i].flLimit[j].fl == '@')
			{
				printf("%s", fault[1]); // Redeclaration
			}
			else if(strcmp(varLogMatrix[i].flLimit[j].id, name) == 0 && varLogMatrix[i].flLimit[j].fl == '$') 
			{
				return varLogMatrix[i].sortCode;
			}
		}
	}
	return 0;  
}

void incrTypeVar(int type, char* name)
{
	int i, buffer = -1, spot;
	for(i = 0; i < 100; i++)
	{
		if(varLogMatrix[i].sortCode == type)
		{
			buffer = i;
		}
		if(varLogMatrix[i].argint == 0 && buffer == -1)
		{
			buffer = i;
		}
	}
	
	spot= varLogMatrix[buffer].argint;
	strcpy(varLogMatrix[buffer].flLimit[spot].id, name);
	varLogMatrix[buffer].argint++;
	varLogMatrix[buffer].flLimit[spot].fl = '$'; 

	if(varLogMatrix[buffer].sortCode == 0)
	{
		varLogMatrix[buffer].sortCode = type;
	}

	varLogMatrix[buffer].flLimit[spot].implicit = count1;
	return;
}

int markexp(struct exprNode* exp)
{
	int current, next, initial;
	if(exp->tag == PRIMARY)
	{
		if(exp->primary->tag != ID)
		{
			return exp->primary->tag;
		}
		else
		{
			current = opSymbol(exp->primary->id);
			if(current == 0)
			{
				optypevar++;
				incrTypeVar(9 + optypevar, exp->primary->id);
			}
			return current;
		}
	}
	else
	{
		next = markexp(exp->leftOperand);
		initial = markexp(exp->rightOperand);
		
		if(next == 0 && initial == 0)
		{
			return 0;
		}
		else if(next == 0 && initial != 0)
		{
			tUpdate(initial, 9 + optypevar);
			return initial;
		}
		else if(next < 14 && initial < 14)
		{
			if(next == initial)
				return next;
			else
			{
				printf("%s", fault[3]);
				exit(0);
			}
		}
		else if(next != 0 && initial == 0)
		{
			tUpdate(next, 9 + optypevar);
			return next;
		}
		else if(next < 14 && initial > 13)
		{
			tUpdate(next, initial);
			return next;
		}
		else if(next > 13 && initial > 13)
		{
			if(next <= initial)
			{
				tUpdate(next, initial);
				return next;
			}
			else
			{
				tUpdate(initial, next);
				return initial;
			}
		}
		else if(next > 13 && initial < 14)
		{
			tUpdate(initial, next);
			return initial;
		}
	}
}

void tUpdate(int type1, int type2)
{
	int i;
	for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
	{
		if(varLogMatrix[i].sortCode == type2)
			 varLogMatrix[i].sortCode = type1;
	}
	return;
}

void conditionMat(struct primaryNode* priNode1, struct primaryNode* priNode2) 
{
	int movelt, movert, joinlt, joinrt, i;
	movelt = opSymbol(priNode1->id);
	movert = opSymbol(priNode2->id);
	joinlt = priNode1->tag;
	joinrt = priNode2->tag;

	if(joinlt == joinrt && joinlt != ID)
	{
		if(movelt == 0)
		{
			incrTypeVar(joinlt, priNode1->id);
		}
		
		if(movert == 0)
		{
			incrTypeVar(joinrt, priNode2->id);
		}				
	}
	else if(joinlt != joinrt && joinlt != ID && joinrt != ID)
	{
		printf("%s", fault[3]);
		exit(0);
	}
	else if(joinlt != ID && joinrt == ID)
	{
		if(movert == 0 || movert >= 14 || movert == joinlt)
		{
			incrTypeVar(joinlt, priNode2->id);
			
			if (movert >= 14)
			{
				for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
				{
					if(varLogMatrix[i].sortCode == movert)
					 varLogMatrix[i].sortCode = joinlt;
				}	
			}
		}
		else
		{
			printf("%s", fault[3]);
			exit(0);
		}					
	}
	else if(joinrt != ID && joinlt == ID)
	{
		if(movelt == 0 || movelt >= 14 || movelt == joinrt)
		{
			incrTypeVar(joinrt, priNode1->id);
			
			if (movelt >= 14)
			{	
				for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
				{
					if(varLogMatrix[i].sortCode == movelt)
					 {varLogMatrix[i].sortCode = joinrt;}
				}			
			}
		}
		else
		{
			printf("%s", fault[3]);
			exit(0);
		}
	}
	else if(joinlt == joinrt && joinlt == ID)
	{
		if(movelt == movert && movelt == 0)
		{
			optypevar++;
			incrTypeVar(9 + optypevar, priNode1->id);
			incrTypeVar(9 + optypevar, priNode2->id);
		}
		else if(movelt == 0 && movert != 0)
		{
			incrTypeVar(movert, priNode1->id);
		}
		else if(movert == 0 && movelt != 0)
		{
			incrTypeVar(movelt, priNode2->id);
		}
		else if(movelt < 14 && movert < 14) //Check till type boolean
		{
			if(movelt != movert)
			{
				printf("%s", fault[3]);
				exit(0);
			}
		}
		else if(movelt <= 13 && movert >= 14) 
		{
			for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
			{
				if(varLogMatrix[i].sortCode == movert)
				{ 
					varLogMatrix[i].sortCode = movelt;
				}
			}
		}
		else if(movelt >= 14 && movert < 14) 
		{
			for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
			{
				if(varLogMatrix[i].sortCode == movelt)
				{ 
					varLogMatrix[i].sortCode = movert;
				}
			}
		}
		else if(movelt >= 14 && movert >= 14)
		{
			if(movelt <= movert)
			{
				for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
				{
					if(varLogMatrix[i].sortCode == movert)
					{ 
					varLogMatrix[i].sortCode = movelt;
					}
				}
			}
			
			else
			{
				for (i = 0; i < 100 && varLogMatrix[i].argint > 0; i++)
				{
					if(varLogMatrix[i].sortCode == movelt)
					{ 
						varLogMatrix[i].sortCode = movert;
					}
				}
			}
		}
	}
}


void parseTyping()
{
	int i, j, k, l, m, store, currentid = 0, pointfwd, pointbkd, pointlt;
	int* pointrt;

	for(i = 10; i < 100; i++)
	{
		for(j = 0; varLogMatrix[j].sortCode != 0; j++)
		{
			if(varLogMatrix[j].sortCode == i)
			{
				checker++;
				break;
			}
		}
	}
	pointrt = (int*)malloc(sizeof(int)*checker);
	
	for(i = 0, k = 0; varLogMatrix[i].sortCode != 0; i++)
	{
		currentid = 0;
		for(j = 0; j < i; j++)
		{
			if(varLogMatrix[i].sortCode == varLogMatrix[j].sortCode)
			{
				currentid = 1;
				break;
			}
		}
		if(currentid == 0)
		{
			pointrt[k] = varLogMatrix[i].sortCode;
			k++;
		}
	}
	
	for(l=0; l<checker; l++)
	{
		for(m=l+1; m<checker; m++)
		{
			if(pointrt[l] > pointrt[m])
			{
				store = pointrt[l];
				pointrt[l] = pointrt[m];
				pointrt[m] = store;
			}
		}
	}
	
	declMat =(struct varLogType*)malloc(sizeof(struct varLogType)*checker);
	
	for(i = 0; i < checker; i++)
	{
		declMat[i].flLimit = (struct varLog*)malloc(sizeof(struct varLog)*20);
 		
		for (j = 0; j < 20; j++)
		{
			declMat[i].flLimit[j].id = (char*)malloc(sizeof(char)*MAX_TOKEN_LENGTH);
		}
		declMat[i].sortCode = pointrt[i];
		declMat[i].argint = 0;
	}
	
	for(i = 0; varLogMatrix[i].sortCode != 0; i++)
	{
		pointfwd = varLogMatrix[i].sortCode;
		pointbkd = -1;
	
		for(j = 0; j < checker; j++)
		{
			if(declMat[j].sortCode == pointfwd)
			{
				pointbkd = j;
				break;
			}
		}
		pointlt = declMat[pointbkd].argint;

		for(j = 0; j < varLogMatrix[i].argint; j++)
		{	
			if(varLogMatrix[i].flLimit[j].fl == '@' && varLogMatrix[i].flLimit[j].implicit == 0) 
			{				
				strcpy(declMat[pointbkd].flLimit[pointlt].id, varLogMatrix[i].flLimit[j].id); 				
				declMat[pointbkd].flLimit[pointlt].implicit = varLogMatrix[i].flLimit[j].implicit;
				declMat[pointbkd].flLimit[pointlt].fl = varLogMatrix[i].flLimit[j].fl;
				pointlt++;
			}
		}
		declMat[pointbkd].argint =  pointlt;
	}

	for(i = 0; varLogMatrix[i].sortCode != 0; i++)
	{
		pointfwd = varLogMatrix[i].sortCode;
		pointbkd = -1;
		for(j = 0; j < checker; j++)
		{
			if(declMat[j].sortCode == pointfwd)
			{
				pointbkd = j;
				break;
			}
		}
		
		pointlt = declMat[pointbkd].argint;

		for(j = 0; j < varLogMatrix[i].argint; j++)
		{	
			if(varLogMatrix[i].flLimit[j].fl == '@' && varLogMatrix[i].flLimit[j].implicit == 1)
			{				
				strcpy(declMat[pointbkd].flLimit[pointlt].id, varLogMatrix[i].flLimit[j].id); 				
				declMat[pointbkd].flLimit[pointlt].implicit = varLogMatrix[i].flLimit[j].implicit;
				declMat[pointbkd].flLimit[pointlt].fl = varLogMatrix[i].flLimit[j].fl;
				pointlt++;
			}
		}

		declMat[pointbkd].argint =  pointlt;
	}

	for(i = 0; varLogMatrix[i].sortCode != 0; i++)
	{
		pointfwd = varLogMatrix[i].sortCode;
		pointbkd = -1;
		for(j = 0; j < checker; j++)
		{
			if(declMat[j].sortCode == pointfwd)
			{
				pointbkd = j;
				break;
			}
		}
		pointlt = declMat[pointbkd].argint;
		
		for(j = 0; j < varLogMatrix[i].argint; j++)
		{	
			if(varLogMatrix[i].flLimit[j].fl == '$' && varLogMatrix[i].flLimit[j].implicit == 0) 
			{				
				strcpy(declMat[pointbkd].flLimit[pointlt].id, varLogMatrix[i].flLimit[j].id); 				
				declMat[pointbkd].flLimit[pointlt].implicit = varLogMatrix[i].flLimit[j].implicit;
				declMat[pointbkd].flLimit[pointlt].fl = varLogMatrix[i].flLimit[j].fl;
				pointlt++;
			}
		}
		declMat[pointbkd].argint =  pointlt;
	}

	for(i = 0; varLogMatrix[i].sortCode != 0; i++)
	{
		pointfwd = varLogMatrix[i].sortCode;
		pointbkd = -1;
		
		for(j = 0; j < checker; j++)
		{
			if(declMat[j].sortCode == pointfwd)
			{
				pointbkd = j;
				break;
			}
		}
		
		pointlt = declMat[pointbkd].argint;
		
		for(j = 0; j < varLogMatrix[i].argint; j++)
		{	
			if(varLogMatrix[i].flLimit[j].fl == '$' && varLogMatrix[i].flLimit[j].implicit == 1) 
			{				
				strcpy(declMat[pointbkd].flLimit[pointlt].id, varLogMatrix[i].flLimit[j].id); 				
				declMat[pointbkd].flLimit[pointlt].implicit = varLogMatrix[i].flLimit[j].implicit;
				declMat[pointbkd].flLimit[pointlt].fl = varLogMatrix[i].flLimit[j].fl;
				pointlt++;
			}
		}
		declMat[pointbkd].argint =  pointlt;
	}
	
}

void print()
{
	int i, j, pointfwd, pointlt;
	char* typearray[4] = {"INT", "REAL", "STRING", "BOOLEAN"};

	for (i = 0; i < checker; i++)
	{
		pointfwd = declMat[i].sortCode;
		pointlt = declMat[i].argint;

		if(pointfwd > 9 && pointfwd < 14) //Determining the type(INT, REAL, etc.)
		{
			printf("%s : ", typearray[pointfwd - 10]); 
			for(j = 0; j < pointlt; j++)
			{
				printf("%s ", declMat[i].flLimit[j].id); //Printing the variable associated
			}
			printf("#\n"); // Endpoint hash
		}
		else
		{
			printf("%s : ",  declMat[i].flLimit[0].id); //Other types
			for(j = 1; j < pointlt; j++)
			{
				printf("%s ", declMat[i].flLimit[j].id); //Printing the variable associated
			}
			printf("#\n"); // Endpoint hash
		}		
	}
}

//--------------------------------------------------------------


// COMMON mistakes:
//    *     = instead of ==
//    *     not allocating space before strcpy
int main()
{
	struct programNode* parseTree;
	matrixComp();
	parseTree = program();
//	print_parse_tree(parseTree);
//	printf("\nSUCCESSFULLY PARSED INPUT!\n");

	parseTyping();
	print();
	return 0;
}
