/**Ratscript Norvegicus: Math<p>
* A math class.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_MATH_HPP_INCLUDED
#define R_MATH_HPP_INCLUDED

#include <math.h>

#include "r_var.hpp"
#include "r_error.h"

using std::ostringstream;

class r_math
{
    public:
        static r_var* add(r_var*, r_var*);
        static string add(string, string);
        static r_var* subtract(r_var*, r_var*);
        static string subtract(string, string);
        static r_var* multiply(r_var*, r_var*);
        static string multiply(string, string);
        static r_integer* integerDivision(r_var*, r_var*);
        static string integerDivision(string, string);
        static r_number* floatingPointDivision(r_var*, r_var*);
        static string floatingPointDivision(string, string);
        static r_var* exponentiate(r_var*, r_var*);
        static string exponentiate(string, string);
        static r_var* mod(r_var*, r_var*);
        static string mod(string, string);
    private:

};

#endif // R_MATH_HPP_INCLUDED
