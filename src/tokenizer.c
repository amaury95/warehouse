/*
 * tokenizer.c
 * 
 * Filled in by Brian Battell (135003877)
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tokenizer.h"

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
	
	TokenizerT *tokenizer = malloc(sizeof(struct TokenizerT_));
	tokenizer->sepArray = malloc(sizeof(char)*strlen(separators)+1);
	tokenizer->tsArray = malloc(sizeof(char)*strlen(ts)+1);
	//tokenizer->tsArray = malloc(sizeof(ts));
	
	strcpy(tokenizer->sepArray,separators);
	strcpy(tokenizer->tsArray,ts);
	
	tokenizer->tsPtr = tokenizer->tsArray; // gets delims
	tokenizer->tsPtr2 = tokenizer->tsArray; //holds starting place
	
	printf("Separators: %s\n", tokenizer->sepArray);
	printf("Token String: %s\n", tokenizer->tsArray);
  return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	free(tk->sepArray);
	free(tk->tsArray);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk){
	tk->sepPtr = tk->sepArray;

	int length=0;
		
	while(((tk->sepPtr && tk->tsPtr) != '\0') || NULL)
	{
		tk->tsPtr = strpbrk(tk->tsPtr,tk->sepPtr); // should give you a pointer to the first separator.
		
		if(tk->tsPtr == tk->tsPtr2)	//if the first element in ts is a separator
		{
		//	fprintf(stderr, "%s\n", "hi man");
			tk->tsPtr2++;
			tk->tsPtr = strpbrk(++tk->tsPtr, tk->sepPtr);
		//	fprintf(stderr, "hi man tsPtr: %s\n", tk->tsPtr); 
		//	fprintf(stderr, "hi man tsPtr2: %s\n", tk->tsPtr2); 
		}
		
			else if(tk->tsPtr != NULL)
		{
			//fprintf(stderr, "%s\n", "help man");
			length = strlen(tk->tsPtr2) - strlen(tk->tsPtr);
			tk->token = (char*)malloc(length+1);
			strncpy(tk->token,tk->tsPtr2,length);
			tk->token[length]= '\0';
		//	fprintf(stderr, "help man tsPtr: %s\n", tk->tsPtr);
		//	fprintf(stderr, "help man tsPtr2: %s\n", tk->tsPtr2);
			tk->tsPtr2 = ++tk->tsPtr;
		//	fprintf(stderr, "help man tsPtr2: %s\n", tk->tsPtr2);
		//	fprintf(stderr, "help man tsPtr: %s\n", tk->tsPtr );
					
			return tk->token;
		}
	}
	if(tk->tsPtr == NULL && tk->tsPtr2[0] != '\0' )
	{
		tk->token = (char*)malloc(strlen(tk->tsPtr2)+1);
		strcpy(tk->token, tk->tsPtr2);
		tk->token[strlen(tk->tsPtr2)] = '\0';
		tk->tsPtr2[0] ='\0';
		//fprintf(stderr, "almost there tsPtr2: %s\n", tk->token);

		return tk->token;	
	}
	return NULL; 
}