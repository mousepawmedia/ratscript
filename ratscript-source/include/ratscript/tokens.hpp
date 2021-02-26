/** Tokens [Ratscript]
  * Version: 0.1
  *
  * Token class. Used by lexer to separate input into logical units or lexemes
  * in order to pass on to the parser.
  *
  * Author(s): Anna R. Dunster
  */

/* LICENSE
 * Copyright (c) 2020 MousePaw Media.
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

/* In each token, we need to store the actual literal content of the token,
 * the type of token it is, and where in the input it was found (for error
 * reporting).  This information should be accessible, but not changeable
 * after initialization.
 */

#ifndef R_TOKENS_HPP
#define R_TOKENS_HPP

#include <iostream>
#include "ratscript/tokentypes.hpp"

class Token
{
private:
	const char *literal_;
	TokenType token_type_;
	int line_;
	int position_;

public:
	Token(const char *literal, TokenType token_type, int line, int position) :
		literal_(literal), token_type_(token_type), line_(line), position_(position)
	{
	}

	const char *literal()
	{
		return literal_;
	}

	int token_type()
	{
		return int(token_type_);
	}

	int line()
	{
		return line_;
	}

	int position()
	{
		return position_;
	}

};

#endif