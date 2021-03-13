/** Lexer [Ratscript]
 * Version: 0.1
 *
 * Lexes input strings, splitting them into their appropriate parts and sending
 * them on to the parser.
 *
 * Author(s): Anna R. Dunster, Jason C. McDonald, Michael Parkman
 */

/* LICENSE
 * Copyright (c) 2020-2021 MousePaw Media.
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

#ifndef R_LEXER_HPP
#define R_LEXER_HPP

#include <iostream>

#include "nimbly/flexarray.hpp"
#include "onestring/onestring.hpp"
#include "simplexpress/simplex.hpp"

#include "ratscript/token_types.hpp"
#include "ratscript/tokens.hpp"

class Lexer
{
private:
	/** Stored source code before lexing */
	onestring source;

	/** Keep track of position in source code, for Token generation and other
	 * things. */
	uint_fast32_t start;
	uint_fast32_t current;
	uint_fast32_t line;
	uint_fast32_t last_line_pos;

public:
	/** Array of Tokens, the output of the lexer creation */
	FlexArray<Token> tokens;

	/** Constructs the Lexer object and lexes the source code into an array of
	 * tokens.
	 * \param: source code to lex */
	explicit Lexer(onestring source);

private:
	/** Lexer loop, runs in constructor */
	void lex_tokens();

	/** Identifies and stores the current token. */
	void lex_token();

	/** Add a token with the current information
	 * \param: TokenType enum */
	void add_token(TokenType type);

	/** Add token with specified lexeme value
	 * \param: TokenType enum
	 * \param: onestring lexeme */
	void add_token(TokenType type, onestring lexeme);

	/** Used by lexer to obtain the next character. */
	onechar advance();

	/** Check for end of input source */
	bool at_end();

	/** Parse and ignore comments
	 * \param: initial character being parsed */
	void comment(onechar &character);

	/** Tokenize identifiers */
	void identifier();

	/** Check whether the next character matches an input. Advances if true.
	 * \param: the character to check against */
	bool match_next(onechar expected);

	/** Advances line and line position variables when needed */
	void newline();

	/** Tokenize numerical content */
	void number();

	/** Returns the next character without advancing the model. */
	onechar peek();

	/** Returns the character after next without advancing the model. */
	onechar peek_next();

	/** Tokenize strings. */
	void string(onechar &character);
};

#endif  // !R_LEXER_HPP
