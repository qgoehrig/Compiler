/* Author:      Quentin Goehrig
   Created:     02/12.18
   Resources:   -
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"

#define CONSTRAIN(x,min,max) ( ((x) < (min)) ? (min) : ( ((x) > (max)) ? (max) : (x)))

FILE *sourceFile;
char buffer[MAXLINE];
int bufLen, nextPos;
int curLine;
bool echoMode; //If true,
bool needsDisplay;

struct message {
  int startColumn;
  int endColumn;
  char * message;
};
#define MAX_MESSAGES 26
struct message messages[MAX_MESSAGES];
int messageCnt;

bool OpenSource(const char * aFileName, bool mode) {
    sourceFile = fopen(aFileName, "r");
    echoMode = mode;
    printf("Opened Source with with echoMode: ");
    printf(echoMode ? "on\n" : "off\n");
    messageCnt, bufLen = 0;
    curLine = 1;
    return sourceFile != NULL;
}

void CloseSource() {
    fclose( sourceFile );
    // perform other final actions
}

int lastCharCol = 0;

char GetSourceChar() {
    char cChar = fgetc( sourceFile );
    buffer[bufLen] = cChar;
    if(cChar == '\n') {
        //if (messages exist || echoMode) print,
        //sprintf(buffer);
        printf("%s", buffer);
        memset(buffer, 0, MAXLINE);
        //
        curLine++;
    }
    lastCharCol++;
    return cChar;
}

void PostMessage(int aColumn, int aLength, const char * aMessage) {
    char * sequence = malloc( aLength + 4 ); //
    char start = buffer[aColumn];
    struct message cMessage = {
        aColumn, (aColumn + aLength), strdup(aMessage)
    };
    /*cMessage->message = (aMessage) ? strdup(aMessage) : NULL;
    cMessage->startColumn = aColumn;
    cMessage->endColumn = aColumn + aLength; //don't think need the -1*/
    messages[messageCnt] = cMessage;
    messageCnt++;

}

int GetCurrentLine() {
    return curLine;
}

int GetCurrentColumn() {
    return lastCharCol; //?
}
