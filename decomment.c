// Library's used for de-commenting Proccess
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Phases the file go through to in order to de-comment it*/
enum Transition
{
    IN_PRINTED_CODE,
    IN_STRING,
    STRING_ESCAPE,
    IN_CHAR,
    CHAR_ESCAPE,
    ABOUT_TO_START_A_COMMENT,
    IN_COMMENT,
    ABOUT_TO_END_A_COMMENT
};

typedef enum Transition Phase;

/* Function Declaration For the Phase handlers */
void countLine(int *line, char c); /* Keeps Track of number of lines */

void finishedHandler(Phase *phase, int lastCommentline); /* To check if there are any unclosed comments in the file */

void printedCodePhaseHandle(Phase *Phase, char c); /* writes the characters it read if it's not in ABOUT_TO_START_A_COMMENT phase and changes the phase */

void aboutToStartACommentPhaseHandler(Phase *phase, char c, int *startLine, int *currentLine); /* checks if it's about to start a comment and tracks the line it started at, in order to check if there are unterminated comments and changes the phase */

void inCharPhaseHandler(Phase *phase, char c); /* writes all characters that are in ' ' and changes phase */

void inStringPhaseHandler(Phase *phase, char c); /* writes the characters that are in "" and changes phase*/

void inCommentPhaseHandler(Phase *phase, char c); /* discard the characters that are in the comments and changes phase to appropriate phase */

void charEscapePhaseHandler(Phase *phase, char c); /* writes all characters that are escaped characters and change the phase */

void stringEscapePhaseHandler(Phase *phase, char c); /* writes the characters that are escaped strings and changes the phase */

void aboutToEndACommentPhaseHandler(Phase *phase, char c); /* checks if it's end of a comment and changes phase */

// Function definations
void countLine(int *line, char c)
{

    if (c == '\n')
    {
        *line += 1; /* add line number*/
    }
}
void printedCodePhaseHandler(Phase *phase, char c)
{
    if (c == '\"')
    {
        *phase = IN_STRING;
    }
    else if (c == '\'')
    {
        *phase = IN_CHAR;
    }
    else if (c == '/')
    {
        *phase = ABOUT_TO_START_A_COMMENT;
    }
    if (*phase != ABOUT_TO_START_A_COMMENT)
    {
        putchar(c);
    }
};

void aboutToStartACommentPhaseHandler(Phase *phase, char c, int *startLine, int *currentLine)
{

    if (c == '*')
    {

        *phase = IN_COMMENT;
        *startLine = *currentLine;
    }
    else if ((c == '/') && *phase == ABOUT_TO_START_A_COMMENT)
    {
        putchar(c);
        *phase = ABOUT_TO_START_A_COMMENT;
    }
    else if (c == '/')
    {
        *phase = ABOUT_TO_START_A_COMMENT;
    }

    else
    {
        *phase = IN_PRINTED_CODE;
    }

    if (*phase == IN_PRINTED_CODE)
    {
        putchar('/');
        putchar(c);
    }
};

void inCharPhaseHandler(Phase *phase, char c)
{
    if (c == '\\')
    {
        *phase = CHAR_ESCAPE;
    }
    else if (c == '\'')
    {
        *phase = IN_PRINTED_CODE;
    }
    putchar(c);
};

void inStringPhaseHandler(Phase *phase, char c)
{
    if (c == '\\')
    {
        *phase = STRING_ESCAPE;
    }
    else if (c == '\"')
    {
        *phase = IN_PRINTED_CODE;
    }
    putchar(c);
};

void inCommentPhaseHandler(Phase *phase, char c)
{
    if (c == '\n')
    {
        putchar(c);
    }
    if (c == '*')
    {
        *phase = ABOUT_TO_END_A_COMMENT;
    }
};
void charEscapePhaseHandler(Phase *phase, char c)
{

    *phase = IN_CHAR;
    putchar(c);
};

void stringEscapePhaseHandler(Phase *phase, char c)
{
    *phase = IN_STRING;
    putchar(c);
};

void aboutToEndACommentPhaseHandler(Phase *phase, char c)
{
    if (c == '/')
    {
        *phase = IN_PRINTED_CODE;
    }
    else if (c == '*')
    {
        *phase = ABOUT_TO_END_A_COMMENT;
    }
    else
    {
        *phase = IN_COMMENT;
    }
    if (*phase == IN_PRINTED_CODE)
    {
        putchar(' '); /* places space in replace of the comment*/
    }
};

void finishedHandler(Phase *phase, int lastline)
{

    if (*phase == IN_COMMENT || *phase == ABOUT_TO_END_A_COMMENT)
    {
        fprintf(stderr, "Error: line %d: unterminated comment\n", lastline);
    }
}

// Reads File as ac character and removes the comments
void main()
{

    Phase phase = IN_PRINTED_CODE;

    int line = 1;        /*  Track of the number of lines  */
    int commentLine = 1; /*  track of where the last unterminated comment started  */
    char c;
    for (c = getchar(); c != EOF; c = getchar())
    {
        countLine(&line, c); /* Incremnts Line Number*/
        switch (phase)
        {
        case IN_PRINTED_CODE:
            printedCodePhaseHandler(&phase, c);
            break;
        case ABOUT_TO_START_A_COMMENT:
            aboutToStartACommentPhaseHandler(&phase, c, &commentLine, &line);
            break;
        case IN_CHAR:
            inCharPhaseHandler(&phase, c);
            break;
        case IN_STRING:
            inStringPhaseHandler(&phase, c);
            break;
        case IN_COMMENT:
            inCommentPhaseHandler(&phase, c);
            break;
        case CHAR_ESCAPE:
            charEscapePhaseHandler(&phase, c);
            break;
        case STRING_ESCAPE:
            stringEscapePhaseHandler(&phase, c);
            break;
        case ABOUT_TO_END_A_COMMENT:
            aboutToEndACommentPhaseHandler(&phase, c);
            break;
        }
    }
    finishedHandler(&phase, commentLine);
}
