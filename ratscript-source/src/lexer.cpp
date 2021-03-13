#include "ratscript/lexer.hpp"

/** The Model namespace houses various simplexes the lexer may need to call
while processing input. */

namespace Model
{
// Simple one or two character literal models
Simplex l_paren("(");
Simplex r_paren(")");
Simplex l_brace("{");
Simplex r_brace("}");
Simplex comma(",");
Simplex dot(".");
Simplex minus("-");
Simplex plus("+");
Simplex semicolon(";");
Simplex colon(":");
Simplex star("*");
Simplex bang("!");
Simplex equal("=");
Simplex less("<");
Simplex greater(">");
Simplex hash("#");
Simplex quote('"');

// Miscellaneous single character specifier models
Simplex whitespace("^w/");
Simplex newline("^n/");

// Math models
Simplex integer("~d+/");
Simplex decimal("~d+/.~d+/");

// Alphanumeric models
Simplex alpha("~a+/");
}  // namespace Model

Lexer::Lexer(onestring source)
: source(source), start(0), current(0), line(1), last_line_pos(0)
{
	lex_tokens();
}

void Lexer::lex_tokens()
{
	while (!at_end()) {
		// We are at the beginning of the next lexeme.
		start = current;
		lex_token();
	}

	// Calculate final character position in the line.
	int line_position = current - last_line_pos;
	// End of file token.
	tokens.push_back(Token(TokenType::ENDOFFILE, "", line, line_position));
}

void Lexer::lex_token()
{
	onechar character = advance();

	if (Model::l_paren.match(character)) {
		add_token(TokenType::LEFT_PAREN);
	} else if (Model::r_paren.match(character)) {
		add_token(TokenType::RIGHT_PAREN);
	} else if (Model::l_brace.match(character)) {
		add_token(TokenType::LEFT_BRACE);
	} else if (Model::r_brace.match(character)) {
		add_token(TokenType::RIGHT_BRACE);
	} else if (Model::colon.match(character)) {
		add_token(TokenType::COLON);
	} else if (Model::comma.match(character)) {
		add_token(TokenType::COMMA);
	} else if (Model::dot.match(character)) {
		add_token(TokenType::DOT);
	} else if (Model::minus.match(character)) {
		add_token(TokenType::MINUS);
	} else if (Model::plus.match(character)) {
		add_token(TokenType::PLUS);
	} else if (Model::semicolon.match(character)) {
		add_token(TokenType::SEMICOLON);
	} else if (Model::star.match(character)) {
		add_token(TokenType::STAR);
	} else if (Model::bang.match(character)) {
		add_token(match_next("=") ? TokenType::BANG_EQUAL : TokenType::BANG);
	} else if (Model::equal.match(character)) {
		add_token(match_next("=") ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
	} else if (Model::less.match(character)) {
		add_token(match_next("=") ? TokenType::LESS_EQUAL : TokenType::LESS);
	} else if (Model::greater.match(character)) {
		add_token(match_next("=") ? TokenType::GREATER_EQUAL
								  : TokenType::GREATER);
	} else if (Model::hash.match(character)) {
		comment(character);
	} else if (Model::newline.match(character)) {
		newline();
	} else if (Model::whitespace.match(character)) {
		// Ignore whitespace, do nothing in this loop.
	} else if (Model::quote.match(character)) {
		string(character);
	} else {
		if (Model::integer.match(character)) {
			// Handle numbers
			number();
		} else if (Model::alpha.match(character)) {
			// Handle identifiers including keywords
			identifier();
		} else {
			// TODO: Throw error once errors functional
			channel
				<< "Unexpected character. Type 'exit' without quotes to exit."
				<< IOCtrl::endl;
			++current;
		}
	}
}

void Lexer::add_token(TokenType type)
{
	channel << IOCat::debug << "Single arg: " << type << " Start: " << start
			<< " Current: " << current << IOCtrl::endl;

	onestring lexeme = source.substr(start, (current - start));
	int line_position = start - last_line_pos;
	channel << IOCat::debug << "Lexeme: " << lexeme << IOCtrl::endl;
	tokens.push_back(Token(type, lexeme, line, line_position));
}

void Lexer::add_token(TokenType type, onestring lexeme)
{
	channel << IOCat::debug << "Double arg: " << type << " Start: " << start
			<< " Current: " << current << IOCtrl::endl;

	int line_position = start - last_line_pos;
	tokens.push_back(Token(type, lexeme, line, line_position));
}

onechar Lexer::advance() { return source.at(current++); }

bool Lexer::at_end() { return current >= source.length(); }

void Lexer::comment(onechar& character)
{
	if (match_next("#")) {
		// Logic for multi line comment: Initially, until we hit a new line
		// or a character that isn't a newline, ignore hashes and whitespace...
		while ((Model::hash.match(character) ||
				Model::whitespace.match(character)) &&
			   peek() != "\n" && !at_end()) {
			character = advance();
		}
		// ... Then ignore everything except newlines after that until we
		// encounter two hashes in a row...
		while (!(peek() == "#" && peek_next() == "#")) {
			character = advance();
			if (Model::newline.match(character)) {
				newline();
			}
		}
		// .. then advance the model to the next newline.
		while (peek() != "\n" && !at_end()) {
			advance();
		}
	} else {
		// Logic for single line or end of line line comment.
		while (peek() != "\n" && !at_end()) {
			advance();
		}
	}
}

void Lexer::identifier()
{
	TokenType type;
	onestring lexeme;

	// If we're at the last character, we don't want to try to lex a substring,
	// because it will break.
	if (at_end()) {
		lexeme = source.at(start);
	} else {
		SimplexResult lex_alpha = Model::alpha.lex(source.substr(start));
		// Advance current according to length of lexed identifier.
		current = (start + lex_alpha.match_length);
		lexeme = lex_alpha.snag_array[0];
	}
	// Check if identifier is a keyword, and if so assign type to token.
	if (keyword_string_to_type.count(lexeme)) {
		type = keyword_string_to_type[lexeme];
	} else {
		type = TokenType::IDENTIFIER;
	}

	add_token(type, lexeme);
}

bool Lexer::match_next(onechar expected)
{
	if (at_end()) {
		return false;
	} else if (source.at(current) != expected) {
		return false;
	} else {
		++current;
		return true;
	}
}

void Lexer::newline()
{
	// Advance the line and set the position of the last line for
	// calculating token positions in line.
	++line;
	last_line_pos = current;
}

void Lexer::number()
{
	onestring lexeme;

	// If we're at the last character, we don't want to try to lex a substring,
	// because it will break.
	if (at_end()) {
		lexeme = source.at(start);
	} else {
		SimplexResult lex_decimal = Model::decimal.lex(source.substr(start));
		SimplexResult lex_number = Model::integer.lex(source.substr(start));
		// Check for and include decimal, and advance current according to
		// length of lexed number.
		if (lex_decimal.match) {
			current = (start + lex_decimal.match_length);
			lexeme =
				lex_decimal.snag_array[0] + "." + lex_decimal.snag_array[1];
		} else {
			current = (start + lex_number.match_length);
			lexeme = lex_number.snag_array[0];
		}
	}
	add_token(TokenType::NUMBER, lexeme);
}

onechar Lexer::peek()
{
	if (at_end()) {
		return "\0";
	} else {
		return source.at(current);
	}
}

onechar Lexer::peek_next()
{
	if (current + 1 >= source.length()) {
		return "\0";
	} else {
		return source.at(current + 1);
	}
}

void Lexer::string(onechar& character)
{
	int newline_count = 0;
	// TODO: This needs adjustment, does not currently handle escaped quotes in
	// string.
	while (peek() != '"' && !at_end()) {
		character = advance();
		if (Model::newline.match(character)) {
			// Not 100% sure this will do what I expect when it comes to reading
			// files; ideally, this allows proper tracking of line number when
			// a string is spread across multiple files. Once file reading
			// is implemented, further testing and adjustment may be needed.
			++newline_count;
		}
	}

	// TODO: Add unterminated string (no closing quote) error here once
	// errors functional. For now, just create end of file token.
	if (at_end()) {
		// Calculate final character position in the line.
		int line_position = current - last_line_pos;
		// End of file token.
		tokens.push_back(Token(TokenType::ENDOFFILE, "", line, line_position));
	} else {
		// Advance for the closing ".
		advance();

		// Trim surrounding quotes.
		onestring value = source.substr(start + 1, (current - start - 2));
		add_token(TokenType::STRING, value);
	}
	for (int i = 0; i < newline_count; i++) {
		newline();
	}
}
