#include "ratscript/math.hpp"

//adds to variables together and returns the output
//if one variable is a string, return a concatenation
//if neither of the variables is a string, return the sum
r_var* r_math::add(r_var* var1, r_var* var2)
{
    //if the first variable is a string
    if(var1->getType() == r_utils::S)
    {
        //variable to convert a number or integer to a string
        ostringstream os;
        //create a variable temp to store the value of the second variable
        string temp;
        //if the second variable is an integer
        if(var2->getType() == r_utils::I)
        {
            //convert the integer value of the second variable to a string
           os << ((r_integer*)var2)->getValue();
           //save the converted value in the variable temp
           temp = os.str();
        }
        //if the second variable is a number
        else if(var2->getType() == r_utils::N)
        {
            //convert the number value of the second variable to a string
            os << ((r_number*)var2)->getValue();
            //save the converted value in the variable temp
            temp = os.str();
        }
        //if the second variable is a string
        else if(var2->getType() == r_utils::S)
        {
            //store the value of the string into temp
            temp = ((r_string*)var2)->convert();
        }
        //if the type of the second variable does not match string, number or integer
        else
        {
            //throw an error
            r_error e(r_error::EVAL_INVALID_TYPE, "The variable entered was not of type string, number or integer.");
            e.throwError();
        }
        //return a new string after the concatenation
        return new r_string("temp", ((r_string*) var1)->convert() + temp);
    }
    else if(var2->getType() == r_utils::S)
    {
        //variable to convert a number or integer to a string
        ostringstream os;
        //create a variable temp to store the value of the first variable
        string temp;
        //if the first variable is an integer
        if(var1->getType() == r_utils::I)
        {
            //convert the integer value of the first variable to a string
           os << ((r_integer*)var1)->getValue();
           //save the converted value in the variable temp
           temp = os.str();
        }
        //if the first variable is a number
        else if(var1->getType() == r_utils::N)
        {
            //convert the number value of the first variable to a string
            os << ((r_number*)var1)->getValue();
            //save the converted value in the variable temp
            temp = os.str();
        }
        //if the first variable is a string
        else if(var1->getType() == r_utils::S)
        {
            //store the value of the string into temp
            temp = ((r_string*)var1)->convert();
        }
        //if the type of the first variable does not match string, number or integer
        else
        {
            //throw an error
            r_error e(r_error::EVAL_INVALID_TYPE, "The variable entered was not of type string, number or integer.");
            e.throwError();
        }
        //return a new string after the concatenation
        return new r_string("temp", temp + ((r_string*) var2)->convert());
    }
    //if neither of the variables are a string
    else
    {
        //if both variables are integers
        if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
        {
            return new r_integer("temp",((r_integer*)var1)->getValue() + ((r_integer*)var2)->getValue());
        }
        //if both variables are numbers
        else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
        {
            return new r_integer("temp",((r_number*)var1)->getValue() + ((r_number*)var2)->getValue());
        }
        //if the first variable is an integer and the second is a number
        else if(var1->getType() == r_utils::I && var2->getType() == r_utils::N)
        {
            return new r_integer("temp",((r_integer*)var1)->getValue() + ((r_number*)var2)->getValue());
        }
        //if the first variable is a number and the second is an integer
        else if(var1->getType() == r_utils::N && var2->getType() == r_utils::I)
        {
            return new r_integer("temp",((r_number*)var1)->getValue() + ((r_integer*)var2)->getValue());
        }
        //if the type of the first and the second variables do not match string, number or integer
        else
        {
            //throw an error
            r_error e(r_error::EVAL_INVALID_TYPE, "The variable entered was not of type string, number or integer.");
            e.throwError();
        }
    }
}

//adds two values together
//return a string representation of the final value
string r_math::add(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value back to a string
        s << (var1 + var2) ;
        return s.str();
    }
    //if one of the values can not be converted to a double
    catch(const std::invalid_argument& ia)
    {
        //concatenate the two strings and return the new value
        return variable1 + variable2;
    }
}

//Takes in two variables
//Subtracts the two variables and returns the appropriate value
//If there is a decimal return a floating point otherwise return an integer
//Throws error if one or both of the variables are not an integer or floating point number
r_var* r_math::subtract(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        int result = temp1->getValue() - temp2->getValue();
        return new r_integer("temp", result);
    }
    //If both variables are floating point numbers
    else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() - temp2->getValue();
        return new r_number("temp", result);
    }
    //If the first variables is a floating point number
    else if(var1->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = temp1->getValue() - temp2->getValue();
        return new r_number("temp", result);
    }
    //If the second variable is a floating point number
    else if(var2->getType() == r_utils::N)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() - temp2->getValue();
        return new r_number("temp", result);
    }
    //If neither variable is an integer or a floating point number
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to subtract");
        e.throwError();
        return 0;
    }
}

//subtract two values
//returns a string representation of the new value
string r_math::subtract(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert to new value back to a string
        s << (var1 - var2);
        return s.str();
    }
    //if one of the values can not be converted to a double
    catch(const std::invalid_argument& ia)
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to subtract");
        e.throwError();
        return 0;
    }
}

//Takes in two variables
//Multiplies the variables and returns the correct value
//Throws an error if one or both of the variables are not an integer or floating point number
r_var* r_math::multiply(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        int result = temp1->getValue() * temp2->getValue();
        return new r_integer("temp", result);
    }
    //If both variables are floating point numbers
    else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() * temp2->getValue();
        return new r_number("temp", result);
    }
    //If the first variables is a floating point number
    else if(var1->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = temp1->getValue() * temp2->getValue();
        return new r_number("temp", result);
    }
    //If the second variable is a floating point number
    else if(var2->getType() == r_utils::N)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() * temp2->getValue();
        return new r_number("temp", result);
    }
    //If neither variable is an integer or a floating point number
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return 0;
    }
}

//multiplies two numbers together
//returns a string representation of the new value
string r_math::multiply(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value to a string
        s << (var1 * var2);
        return s.str();
    }
    //if one of the values can not be converted to a double
    catch(const std::invalid_argument& ia)
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return "";
    }
}

//Takes in two variables
//Divides the first variable by the second and returns an integer
//Throws an error if one or both of the variables are not an integer or floating point number
r_integer* r_math::integerDivision(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        int result = temp1->getValue() / temp2->getValue();
        return new r_integer("temp", result);
    }
    //If both variables are floating point numbers
    else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_number* temp2 = ((r_number*) var2);
        int result = temp1->getValue() / temp2->getValue();
        return new r_integer("temp", result);
    }
    //If the first variables is a floating point number
    else if(var1->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        int result = temp1->getValue() / temp2->getValue();
        return new r_integer("temp", result);
    }
    //If the second variable is a floating point number
    else if(var2->getType() == r_utils::N)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_number* temp2 = ((r_number*) var2);
        int result = temp1->getValue() / temp2->getValue();
        return new r_integer("temp", result);
    }
    //If neither variable is an integer or a floating point number
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to divide");
        e.throwError();
        return 0;
    }
}

//divide two numbers and convert them back to an integer
//return a string representation of the new value
string r_math::integerDivision(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value to a string
        s << ((int)(var1 / var2));
        return s.str();
    }
    //if one of the values can not be converted to a string
    catch(const std::invalid_argument& ia)
    {
        //create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return "";
    }
}

//Takes in two variables
//Divides the first variable by the second and returns a floating point number
//Throws an error if one or both of the variables are not an integer or floating point number
r_number* r_math::floatingPointDivision(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = (double)temp1->getValue() / temp2->getValue();
        return new r_number("temp", result);
    }
    //If both variables are floating point numbers
    else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() / temp2->getValue();
        return new r_number("temp", result);
    }
    //If the first variables is a floating point number
    else if(var1->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = temp1->getValue() / temp2->getValue();
        return new r_number("temp", result);
    }
    //If the second variable is a floating point number
    else if(var2->getType() == r_utils::N)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = temp1->getValue() / temp2->getValue();
        return new r_number("temp", result);
    }
    //If neither variable is an integer or a floating point number
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to divide");
        e.throwError();
        return 0;
    }
}

//divide the first number by the second number
//return a string representation of the new value
string r_math::floatingPointDivision(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value to a string
        s << (var1 / var2);
        return s.str();
    }
    //if one of the values can not be converted to a double
    catch(const std::invalid_argument& ia)
    {
        //create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return "";
    }
}

//Takes in two variables
//Takes the first variable to the power of the second variable
//Returns either an integer or floating point number
r_var* r_math::exponentiate(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = pow(temp1->getValue(),temp2->getValue());
        return new r_integer("temp", result);
    }
    //If both variables are floating point numbers
    else if(var1->getType() == r_utils::N && var2->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = pow(temp1->getValue(), temp2->getValue());
        return new r_number("temp", result);
    }
    //If the first variables is a floating point number
    else if(var1->getType() == r_utils::N)
    {
        r_number* temp1 = ((r_number*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        double result = pow(temp1->getValue(), temp2->getValue());
        return new r_number("temp", result);
    }
    //If the second variable is a floating point number
    else if(var2->getType() == r_utils::N)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_number* temp2 = ((r_number*) var2);
        double result = pow(temp1->getValue(), temp2->getValue());
        return new r_number("temp", result);
    }
    //If neither variable is an integer or a floating point number
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to use exponents");
        e.throwError();
        return 0;
    }
}

//take the first value to the power of the second value
//return a string representation of the new value
string r_math::exponentiate(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a double
        double var1 = stod(variable1);
        //convert the right string value to a double
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value to a string
        s << pow(var1, var2);
        return s.str();
    }
    //if one of the values can not be converted to double
    catch(const std::invalid_argument& ia)
    {
        //create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return "";
    }
}

//Takes in two variables
//Modulus the first variable by the second returns an integer
//Throws an error if one or both of the variables are not an integer
r_var* r_math::mod(r_var* var1, r_var* var2)
{
    //If both variables are integers
    if(var1->getType() == r_utils::I && var2->getType() == r_utils::I)
    {
        r_integer* temp1 = ((r_integer*) var1);
        r_integer* temp2 = ((r_integer*) var2);
        int result = temp1->getValue() % temp2->getValue();
        return new r_integer("temp", result);
    }
    //If one or both variable are not an integer
    else
    {
        //Create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of Integer to use the modulus");
        e.throwError();
        return 0;
    }
}

//mod the first value by the second value
//return a string representation of the new value
string r_math::mod(string variable1, string variable2)
{
    try
    {
        //convert the left string value to a string
        double var1 = stod(variable1);
        //convert the right string value to a string
        double var2 = stod(variable2);
        stringstream s;
        //convert the new value to a string
        s << fmod(var1, var2);
        return s.str();
    }
    //if one of the values can not be converted to a double
    catch(const std::invalid_argument& ia)
    {
        //create and throw an error
        r_error e(r_error::EVAL_INVALID_TYPE, "Must be a type of number to multiply");
        e.throwError();
        return "";
    }
}
