/** Conditional [Ratscript]
  * Version: 0.1
  *
  * A conditional object contains a header and subordinated code.
  * IF, ELSE IF, LOOP FOR, CASE (SWTICH), etc.
  * These get passed to the parser.
  *
  * Author(s): Jason C. McDonald
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

#ifndef R_CONDITIONAL_HPP
#define R_CONDITIONAL_HPP

#include <iostream>
#include <queue>
#include <string>
#include "ratscript/utils.hpp"

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
