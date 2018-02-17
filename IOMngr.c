/* Author:      Quentin Goehrig
   Created:     02/12.18
   Resources:   https://linux.die.net/man/3/memmove - memmove
                http://man7.org/linux/man-pages/man3/printf.3.html - print format
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
    //printf("Opened Source with with echoMode: ");
    //printf(echoMode ? "on\n" : "off\n");
    messageCnt, nextPos = 0;
    bufLen = 0;
    curLine = 0;
    return sourceFile != NULL;
}

void printMessages() {
    for(int i = 0; i < messageCnt; i++) {
        printf("       -%c %s\n", (char) i + 65, messages[i].message);
    }
}
bool init = false;
bool finished = false;
bool highlighting = false;
int highlightEndCol = -1;

void CloseSource() {
    if( !finished ) { //flush buffer and print messages
        printf("%s", buffer);
        printf("%s", messages[0].message);
    }
    fclose( sourceFile );
}

bool fillBuffer() {
    //printf("filling\n");
    char * valid = fgets( buffer, MAXLINE, sourceFile );
    bufLen = strlen(buffer);
    curLine++;
    nextPos = 0;
    //printf("valid?=%d", valid != NULL);
    return valid != NULL;
}

char GetSourceChar() {
    if( curLine == 3 ) {
        char * c = strdup(buffer);
        int a = 3;
        int np = nextPos;
        int bl = bufLen;
    }
    if( !init ) {
        fillBuffer();
        init = true;
    }
    if( nextPos == bufLen ) {
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
        messageCnt = 0;
        if( !fillBuffer() ) {
            //printf("cunt\n");
            finished = true;
            //flush buffer
        }
    }
    char c = finished ? EOF : buffer[nextPos];
    nextPos++;
    return c;
}

void PostMessage(int aColumn, int aLength, const char * aMessage) {
    //check prev msg
    //printf("COL: %d, LEN: %d, MSG: %s\n", aColumn, aLength, aMessage);
    struct message cMessage = {
        aColumn, (aColumn + aLength), strdup(aMessage)
    };
    messages[messageCnt] = cMessage;
    messageCnt++;
}

int GetCurrentLine() {
    return curLine;
}

int GetCurrentColumn() {
    return nextPos;
}
