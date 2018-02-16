/* Author:      Quentin Goehrig
   Created:     02/12.18
   Resources:   https://linux.die.net/man/3/memmove - memmove
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
    messageCnt, nextPos = 0;
    //bufLen = MAXLINE + (26 * 3); //for the extra highlighted
    bufLen = 0;
    curLine = 1;
    return sourceFile != NULL;
}

void CloseSource() {
    fclose( sourceFile );
    // perform other final actions
}

char GetSourceChar() {
    char cChar = fgetc( sourceFile );
    buffer[bufLen] = cChar;
    nextPos++;
    bufLen++;
    struct message nextMessage;
    if(cChar == '\n') {
        //check for echomode
        char printChar = 65; // A
        int bufferIndex = 0;
        nextPos = 1;
        int msgIndex = 0;
        int nextMsgStart, nextMsgEnd;
        nextMessage = messages[msgIndex];
        nextMsgStart = nextMessage.startColumn;
        nextMsgEnd = nextMessage.endColumn;
        bool onToken = false;
        //printf("%d: ", curLine);

        while( bufferIndex < bufLen ) {
            char cChar = buffer[nextPos];
            // Encounter start of token message
            if( !onToken && (msgIndex < messageCnt) && (nextPos == nextMsgStart) ) {
                // Print letter and start underlining
                printf("\033[7m %c \033[0m\033[4m", printChar);
                onToken = true;
                printChar++;
            }
            // Encounter end of token message
            else if( onToken && (bufferIndex == nextMsgEnd) ) {
                printf("%c", cChar);
                printf("\033[0m"); // Print current char and stop underlining
                onToken = false;
                msgIndex++;
                nextMessage = messages[msgIndex];
                nextMsgStart = nextMessage.startColumn;
                nextMsgEnd = nextMessage.endColumn;
            }
            // Encounter reg character
            else {
                printf("%c", buffer[bufferIndex]);
            }

            bufferIndex++;
            nextPos++;
        }
        // Finish
        memset(buffer, 0, MAXLINE); // Reset buffer
        bufLen, messageCnt, msgIndex, bufferIndex = 0; // Reset vars
        msgIndex = 0;
        curLine++;
        nextPos = 1;
        bufferIndex = 0;
    }
    return cChar;
}

void PostMessage(int aColumn, int aLength, const char * aMessage) {
    //printf("col: %d, len: %d, msg: %s\n", aColumn, aLength, aMessage);
    //printf("%s", aMessage);
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
    return nextPos;
}
