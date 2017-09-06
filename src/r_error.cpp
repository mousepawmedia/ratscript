#include "r_error.hpp"

//Define all of the proper names for the error codes (enums).
map<r_error::ErrorType, string> r_error::errorNames = {
    {UNKNOWN_ERROR, "Unknown Error"},
    {NAME_RESERVED, "Reserved Name"},
    {NAME_LIB_CONFLICT, "Library Name Conflict"},
    {NAME_CONFLICT, "Name Conflict"},
    {NAME_ILLEGAL, "Illegal Name"},
    {NAME_INVALID, "Invalid Name"},
    {TYPE_ERROR, "Type Error"},
    {TYPE_CAST, "Cast Error"},
    {TYPE_NULL, "Null Error"},
    {FUNC_CALL, "Function Call Error"},
    {FUNC_MISS_ARG, "Missing Argument"},
    {FUNC_LATE_ARG, "Late Required Argument"},
    {ACCESS_ERROR, "Access Error"},
    {ACCESS_SCOPE, "Out of Scope"},
    {ACCESS_ASSIGN, "Illegal Assignment"},
    {ACCESS_REF, "Reference Error"},
    {ETOK_GENERAL, "Expected Token"},
    {ETOK_OPEN_BRACK, "Opening Bracket Expected"},
    {ETOK_CLOSE_BRACK, "Closing Bracket Expected"},
    {ETOK_OPEN_PAREN, "Opening Parenthesis Expected"},
    {ETOK_CLOSE_PAREN, "Closing Parenthesis Expected"},
    {ETOK_OPEN_BRACE, "Opening Brace Expected"},
    {ETOK_CLOSE_BRACE, "Closing Brace Expected"},
    {ETOK_INCOMPLETE_COMM, "Incomplete Comment"},
    {ETOK_SUBORD_MISS, "Subordination Expected"},
    {ETOK_IMPLICIT_VAR, "Implicit Variable Call"},
    {ETOK_MISSING_SPACE, "Space Expected"},
    {UTOK_GENERAL, "Unexpected Token"},
    {UTOK_INCOMPLETE, "Incomplete Statement"},
    {UTOK_ASSIGN, "Assignment Unexpected"},
    {UTOK_LANG, "Wrong Language Error"},
    {UTOK_SUBORD, "Unexpected Subordination"},
    {EKEY_GENERAL, "Expected Keyword"},
    {EKEY_TYPE, "Type Expected"},
    {EKEY_CASE, "CASE Expected"},
    {EKEY_CATCH, "CATCH Expected"},
    {EKEY_END, "END Expected"},
    {EKEY_RETURN, "RETURN Expected"},
    {UKEY_GENERAL, "Unexpected Keyword"},
    {UKEY_CONTEXT, "Context Error"},
    {MATH_GENERAL, "Math Error"},
    {MATH_DIV_DOMAIN, "Division Error"},
    {MATH_ROOT_DOMAIN, "Root Error"},
    {MATH_LOG_DOMAIN, "Log Error"},
    {MATH_INFINITY, "Infinity Error"},
    {MATH_INVALID_OP, "Invalid Operator"},
    {MATH_NAN, "Invalid Operand"},
    {NDMATH_GENERAL, "Non-Decimal Math Error"},
    {NDMATH_DIGIT, "Illegal Digit"},
    {NDMATH_HEX, "Invalid Hexadecimal"},
    {LOOP_INFINITE, "Infinite Loop Error"},
    {LOOP_MISS_ITR, "Missing Iterator Modifier"},
    {LOOP_MISS_COND, "Missing Loop Condition"},
    {LOOP_MISS_WHILE, "Missing WHILE"},
    {LOOP_INCOMPLETE_FOR, "Incomplete FOR"},
    {LOGIC_GENERAL, "Logic Error"},
    {LOGIC_PARADOX, "Paradox Error"},
    {EVAL_GENERAL, "Evaluation Error"},
    {EVAL_INCOMPLETE, "Invalid Question"},
    {EVAL_INVALID_TYPE, "Invalid Type"},
    {EVAL_QUESTION, "Question Error"},
    {SYS_GENERAL, "System Error"},
    {SYS_MEM_ALLOC, "Memory Allocation Error"},
    {SYS_UNDERFLOW, "Underflow Error"},
    {SYS_OVERFLOW, "Overflow Error"},
    {SYS_NOT_FOUND, "Not Found"},
    {SYS_ACCESS, "System Access Error"},
    {SYS_SEG_FAULT, "Segmentation Fault"},
    {SYS_UNEX_TERM, "Unexpected Termination"},
};

/**Creates a new error object.
*@param errorCode the Ratscript error code (enum)
*@param additionalInfo a string describing the error
*/
r_error::r_error(r_error::ErrorType errorCode, string additionalInfo)
{
    //Store the errorCode as an integer.
    code = errorCode;
    //Store the title from the static map.
    title = errorNames[errorCode];
    //Store the provided information.
    info = additionalInfo;
}

//Broadcasts the error message.
void r_error::throwError()
{
    //Set up message.
    string msg;

    /*Set up stringstream for creating a message.
    Needed since we're including the "code" integer.*/
    std::stringstream ss;

    //If there is an info string...
    if(info != "")
    {
        //Create the message with the code, title and info. (Using stringstream)
        ss << code << " " << title << ": " << info;
    }
    //Otherwise, if there is no info string...
    else
    {
        //Create the message with only the code and title. (Using stringstream)
        ss << code << " " << title;
    }

    //Convert stringstream to "msg" string.
    msg = ss.str();

    //Broadcast message as error, with bold red ANSI formatting.
    r_utils::broadcast(msg, 31, 1, true);

}
