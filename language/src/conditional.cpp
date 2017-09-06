#include "ratscript/conditional.hpp"

namespace ratscript
{
    /**Defines a new conditional.
    * @param base: the keyword for the type of conditional
    * this belongs to (i.e. IF, LOOP)
    * @param type: the type of conditional (i.e. ELSE IF, LOOP FOR, CASE)
    * @param eval: the evaluation statement
    */
    r_conditional::r_conditional(string base, string type, string eval)
    {
        baseConditional = base;
        conditionalType = type;
        evalStatement = eval;
    }

    void r_conditional::setEval(string eval)
    {
        evalStatement = eval;
    }

    void r_conditional::queueCode(string lineOfCode)
    {
        //Push line of code to queue.
        codeQueue.push(lineOfCode);
    }

    bool r_conditional::queueIsEmpty()
    {
        bool r = true;
        if(codeQueue.size() > 0)
        {
            r = false;
        }

        return r;
    }

    string r_conditional::dequeCode()
    {
        //Get line of code from front of queue.
        string lineOfCode = codeQueue.front();
        //Remove the line of code from the queue.
        codeQueue.pop();
        //Return line of code.
        return lineOfCode;
    }

    r_conditional::~r_conditional()
    {
        //dtor
    }

}
