/**Ratscript Norvegicus: Lexer<p>
* Lexes input strings, splitting them into their appropriate parts and sending
* them on to the parser.
* @since 2.0
* @author Jason C. McDonald
* @author Michael Parkman
*/

#ifndef R_LEXER_H
#define R_LEXER_H

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "r_catalog.hpp"
#include "r_conditional.h"
#include "r_error.h"
#include "r_parser.h"
#include "r_utils.h"

using std::cin;
using std::cout;
using std::string;
using std::vector;

using ratscript::r_conditional;
using ratscript::r_utils;

/*STATUS CODES
00: Normal
01: Waiting for comment end

10: Waiting for rest of construct instructions. (?)

11: Waiting for END IF
12: Waiting for END SWITCH
13: Waiting for END LOOP
14: Waiting for END ERROR

21: Waiting for END MAKE
*/

/*TRACK CODES
00: Normal

11: IF track
12: SWITCH track
13: LOOP track
14: TRY track

21: MAKE track
22: DEFINE track
*/

class r_lexer
{
    public:
        r_lexer(int, bool, r_catalog*);

        ///Instance of the utils class.
        r_utils utils;
        //TODO: Shouldn't we use namespace instead of an object?

        ///LexerStatus enumerates the different states for the lexer.
        enum LexerStatus
        {
            ///Lexing as base-level code.
            NORMAL = 0,
            ///Ignoring everything until a certain command.
            SLEEP = 1,
            ///Ignoring everything, because it's a comment.
            COMMENT = 2,
            ///Lexing an if statement.
            IF = 11,
            ///Lexing a switch statement.
            SWITCH = 12,
            ///Lexing a loop statement.
            LOOP = 13,
            ///Lexing a try statement.
            TRY = 14,
            //TODO: We're missing the rest of the code for TRY.
            ///Lexing a make statement.
            MAKE = 21,
        };

        void lex(string);
        LexerStatus checkStatus();
        void flush();

    protected:
    private:
        ///Pointer to the lexer's parser object.
        r_parser* parser;

        ///The lexer's current status.
        LexerStatus status;

        /**The lexer's last status, such as before SLEEP or COMMENT, for
        resuming regular lexing later.*/
        LexerStatus oldstatus;
        ///The old lexer status. We return to this in case of error.
        LexerStatus fallbackStatus;
        //TODO: Fallback needs to be changed to a stack.
        /*TODO: Make sure a fallback status is being stored at the start of EACH
        line of Ratscript code!*/

        void switchStatus(LexerStatus);
        void fallback();

        bool lex_char(char, int);

        //These variables are used globally for lexing.

        ///Count of open parenthesis.
        int openParen;
        ///Flag whether we have encountered the statement header (i.e. IF, TRY)
        bool header;
        ///Rule for lexing track.
        int rule;
        //Flags whether we were expecting subordinated code.
        bool expectingSubordinated;
        ///Flag whether we are ready to exit the construct.
        bool endConstruct;

        /**A vector of r_conditional objects.
        Should be accessed mainly via conditionals.back().
        Watch for undefined behavior! Check if empty() first.*/
        vector<r_conditional> conditionals;

        ///A string to serve as a buffer for eval statements.
        string eval_buffer;
        ///A string to serve as a buffer to meta keywords.
        string meta_buffer;

        /*MAKE LEXING*/

        //We're commenting this out and using literals instead.
        //static const int MAKE_RULE_SIZE = 4;
        //static const char makeRuleSet[MAKE_RULE_SIZE];

        ///Buffer; the name of the variable to MAKE.
        string MAKE_varName;
        ///Buffer; the command between name and type in MAKE, typically "AS".
        string MAKE_varCommand;
        ///Buffer; the data type of the variable to MAKE.
        string MAKE_varType;
        ///Buffer; the initial value of the variable to MAKE.
        string MAKE_varVal;

        void prepMake();
        bool lexMake(char, int);
        void finalizeMake();

        /*IF LEXING*/

        //We're commenting this out and using literals instead.
        //static const int IF_RULE_SIZE = 2;
        //static const char ifRuleSet[IF_RULE_SIZE];

        //We probably will be removing this, in favor of r_conditional.
        /*
        ///Buffer; the conditional expression to eval in the IF statement.
        string IF_eval;
        ///Meta keyword (or command) in IF construct.
        string IF_meta;
        ///Sub-Meta buffer, when we're looking for another IF after a META...
        string IF_submeta;
        */

        void prepIf();
        bool lexIf(char, int);
        void endLineIf();
        void finalizeIf();

        //We probably won't need this; rewrite.
        /*
        //execution lexing
        string EXECUTE_command;
        string EXECUTE_keyword;
        int EXECUTE_rule;

        void prepExecute();
        bool lexecute(char, int);
        */

        ///The total length of the code string (input).
        int inputLength;

        ///If true, the code will not be executed (by the parser).
        bool checkOnly;

        ///Possible comment (We just saw a '>')
        bool possComment;
        ///Possible end-of-comment. (We're in a multiline comment and found a '*')
        bool possEndComment;

        ///Holds the characters we already lexed.
        string buffer;
        ///Holds the characters lexed while looking for a keyword.
        string keybuffer;
        ///The (reserved) keyword we found.
        string keyword;

        int build_keyword(char, int, string*);

        ///TODO: What the heck is this thing??
        bool flagSpace;

};

#endif // R_LEXER_H
