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

// struct message getNextMessage(int msgIndex) {
//     if(messages[msgIndex]) {
//         return messages[msgIndex];
//     }
//     return NU
// }

char GetSourceChar() {
    char cChar = fgetc( sourceFile );
    buffer[nextPos] = cChar;
    nextPos++;
    bufLen++;
    struct message nextMessage;
    if(cChar == '\n') {
        //check for echomode
        char printChar = 65; // A
        int bufferIndex = 0;
        int readMsgIndex = 0;
        int nextMsgStart, nextMsgEnd;
        nextMessage = messages[readMsgIndex];
        nextMsgStart = nextMessage.startColumn;
        nextMsgEnd = nextMessage.endColumn;
        bool onToken = false;

        printf("%d: ", curLine);
        while( bufferIndex < bufLen ) {
            if( onToken ) {
                printf("%c", buffer[bufferIndex]);
                if( bufferIndex == nextMsgEnd ) {
                    printf("\033[0m");
                    onToken = false;
                    readMsgIndex++;
                    nextMessage = messages[readMsgIndex];
                    nextMsgStart = nextMessage.startColumn;
                    nextMsgEnd = nextMessage.endColumn;
                }
                bufferIndex++;
            }
            else if( bufferIndex == nextMsgStart ) {
                //print letter and start highlighting (ditch cast)
                printf("\033[7m %c \033[0m\033[4m", printChar);
                onToken = true;
                printChar++;
                bufferIndex++;
            }
            else {
                printf("%c", buffer[bufferIndex]);
                bufferIndex++;
            }

        }
        printf("\n");
        memset(buffer, 0, MAXLINE); // Reset buffer
        curLine++;
        nextPos = 0;
    }
    return cChar;
}

void PostMessage(int aColumn, int aLength, const char * aMessage) {
    //printf("MESSAGE: %s, @COL: %d, LENGTH: %d\n", aMessage, aColumn, aLength);
    //printf("ORIG: %d, CONSTRAIN:%d\n", aColumn, CONSTRAIN(aColumn, 0, MAXLINE));
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
