/*RATSCRIPT 2.X NORVEGICUS
Reserved

Manages reserved keywords in Ratscript.
*/

#ifndef R_RESERVED_H
#define R_RESERVED_H

#include <iostream>
#include <string>

#include "r_utils.h"

using namespace std;

class r_reserved
{
    public:
        r_reserved();

        enum ReserveType
        {
            NONE = 0,
            KEYWORD = 1,
            META = 2,
            COMMAND = 3,
        };

        ReserveType checkReserved(string);
    protected:
    private:
        //Keywords open (and close) constructs.
        static const int COUNT_KEYWORD = 5;
        string SYS_KEYWORD[COUNT_KEYWORD] = {"ERROR", "IF", "LOOP", "MAKE", "SWITCH"};
        //Metas modify constructs.
        static const int COUNT_META = 7;
        string SYS_META[COUNT_META] = {"CASE", "DEFAULT", "DO", "ELSE", "END", "FOR", "WHILE"};
        //Commands perform reserved actions, usually on constructs.
        static const int COUNT_COMMAND = 2;
        string SYS_COMMAND[COUNT_COMMAND] = {"BREAK", "CANCEL"};
};

#endif // R_RESERVED_H
