#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define LIMIT 100
#define ERRORCODERS 5

struct grammar{
	char LHS[LIMIT+1];
	char RHS[LIMIT+1][LIMIT+1];
};

struct fiAndFo{
	char nonT[LIMIT+1];
	
	int ebayinc[LIMIT+1];
	int numT;
};


int errors[ERRORCODERS] = {0, 0, 0, 0, 0};


char NT[LIMIT+1][LIMIT+1];


char T[LIMIT+1][LIMIT+1];


struct grammar rules[LIMIT];


struct fiAndFo FIRST[LIMIT+1];
struct fiAndFo FOLLOW[LIMIT+1];

int cNterminals;
int cTerminals;
int cRules;


void skipSpace()
{
	char c;

	c = getchar();
	while (!feof(stdin) && isspace(c))
	{
		c = getchar();
	}

	
	if (!feof(stdin))
		ungetc(c,stdin);
}

void getNT(){
	char c;
	
	char temp[LIMIT+1];
	
	
	int cSpot;
	
	int cToken;
	
	cToken = 0;
	
	skipSpace();
	c = getchar();
	
	
	while(c != '#'){
		
		cSpot = 0;
		
		if(!isalpha(c)){
			errors[0] = 1;
			break;
		}
		
		
		while(!isspace(c)){
			
			if(!isalpha(c) && !isdigit(c)){
				errors[0] = 1;
				break;
			}
			
			
			temp[cSpot] = c;
			cSpot ++;
			c = getchar();
		}
		
		
		if(errors[0] == 1){
			break;
		}
		
		
		temp[cSpot] = '\0';
		cNterminals++;
		
	
		strcpy(NT[cToken], temp);
		cToken++;
		
		skipSpace();
		c = getchar();
	}
	
	
	temp[0] = '\0';
	strcpy(NT[cToken], temp);
	
}

void getT(){
	char c;
	
	char temp[LIMIT+1];
	
	
	int cSpot;
	
	int cToken;
	
	cToken = 0;
	

	skipSpace();
	c = getchar();
	
	while(c != '#'){
		
		cSpot = 0;

		if(!isalpha(c)){
			errors[0] = 1;
			break;
		}
		
		
		while(!isspace(c)){
			
			if(!isalpha(c) && !isdigit(c)){
				errors[0] = 1;
				break;
			}
			
			
			temp[cSpot] = c;
			cSpot ++;
			c = getchar();
		}
		
		
		if(errors[0] == 1){
			break;
		}
		
		
		temp[cSpot] = '\0';
		cTerminals++;
		
	
		strcpy(T[cToken], temp);
		cToken++;
		
		skipSpace();
		c = getchar();
	}
	
	temp[0] = '\0';
	strcpy(T[cToken], temp);
	
	
}


int rightChecking(char* input){
	int i;
	
	if(!(isalpha(input[0]))){
		return FALSE;
	}
	
	for(i = 1; i < LIMIT; i++){
		if(input[i] == '\0')
			break;
		if(!isalpha(input[i]) && !isdigit(input[i])){
				return FALSE;
				break;
		}
	}
	
	return TRUE;
}


void gRules(){
	char c;
	char left[LIMIT];
	char right[LIMIT * LIMIT + LIMIT + 2];
	
	int cSpot;
	
	int currentRule;

	int currentRHS;
	
	currentRule = 0;
	
	skipSpace();
	c = getchar();
	
	
	while(c != '#'){
		cSpot = 0;
		
		while(!isspace(c)){
			left[cSpot] = c;
			cSpot ++;
			c = getchar();
		}
		left[cSpot] = '\0';
		skipSpace();
		c=getchar();
		
		if(c == '-'){
			c=getchar();
			if(c == '>'){
			}
			else{
				errors[0] = 1;
				break;
			}
		}
		else{
			errors[0] = 1;
			break;
		}
		
		cSpot = 0;
		currentRHS = 0;
		skipSpace();
		c=getchar();
		
		
		if(c == '#'){
			right[cSpot] = '#';
			right[cSpot+1] = '\0';
			strcpy(rules[currentRule].RHS[currentRHS], right);
			currentRHS++;
			cSpot++;
		}
		else{
			while(c != '#'){
				
				while(!isspace(c)){
					right[cSpot] = c;
					cSpot++;
					c=getchar();
				}
				
			
				right[cSpot] = '\0';
				if(rightChecking(right) == FALSE){
					errors[0] = 1;
					break;
				}
				strcpy(rules[currentRule].RHS[currentRHS], right);
				currentRHS++;
				
				cSpot=0;
				
			
				skipSpace();
				c=getchar();
			}
			
		}
		right[0] = '\0';

		strcpy(rules[currentRule].RHS[currentRHS], right);
		cRules++;
		

		strcpy(rules[currentRule].LHS, left);
		
		
	
		currentRule++;
		
		
		skipSpace();
		c=getchar();
	}
	
	
	left[0] = '\0';
	strcpy(rules[currentRule].LHS, left);
	
}

int fTerminal(char* terminal){
	int i;
	for(i = 0; i < cTerminals; i++){
		char temp[LIMIT+1];
		strcpy(temp, T[i]);
		if(0 == strcmp(temp, terminal)){
			return i;
		}
	}
	return -1;
}

int fNTNT(char* nonT){
	int i;
	for(i = 0; i < cNterminals; i++){
		char temp[LIMIT+1];
		strcpy(temp, NT[i]);
		if(0 == strcmp(temp, nonT)){
			return i;
		}
	}
	return -1;
}


int uFirst(int to, int from){
	int i;
	int hasEmpty;
	hasEmpty = -1;
	for(i = 1; i < cTerminals + 1; i++){
		if(FIRST[to].ebayinc[i] == FALSE && FIRST[from].ebayinc[i] == TRUE){
			FIRST[to].ebayinc[i] = TRUE;
			hasEmpty = FALSE;
		}
	}
	
	
	if(FIRST[from].ebayinc[0] == TRUE){
		hasEmpty = TRUE;
	}
	return hasEmpty;
}


int uFoFi(int to, int from){
	
	int i;
	int hasEmpty;
	hasEmpty = -1;
	for(i = 1; i < cTerminals + 1; i++){
		if(FOLLOW[to].ebayinc[i] == FALSE && FIRST[from].ebayinc[i] == TRUE){
			FOLLOW[to].ebayinc[i] = TRUE;
			hasEmpty = FALSE;
		}
	}
	
	
	if(hasEmpty == FALSE){
		if(FIRST[from].ebayinc[0] == TRUE){
			hasEmpty = TRUE;
		}
	}
	
	return hasEmpty;
}

int uFoll(int to, int from){
	int i;
	int vary;
	vary = -1;
	for(i = 0; i < cTerminals + 1; i++){
		if(FOLLOW[to].ebayinc[i] == FALSE && FOLLOW[from].ebayinc[i] == TRUE){
			if(to >=6){
			
			}
			FOLLOW[to].ebayinc[i] = TRUE;
			vary = TRUE;
		}
	}
	
	
	return vary;
}

void fFirst(){
	int i, j;
	int vary= TRUE;
	
	
	for(i = 0; i < cNterminals; i++){
		strcpy(FIRST[i].nonT, NT[i]);
		FIRST[i].numT = 0;
		FIRST[i].ebayinc[0] = 0;
	}
	FIRST[cNterminals].nonT[0] = '0';
	
	
	while(vary == TRUE){
		vary = FALSE;
		
		for(i = 0; i < cRules; i++){
			char currentLHS[LIMIT+1];
			strcpy(currentLHS, rules[i].LHS);
			int indexLHS = fNTNT(rules[i].LHS);
			
			int hasEmpty = FALSE;
			for(j = 0; j < LIMIT; j++){
				char currentRHS[LIMIT+1];
				strcpy(currentRHS, rules[i].RHS[j]);
				
				
				if(currentRHS[0] == '#'){
					
					if(FIRST[indexLHS].ebayinc[0] == 0){
						FIRST[indexLHS].ebayinc[0] = 1;
						vary = TRUE;	
					}
					
					break;
				}
				
				else if(currentRHS[0] == '\0'){
					FIRST[indexLHS].ebayinc[0] = 1;
					break;
				}
				
				else if(- 1 < fTerminal(rules[i].RHS[j])){
					if(FIRST[indexLHS].ebayinc[fTerminal(rules[i].RHS[j])+1] == 0){
						FIRST[indexLHS].ebayinc[fTerminal(rules[i].RHS[j])+1] = 1;
						vary = TRUE;
					}
					break;
				}
				
				else if(fNTNT(currentRHS)){
					hasEmpty = uFirst(indexLHS, fNTNT(currentRHS));
					if(hasEmpty == -1){
						break;
					}
					else if(hasEmpty == FALSE){
						vary = TRUE;
						break;
					}
				 }
				
			}
		}
	}
	
}

void computeFirst(){
	int i, j;
	int vary=TRUE;
	
	for(i = 0; i < cNterminals; i++){
		strcpy(FIRST[i].nonT, NT[i]);
		FIRST[i].numT = 0;
	}
	FIRST[cNterminals].nonT[0] = '0';
	
	
	while(vary == TRUE){
		vary = FALSE;
		for(i = 0; i < cNterminals; i++){
			
			char current[LIMIT+1];
			strcpy(current, FIRST[i].nonT);
			for(j = 0; j < cRules; j++){
				if(strcmp(current, rules[j].LHS) == 0){
					int spot;
					
					if(rules[j].RHS[FIRST[i].numT] == '\0' && FIRST[i].ebayinc[0] == 0){
						FIRST[i].ebayinc[0] = 1;
						vary = TRUE;
					}
					if(-1 < fTerminal(rules[j].RHS[FIRST[i].numT])){
						spot = fTerminal(rules[j].RHS[FIRST[i].numT]) + 1;
						if(FIRST[i].ebayinc[spot] == 0){
							FIRST[i].ebayinc[spot] = 1;
							vary = TRUE;
						}
					}
					else if(rules[j].RHS[FIRST[i].numT][0] == '#'){
						if(FIRST[i].ebayinc[0] == 0){
							FIRST[i].ebayinc[0] = 1;
							vary = TRUE;
						}
					}
					
					else if(-1 < fNTNT(rules[j].RHS[FIRST[i].numT])){
						int hasEmpty = FALSE;
						spot= fNTNT(rules[j].RHS[FIRST[i].numT]);
						hasEmpty = uFirst(i, spot);
						if(hasEmpty == TRUE){
							FIRST[i].numT++;
							vary = TRUE;
						}
						else if(hasEmpty == FALSE){
							vary = TRUE;
						}
					}
					else{
						
					}
				}
			}
		}
	}
}

void fFollS(){
	int i, j, k;
	int vary=TRUE;
	int emptyRun=TRUE;
	
	for(i = 0; i < cNterminals; i++){
		strcpy(FOLLOW[i].nonT, NT[i]);
		FOLLOW[i].numT = 0;
	}
	FOLLOW[cNterminals].nonT[0] = '0';
	FOLLOW[0].ebayinc[0] = 1;
	
	while(vary == TRUE){
		vary = FALSE;
		for(i = 0; i < cNterminals; i++){
			char currentNT[LIMIT+1];
			strcpy(currentNT, FOLLOW[i].nonT);
			
			for(j = 0; j < cRules; j++){
				
				for(k = 0; k < LIMIT; k++){
					
					if( -1 < fNTNT(rules[j].RHS[k])){
						if(rules[j].RHS[k+1][0] == '\0'){
							int indexLHS = fNTNT(rules[j].LHS);
							int indexRHS = fNTNT(rules[j].RHS[k]);
							
							int didChange = uFoll(indexRHS, indexLHS);
							if(didChange == TRUE){
								
								vary = TRUE;
							}
						}
						
						else if( -1 < fTerminal(rules[j].RHS[k+1])){
							char nextTerminal[LIMIT+1];
							strcpy(nextTerminal, rules[j].RHS[k+1]);
							int indexOfTerminal = fTerminal(rules[j].RHS[k+1]);
							int indexOfNT = fNTNT(rules[j].RHS[k]);
							if(FOLLOW[indexOfNT].ebayinc[indexOfTerminal+1] == 0){
								FOLLOW[indexOfNT].ebayinc[indexOfTerminal+1] = 1;
								vary = TRUE;
							}
						}
						else if( -1 < fNTNT(rules[j].RHS[k+1])){
							int index = fNTNT(rules[j].RHS[k+1]);
							int indexOfNT = fNTNT(rules[j].RHS[k]);
							
							int hasEmpty = uFoFi(indexOfNT, index);
							
							if(hasEmpty == -1){
								
							}
							else if(hasEmpty == FALSE){
								
								vary = TRUE;
							}
							else if(hasEmpty == TRUE){
								
								vary = TRUE;
								
								uFoll(indexOfNT, index);
							}
							
							
							if(FIRST[index].ebayinc[0] == 1){
								
								int didChange = uFoll(indexOfNT, index);
								if(didChange == TRUE){
									
									vary = TRUE;
								}	
							}
							
						}
					}
				}
				
			}
		}
		
		
	}
	
}


void computeFollowSets(){
	int i, j, k;
	int vary=TRUE;
	char emptyString[1] = {'\0'};
	
	for(i = 0; i < cNterminals; i++){
		strcpy(FOLLOW[i].nonT, NT[i]);
		FOLLOW[i].numT = 0;
	}
	FOLLOW[cNterminals].nonT[0] = '0';
	FOLLOW[0].ebayinc[0] = 1;
	
	while(vary == TRUE){
		vary = FALSE;
		for(i = 0; i < cNterminals; i++){
			char currentNT[LIMIT+1];
			strcpy(currentNT, FOLLOW[i].nonT);
			
			for(j = 0; j < cRules; j++){
				
				for(k = 0; k < LIMIT; k++){
					
					if(strcmp(currentNT, rules[j].RHS[k]) == 0){
						 if(strcmp(rules[j].RHS[k+1], emptyString) == 0){
							
							int didChange;
							int didChange2;
							
							int index = fNTNT(rules[j].LHS);
							didChange = uFoll(i, index);
							didChange2 = uFoll(index, i);
							
							if(didChange == TRUE || didChange2 == TRUE){
								vary = TRUE;
							}
						}
						
						else if(-1 < fTerminal(rules[j].RHS[k+1])){
							
							char nextRule[LIMIT+1];
							strcpy(nextRule, rules[j].RHS[k+1]);
							int index = fTerminal(rules[j].RHS[k+1]);
							if(FOLLOW[i].ebayinc[index+1] == 0){
								FOLLOW[i].ebayinc[index+1] = 1;
								vary = TRUE;
							}
						}
						else{
							
						}
					}
				}
			}
		}
	}
	
}

void ordering(){
	int i, j;
	for(i = 0; i < cTerminals; i++){
		for(j = 0; j < cTerminals-1; j++){
			if(1 <= strcmp(T[j], T[j+1])){
				char temp[LIMIT+1];
				strcpy(temp, T[j+1]);
				strcpy(T[j+1], T[j]);
				strcpy(T[j], temp);
			}
		}
	}
}

int rectify(){

	int i, j, k, l, errorFound, returnError;
	char emptyString[1] = {'\0'};
	char pound[1] = {'#'};
	returnError = FALSE;
	
	
	for(i = 0; i < cNterminals; i++){
		errorFound = TRUE;
		for(j =0; j < cRules; j++){
			if(strcmp(NT[i], rules[j].LHS) == 0){
				errorFound = FALSE;
			}
		}
		if(errorFound == TRUE)
			break;
	}
	
	if(errorFound == TRUE){
		errors[1] = TRUE;
		returnError = TRUE;
	}
	
	
	for(i = 0; i < cRules; i++){
		errorFound = TRUE;
		
		
		
		for(j =0; j < cNterminals; j++){
			if(strcmp(NT[j], rules[i].LHS) == 0){
				errorFound = FALSE;
			}
		}
		
		for(j =0; j < cTerminals; j++){
			if(strcmp(T[j], rules[i].LHS) == 0){
				errorFound = FALSE;
			}
		}
		
		if(errorFound == TRUE)
			break;
		
		
		for(j =0; j < LIMIT; j++){
			
			if(strcmp(rules[i].RHS[0], pound) == 0){
				errorFound = FALSE;
				break;
			}
			if(strcmp(rules[i].RHS[j+1], emptyString) == 0)
				break;
			
			errorFound = TRUE;
			
			for(k =0; k < cNterminals; k++){
				if(strcmp(NT[k], rules[i].RHS[j]) == 0){
					errorFound = FALSE;
					break;
				}
			}
			
			
			for(k =0; k < cTerminals; k++){
				if(strcmp(T[k], rules[i].RHS[j]) == 0){
					errorFound = FALSE;
					break;
				}
			}
			
			if(errorFound == TRUE){
				break;
			}
			
		}
		
		if(errorFound == TRUE)
			break;
	}
	
	if(errorFound == TRUE){
		errors[2] = TRUE;
		returnError = TRUE;
	}
	
	
	errorFound = FALSE;
	for(i = 0; i < cRules; i++){
		for(j =0; j < cTerminals; j++){
			if(strcmp(T[j], rules[i].LHS) == 0){
				errorFound = TRUE;
				break;
			}
		}
		if(errorFound == TRUE)
			break;
	}
	
	if(errorFound == TRUE){
		errors[3] = TRUE;
		returnError = TRUE;
	}

	
	
	for(i = 0; i < cTerminals; i++){errorFound = TRUE;
		for(j =0; j < cRules; j++){
			
			for(l=0;l<LIMIT;l++)
			{
				if(strcmp(T[i], rules[j].RHS[l]) == 0)
				{	
				errorFound = FALSE;
				
				break;
				}
			}
			
			
		}
	if(errorFound == TRUE){
		break;}	
		
	}
	
	if(errorFound == TRUE){
		errors[4] = TRUE;
		returnError = TRUE;
	}

	 
	
	return returnError;
}


main()
{
	
	int i;
	int j;
	
	cNterminals = 0;
	cTerminals = 0;
	cRules = 0;
	
	getNT();
	if(errors[0] == 1){
		printf("ERROR CODE %d\n", 0);
		return 0;
	}
	
	getT();
	if(errors[0] == 1){
		printf("ERROR CODE %d\n", 0);
		return 0;
	}
	
	gRules();
	
	if(errors[0] == 1){
		printf("ERROR CODE %d\n", 0);
		return 0;
	}
	
	int errorFound;
	
	errorFound = rectify();
	
	if(errorFound == TRUE){
		if(errors[1] == TRUE)
			printf("ERROR CODE %d\n", 1);
		
		if(errors[2] == TRUE)
			printf("ERROR CODE %d\n", 2);
		
		if(errors[3] == TRUE)
			printf("ERROR CODE %d\n", 3);
		
		if(errors[4] == TRUE)
			printf("ERROR CODE %d\n", 4);

		return 0;
	}
	
	ordering();
	
	
	fFirst();
	fFollS();

	
	int hasEmpty;
	for(i = 0; i < cNterminals; i++){
		hasEmpty = FALSE;
		printf("FIRST(");
		printf("%s", FIRST[i].nonT);
		printf(") = { ");
		if(FIRST[i].ebayinc[0] == 1){
			printf("#");
			hasEmpty = TRUE;
		}
		for(j = 0; j < cTerminals; j++){
			if(FIRST[i].ebayinc[j+1] == 1){
				switch(hasEmpty){
					case TRUE:
						printf(", %s", T[j]);
						break;
					case FALSE:
						printf("%s", T[j]);
						hasEmpty = TRUE;
						break;
				}
			}
		}
		printf(" }\n");
	}

	for(i = 0; i < cNterminals; i++){
		hasEmpty = FALSE;
		printf("FOLLOW(");
		printf("%s", FOLLOW[i].nonT);
		printf(") = { ");
		if(FOLLOW[i].ebayinc[0] == 1){
			printf("$");
			hasEmpty = TRUE;
		}
		for(j = 0; j < cTerminals; j++){
			if(FOLLOW[i].ebayinc[j+1] == 1){
				switch(hasEmpty){
					case TRUE:
						printf(", %s", T[j]);
						break;
					case FALSE:
						printf("%s", T[j]);
						hasEmpty = TRUE;
						break;
				}
			}
		}
		printf(" }\n");
	}
}

