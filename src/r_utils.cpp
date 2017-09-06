#include "r_utils.hpp"

namespace ratscript
{
    //KEEP THESE ALPHABETICAL, otherwise binary search (the default method) fails.
    const string r_utils::SYS_KEYWORD[COUNT_KEYWORD] = {"IF", "LOOP", "MAKE"
                                                        "SWITCH", "TRY"};
    const string r_utils::SYS_META[COUNT_META] = {"CASE", "CATCH", "DEFAULT",
                                                  "DO", "ELSE", "END", "FOR",
                                                  "WHILE"};
    const string r_utils::SYS_COMMAND[COUNT_COMMAND] = {"BREAK", "CANCEL"};

    r_utils::r_utils()
    {
        //cstr
    }

//    bool r_utils::createVariable(string type, string name, string data, r_mem mem)
//    {
//        if(type == "BOOLEAN")
//        {
//            //createBoolean(name, data, mem);
//        }
//        else if(type == "ternary")
//        {
//            //createternary(name, data, mem);
//        }
//        else if(type == "INTEGER")
//        {
//            //createInteger(name, data, mem);
//        }
//        else if(type == "DOUBLE")
//        {
//            //createDouble(name, data, mem);
//        }
//        else if(type == "STRING")
//        {
//            //createString(name, data, mem);
//        }
//        else
//        {
//            r_dev::debug_broadcast("Invalid data type!", "createVariable", false);
//            return false;
//        }
//        return true;
//    }

//    bool r_utils::createBoolean(string name, string data, r_mem mem)
//    {
//        bool val;
//        if(data == "true")
//        {
//            val = true;
//        }
//        else if(data == "false")
//        {
//            val = false;
//        }
//        else
//        {
//            r_dev::debug_broadcast("Invalid input type", "createBoolean", false);
//            return false;
//        }
//        val = mem->newValue(name, r_utils::B, r_utils::toBinary(val));
//        if(val)
//        {
//            r_dev::debug_broadcast("SUCCESSFULLY created variable", "createBoolean", false);
//        }
//        else
//        {
//            r_dev::debug_broadcast("SUCCESSFULLY created variable", "createBoolean", false);
//        }
//        return val;
//    }
//
//    bool r_utils::createternary(string name, string data, r_mem mem)
//    {
//        int val;
//        if(data == "true")
//        {
//            val = 1;
//        }
//        else if(data == "false")
//        {
//            val = 0;
//        }
//        else if(data == "maybe")
//        {
//            val = 2;
//        }
//        else
//        {
//            r_dev::debug_broadcast("Invalid input type", "createBoolean", false);
//            return false;
//        }
//        val = mem->newValue(name, r_utils::B, r_utils::toBinary(val, 8));
//        if(val)
//        {
//            r_dev::debug_broadcast("SUCCESSFULLY created variable", "createBoolean", false);
//        }
//        else
//        {
//            r_dev::debug_broadcast("SUCCESSFULLY created variable", "createBoolean", false);
//        }
//        return val;
//    }
//
////    bool r_utils::createInt(string name, string data, r_mem* mem)
//    {
//        try
//        {
//            int val = stoi(data);
//            if(r_mem->newValue(name, r_utils::I, r_utils::toBinary(val)))
//            {
//                r_dev::debug_broadcast("SUCCESSFULLY created variable", "bool createInt", false);
//                return true;
//            }
//            else
//            {
//                r_dev::debug_broadcast("ERROR creating new variable", "bool createInt", false);
//            }
//            return false;
//        }
//        catch(std::invalid_argument e)
//        {
//            r_dev::debug_broadcast("Error converting data to integer", "bool createInt", false);
//        }
//    }
//
//    bool r_utils::createDouble(string name, string data, r_mem* mem)
//    {
//        try
//        {
//            int val = stod(data);
//            if(r_mem->newValue(name, r_utils::I, r_utils::toBinary(val)))
//            {
//                r_dev::debug_broadcast("SUCCESSFULLY created variable", "bool createDouble", false);
//                return true;
//            }
//            else
//            {
//                r_dev::debug_broadcast("ERROR creating new variable", "bool createDouble", false);
//            }
//        }
//        catch(std::invalid_argument e)
//        {
//            r_dev::debug_broadcast("Error converting data to integer", "bool createDouble", false);
//        }
//        return false;
//    }
//
//    bool r_utils::createBoolean()

    /**Outputs messages to the command-line in the proper format.
    *
    *@param msg: the message to be displayed
    *@param ansi_color: the optional ansi color code (0 is default)
    *@param ansi_format: the optional ansi format code (0 is default)
    *@param error: the message is to be displayed as an error (false is default)
    */
    void r_utils::broadcast(string msg, int ansi_color, int ansi_format, bool error)
    {
        stringstream s_format;
        string format, clear_format;

        //Holds either ">>" or ">!", depending on error status. ">>" by default.
        string token = ">>";

        //If the message is an error...
        if(error)
        {
            //Change the token to the error comment token ">!"
            token = ">!";
        }

        if(r_sys::useANSI())
        {
            s_format << "\033[" << ansi_format << ";" << ansi_color << "m";
            s_format >> format;
            clear_format = "\033[0m";

            cout << format << token << msg << clear_format << endl;
        }
        else
        {
            cout << token << msg << endl;
        }
    }

    //See note about template on r_utils.h

    /*Searches the given array INARRAY for the string VALUE.
    If BINARY is true, performs a binary search,
    otherwise, performs linear search.
    */
    /*template <class T>
    int r_utils::searchArray(T value, T inArray[], int size, bool binary)
    {
        int position = -1;
        if(binary)
        {
            int first = 0,
                last = size - 1,
                middle;
            bool found = false;

            while(!found && first <= last)
            {
                middle = (first + last) / 2;
                if (inArray[middle] == value)
                {
                    found = true;
                    position = middle;
                }
                else if(inArray[middle] > value)
                {
                    last = middle - 1;
                }
                else
                {
                    first = middle + 1;
                }
            }
        }
        else
        {
            int index = 0;
            bool found = false;

            while(index < size && !found)
            {
                if(inArray[index] == value)
                {
                    found = true;
                    position = index;
                }
                index++;
            }
        }

        return position;
    }*/

    int r_utils::searchArray(string value, string inArray[], int size, bool binary)
    {
        int position = -1;

        if(binary == true)		//Perform binary search (standard algorithm).
        {
            int first = 0,
                last = size - 1,
                middle;
            bool found = false;

            while(!found && first <= last)
            {
                middle = (first + last) / 2;
                if (inArray[middle] == value)
                {
                    found = true;
                    position = middle;
                }
                else if(inArray[middle] > value)
                {
                    last = middle - 1;
                }
                else
                {
                    first = middle + 1;
                }
            }
        }
        else 	//Perform linear search (standard algorithm).
        {
            int pageIndex = 0;
            bool found = false;

            while(pageIndex < size && !found)
            {
                if(inArray[pageIndex] == value)
                {
                    found = true;
                    position = pageIndex;
                }
                pageIndex++;
            }
        }

        return position;
    }

    /*Searches the given array INARRAY for the integer VALUE.
    If BINARY is true, performs a binary search,
    otherwise, performs linear search.
    (overloaded version of above function)
    */

    int r_utils::searchArray(string value, const string inArray[], int size, bool binary)
    {
        int position = -1;
        if(binary)
        {
            int first = 0,
                last = size - 1,
                middle;
            bool found = false;

            while(!found && first <= last)
            {
                middle = (first + last) / 2;
                if (inArray[middle] == value)
                {
                    found = true;
                    position = middle;
                }
                else if(inArray[middle] > value)
                {
                    last = middle - 1;
                }
                else
                {
                    first = middle + 1;
                }
            }
        }
        else
        {
            int pageIndex = 0;
            bool found = false;

            while(pageIndex < size && !found)
            {
                if(inArray[pageIndex] == value)
                {
                    found = true;
                    position = pageIndex;
                }
                pageIndex++;
            }
        }

        return position;
    }

    int r_utils::searchArray(int value, int inArray[], int size, bool binary)
    {
        int position = -1;
        if(binary)
        {
            int first = 0,
                last = size - 1,
                middle;
            bool found = false;

            while(!found && first <= last)
            {
                middle = (first + last) / 2;
                if (inArray[middle] == value)
                {
                    found = true;
                    position = middle;
                }
                else if(inArray[middle] > value)
                {
                    last = middle - 1;
                }
                else
                {
                    first = middle + 1;
                }
            }
        }
        else
        {
            int pageIndex = 0;
            bool found = false;

            while(pageIndex < size && !found)
            {
                if(inArray[pageIndex] == value)
                {
                    found = true;
                    position = pageIndex;
                }
                pageIndex++;
            }
        }

        return position;
    }

    /*Returns the type of reservation the string matches, if any.
    0=none, 1=keyword, 2=command

    check: the string to check
    Returns the reservation type
    */

    r_utils::ReserveType r_utils::checkReserved(string check)
    {
        ReserveType type;

        if(searchArray(check, SYS_KEYWORD, COUNT_KEYWORD) > -1)
        {
            type = RESERVE_KEYWORD;
        }
        else if(searchArray(check, SYS_META, COUNT_META) > -1)
        {
            type = RESERVE_META;
        }
        else if(searchArray(check, SYS_COMMAND, COUNT_COMMAND) > -1)
        {
            type = RESERVE_COMMAND;
        }
        else
        {
            type = RESERVE_NONE;
        }

        return type;
    }

    std::string r_utils::getDataTypeName(int enumVal){
        switch(enumVal){
            case U:
                return "Undefined";
            case B:
                return "Boolean";
            case T:
                return "Ternary";
            case I:
                return "Integer";
            case N:
                return "Number";
            case S:
                return "String";
            case A:
                return "Array";
            case C:
                return "Catalog";
            case R:
                return "Regex";
            case F:
                return "Function";
            case D:
                return "Datetime";
            case X:
                return "XML";
            default:
                return "Invalid Value";
        }
    }

    r_utils::DataType r_utils::getDataType(std::string type){
        if(type == getDataTypeName(U)){
            return U;
        }else if(type == getDataTypeName(B)){
            return B;
        }else if(type == getDataTypeName(T)){
            return T;
        }else if(type == getDataTypeName(I)){
            return I;
        }else if(type == getDataTypeName(N)){
            return N;
        }else if(type == getDataTypeName(S)){
            return S;
        }else if(type == getDataTypeName(A)){
            return A;
        }else if(type == getDataTypeName(C)){
            return C;
        }else if(type == getDataTypeName(R)){
            return R;
        }else if(type == getDataTypeName(F)){
            return F;
        }else if(type == getDataTypeName(D)){
            return D;
        }else if(type == getDataTypeName(X)){
            return X;
        }
        return U;
    }

    //TODO add support for if it is an operator?
        /*r_utils::DataType r_utils::getDataType2(std::string statement, r_catalog* catalog){
        if(statement.at(0) == '@')
        {
            return (catalog -> retrieve(statement.substr(i + 1, statement.size())))->getType();
        }
        else if(statement.at(0) == '"')
        {
            return r_utils::DataType::S;
        }
        else if(statement.at(0) == '0')
        {
            if(statement.at(1) == 'x' || statement.at(1) == 'X')
            {
                //TODO Datatype for Hexadecimal?
                return r_utils::DataType::I;
            }
            else
            {
                //TODO Datatype for Octal?
                return r_utils::DataType::I;
            }
        }
        else if(statement == "true" || statement == "false" || statement == "TRUE" || statement == "FALSE")
        {
            //TODO what if we want it to be ternary?
            return r_utils::DataType::B;
        }

        for(unsigned int i = 0; i < statement.size(); i++)
        {
            if(statement.at(i) == '.')
            {
                return r_utils::DataType::N;
            }
        }
        try
        {
            int i = stoi(statement);
            return r_utils::DataType::I;
        }
        catch(invalid_argument)
        {
            //TODO return something if it is a pointer thingy?
        }
    }
*/
    /**cleanString strips the leading and trailing whitespace from the string.
    * @param input: The string to clean.
    * @return The cleaned string.
    */
    string r_utils::cleanString(string input)
    {
        //While the first character is either a space or tab...
        while (input[0] == ' ' || input[0] == '\t')
        {
            //Erase the first character.
            input.erase(0, 1);
        }
        //While the last character is either a space or tab...
        while (input[input.length() - 1] == ' ' || input[input.length() - 1] == '\t')
        {
            //Erase the last character.
            input.erase(input.length() - 1, 1);
        }
        //Return the cleaned string.
        return input;
    }

    //Need to handle commas
 //precedence levels. Top is highest precedence

    //(                     1
    // ++ --                2
    // ^  root log          3
    // * / \\ %             4
    // + -                  5
    // > < >= <=            6
    // == !=                7
    // && &| ||             8
    // =                    9
    // ,                    10

    //the value that is returned represents the precedence of the the given operator
    //though this is returning a value, it is being used for the initialization of the
    //variable precedence. Also there is already a getter called getPrecedence, thus it is set instead of get
    int r_utils::getPrecedence(string statement)
    {
        int r = 0;
        switch(statement.at(0))
        {
            //if one of these characters there is a possibility for there to be a second character in the operator
            case '+':
            case '-':
            {
                if(statement.size() == 1)
                {
                    //if the operator is + or -
                    r = 5;
                }
                else
                {
                    //if the operator is ++ or --
                    r = 2;
                }
                break;
            }
            case '^':
            {
                r = 3;
                break;
            }
            case '*':
            case '/':
            case '\\':
            case '%':
            {
                r = 4;
                break;
            }
            case '<':
            case '>':
            {
                //the operators > >= < <= have same precedence
                r = 6;
                break;
            }
            case '&':
            case '|':
            {
                //the operators &&, ||, or &| have the same precedence
                r = 8;
                break;
            }
            case '(':
            {
                r = 1;
                break;
            }
            case '=':
            {
                if(statement.size() == 1)
                {
                    //the operator is =
                    r = 9;
                }
                else
                {
                    //the operator is ==
                    r = 7;
                }
                break;
            }
            case '!':
            {
                //the operator is !=
                r = 7;
                break;
            }
            case ',':
            {
                r = 10;
                break;
            }
            //by default this character set is not an operator so return 0
            default:
                r = 0;
        }
        return r;
    }

    string r_utils::toBinary(bool b)
    {
        string binary(8, '0');
        if(b)
            binary[7] = '1';
        return binary;
    }

    bool r_utils::toBool(string binary)
    {
        return binary[7] == '1';
    }

    int r_utils::toTernary(string binary)
    {
        return toLong(binary);
    }

    //returns a 32 bit representation by default. Can return a different size as desired
    string r_utils::toBinary(int num, int numBits)
    {
        string binary(numBits, '0');
        for(int i = numBits - 1; i > -1; i--)
        {
            if(num % 2 == 1)
                binary[i] = '1';
            num /= 2;
        }
        return binary;
    }

    int r_utils::toInt(string binary)
    {
        return (int) toLong(binary);
    }

    string r_utils::toBinary(long num, int numBits)
    {
        string binary(numBits, '0');
        for(int i = numBits - 1; i > -1; i--)
        {
            if(num % 2 == 1)
                binary[i] = '1';
            num /= 2;
        }
        //ss.str();
        return binary;
    }

    long r_utils::toLong(string binary)
    {
        long value = 0;
        for(unsigned int i = 0; i < binary.length(); i++)
        {
            if(binary[binary.length() - 1 - i] == '1')
                value += pow(2, i);
        }
        return value;
    }

    //conversion from double to binary with single floating point precision
    //returns a 32 bit representation
    string r_utils::toBinary(float num)
    {
        //the number of bits used to store this type of number (8 bytes)
        int numBits = 32;
        //stores the new binary format
        string binary(numBits, '0');
        if(num != 0)
            {
            //stores a 0 if the number is positive
            if(num < 0)
            {
                //stores a 1 if negative
                binary[0] = '1';
                //convert the number to a positive number to be used later
                num -= num * 2;
            }
            //stores the number that is before the decimal point
            int beforeDecimal = (int) num;
            //stores the number that is after the decimal point
            double afterDecimal = num - beforeDecimal;
            //determines the size of characters in the number
            //Examples: 9 = 1, 10 = 2, 100 = 3
            int decimalIndex;
            if(beforeDecimal > 0)
                decimalIndex = (int)log2(beforeDecimal);
            else
                decimalIndex = 0;
            //convert the number before the decimal to binary
            //don't include the 1 from the mantissa
            for(int i = decimalIndex; i > 0; i--)
            {
                //the bits value is the remainer of the current number divided by 2
                //there are 9 bits before the mantissa value begins and we don't include the first mantissa bit
                if(beforeDecimal % 2 == 1)
                    binary[i + 8] = '1';
                //divide by 2 to prepare for the next bit
                beforeDecimal /= 2;
            }
            //the starting point of the binary representation of the number after the decimal
            int pageIndex = decimalIndex + 9 - 1;
            bool isFirst = true;
            //a temporary representation of the value after the decimal. afterDecimal is needed later
            double temp = afterDecimal;
            //keep going until all of the bits of the mantissa are used
            while(pageIndex < numBits)
            {
                //multiply the value by 2
                temp *= 2;
                //the binary representation is if the new value is greater than or euqal to 1 or not
                //if the number is greater than or equal to 1, subtract 1 to prepare for the next bit
                if((int)(temp / 1) == 1)
                {
                    if(isFirst)
                        isFirst = false;
                    else
                        binary[pageIndex] = '1';
                    temp -= 1;
                }
                //move on to the next index
                if(!isFirst)
                    pageIndex++;
            }
            //the exponent used for conversion
            int exponent;
            //if the absolute value of the number(taken earlier) is greater than 1
            if(num > 1)
            {
                //the single point precission bias is 127
                exponent = 127 + decimalIndex;
            }
            else
            {
                //determine how many places after the decimal the first non 0 number is
                for(pageIndex = 0; ((int)afterDecimal / 1) == 0; pageIndex++)
                {
                    afterDecimal *= 2;
                }
                exponent = 127 - pageIndex;
            }
            //now convert the exponents value to binary
            //there are 8 bits for the exponent
            for(int i = 8; i > 0; i--)
            {
                if(exponent % 2 == 1)
                    binary[i] = '1';
                exponent /= 2;
            }
        }
        return binary;
    }

    //converts the passed in binary string into a long
    float r_utils::toFloat(string binary)
    {
        //the value starts at 0
        float val = 0;
        if(binary.find("1") == -1)
            return 0;
        //used to determine if the number is a positive or negative number
        int theSign = 1;
        //if the first bit is a 1, the number is negative
        if(binary[0] == '1')
            theSign = -1;
        //gets the bits that store the exponent
        string temp = binary.substr(1, 8);
        //convert the exponent bits to a number
        int theExponent = toInt(temp) - 127;
        //if the exponent is a positive number
        if(theExponent > 0)
        {
            //get the number before the decimal
            temp = binary.substr(9, theExponent);
            //convert the number before the decimal to a base 10 number
            for(int i = 0; i < theExponent; i++)
            {
                if(temp[theExponent - i - 1] == '1')
                    val += pow(2, i);
            }
            //add the value of the 1 that was chopped off for the mantissa
            val += pow(2, theExponent);
            //get the value after the decimal
            temp = binary.substr(9 + theExponent, 32 - 9 - theExponent);
            //convert the number after the decimal  to a base 10 number
            for(unsigned int i = 0; i < temp.size(); i++)
            {
                if(temp[i] == '1')
                {
                    val += (1 / pow(2, i + 1));
                }
            }
        }
        //If the exponent is a negative number
        else
        {
            //get the entire mantissa part of the format
            temp = binary.substr(9, 32 - 9);
            //get the value of the number by adding 1 over the current power of 2
            val += (1 / pow(2, -1 * theExponent));
            for(unsigned int i = 0; i < temp.size(); i++)
            {
                if(temp[i] == '1')
                    val += (1 / pow(2, i + 1 + -1 * theExponent));
            }
        }
        //multiply the value by the sign to get the value and return it
        return theSign * val;
    }

    //conversion from double to binary with double floating point precision
    //returns a 64 bit representation
    string r_utils::toBinary(double num)
    {
        //the number of bits used to store this type of number (16 bytes)
        int numBits = 64;
        //stores the new binary format
        string binary(64, '0');
        if (num != 0)
        {
            //stores a 0 if the number is positive
            if(num < 0)
            {
                //stores a 1 if negative
                binary[0] = '1';
                //convert the number to a positive number to be used later
                num -= num * 2;
            }
            //stores the number that is before the decimal point
            int beforeDecimal = (int) num;
            //stores the number that is after the decimal point
            double afterDecimal = num - beforeDecimal;
            //determines the size of characters in the number
            //Examples: 9 = 1, 10 = 2, 100 = 3
            int decimalIndex;
            if(beforeDecimal > 0)
                decimalIndex = (int)log2(beforeDecimal);
            else
                decimalIndex = 0;
            //convert the number before the decimal to binary
            for(int i = decimalIndex; i > 0; i--)
            {
                //the bits value is the remainer of the current number divided by 2
                //there are 12 bits before the mantissa value starts
                if(beforeDecimal % 2 == 1)
                    binary[i + 11] = '1';
                beforeDecimal /= 2;
            }
            //the starting point of the binary representation of the number after the decimal
            int pageIndex = decimalIndex + 11;
            bool isFirst = true;
            //a temporary representation of the value after the decimal. afterDecimal is needed later
            double temp = afterDecimal;
            while(pageIndex < numBits && ((int) (temp * 1000)) != 0)
            //keep going until all of the bits of the mantissa are used
            {
                //multiply the value by 2
                temp *= 2;
                //the binary representation is if the new value is greater than or euqal to 1 or not
                //if the number is greater than or equal to 1, subtract 1 to prepare for the next bit
                if((int)(temp / 1) == 1)
                {
                    if(isFirst)
                        isFirst = false;
                    else
                        binary[pageIndex] = '1';
                    temp -= 1;
                }
                //move on to the next index
                if(!isFirst)
                    pageIndex++;
            }
            //the exponent used for conversion
            int exponent;
            pageIndex = 0;
            //if the absolute value of the number(taken earlier) is greater than 1
            if(num > 1)
            {
                //the single point precission bias is 1023
                exponent = 1023 + decimalIndex;
            }
            else
            {
                //determine how many places after the decimal the first non 0 number is
                for(pageIndex = 0; ((int)afterDecimal / 1) == 0; pageIndex++)
                {
                    afterDecimal *= 2;
                }
                exponent = 1023 - pageIndex;
            }
            //now convert the exponents value to binary
            //there are 11 bits for the exponent
            for(int i = 11; i > 0; i--)
            {
                if(exponent % 2 == 1)
                    binary[i] = '1';
                exponent /= 2;
            }
        }
        return binary;
    }

    double r_utils::toDouble(string binary)
    {
        //stores the new decimal value
        double val = 0;
        if(binary.find("1") == -1)
            return 0;
        int theSign = 1;
        //if the sign bit is set, the number needs to be multiplied by a negative 1 at the end
        if(binary[0] == '1')
            theSign = -1;
        //get the exponent bits
        string temp = binary.substr(1, 11);
        //convert the exponent to a base 10 number
        int theExponent = toInt(temp) - 1023;
        //if the exponent is a positive number
        if(theExponent > 0)
        {
            //get the binary values before the decimal
            temp = binary.substr(12, theExponent);
            //convert the number before the decimal to a base 10 number
            for(int i = 0; i < theExponent; i++)
            {
                if(temp[theExponent - i - 1] == '1')
                    val += pow(2, i);
            }
            val += pow(2, theExponent);
            //get the binary after the decimal
            temp = binary.substr(12 + theExponent, 32 - 12 - theExponent);
            //convert the number after the decimal to a base 10 number
            for(unsigned int i = 0; i < temp.size(); i++)
            {
                if(temp[i] == '1')
                {
                    val += (1 / pow(2, i + 1));
                }
            }
        }
        //if the exponent is a negative number
        else
        {
            //get the mantissa value
            temp = binary.substr(12, 32 - 12);
            val += (1 / pow(2, -1 * theExponent));
            for(unsigned int i = 0; i < temp.size(); i++)
            {
                if(temp[i] == '1')
                    val += (1 / pow(2, i + 1 + -1 * theExponent));
            }
        }
        //multiply the value by the sign value
        return theSign * val;
    }

    string r_utils::toBinary(char c)
    {
        string binary(8, '0');
        int charVal = (int)c;
        for(int i = 7; i > -1; i--)
        {
            if(charVal % 2 == 1)
                binary.at(i) = '1';
            charVal /= 2;
        }
        return binary;
    }

    char r_utils::toChar(string binary)
    {
        return (char) toLong(binary);
    }

    string r_utils::toBinary(string theString)
    {
        stringstream ss;
        for(unsigned int i = 0; i < theString.size(); i++)
        {
            ss << toBinary(theString.at(i));
        }
        return ss.str();
    }

    string r_utils::binaryToString(string binary)
    {
        stringstream r;
        string character;
        for(int i = 0; i < binary.length() / 8; i++)
        {
            character = binary.substr(i * 8, 8);
            r << toChar(character);
        }
        return r.str();
    }

}
