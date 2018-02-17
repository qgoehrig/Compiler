/* Author:      Quentin Goehrig
   Created:     02/12.18
   Resources:   https://linux.die.net/man/3/memmove - memmove (not used)
                http://man7.org/linux/man-pages/man3/printf.3.html - printformat
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
bool echoMode;

struct message {
  int startColumn;
  int endColumn;
  char * message;
};
#define MAX_MESSAGES 26
struct message messages[MAX_MESSAGES];
int messageCnt;

bool init = false;
bool finished = false;
bool highlighting = false;
int highlightEndCol = -1;

bool OpenSource(const char * aFileName, bool mode) {
    sourceFile = fopen(aFileName, "r");
    echoMode = mode;
    messageCnt, nextPos, bufLen, curLine = 0;
    return sourceFile != NULL;
}

void printMessages() {
    for(int i = 0; i < messageCnt; i++) {
        printf("       -%c %s\n", (char) i + 65, messages[i].message);
    }
}

// Prints source line, token identifiers and messages according to echo rules
void printLineAndMessages() {
    if( !echoMode || messageCnt > 0) {
        printf("%*d: ", 5, curLine);
    }
    int msgIndex = 0;
    struct message msg = messages[msgIndex];
    for(int i = 0; i < bufLen; i++) {
        if( highlighting && highlightEndCol == i + 1) {
            printf("\033[0m");
            highlighting = false;
        }
        else if(msgIndex < messageCnt && msg.startColumn == i + 1) {
            printf("\033[7m %c \033[0m\033[4m", (char) msgIndex + 65);
            highlightEndCol = msg.endColumn;
            highlighting = true;
            msgIndex++;
            msg = messages[msgIndex];
        }
        if( !echoMode || messageCnt > 0 ) {
            printf("%c", buffer[i]);
        }
    }
    printMessages();
}

void CloseSource() {
    if( !finished ) {
        printLineAndMessages();
    }
    //clear memory?
    fclose( sourceFile );
}

// Returns true when line is not at EOF, otherwise false
bool fillBuffer() {
    char * valid = fgets( buffer, MAXLINE, sourceFile );
    bufLen = strlen(buffer);
    curLine++;
    nextPos = 0;
    return valid != NULL;
}

char GetSourceChar() {
    if( !init ) {
        fillBuffer();
        init = true;
    }
    if( nextPos == bufLen ) {
        printLineAndMessages();
        messageCnt = 0;
        if( !fillBuffer() ) {
            finished = true;
        }
    }
    char c = finished ? EOF : buffer[nextPos];
    nextPos++;
    return c;
}

// Checks for overlaping messages and 26 message limit before posting message
void PostMessage(int aColumn, int aLength, const char * aMessage) {
    int prevMsgEndCol = 0;
    if( messageCnt > 0) {
        prevMsgEndCol = messages[messageCnt - 1].endColumn;
    }
    if( messageCnt == 0 || ((messageCnt < 26) && (prevMsgEndCol <= aColumn)) ){
        struct message cMessage = {
            aColumn, (aColumn + aLength), strdup(aMessage)
        };
        messages[messageCnt] = cMessage;
        messageCnt++;
    }
}

int GetCurrentLine() {
    return curLine;
}

int GetCurrentColumn() {
    return nextPos;
}
