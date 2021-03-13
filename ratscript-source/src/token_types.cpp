#include "ratscript/token_types.hpp"

// Ratscript keywords
std::unordered_map<std::string, TokenType> keyword_string_to_type =
	{{"assert", TokenType::ASSERT},   {"break", TokenType::BREAK},
	 {"and", TokenType::AND},         {"case", TokenType::CASE},
	 {"class", TokenType::CLASS},     {"continue", TokenType::CONTINUE},
	 {"define", TokenType::DEFINE},   {"elif", TokenType::ELIF},
	 {"else", TokenType::ELSE},       {"end", TokenType::END},
	 {"fall", TokenType::FALL},       {"false", TokenType::FALSE},
	 {"finally", TokenType::FINALLY}, {"for", TokenType::FOR},
	 {"if", TokenType::IF},           {"let", TokenType::LET},
	 {"make", TokenType::MAKE},       {"nil", TokenType::NIL},
	 {"or", TokenType::OR},           {"panic", TokenType::PANIC},
	 {"pass", TokenType::PASS},       {"print", TokenType::PRINT},
	 {"return", TokenType::RETURN},   {"super", TokenType::SUPER},
	 {"switch", TokenType::SWITCH},   {"test", TokenType::TEST},
	 {"this", TokenType::THIS},       {"true", TokenType::TRUE},
	 {"until", TokenType::UNTIL},     {"warn", TokenType::WARN},
	 {"while", TokenType::WHILE}};

/** If necessary, add cases in same order as enum class. */
std::ostream& operator<<(std::ostream& stream, const TokenType& type)
{
	std::string type_str;
	switch (type) {
		case TokenType::LEFT_PAREN:
			type_str = "LEFT_PAREN";
			break;
		case TokenType::RIGHT_PAREN:
			type_str = "RIGHT_PAREN";
			break;
		case TokenType::LEFT_BRACE:
			type_str = "LEFT_BRACE";
			break;
		case TokenType::RIGHT_BRACE:
			type_str = "RIGHT_BRACE";
			break;
		case TokenType::COLON:
			type_str = "COLON";
			break;
		case TokenType::COMMA:
			type_str = "COMMA";
			break;
		case TokenType::DOT:
			type_str = "DOT";
			break;
		case TokenType::MINUS:
			type_str = "MINUS";
			break;
		case TokenType::PLUS:
			type_str = "PLUS";
			break;
		case TokenType::SEMICOLON:
			type_str = "SEMICOLON";
			break;
		case TokenType::SLASH:
			type_str = "SLASH";
			break;
		case TokenType::STAR:
			type_str = "STAR";
			break;
		case TokenType::BANG:
			type_str = "BANG";
			break;
		case TokenType::BANG_EQUAL:
			type_str = "BANG_EQUAL";
			break;
		case TokenType::EQUAL:
			type_str = "EQUAL";
			break;
		case TokenType::EQUAL_EQUAL:
			type_str = "EQUAL_EQUAL";
			break;
		case TokenType::GREATER:
			type_str = "GREATER";
			break;
		case TokenType::GREATER_EQUAL:
			type_str = "GREATER_EQUAL";
			break;
		case TokenType::LESS:
			type_str = "LESS";
			break;
		case TokenType::LESS_EQUAL:
			type_str = "LESS_EQUAL";
			break;
		case TokenType::IDENTIFIER:
			type_str = "IDENTIFIER";
			break;
		case TokenType::STRING:
			type_str = "STRING";
			break;
		case TokenType::NUMBER:
			type_str = "NUMBER";
			break;
		case TokenType::ASSERT:
			type_str = "ASSERT";
			break;
		case TokenType::AND:
			type_str = "AND";
			break;
		case TokenType::BREAK:
			type_str = "BREAK";
			break;
		case TokenType::CASE:
			type_str = "CASE";
			break;
		case TokenType::CLASS:
			type_str = "CLASS";
			break;
		case TokenType::CONTINUE:
			type_str = "CONTINUE";
			break;
		case TokenType::DEFINE:
			type_str = "DEFINE";
			break;
		case TokenType::ELIF:
			type_str = "ELIF";
			break;
		case TokenType::ELSE:
			type_str = "ELSE";
			break;
		case TokenType::END:
			type_str = "END";
			break;
		case TokenType::FALL:
			type_str = "FALL";
			break;
		case TokenType::FALSE:
			type_str = "FALSE";
			break;
		case TokenType::FINALLY:
			type_str = "FINALLY";
			break;
		case TokenType::FOR:
			type_str = "FOR";
			break;
		case TokenType::IF:
			type_str = "IF";
			break;
		case TokenType::LET:
			type_str = "LET";
			break;
		case TokenType::MAKE:
			type_str = "MAKE";
			break;
		case TokenType::NIL:
			type_str = "NIL";
			break;
		case TokenType::OR:
			type_str = "OR";
			break;
		case TokenType::PANIC:
			type_str = "PANIC";
			break;
		case TokenType::PASS:
			type_str = "PASS";
			break;
		case TokenType::PRINT:
			type_str = "PRINT";
			break;
		case TokenType::RETURN:
			type_str = "RETURN";
			break;
		case TokenType::SUPER:
			type_str = "SUPER";
			break;
		case TokenType::SWITCH:
			type_str = "SWITCH";
			break;
		case TokenType::TEST:
			type_str = "TEST";
			break;
		case TokenType::THIS:
			type_str = "THIS";
			break;
		case TokenType::TRUE:
			type_str = "TRUE";
			break;
		case TokenType::UNTIL:
			type_str = "UNTIL";
			break;
		case TokenType::WARN:
			type_str = "WARN";
			break;
		case TokenType::WHILE:
			type_str = "WHILE";
			break;
		case TokenType::ENDOFFILE:
			type_str = "ENDOFFILE";
			break;
		default:
			type_str = "Unknown";
			break;
	}
	stream << type_str;
	return stream;
}