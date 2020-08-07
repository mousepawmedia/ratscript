#include "ratscript/evaluator_postfix.hpp"

///Getting a random 0 in the evaluation


//constructor used to gain access to the scope
r_evaluator_postfix::r_evaluator_postfix(r_scope* scope)
{
    //set up the scope
    this->scope = scope;
}

//Used to start the valuation of a statment
//each "evaluation" requires two vectors. One to store operators and the other to store operands
//returns the final result
r_var* r_evaluator_postfix::getEvaluation(string statement)
{
    //vector to store the operators
    vector<string>* theStack = new vector<string>();
    //vectore to store the variables/operands
    queue<string>* theQueue = new queue<string>();
    //buffer to help determine the variables in the statement
    string* buffer = new string("");

    //return the final evaluation of the statement solved by "evaluate"
    return evaluate(r_utils::cleanString(statement), theQueue, theStack, buffer);
}

r_var* r_evaluator_postfix::evaluate(string statement, queue<string>* thePostfixExpression, vector<string>* operatorStack, string* buffer)
{
    //loops through the entire statement
    for(unsigned int i = 0; i < statement.size(); i++)
    {
        //checks to see if the current character(s) is(are) an operator. Stores the number of characters that are
        int numOperators = 0;
        if(i < statement.size() - 1)
        {
            //if this is not the last character in the statement call isOperator normally
            numOperators = isOperator(statement.at(i), statement.at(i+1));
        }
        else
        {
            //if this is the last character pass in a temporary character that is not an operator
            numOperators = isOperator(statement.at(i), 'e');
        }
        switch(numOperators)
        {
            case 0:
            {
                *buffer += statement.at(i);
                    if(i == statement.size() - 1)
                {
                    //clean the string then check to see if it is a variable
                    string r = getVariable(r_utils::cleanString(*buffer));
                    //TODO make sure it is a variable before pushing into the vector. If it isn't a variable throw an error
                    //Push the variable to the back of the vector if there is a variable
                    thePostfixExpression->push(r);
                }
                break;
            }
            case 1:
            case 2:
            {
                //anything between operators is a variable
                string r = isVariable(buffer);
                //if r is null there is no variable
                if(r.size() > 0)
                {
                    //if there is a variable in the buffer push it to the back of the postfix vector
                    thePostfixExpression->push(r);
                }
                string temp = statement.substr(i, numOperators);
                //push that number of characters onto the back of the operators vector
                bool keepGoing = true;
                while(keepGoing)
                {
                    if(operatorStack->size() == 0 || r_utils::getPrecedence(operatorStack->back()) > r_utils::getPrecedence(temp) || operatorStack->back() == "(")
                    {
                        operatorStack->push_back(temp);
                        keepGoing = false;
                    }
                    else
                    {
                        thePostfixExpression->push(operatorStack->back());
                        operatorStack->pop_back();
                    }
                }
                //if there are two characters in this operator it makes sure not to check the second character twice
                i+=(numOperators-1);
                break;
            }
            //This is if the operator is a ')'
            case 3:
            {
                //anything between operators is a variable
                string r = isVariable(buffer);
                //if r is null there is no variable
                if(r.size() > 0)
                {
                    //if there is a variable in the buffer push it to the back of the postfix vector
                    thePostfixExpression->push(r);
                }

                bool keepGoing = true;
                //keep going until you run into an open parenthesis
                while(keepGoing)
                {
                    //if there is nothing else in the stack, a parenthesis is missing
                    if(operatorStack->size() == 0)
                    {
                        r_error e(r_error::EVAL_INCOMPLETE, "Missing Open Parenthesis");
                        keepGoing = false;
                    }
                    else
                    {
                        //check the operator at the back
                        string temp = operatorStack->back();
                        if(temp == "(")
                        {
                            //stop checking if there is an open parenthesis
                            keepGoing = false;
                        }
                        else
                        {
                            //push the operator at the back of the stack into the queue
                            thePostfixExpression->push(temp);
                        }
                        //remove the operator at the back of the stack
                        operatorStack->pop_back();
                    }
                }
            }
        }
    }

    //to finish up the postfix expression, place all operators that remain in the stack into the queue
    while(operatorStack->size() > 0)
    {
        thePostfixExpression->push(operatorStack->back());
        operatorStack->pop_back();
    }

    //Then evaluate this postfix expression that has been created
    postFixEvaluation(thePostfixExpression, operatorStack);

    //if there is more than 1 variable remaining in the queue, something is missing
    if(thePostfixExpression->size() > 1)
    {
        r_error e(r_error::EVAL_INCOMPLETE, "Missing an operator");
    }
    //print out the operators and the postfix that are stored in the vectors
    //used for debugging
    cout << "The solution: " << operatorStack->back() << endl;
    //end of print statements
    //return the final result. Since this isn't finished yet, return a 0
    return NULL;
}

//returns the number of characters in the operator.
//if there is no operator return a 0
int r_evaluator_postfix::isOperator(char currChar, char nextChar)
{
    //the number of characters that are operators.
    //Could be 1 like +, Could be 2 like ++, or if it is not an operator defaults to 0
    int numChar = 0;

    //switch based on the current character in the statement
    switch(currChar)
    {
        //if one of these characters there is a possibility for there to be a second character in the operator
        case '+':
        case '-':
        case '=':
            //if the next character is the same character return 2
            if(nextChar == currChar)
            {
                numChar = 2;
            }
            //if the next character is different then this is a single character operator return 1
            else
            {
                numChar = 1;
            }
            break;
        //these characters are all 1 character operators that will execute right away return 3
        case '*':
        case '/':
        case '\\':
        case '%':
        case '^':
        case'(':
            numChar = 1;
            break;
        //these characters can have a second character in the operator
        case '<':
        case '>':
            //if the character is followed by an equals sign return 2
            if(nextChar == '=')
            {
                numChar = 2;
            }
            //otherwise just return 1 because the character is a single character operator
            else
            {
                numChar = 1;
            }
            break;
        //must be followed by a second character in order to be an operator
        case '&':
        case '|':
        {
            //if the character set is &&, ||, or &| return 2
            if(nextChar == currChar || nextChar == '|')
            {
                numChar = 2;
            }
            //TODO Ask if we want to throw an error or not
            //otherwise return a 0 because it is not an operator
            else
            {
                numChar = 0;
            }
            break;
        }
        case ')':
        {
            numChar = 3;
            break;
        }
        //by default this character set is not an operator so return 0
        default:
            numChar = 0;
    }
    //return the value determined by the switch
    return numChar;
}

//determines if there is a variable here
//if the current character isn't a space then the variable is still being built so return a NULL reference
//If the character is a space then get the r_var* for the variable and return it
string r_evaluator_postfix::isVariable(string* buffer)
{
    //stores the r_var* which is to be found
    string r = "";
    if(buffer != NULL && *buffer != "")
    {
        string cleanedString = r_utils::cleanString(*buffer);
        if(cleanedString.size() > 0)
        {
            r = getVariable(cleanedString);
            *buffer = "";
        }
        else
        {
            r = "";
        }
    }
    else
    {
        //if there is nothing there return null
        r = "";
    }
    //return the r_var* that was found by this method
    return r;
}

void r_evaluator_postfix::postFixEvaluation(queue<string>* thePostfixExpression, vector<string>* operandStack)
{
    //while there is still more to be evaluated in the postfix expression
    while(thePostfixExpression->size() > 0)
    {
        //get the next item in the expression and remove it from the queue
        string temp = thePostfixExpression->front();
        thePostfixExpression->pop();
        //if the item is and operand
        if(r_utils::getPrecedence(temp) == 0)
        {
            //push the operand into the stack
            operandStack->push_back(temp);
        }
        //if the operator is the assignment operator
        else if(temp == "=")
        {
            //the value to be assigned
            string var2 = operandStack->back();
            operandStack->pop_back();
            //the variable to be assigned a new value
            string var1 = operandStack->back();
            operandStack->pop_back();

            r_var* variable1 = getScopeVariable(var1);

            //do the assignment
            bool result = variable1->setValue(var2);
            //if the assignment was successful
            if(result)
            {
                //TODO Push back the value of the variable or push back the variable name itself?
                operandStack->push_back(var1);
            }
            else
            {
                //If the assignment failed throw an error
                r_error e(r_error::TYPE_ERROR, "Assignment go wrong");
                e.throwError();
            }
        }
        //if the operator is the increment operator
        else if(temp == "++")
        {
            //the value to be incremented
            string variable = operandStack->back();
            operandStack->pop_back();
            try
            {
                //used to convert the number to a string
                stringstream s;
                //convert the number + 1 to a string. Throws an error if the variable isn't a number
                s << stod(variable) + 1;
                //push the new value to the back of the stack
                operandStack->push_back(s.str());
            }
            //throw an error if it is not a numeric value
            catch(const std::invalid_argument& ia)
            {
                r_error e(r_error::TYPE_ERROR, "Increment variable cast gone wrong");
                e.throwError();
            }

        }
        //if the operator is the decrement operator
        else if(temp == "--")
        {
            //the value to be decremented
            string variable = operandStack->back();
            operandStack->pop_back();
            try
            {
                //used to convert the number to a string
                stringstream s;
                //convert the number - 1 to a string. Throws an error if the variable isn't a number
                s << stod(variable) - 1;
                //push the new value to the back of the stack
                operandStack->push_back(s.str());
            }
            //throws an error if it is not a numeric value
            catch(const std::invalid_argument& ia)
            {
                r_error e(r_error::TYPE_ERROR, "Decrement variable cast gone wrong");
                e.throwError();
            }
        }
        //if it is another operator
        else
        {
            //the right hand value
            string var2 = operandStack->back();
            operandStack->pop_back();
            //the left hand value
            string var1 = operandStack->back();
            operandStack->pop_back();
            //do the operation between the two values
            string result = doOperation(var1, var2, temp);
            //push the resulting value onto the back of the stack
            operandStack->push_back(result);
        }
    }
}

//does the desired operation between the two string values passed in
//throws errors if the values are not of the right type for the operator
//returns the new string value after the operation has been completed
//returns an empty string if there was an error
string r_evaluator_postfix::doOperation(string var1, string var2, string theOperator)
{
    //the value to return
    string r = "";
    //switch by the first character of the operator
    switch(theOperator.at(0))
    {
        case '+':
        {
            //if the operator is a +
            r = r_math::add(var1, var2);
            break;
        }
        case '-':
        {
            //if the operator is a -
            r = r_math::subtract(var1, var2);
            break;
        }
        case '*':
        {
            //do multiplication
            r = r_math::multiply(var1, var2);
            break;
        }
        case '/':
        {
            //do floating point division
            r = r_math::floatingPointDivision(var1, var2);
            break;
        }
        case '\\':
        {
            //do interger division
            r = r_math::integerDivision(var1, var2);
            break;
        }
        case '^':
        {
            //take the first variable to the power of the second variable
            r = r_math::exponentiate(var1, var2);
            break;
        }
        case '%':
        {
            //mod the first variable by the second
            r = r_math::mod(var1, var2);
            break;
        }
        case '&':
        {
            if(theOperator.at(1) == '&')
            {
                //if the operator is a &&
                if(var1 == "true" && var2 == "true")
                {
                    r = "true";
                }
                else
                {
                    r = "false";
                }
            }
            else
            {
                //if the operator is a &|
                if(var1 == "true" || var2 == "true")
                {
                    r = "true";
                }
                else
                {
                    r = "false";
                }
            }
            break;
        }
        case '|':
        {
            //if the operator is a ||
            if(var1 == var2)
            {
                r = "false";
            }
            else
            {
                r = "true";
            }
            break;
        }
        case '<':
        {
            //TODO Comparisons for strings. Search types?
            try
            {
                //left hand value
                double variable1 = stod(var1);
                //right hand value
                double variable2 = stod(var2);

                //if operator is <       OR  if (operator is <=     AND the values are equal)
                if(variable1 < variable2 || (theOperator.size() > 1 && variable1 == variable2))
                {
                    r = "true";
                }
                else
                {
                    r = "false";
                }
            }
            //Throws error if one of them is not a numeric value
            catch(const std::invalid_argument& ia)
            {
                r_error e(r_error::TYPE_ERROR, "The type needs to be some sort of number");
            }
            break;
        }
        case '>':
        {
            try
            {
                //left hand value
                double variable1 = stod(var1);
                //right hand value
                double variable2 = stod(var2);
                //if operator is >       OR if (operator is >=      AND the values are equal)
                if(variable1 > variable2 || (theOperator.size() > 1 && variable1 == variable2))
                {
                    r = "true";
                }
                else
                {
                    r = "false";
                }
            }
            //Throws error if one of them is not a numeric value
            catch(const std::invalid_argument& ia)
            {
                r_error e(r_error::TYPE_ERROR, "The type needs to be some sort of number");
            }
            break;
        }
        case '=':
        {
            //determines if the two values are equal
            bool returnVal = false;
            try
            {

                //left hand value
                double variable1 = stod(var1);
                //right hand value
                double variable2 = stod(var2);
                //if values are equal
                if(variable1 == variable2)
                {
                    returnVal = true;
                }
            }
            //if one of the values is not numeric, compare as strings
            catch(const std::invalid_argument& ia)
            {
                //if the two strings are equal
                if(var1 == var2)
                {
                    returnVal = true;
                }
            }
            //convert to return value to a string
            if(returnVal)
            {
                r = "true";
            }
            else
            {
                r = "false";
            }
            break;
        }
        case '(':
        {
            r = var1;
            break;
        }
    }
    return r;
}

//sets the first value equal to the second value
bool r_evaluator_postfix::doAssignment(string var1, string var2)
{
    //TODO Think of any errors that might need to be thrown
    var1 = var2;
    return true;
}

//uses the r_var's setValue(string) method to set the first variable equal to the second value
//returns true if they are compatible datatypes
bool r_evaluator_postfix::doAssignment(r_var* var1, string var2)
{
    //if compatible datatypes
    if(var1->setValue(var2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//pass in a string that is a variable or literal
//returns that variable or a variable representing the literal
string r_evaluator_postfix::getVariable(string statement)
{
    //stores the name of the variable, if it is a variable
    string varName;
    if(statement[0] == '@')
    {
        //stores the variable
        r_var* variable;
        //the variable name is equal to
        varName = statement.substr(1, statement.size() - 1);
        //retrieves the variable that has the name discovered above
        variable = getScopeVariable(varName);
        //return the variables value as a string
        return variable->toString();
    }
    //if it is requesting direct access
    else if(statement[0] == 'r' && statement[1] == ':')
    {
        //return the string
        return statement;
    }
    //if the current character is a " this denotes a literal string until the next " is reached
    else if(statement[0] == '"')
    {
        string temp = statement.substr(1,statement.find('"', 1) - 1);
        return temp;
    }
    else if(statement == "true")
    {
        return statement;
    }
    else if (statement == "false")
    {
        return statement;
    }
    else if(statement == "maybe")
    {
        return statement;
    }
    //loops through the characters in the statement
    for(unsigned int i = 0; i < statement.size(); i++)
    {
        //if the current character is @ this denotes a variable

        //if the current character is a . this denotes a literal double
        if(statement[i] == '.')
        {
            return statement;
        }
    }
    //if the statement hasn't matched anything it should be an integer
    //TODO check to see if it is an integer otherwise throw error
    return statement;
}

r_var* r_evaluator_postfix::getScopeVariable(string scopeVar)
{
    //a pointer to the current scope
    r_scope* currScope = scope;
    //keeps track of the index of the latest colon being looked at
    int colon = -1;
    //while there are more statements to be looking at
    //either more subScopes or the variable to get
    while((colon = scopeVar.find(':', colon + 1)) != -1)
    {
        //TODO Throw error if there is no open curly brace?
        //get the bracket that comes right after the colon
        int bracket = scopeVar.find('{', colon);
        //if the next string is a scope name
        if(scopeVar[colon + 1] == 's')
        {
            //get the string located between the two brackets
            currScope = currScope->getScope(scopeVar.substr(bracket + 1, scopeVar.find('}', bracket + 1) - (bracket + 1)));
        }
        //should be a 'c' otherwise
        //else the next string is the name of the variable to get from the catalog
        else
        {
            //get the string located between the two brackets
            return currScope->retrieve(scopeVar.substr(bracket + 1, scopeVar.find('}', bracket + 1) - (bracket + 1)));
        }
    }
    //if it gets this far there is something wrong.
    //create and throw an error. return NULL
    r_error e(r_error::ACCESS_SCOPE, "Something wrong with getting the variable from the scope");
    e.throwError();
    return NULL;
}

//pass in a variable
//determines what type of variable it is
//prints out the value stored in the variable
string r_evaluator_postfix::getStringValue(r_var* variable)
{
    //If the variable is not null/empty
    if(variable != 0)
    {
        return variable->toString();
    }
    else
    {
        cout << "r_evaluator returned null" << endl;
        return "";
    }
}
