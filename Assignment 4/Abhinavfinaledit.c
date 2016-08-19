#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
void storemat();
struct varNode* make_varNode();
struct gotoStatement* make_gotoStmt();
struct assignmentStatement* make_assignStmt();
struct ifStatement* make_ifStmt();
struct printStatement* make_printStmt();
struct statementNode* make_statementNode();
struct statementNode* stmt_list();
struct statementNode* body();
//struct var_declNode* var_decl()
struct assignmentStatement* assign_stmt();
struct printStatement* print_stmt();
struct ifStatement* if_stmt();
struct ifStatement* while_stmt();
struct ifStatement* repeat_stmt();
struct varNode* operator();
struct statementNode* stmt();

struct varNode* matCount[100];
int flag = 0;
extern int line_no;

void syntax_error(char* NT, int line_no)
{
	printf("Syntax error while parsing %s line %d\n", NT, line_no);
}

struct varNode* make_varNode() 
{
	struct varNode* var = (struct varNode*) malloc(sizeof(struct varNode));
	return var;
}

struct ifStatement* make_ifStmt() 
{
	struct ifStatement* ifStmt = (struct ifStatement*) malloc(sizeof(struct ifStatement));
	return ifStmt;
}

struct gotoStatement* make_gotoStmt() 
{
	struct gotoStatement* gotoStmt = (struct gotoStatement*) malloc(sizeof(struct gotoStatement));
	return gotoStmt;
}

struct assignmentStatement* make_assignStmt() 
{
	struct assignmentStatement* assignStmt = (struct assignmentStatement*) malloc(sizeof(struct assignmentStatement));
	return assignStmt;
}

struct printStatement* make_printStmt() 
{
	struct printStatement* printStmt = (struct printStatement*) malloc(sizeof(struct printStatement));
	return printStmt;
}

struct statementNode* make_statementNode() 
{
	struct statementNode* stmt = (struct statementNode*) malloc(sizeof(struct statementNode));
	return stmt;
}

struct statementNode* parse_program_and_generate_intermediate_representation() 
{
	ttype = getToken();

	if (ttype == ID) 
	{		
		storemat();		
		ttype = getToken();

		if (ttype == LBRACE) 
		{
			//ungetToken();
   			//dec->var_decl_section = var_decl();
			struct statementNode* program = body();
			return program;
		} 
		else 
		{
			syntax_error("parse_program_and_generate_intermediate_representation(). Expected LBRACE.", line_no);
			exit(0);
		}
	} 
	else 
	{
		syntax_error("parse_program_and_generate_intermediate_representation(). Expected ID.", line_no);
		exit(0);
		//ungetToken();
		//dec->var_decl_section = NULL;
	}
}

/*struct var_declNode* var_decl()
{
	
	struct var_declNode* varDecl;
	varDecl = make_var_declNode();

	varDecl->id_list = id_list();

	_ttype = getToken();
	if (_ttype == SEMICOLON)
	{				
		return varDecl;
	}
	else
	{	syntax_error("var_decl. SEMICOLON expected", _line_no);
		exit(0);
	}
}*/

struct statementNode* body() 
{	
	struct statementNode* bod;

	if (ttype == LBRACE) 
	{
		//print_debug(_depth); 
		//printf("!}}\n");
		ttype = getToken();

		if (ttype == ID || ttype == WHILE || ttype == REPEAT || ttype == IF || ttype == PRINT) 
		{
			bod = stmt_list();
		} 
		else 
		{
			syntax_error("body(). Expected ID or IF or WHILE or REPEAT or PRINT.", line_no);
			exit(0);
		}
		ttype = getToken();

		if (ttype == RBRACE) 
		{
			//print_debug(_depth); 
			//printf("!Returning bod\n");			
			return bod;
		} 
		else 
		{
			syntax_error("body(). Expected RBRACE.", line_no);
			exit(0);			
		}
	} 
	else 
	{
		syntax_error("body(). Expected LBRACE.", line_no);		
		exit(0);
	}
}

struct statementNode* stmt_list() 
{
	struct statementNode* stmtNode;
	stmtNode = make_statementNode();

	if (ttype == ID || ttype == IF || ttype == WHILE || ttype == REPEAT || ttype == PRINT) 
	{
		//print_debug(_depth); 
		//printf("!stmt %d next is set to stmt_list %d\n", st->stmt_type, stL->stmtType);
		ungetToken();
		stmtNode = stmt(); 
		ttype = getToken();

		if (ttype == RBRACE) 
		{
			ungetToken();
			return stmtNode;
		} 
		else if (ttype == ID || ttype == IF || ttype == WHILE || ttype == REPEAT || ttype == PRINT) 
		{	
			//ungetToken();
			stmtNode->next = stmt_list();
			return stmtNode;
		} 
		else 
		{
			syntax_error("stmt_list(). Expected RBRACE or ID or IF or WHILE or REPEAT or PRINT.", line_no);			
			exit(0);
		}
	} 
	else 
	{
		syntax_error("stmt_list(). Expected RBRACE or ID or IF or WHILE or REPEAT or PRINT.", line_no);		
		exit(0);
	}
}

struct statementNode* stmt()
{
	struct statementNode* stats;
	stats = make_statementNode();
	stats->stmt_type = 0;
	stats->assign_stmt = 0;
	stats->print_stmt = 0;
	stats->if_stmt = 0;
	stats->goto_stmt = 0;
	stats->next = 0;	ttype = getToken();
		
	if (ttype == ID) 
	{	stats->stmt_type = ASSIGNSTMT;
		stats->assign_stmt = assign_stmt();
		//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmt_type, _line_no);	
		//_ttype = getToken();
		return stats;
	}
	else if (ttype == REPEAT) //REPEAT Statement
	{	
		//struct statementNode* repeatStmt = make_statementNode();
		stats->stmt_type = IFSTMT;
		stats->if_stmt = repeat_stmt();
		stats->next = stats->if_stmt->false_branch;
		struct statementNode* gotoStmt = make_statementNode();
		gotoStmt->stmt_type = 0;
		gotoStmt->assign_stmt = 0;
		gotoStmt->print_stmt = 0;
		gotoStmt->if_stmt = 0;
		gotoStmt->goto_stmt = 0;
		gotoStmt->next = 0;
		gotoStmt->stmt_type = GOTOSTMT;
		gotoStmt->goto_stmt = make_gotoStmt();
		struct statementNode* nestedStmt = stats->if_stmt->false_branch;

		while (nestedStmt->next != NULL) 
		{
			nestedStmt = nestedStmt->next;
		}

		gotoStmt->goto_stmt->target = stats;
		nestedStmt->next = gotoStmt;
		return stats;
	} 
	else if (ttype == WHILE) 
	{	
		stats->stmt_type = IFSTMT;
		stats->if_stmt = while_stmt();
//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmt_type, _line_no);
		stats->next = stats->if_stmt->false_branch;
		struct statementNode* gotoStmt = make_statementNode();
		gotoStmt->stmt_type = 0;
		gotoStmt->assign_stmt = 0;
		gotoStmt->print_stmt = 0;
		gotoStmt->if_stmt = 0;
		gotoStmt->goto_stmt = 0;
		gotoStmt->next = 0;
		// create goto
		gotoStmt->stmt_type = GOTOSTMT;
		gotoStmt->goto_stmt = make_gotoStmt();
		struct statementNode* nestedStmt = stats->if_stmt->true_branch;

		while (nestedStmt->next != NULL) 
		{
			nestedStmt = nestedStmt->next;
		//print_debug(_depth);
		//printf("!Next stmt is %d\n", while_stmtlist->stmt_type);
		}
		gotoStmt->goto_stmt->target = stats; // create goto
		nestedStmt->next = gotoStmt;
		return stats;
		//print_debug(_depth);
		//printf("!set NULL to GOTO\n");
	} 
	else if (ttype == IF)
	{	
		stats->stmt_type = IFSTMT;
		stats->if_stmt = if_stmt();
		//print_debug(_depth); 
		//printf("!!stmt is %d at line%d\n", stm->stmt_type, _line_no);
		stats->next = stats->if_stmt->false_branch;
		return stats;
	} 
	else if (ttype == PRINT)
	{	
		stats->stmt_type = PRINTSTMT;
		stats->print_stmt = print_stmt();
		//print_debug(_depth); 
		//printf("!!stmt is %d at line%d\n", stm->stmt_type, _line_no);
		return stats;
	} 
	else //Syntax Error
	{
		syntax_error("stmt(). Statement expected", line_no);
		exit(0);
	}	
}

struct assignmentStatement* assign_stmt() 
{
	struct assignmentStatement* assignStmt;
	assignStmt = make_assignStmt();	
	assignStmt->lhs = 0;
	assignStmt->op1 = 0;
	assignStmt->op2 = 0;
	assignStmt->op = 0;

	if (ttype == ID) 
	{
		int i;
		
		for (i = 0; i < flag; i++) 
		{
			if (strcmp(matCount[i]->name, token) == 0)
			{
				assignStmt->lhs = matCount[i];
				break;
			}
		}
		//printf("NAME%s\n", assignStmt->lhs->name);
		//printf("VALUE%d\n", assignStmt->lhs->value);
		ttype = getToken();

		if (ttype == EQUAL) 
		{
			ttype = getToken();

			if (ttype == NUM || ttype == ID) 
			{
				assignStmt->op1 = operator();
				ttype = getToken();
				//one operand
				if (ttype == SEMICOLON) 
				{
					// //printf("NAME= %s", assignStmt->lhs->name);
			 		// //printf("Value= %d", assignStmt->lhs->value);
					return assignStmt;
				} 
				else if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV) 
				{
					assignStmt->op = ttype;
					ttype = getToken();

					if (ttype == NUM || ttype == ID) 
					{
						assignStmt->op2 = operator();
						ttype = getToken();
						//ungetToken();
						if (ttype == SEMICOLON) 
						{
							return assignStmt;
						} 
						else 
						{
							syntax_error("assign_stmt(). Expected SEMICOLON.",line_no);
							exit(0);
						}
					} 
					else 
					{
						syntax_error("assign_stmt(). Expected NUM or ID.",line_no);
						exit(0); 
					}	
				}
				//printf("Se fault\n");
				else 
				{
					syntax_error("assign_stmt(). Expected SEMICOLON.",line_no);					
					exit(0);
				}
			} 
			else 
			{
				syntax_error("assign_stmt(). Expected NUM or ID.",line_no);
				exit(0);
			}
		} 
		else 
		{
			syntax_error("assign_stmt(). Expected EQUAL operator.",line_no);			
			exit(0);
		}
	} 
	else 
	{
		syntax_error("assign_stmt(). Expected ID.",line_no);
		exit(0);
	}
}

struct ifStatement* repeat_stmt() 
{//Repeat until the condition is true. Loop will be executed atleast once.
	struct ifStatement* ifStmt;
	ifStmt = make_ifStmt();
	ifStmt->op1 = 0;
	ifStmt->op2 = 0;
	ifStmt->relop = 0;
	ifStmt->true_branch = 0;
	ifStmt->false_branch = 0;
	
	if (ttype == REPEAT) 
	{		
		getToken();
		
		if (ttype == LBRACE) 
		{
			ifStmt->false_branch = body();
			
			getToken();
			
			if (ttype == UNTIL) 
			{
				getToken();
				
				if (ttype == NUM || ttype == ID) 
				{
					ifStmt->op1 = operator();
					getToken();

					if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL) 
					{
						ifStmt->relop = ttype;
					} 
					else 
					{
						syntax_error("repeat_stmt(). No relop in condition.", line_no);
						exit(0);
					}
					
					getToken();
					
					if (ttype == NUM || ttype == ID) 
					{
						ifStmt->op2 = operator();
						ttype = getToken();
						//printf("here's yer token = %d \n", ttype);
						
						if (ttype == ID || ttype == IF || ttype == WHILE || ttype == REPEAT || ttype == PRINT) 
						{
							ifStmt->true_branch = make_statementNode();
							ifStmt->true_branch->stmt_type = NOOPSTMT;
							ifStmt->true_branch->next = stmt_list();
							//printf("ifStmt->true type %d\n", ifStmt->true_branch->stmt_type);
							return ifStmt;
						} 
						else if (ttype == RBRACE) 
						{
							ungetToken();
							ifStmt->true_branch = make_statementNode();
							ifStmt->true_branch->stmt_type = NOOPSTMT;
							//printf("workin fine\n");
							return ifStmt;
						} 
						else 
						{
							syntax_error("repeat_stmt(). Expected ID or IF or WHILE or REPEAT or PRINT or RBRACE.", line_no);
							exit(0);
						}
					} 
					else 
					{
						syntax_error("repeat_stmt(). Expected ID or NUM.", line_no);
						exit(0);
					}
				} 
				else 
				{
					syntax_error("repeat_stmt(). Expected ID or NUM.", line_no);
					exit(0);
				}
			} 
			else 
			{
				syntax_error("repeat_stmt(). Expected UNTIL keyword.", line_no);				exit(0);
			}
		} 
		else 
		{
			syntax_error("repeat_stmt(). Expected LBRACE.", line_no);
			exit(0);
		}
	} 
	else 
	{
		syntax_error("repeat_stmt(). Expected REPEAT keyword.", line_no);
		exit(0);
	}
}

struct ifStatement* while_stmt() 
{//While statement has condition and the body is executed till the condition is true
	struct ifStatement* ifStmt;
	ifStmt = make_ifStmt();
	ifStmt->op1 = 0;
	ifStmt->op2 = 0;
	ifStmt->relop = 0;
	ifStmt->true_branch = 0;
	ifStmt->false_branch = 0;
	
	if (ttype == WHILE) 
	{
		ttype = getToken();

		if (ttype == NUM || ttype == ID) 
		{
			ifStmt->op1 = operator();
			ttype = getToken();

			if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL) 
			{
				ifStmt->relop = ttype;
			} 
			else 
			{
				syntax_error("while_stmt(). Expected GREATER, LESS, or NOTEQUAL.", line_no);				
				exit(0);
			}

			ttype = getToken();

			if (ttype == NUM || ttype == ID) 
			{
				ifStmt->op2 = operator();
				ttype = getToken();

				if (ttype == LBRACE) 
				{
					ifStmt->true_branch = body();
					ttype = getToken();

					if (ttype == ID || ttype == IF || ttype == WHILE || ttype == REPEAT || ttype == PRINT) 
					{
						ifStmt->false_branch = make_statementNode();
						ifStmt->false_branch->stmt_type = NOOPSTMT;
						ifStmt->false_branch->next = stmt_list();
						return ifStmt;
					} 
					else if (ttype == RBRACE) 
					{
						ungetToken();
						ifStmt->false_branch = make_statementNode();
						ifStmt->false_branch->stmt_type = NOOPSTMT;
						return ifStmt;
					} 
					else 
					{
						syntax_error("while_stmt(). Expected ID, IF, WHILE, REPEAT, or PRINT.",line_no);  				
					}
				} 
				else 
				{
					syntax_error("while_stmt(). Expected LBRACE.",line_no);
					exit(0);
				}
			} 
			else 
			{
				syntax_error("while_stmt(). Expected NUM or ID.",line_no);
				exit(0);
			}
		} 
		else 
		{
			syntax_error("while_stmt(). Expected ID or NUM.",line_no);			
			exit(0);
		}
	} 
	else 
	{
		syntax_error("while_stmt(). Expected WHILE keyword.",line_no);
		exit(0);
	}
	return 0;
}

struct ifStatement* if_stmt() 
{
	struct ifStatement* ifStmt;
	ifStmt = make_ifStmt();
	ifStmt->op1 = 0;
	ifStmt->op2 = 0;
	ifStmt->relop = 0;
	ifStmt->true_branch = 0;
	ifStmt->false_branch = 0;
	if (ttype == IF) 
	{
		ttype = getToken();

		if (ttype == NUM || ttype == ID) 
		{
			ifStmt->op1 = operator();
			ttype = getToken();

			if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL) 
			{
				ifStmt->relop = ttype;
			} 
			else 
			{
				syntax_error("if_stmt(). Expected GREATER, LESS, or NOTEQUAL.",line_no);				
				exit(0);
			}
			ttype = getToken();

			if (ttype == NUM || ttype == ID) 
			{
				ifStmt->op2 = operator();
				ttype = getToken();

				if (ttype == LBRACE) 
				{
					ifStmt->true_branch = body();
					ttype = getToken();

					if (ttype == ID || ttype == IF || ttype == WHILE || ttype == REPEAT || ttype == PRINT) 
					{
						ifStmt->false_branch = make_statementNode();
						ifStmt->false_branch->stmt_type = NOOPSTMT;
						ifStmt->false_branch->next = stmt_list();
						struct statementNode* stmtNode = ifStmt->true_branch;

						while (stmtNode->next != NULL) 
						{
							stmtNode = stmtNode->next;
						}

						stmtNode->next = ifStmt->false_branch;
						return ifStmt;
					} 
					else if (ttype == RBRACE) 
					{
						ungetToken();
						ifStmt->false_branch = make_statementNode();
						ifStmt->false_branch->stmt_type = NOOPSTMT;
						struct statementNode* stmtNode = ifStmt->true_branch;

						while (stmtNode->next != NULL) 
						{
							stmtNode = stmtNode->next;
						}

						stmtNode->next = ifStmt->false_branch;
						return ifStmt;
					} 
					else 
					{
						syntax_error("if_stmt(). Expected ID or IF or WHILE or REPEAT or PRINT.",line_no);
						exit(0);	
					}
				} 
				else 
				{
					syntax_error("if_stmt(). Expected LBRACE.",line_no); 
					exit(0);						
				}
			} 
			else 
			{
				syntax_error("if_stmt(). Expected ID or NUM.",line_no);				
				exit(0);
			}
		} 
		else 
		{
			syntax_error("if_stmt(). Expected ID or NUM.",line_no);			
			exit(0);
		}						
	} 
	else 
	{
		syntax_error("if_stmt(). Expected IF keyword.",line_no);		
		exit(0);
	}
}

struct printStatement* print_stmt() 
{
	struct printStatement* printStmt;
	int i;
	printStmt = make_printStmt();
	printStmt->id = 0;

	if (ttype == PRINT) 
	{
		ttype = getToken();

		if (ttype == ID) 
		{
			for (i = 0; i < flag; i++) 
			{
				if (strcmp(matCount[i]->name, token) == 0)
				{
					printStmt->id = matCount[i];
					break;
				}
			}			
			ttype = getToken();

			if (ttype == SEMICOLON) 
			{
				return printStmt;
			} 
			else 
			{
				syntax_error("print_stmt(). Expected SEMICOLON.",line_no);
				exit(0);
			}
		} 
		else 
		{
			syntax_error("print_stmt(). Expected ID.",line_no);			
			exit(0);
		}
	} 
	else 
	{
		syntax_error("if_stmt(). Expected PRINT keyword.",line_no);
		exit(0);
	}
}		

struct varNode* operator() 
{
	if ((ttype == ID) || (ttype == NUM)) 
	{
		if (ttype == ID) 
		{
			int i;
			//printf("hello\n");

			for (i = 0; i < flag; i++) 
			{
				if (strcmp(matCount[i]->name, token) == 0)
				{
				return matCount[i];
				}
			}			
		} 
		else 
		{
			struct varNode* var = make_varNode();
			var->name = 0;
			var->value = 0;
			var->value = atoi(token);
			//printf("yesss\n");
			return var;
		}
	} 
	else 
	{
		syntax_error("operator(). Expected ID or NUM.", line_no);		
		exit(0);
	}
	return 0;
}

void storemat() 
{
	if (ttype == ID) 
	{
		struct varNode* type = make_varNode();
		type->name = 0;
		type->value = 0;
		type->name = (char *) malloc((200) * sizeof (char));
		strcpy(type->name,token);
		matCount[flag] = type;
		flag++;

		ttype = getToken();

		if (ttype == COMMA) 
		{
			ttype = getToken();

		// //printf("token = %s\n", token );
			if (ttype == ID) 
			{
				storemat();
			}
			else 
			{
				syntax_error("storemat(). Expected ID.", line_no);
				exit(0);
			}
		} 
		else if (ttype == SEMICOLON) 
		{
			////printf("Workin!!\n");
			return;
		} 
		else 
		{
			syntax_error("storemat(). COMMA or SEMICOLON expected.", line_no);			
			exit(0);
		}
	} 
	else 
	{
		syntax_error("storemat(). Expected ID.", line_no);			
		exit(0);
	}			
}
