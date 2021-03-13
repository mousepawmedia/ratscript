/** Lexer Tests (Ratscript)
 * Version: 0.1
 *
 * Tests for proper response and returns from Lexer class.
 *
 * Author(s): Anna R. Dunster
 */

/* LICENSE
 * Copyright (c) 2021 MousePaw Media.
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

#ifndef RATSCRIPT_LEXER_TESTS_HPP
#define RATSCRIPT_LEXER_TESTS_HPP

#include "goldilocks/assertions.hpp"
#include "goldilocks/goldilocks.hpp"

#include "ratscript/lexer.hpp"

// R-sB00 //

// R-tB0000
class TestLexerSingleCharacter : public Test
{
	onestring input_one = "_";
	FlexArray<Token> expected_one;
	uint length_one = 2;
	onestring input_two = "( ) { } , . - + ; * ! = < > #";
	FlexArray<Token> expected_two;
	uint length_two = 15;

public:
	TestLexerSingleCharacter() = default;

	testdoc_t get_title() override { return "Lex Single Character Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize various single character tokens";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::IDENTIFIER, "_", 1, 0));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 1));

		expected_two.push_back(Token(TokenType::LEFT_PAREN, "(", 1, 0));
		expected_two.push_back(Token(TokenType::RIGHT_PAREN, ")", 1, 2));
		expected_two.push_back(Token(TokenType::LEFT_BRACE, "{", 1, 4));
		expected_two.push_back(Token(TokenType::RIGHT_BRACE, "}", 1, 6));
		expected_two.push_back(Token(TokenType::COMMA, ",", 1, 8));
		expected_two.push_back(Token(TokenType::DOT, ".", 1, 10));
		expected_two.push_back(Token(TokenType::MINUS, "-", 1, 12));
		expected_two.push_back(Token(TokenType::PLUS, "+", 1, 14));
		expected_two.push_back(Token(TokenType::SEMICOLON, ";", 1, 16));
		expected_two.push_back(Token(TokenType::STAR, "*", 1, 18));
		expected_two.push_back(Token(TokenType::BANG, "!", 1, 20));
		expected_two.push_back(Token(TokenType::EQUAL, "=", 1, 22));
		expected_two.push_back(Token(TokenType::LESS, "<", 1, 24));
		expected_two.push_back(Token(TokenType::GREATER, ">", 1, 26));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 1, 29));

		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		PL_ASSERT_NOT_EQUAL(lex_one.tokens.length(), (length_one + 1));
		PL_ASSERT_EQUAL(lex_one.tokens[0], expected_one[0]);
		PL_ASSERT_EQUAL(lex_one.tokens[1], expected_one[1]);

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}
		return true;
	}

	~TestLexerSingleCharacter() = default;
};

// R-tB0001
class TestLexerTwoCharacter : public Test
{
	onestring input_one = "!= == <= >=";
	FlexArray<Token> expected_one;
	uint length_one = 5;

public:
	TestLexerTwoCharacter() = default;

	testdoc_t get_title() override { return "Lex Two Character Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize various two character tokens";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::BANG_EQUAL, "!=", 1, 0));
		expected_one.push_back(Token(TokenType::EQUAL_EQUAL, "==", 1, 3));
		expected_one.push_back(Token(TokenType::LESS_EQUAL, "<=", 1, 6));
		expected_one.push_back(Token(TokenType::GREATER_EQUAL, ">=", 1, 9));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 11));

		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}
		return true;
	}

	~TestLexerTwoCharacter() = default;
};

// R-tB0002
class TestLexerKeywords : public Test
{
	onestring input_one = "assert and break case class continue define elif";
	FlexArray<Token> expected_one;
	uint length_one = 9;

	onestring input_two = "else end fall false finally for if let make nil";
	FlexArray<Token> expected_two;
	uint length_two = 11;

	onestring input_three = "or panic pass print return super switch test this";
	FlexArray<Token> expected_three;
	uint length_three = 10;

	onestring input_four = "true until warn while";
	FlexArray<Token> expected_four;
	uint length_four = 5;

public:
	TestLexerKeywords() = default;

	testdoc_t get_title() override { return "Lex Keyword Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize keyword tokens";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::ASSERT, "assert", 1, 0));
		expected_one.push_back(Token(TokenType::AND, "and", 1, 7));
		expected_one.push_back(Token(TokenType::BREAK, "break", 1, 11));
		expected_one.push_back(Token(TokenType::CASE, "case", 1, 17));
		expected_one.push_back(Token(TokenType::CLASS, "class", 1, 22));
		expected_one.push_back(Token(TokenType::CONTINUE, "continue", 1, 28));
		expected_one.push_back(Token(TokenType::DEFINE, "define", 1, 37));
		expected_one.push_back(Token(TokenType::ELIF, "elif", 1, 44));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 48));

		expected_two.push_back(Token(TokenType::ELSE, "else", 1, 0));
		expected_two.push_back(Token(TokenType::END, "end", 1, 5));
		expected_two.push_back(Token(TokenType::FALL, "fall", 1, 9));
		expected_two.push_back(Token(TokenType::FALSE, "false", 1, 14));
		expected_two.push_back(Token(TokenType::FINALLY, "finally", 1, 20));
		expected_two.push_back(Token(TokenType::FOR, "for", 1, 28));
		expected_two.push_back(Token(TokenType::IF, "if", 1, 32));
		expected_two.push_back(Token(TokenType::LET, "let", 1, 35));
		expected_two.push_back(Token(TokenType::MAKE, "make", 1, 39));
		expected_two.push_back(Token(TokenType::NIL, "nil", 1, 44));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 1, 47));

		expected_three.push_back(Token(TokenType::OR, "or", 1, 0));
		expected_three.push_back(Token(TokenType::PANIC, "panic", 1, 3));
		expected_three.push_back(Token(TokenType::PASS, "pass", 1, 9));
		expected_three.push_back(Token(TokenType::PRINT, "print", 1, 14));
		expected_three.push_back(Token(TokenType::RETURN, "return", 1, 20));
		expected_three.push_back(Token(TokenType::SUPER, "super", 1, 27));
		expected_three.push_back(Token(TokenType::SWITCH, "switch", 1, 33));
		expected_three.push_back(Token(TokenType::TEST, "test", 1, 40));
		expected_three.push_back(Token(TokenType::THIS, "this", 1, 45));
		expected_three.push_back(Token(TokenType::ENDOFFILE, "", 1, 49));

		expected_four.push_back(Token(TokenType::TRUE, "true", 1, 0));
		expected_four.push_back(Token(TokenType::UNTIL, "until", 1, 5));
		expected_four.push_back(Token(TokenType::WARN, "warn", 1, 11));
		expected_four.push_back(Token(TokenType::WHILE, "while", 1, 16));
		expected_four.push_back(Token(TokenType::ENDOFFILE, "", 1, 21));

		return true;
	}

	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}

		Lexer lex_three(input_three);
		PL_ASSERT_EQUAL(lex_three.tokens.length(), length_three);
		for (uint i = 0; i < length_three; i++) {
			PL_ASSERT_EQUAL(lex_three.tokens[i], expected_three[i]);
		}

		Lexer lex_four(input_four);
		PL_ASSERT_EQUAL(lex_four.tokens.length(), length_four);
		for (uint i = 0; i < length_four; i++) {
			PL_ASSERT_EQUAL(lex_four.tokens[i], expected_four[i]);
		}

		return true;
	}

	~TestLexerKeywords() = default;
};

// R-tB0003
class TestLexerNumbers : public Test
{
	onestring input_one = "0 1 23 45.67";
	FlexArray<Token> expected_one;
	uint length_one = 5;

	// It's not the Lexer's job to validate if this makes sense, it should just
	// return tokens.
	onestring input_two = "1.    ";
	FlexArray<Token> expected_two;
	uint length_two = 3;

public:
	TestLexerNumbers() = default;

	testdoc_t get_title() override { return "Lex Numerical Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize numerical tokens";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::NUMBER, "0", 1, 0));
		expected_one.push_back(Token(TokenType::NUMBER, "1", 1, 2));
		expected_one.push_back(Token(TokenType::NUMBER, "23", 1, 4));
		expected_one.push_back(Token(TokenType::NUMBER, "45.67", 1, 7));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 12));

		expected_two.push_back(Token(TokenType::NUMBER, "1", 1, 0));
		expected_two.push_back(Token(TokenType::DOT, ".", 1, 1));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 1, 6));
		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}
		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}
		return true;
	}

	~TestLexerNumbers() = default;
};

// R-tB0004
class TestLexerIdentifiers : public Test
{
	onestring input_one = "steve Fred _george aunt_betty";
	FlexArray<Token> expected_one;
	uint length_one = 5;

	onestring input_two = "joe       .       petunia  _  ";
	FlexArray<Token> expected_two;
	uint length_two = 5;

public:
	TestLexerIdentifiers() = default;

	testdoc_t get_title() override { return "Lex Identifier Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize non-keyword identifier tokens";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::IDENTIFIER, "steve", 1, 0));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "Fred", 1, 6));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "_george", 1, 11));
		expected_one.push_back(
			Token(TokenType::IDENTIFIER, "aunt_betty", 1, 19));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 29));

		expected_two.push_back(Token(TokenType::IDENTIFIER, "joe", 1, 0));
		expected_two.push_back(Token(TokenType::DOT, ".", 1, 10));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "petunia", 1, 18));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "_", 1, 27));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 1, 30));
		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}
		PL_ASSERT_NOT_EQUAL(lex_one.tokens[1],
							Token(TokenType::IDENTIFIER, "fred", 1, 6));

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}
		return true;
	}

	~TestLexerIdentifiers() = default;
};

// R-tB0005
class TestLexerNewlines : public Test
{
	onestring input_one = "12\nsteve\nfinally:";
	FlexArray<Token> expected_one;
	uint length_one = 5;

	onestring input_two = "joe\n        \n        steve";
	FlexArray<Token> expected_two;
	uint length_two = 3;

public:
	TestLexerNewlines() = default;

	testdoc_t get_title() override { return "Lex Multiline Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize input including newlines";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::NUMBER, "12", 1, 0));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "steve", 2, 0));
		expected_one.push_back(Token(TokenType::FINALLY, "finally", 3, 0));
		expected_one.push_back(Token(TokenType::COLON, ":", 3, 7));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 3, 8));

		expected_two.push_back(Token(TokenType::IDENTIFIER, "joe", 1, 0));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "steve", 3, 8));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 3, 13));
		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}
		return true;
	}

	~TestLexerNewlines() = default;
};

// R-tB0006
class TestLexerComments : public Test
{
	// Single line comment
	onestring input_one = "# Some commented text\ngeorge, stuff, things";
	FlexArray<Token> expected_one;
	uint length_one = 6;

	// Multi line comment style one
	onestring input_two = "##\nSome commented text\n##\ngeorge, stuff, things";
	FlexArray<Token> expected_two;
	uint length_two = 6;

	// Multi line comment style two
	onestring input_three =
		"### ### ###\nSome commented text\n### ### ###\ngeorge, stuff, things";
	FlexArray<Token> expected_three;
	uint length_three = 6;

	// End of line line comment
	onestring input_four = "george, stuff, things # Some commented text\n";
	FlexArray<Token> expected_four;
	uint length_four = 6;

public:
	TestLexerComments() = default;

	testdoc_t get_title() override { return "Lex Tokens Around Comments"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize input including comments";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::IDENTIFIER, "george", 2, 0));
		expected_one.push_back(Token(TokenType::COMMA, ",", 2, 6));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "stuff", 2, 8));
		expected_one.push_back(Token(TokenType::COMMA, ",", 2, 13));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "things", 2, 15));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 2, 21));

		expected_two.push_back(Token(TokenType::IDENTIFIER, "george", 4, 0));
		expected_two.push_back(Token(TokenType::COMMA, ",", 4, 6));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "stuff", 4, 8));
		expected_two.push_back(Token(TokenType::COMMA, ",", 4, 13));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "things", 4, 15));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 4, 21));

		expected_three.push_back(Token(TokenType::IDENTIFIER, "george", 4, 0));
		expected_three.push_back(Token(TokenType::COMMA, ",", 4, 6));
		expected_three.push_back(Token(TokenType::IDENTIFIER, "stuff", 4, 8));
		expected_three.push_back(Token(TokenType::COMMA, ",", 4, 13));
		expected_three.push_back(Token(TokenType::IDENTIFIER, "things", 4, 15));
		expected_three.push_back(Token(TokenType::ENDOFFILE, "", 4, 21));

		expected_four.push_back(Token(TokenType::IDENTIFIER, "george", 1, 0));
		expected_four.push_back(Token(TokenType::COMMA, ",", 1, 6));
		expected_four.push_back(Token(TokenType::IDENTIFIER, "stuff", 1, 8));
		expected_four.push_back(Token(TokenType::COMMA, ",", 1, 13));
		expected_four.push_back(Token(TokenType::IDENTIFIER, "things", 1, 15));
		expected_four.push_back(Token(TokenType::ENDOFFILE, "", 2, 0));
		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}

		Lexer lex_three(input_three);
		PL_ASSERT_EQUAL(lex_three.tokens.length(), length_three);
		for (uint i = 0; i < length_three; i++) {
			PL_ASSERT_EQUAL(lex_three.tokens[i], expected_three[i]);
		}

		Lexer lex_four(input_four);
		PL_ASSERT_EQUAL(lex_four.tokens.length(), length_four);
		for (uint i = 0; i < length_four; i++) {
			PL_ASSERT_EQUAL(lex_four.tokens[i], expected_four[i]);
		}
		return true;
	}

	~TestLexerComments() = default;
};

// R-tB0007
class TestLexerStrings : public Test
{
	onestring input_one = "steve is \"a string\"";
	FlexArray<Token> expected_one;
	uint length_one = 4;

	onestring input_two = "joe is \"a multi line\n string\", 17";
	FlexArray<Token> expected_two;
	uint length_two = 6;

public:
	TestLexerStrings() = default;

	testdoc_t get_title() override { return "Lex String Tokens"; }

	testdoc_t get_docs() override
	{
		return "Successfully lex and tokenize input including quoted strings";
	}
	bool pre() override
	{
		// Add Tokens to comparison
		expected_one.push_back(Token(TokenType::IDENTIFIER, "steve", 1, 0));
		expected_one.push_back(Token(TokenType::IDENTIFIER, "is", 1, 6));
		expected_one.push_back(Token(TokenType::STRING, "a string", 1, 9));
		expected_one.push_back(Token(TokenType::ENDOFFILE, "", 1, 19));

		expected_two.push_back(Token(TokenType::IDENTIFIER, "joe", 1, 0));
		expected_two.push_back(Token(TokenType::IDENTIFIER, "is", 1, 4));
		expected_two.push_back(Token(TokenType::STRING, "a multi line\n string", 1, 7));
		expected_two.push_back(Token(TokenType::COMMA, ",", 2, 0));
		expected_two.push_back(Token(TokenType::NUMBER, "17", 2, 2));
		expected_two.push_back(Token(TokenType::ENDOFFILE, "", 2, 4));
		return true;
	}
	bool run() override
	{
		Lexer lex_one(input_one);
		PL_ASSERT_EQUAL(lex_one.tokens.length(), length_one);
		for (uint i = 0; i < length_one; i++) {
			PL_ASSERT_EQUAL(lex_one.tokens[i], expected_one[i]);
		}

		Lexer lex_two(input_two);
		PL_ASSERT_EQUAL(lex_two.tokens.length(), length_two);
		for (uint i = 0; i < length_two; i++) {
			PL_ASSERT_EQUAL(lex_two.tokens[i], expected_two[i]);
		}
		return true;
	}

	~TestLexerStrings() = default;
};


// R-sB00 //
class TestSuite_Lexer : public TestSuite
{
public:
	TestSuite_Lexer() = default;
	void load_tests() override;
	testdoc_t get_title() override { return "Ratscript: Lexer Function Tests"; }
	~TestSuite_Lexer() = default;
};

#endif  // !RATSCRIPT_LEXER_TESTS_HPP
