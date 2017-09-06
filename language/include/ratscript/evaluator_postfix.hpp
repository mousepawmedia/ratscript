/** Evaluator [Ratscript]
  * Version: 0.1
  *
  * Receives and evaluates a statement.
  * Returns the statements final evaluated result.
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

#ifndef R_EVALUATOR_POSTFIX_HPP_INCLUDED
#define R_EVALUATOR_POSTFIX_HPP_INCLUDED

#include <string>
#include <vector>
#include <queue>

#include "ratscript/var.hpp"
#include "ratscript/scope.hpp"
#include "ratscript/error.hpp"
#include "ratscript/math.hpp"
#include "ratscript/utils.hpp"

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
