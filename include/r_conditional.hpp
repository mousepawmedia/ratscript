/**Ratscript Norvegicus: Conditional<p>
* A conditional object contains a header and subordinated code.
* IF, ELSE IF, LOOP FOR, CASE (SWTICH), etc.
* These get passed to the parser.
* @since 2.0
* @author Jason C. McDonald
*/

#ifndef R_CONDITIONAL_HPP
#define R_CONDITIONAL_HPP

#include <iostream>
#include <queue>
#include <string>
#include "r_utils.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::string;
using ratscript::r_utils;

namespace ratscript
{
    class r_conditional
    {
        public:
            r_conditional(string, string, string = "");
            ~r_conditional();

            string getEval() {return evalStatement;}
            void setEval(string);

            string getBase() {return evalStatement;}
            string getType() {return conditionalType;}

            void queueCode(string);
            bool queueIsEmpty();
            string dequeCode();
        protected:
        private:
            ///The base conditional keyword (i.e. IF, SWITCH)
            string baseConditional;
            ///The exact conditional type (i.e. CASE, ELSE IF).
            string conditionalType;
            ///The eval statement for the conditional, if any.
            string evalStatement;

            queue<string> codeQueue;
    };
}

#endif // R_CONDITIONAL_H
