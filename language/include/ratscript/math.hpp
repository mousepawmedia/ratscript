/** Math [Ratscript]
  * Version: 0.1
  *
  * A math class
  *
  * Author(s): Michael Parkman
  */

/* LICENSE
 * Copyright (c) 2016 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef R_MATH_HPP_INCLUDED
#define R_MATH_HPP_INCLUDED

#include <math.h>

#include "ratscript/var.hpp"
#include "ratscript/error.hpp"

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
