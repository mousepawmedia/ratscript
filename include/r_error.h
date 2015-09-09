/**Ratscript Norvegicus: Error<p>
* A Ratscript error object.
* @since 2.0
* @author Jason C. McDonald
*/

#ifndef R_ERROR_H
#define R_ERROR_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "r_utils.h"

using ratscript::r_utils;
using std::string;
using std::map;

class r_error
{
    public:
        ///All of the error codes (integers) in Ratscript.
        enum ErrorType
        {
            ///0000: General/Unknown error. YOU SHOULD NOT THROW THIS!
            UNKNOWN_ERROR = 0,

            //NAME ERRORS (00xx)
            ///0001: Name is reserved keyword.
            NAME_RESERVED = 1,
            ///0002: Name already exists in Ratscript common library.
            NAME_LIB_CONFLICT = 2,
            ///0003: Name already exists.
            NAME_CONFLICT = 3,
            ///0004: Cannot give literal as name.
            NAME_ILLEGAL = 4,
            ///0005: Names may only contain alphanumeric and _.
            NAME_INVALID = 5,

            //TYPE ERRORS (01xx)
            ///0100: Mismatched type.
            TYPE_ERROR = 100,
            ///0101: Cannot cast.
            TYPE_CAST = 101,
            ///0102: Null passed where value expected; no value defined.
            TYPE_NULL = 102,

            //FUNCTION ERRORS (03xx)
            ///0300: General function call error.
            FUNC_CALL = 300,
            ///0301: One or more arguments are missing.
            FUNC_MISS_ARG = 301,
            ///0302: Required arguments must precede optional arguments.
            FUNC_LATE_ARG = 302,

            //ACCESS ERRORS (04xx)
            ///0400: General access error.
            ACCESS_ERROR = 400,
            ///0401: Cannot access item out of scope.
            ACCESS_SCOPE = 401,
            ///0403: Attempt to write to constant or literal.
            ACCESS_ASSIGN = 403,
            ///0404: Item not found.
            ACCESS_REF = 404,

            //SYNTAX ERRORS (1xxx)
            //EXPECTED TOKEN (10xx)
            ///1000: General expected token error.
            ETOK_GENERAL = 1000,
            ///1001: Missing [
            ETOK_OPEN_BRACK = 1001,
            ///1002: Missing ]
            ETOK_CLOSE_BRACK = 1002,
            ///1003: Missing (
            ETOK_OPEN_PAREN = 1003,
            ///1004: Missing )
            ETOK_CLOSE_PAREN = 1004,
            ///1005: Missing {
            ETOK_OPEN_BRACE = 1005,
            ///1006: Missing }
            ETOK_CLOSE_BRACE = 1006,
            ///1007: Lone leading >. Expected * or another >.
            ETOK_INCOMPLETE_COMM = 1007,
            ///1008: Line should be subordinated (:); Subordinated code expected.
            ETOK_SUBORD_MISS = 1008,
            ///1009: Expected @
            ETOK_IMPLICIT_VAR = 1009,
            ///1010: Expected space.
            ETOK_MISSING_SPACE = 1010,

            //UNEXPECTED TOKEN (11XX)
            ///1100: General unexpected token error.
            UTOK_GENERAL = 1100,
            ///1101: Improper line continuation (...)
            UTOK_INCOMPLETE = 1101,
            ///1102: Encountered = instead of ==
            UTOK_ASSIGN = 1102,
            ///1103: Trailing ; (we're not in C!); Trailing \ (we're not in Python!)
            UTOK_LANG = 1103,
            ///1104: Unexpected subordination (:).
            UTOK_SUBORD = 1104,

            //EXPECTED KEYWORD (12xx)
            ///1200: General expected keyword error.
            EKEY_GENERAL = 1200,
            ///1201: No type given for MAKE.
            EKEY_TYPE = 1201,
            ///1202: No CASE given with SWITCH.
            EKEY_CASE = 1202,
            ///1203: No CATCH given with TRY
            EKEY_CATCH = 1203,
            ///1204: No matching END statement.
            EKEY_END = 1204,
            ///1205: No return in function.
            EKEY_RETURN = 1205,

            //UNEXPECTED KEYWORD (13xx)
            ///General unexpected keyword error.
            UKEY_GENERAL = 1300,
            ///Meta keyword out of context.
            UKEY_CONTEXT = 1301,

            //MATH AND LOGIC ERRORS (2xxx)
            //MATH ERRORS (20xx)
            ///2000: General math error.
            MATH_GENERAL = 2000,
            ///2001: Cannot divide by 0 (domain issue).
            MATH_DIV_DOMAIN = 2001,
            ///2002: Cannot take (even) root of negative number (domain issue).
            MATH_ROOT_DOMAIN = 2002,
            ///2003: Cannot take log of 0 or negative number (domain issue).
            MATH_LOG_DOMAIN = 2003,
            ///2004: Solution is infinite.
            MATH_INFINITY = 2004,
            ///2005: Operator invalid; operator does not exist.
            MATH_INVALID_OP = 2005,
            ///2006: Cannot perform math on NaN. (Invalid operand).
            MATH_NAN = 2006,

            //NON-DECIMAL MATH ERRORS (21xx)
            ///2100: General math error.
            NDMATH_GENERAL = 2100,
            ///2101: Out-of-range (illegal) digit for base.
            NDMATH_DIGIT = 2101,
            ///2102: Decimal point in hexadecimal.
            NDMATH_HEX = 2102,

            //LOOP ERRORS (22xx)
            ///2200: Infinite Loop
            LOOP_INFINITE = 2200,
            ///2201: No increment/decrement command in WHILE or DO WHILE.
            LOOP_MISS_ITR = 2201,
            ///2202: Missing statement in WHILE or DO WHILE.
            LOOP_MISS_COND = 2202,
            ///2203: No WHILE in DO WHILE.
            LOOP_MISS_WHILE = 2203,
            ///2204: Missing element in FOR header.
            LOOP_INCOMPLETE_FOR = 2204,

            //LOGIC ERRORS (23xx)
            ///2300: General logic error.
            LOGIC_GENERAL = 2300,
            ///2322: Contradictory statements. (Mainly novelty.)
            LOGIC_PARADOX = 2322,

            //EVAL ERRORS (24xx)
            ///2400: General eval error.
            EVAL_GENERAL = 2400,
            ///2401: Incomplete eval statement - operator leading/trailing.
            EVAL_INCOMPLETE = 2401,
            ///2402: Invalid type in eval statement.
            EVAL_INVALID_TYPE = 2402,
            ///2442: Ask a bad question, expect a bad answer. (Mainly novelty)
            EVAL_QUESTION = 2442,

            //SYSTEM ERRORS (3xxx)
            SYS_GENERAL = 3000,
            ///3001: Cannot allocate memory for variable.
            SYS_MEM_ALLOC = 3001,
            ///3002: Stack underflow.
            SYS_UNDERFLOW = 3002,
            ///3003: Stack overflow.
            SYS_OVERFLOW = 3003,
            ///3004: File cannot be found.
            SYS_NOT_FOUND = 3004,
            ///3005: File or process cannot be accessed.
            SYS_ACCESS = 3005,
            ///3006: Segmentation fault.
            SYS_SEG_FAULT = 3006,
            ///3007: Process terminated unexpectedly.
            SYS_UNEX_TERM = 3007,
        };

        r_error(ErrorType, string);
        //void broadcast();
        void throwError();
    protected:
    private:
        //Error titles.
        int code;
        string title;
        string info;

        ///The map holding all of the error names, stored by enum.
        static map<ErrorType, string> errorNames;
        //Assignment at the top of r_error.cpp.

        //static const int ERROR_MAX = 43;
        //static const string title[ERROR_MAX];
};

#endif // R_ERROR_H
