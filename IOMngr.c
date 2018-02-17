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
    //printf("Opened Source with with echoMode: ");
    //printf(echoMode ? "on\n" : "off\n");
    messageCnt, nextPos = 0;
    bufLen = 0;
    curLine = 0;
    return sourceFile != NULL;
}

void CloseSource() {
    fclose( sourceFile ); // perform other final actions
}

void printMessages() {
    for(int i = 0; i < messageCnt; i++) {
        printf("       -%c %s\n", (char) i + 65, messages[i].message);
    }
}
bool init = false;
bool finished = false;
bool highlight = false;

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
    if( !init ) {
        fillBuffer();
        init = true;
    }
    if( nextPos == bufLen ) {
        printf("    %d: ", curLine);
        int msgIndex = 0;
        struct message msg = messages[msgIndex];
        for(int i = 0; i < bufLen; i++) {
            if(msgIndex < messageCnt && msg.startColumn == i + 1) {
                printf("\033[7m %c \033[0m", (char) msgIndex + 65);
                msgIndex++;
                msg = messages[msgIndex];
            }
            printf("%c", buffer[i]);
        }
        printMessages();
        messageCnt = 0;
        if( !fillBuffer() ) {
            //printf("DONE YOU PIECE OF SHIT\n");
            finished = true;
        }
    }
    char c = finished ? EOF : buffer[nextPos];
    //printf("%c", c);
    // if( c == '\n' ) {
    //     // printf("    %d: msgs: %d", curLine, messageCnt);
    //     // int msgIndex = 0;
    //     // struct message msg = messages[msgIndex];
    //     for(int i = 0; i < bufLen; i++) {
    //         // if(msgIndex < messageCnt && msg.startColumn == i + 1) {
    //         //     printf("\033[7m %c \033[0m", (char) msgIndex + 65);
    //         //     msgIndex++;
    //         //     msg = messages[msgIndex];
    //         // }
    //         printf("%c", buffer[i]);
    //     }
    //     //printf("    %d: %s", curLine, buffer);
    //     messageCnt = 0;
    // }
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
