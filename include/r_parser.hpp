/**Ratscript Norvegicus: Parser<p>
* Parse lexed strings and execute commands.
* @since 2.0
* @author Jason C. McDonald
* @author Michael Parkman
*/

#ifndef R_PARSER_HPP_INCLUDED
#define R_PARSER_HPP_INCLUDED

#include <iostream>
#include <regex>
#include <string>

#include "r_catalog.hpp"
#include "r_conditional.hpp"

using std::cout;
using std::endl;
using std::regex;
using std::string;

using ratscript::r_conditional;

class r_parser{

    public:
        r_parser(r_catalog*);
        bool parseMake(string, string, string, string);
        bool parseIf(vector<r_conditional>&);
        void setLength(int);
    private:
        int inputLength;
        string keybuffer;
        string varName;
        string type;
        int status;
        r_catalog* catalog;
};

#endif // R_PARSER_H_INCLUDED
