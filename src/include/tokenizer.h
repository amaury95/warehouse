/*
 tokenizer.h
 *
 *  Created on: Feb 18, 2011
 *      Author: Thu D. Nguyen (tdnguyen@cs.rutgers.edu)
 *      Author: Chris Mansley
 *      Author: Brian Russell (morbius@cs.rutgers.edu)
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

/* Define opaque type for tokenizer objects */

typedef struct TokenizerT_ {
 char *tsPtr;
 char *tsPtr2;
 char *sepPtr;
 char *token;
 char *sepArray;
 char *tsArray;
}TokenizerT;

/*
 * TKCreate creates a new TokenizerT object.
 *     separators:  a string containing the characters that separate tokens.
 *     ts:  the string that should be tokenized
 *
 * If the function succeeds, it returns a non-NULL TokenizerT *.
 * Else it returns NULL.
 */
TokenizerT *TKCreate(char *, char *);

/*
 * TKDestroy destroys a TokenizerT object.  It also deallocates
 * all memory allocated dynamically in TKCreate.
 */
void TKDestroy(TokenizerT *);

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token is
 * dynamically allocated.  Caller is responsible for freeing the space
 * once it is no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns NULL.
 */

char *TKGetNextToken(TokenizerT *);


#endif /* TOKENIZER_H_ */


