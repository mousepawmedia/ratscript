#include "ratscript/ratscript.hpp"

Ratscript::Ratscript() : had_error(false){};

onestring Ratscript::get_input()
{
	std::string console_input;

	getline(std::cin, console_input);
	return console_input;
}

void Ratscript::run(onestring source)
{
	Lexer lexer(source);
	// For now, print tokens to verify functionality.
	for (size_t i = 0; i < lexer.tokens.length(); i++) {
		channel << lexer.tokens[i] << IOCtrl::endl;
	}
}

int Ratscript::run_file(std::string path)
{
	channel << "File path entered was: " << path << IOCtrl::endl;
	channel << "Running by file path is not yet implemented." << IOCtrl::endl;

	// TODO: Get data from file and send to run()

	// Return 0 if no errors
	return 0;
}

int Ratscript::run_prompt()
{
	channel << "Welcome to Ratscript v0.1 console!" << IOCtrl::endl;

	Simplex exit("exit");
	onestring input_source = "";

	while (!exit.match(input_source)) {
		channel << "Enter code to tokenize: " << IOCtrl::endl;
		input_source = get_input();
		run(input_source);
	}

	return 0;
}