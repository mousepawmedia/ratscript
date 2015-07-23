/**Ratscript Norvegicus: Evaluator<p>
* Receives and evaluates a statement. Returns the statements final evaluated result.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_EVALUATOR_POSTFIX_HPP_INCLUDED
#define R_EVALUATOR_POSTFIX_HPP_INCLUDED

#include <string>
#include <vector>
#include <queue>

#include "r_var.hpp"
#include "r_scope.hpp"
#include "r_error.h"
#include "r_math.hpp"
#include "r_utils.h"

using std::queue;

class r_evaluator_postfix
{
    public:
        r_evaluator_postfix(r_scope*);
        r_var* getEvaluation(string);
    private:

        r_var* evaluate(string, queue<string>*, vector<string>*, string*);
        int isOperator(char, char);
        void postFixEvaluation(queue<string>*, vector<string>*);
        string doOperation(string, string, string);
        bool doAssignment(string, string);
        bool doAssignment(r_var*, string);
        string isVariable(string*);
        string getVariable(string);
        r_var* getScopeVariable(string);
        string getStringValue(r_var*);
        r_scope* scope;
};

#endif // R_EVALUATOR_POSTFIX_HPP_INCLUDED
