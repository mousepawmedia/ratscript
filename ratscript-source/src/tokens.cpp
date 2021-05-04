#include "ratscript/tokens.hpp"

Token::Token(TokenType token_type,
			 onestring lexeme,
			 uint_fast32_t line,
			 uint_fast32_t line_position)
: token_type(token_type), lexeme(lexeme), line(line),
  line_position(line_position)
{
}

onestring Token::to_string() const
{
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const Token& token)
{
	stream << token.token_type << " \"" << token.lexeme << "\" at position "
		   << token.line << ":" << token.line_position;
	return stream;
}

bool Token::operator==(const Token& other) const
{
	return (this->token_type == other.token_type &&
			this->lexeme == other.lexeme && this->line == other.line &&
			this->line_position == other.line_position);
}

bool Token::operator!=(const Token& other) const { return !(*this == other); }
