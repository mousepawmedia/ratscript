#include "ratscript/tests/lexer_test.hpp"

void TestSuite_Lexer::load_tests()
{
	register_test("R-tB0000", new TestLexerSingleCharacter());
	register_test("R-tB0001", new TestLexerTwoCharacter());
	register_test("R-tB0002", new TestLexerKeywords());
	register_test("R-tB0003", new TestLexerNumbers());
	register_test("R-tB0004", new TestLexerIdentifiers());
	register_test("R-tB0005", new TestLexerNewlines());
	register_test("R-tB0006", new TestLexerComments());
	register_test("R-tB0007", new TestLexerStrings());
}
