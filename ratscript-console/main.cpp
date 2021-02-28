/** Main [Ratscript Console]
 * Version: 0.1
 *
 * The interactive console for Ratscript.
 *
 * Author(s): Michael Parkman, Jason C. McDonald, Anna R. Dunster
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

#include <cstring>  // strncmp
#include <iostream>
#include <string>

#include "onestring/onestring.hpp"
#include "ratscript/lexer.hpp"
#include "ratscript/tokens.hpp"
// #include "simplexpress/simplex.hpp"

#include "goldilocks/shell.hpp"
#include "iosqueak/channel.hpp"

onestring get_input()
{
	std::string console_input;

	getline(std::cin, console_input);
	something(console_input);
	return console_input;
}

void awesome_test_code_function()
{
	channel << "The current home of awesome test code" << IOCtrl::endl;
	onestring steve = "stuff";
	channel << steve << IOCtrl::endl;

	// Simplex simplex("^d+/");
	// channel << simplex.match("1234") << IOCtrl::endl;
	// const char *stuff = "stuff";
	// TokenType tt = TokenType::RIGHT_BRACE;
	// Token test_token(stuff, tt, 2, 1);

	// channel << test_token.literal() << IOCtrl::endl;
}

int goldilocks_console(int argc, char* argv[])
{
	GoldilocksShell* shell = new GoldilocksShell(">> ");
	// shell->register_suite<TestSuite_Basic>("X-sB00");

	if (argc > 1) {
		return shell->command(argc, argv);
	}

	channel << IOFormatTextAttr::bold << IOFormatTextFG::blue
			<< "===== Goldilocks Console =====\n"
			<< IOCtrl::endl;

	// Shift control to the interactive console.
	shell->interactive();

	// Delete our GoldilocksShell.
	delete shell;
	// shell = 0;

	return 0;
}

int main(int argc, char* argv[])
{
	// Set up signal handling.
	channel.configure_echo(IOEchoMode::cout);

	// If we got command-line arguments.
	if (argc > 1 && strncmp(argv[1], "-g", 2) == 0) {
		return goldilocks_console(argc - 1, argv + 1);
	} else if (argc > 1 && strncmp(argv[1], "--help", 6) == 0) {
		channel << "RSConsole" << IOCtrl::n
				<< "-g    Launch Goldilocks Shell or pass all following arguments to it." << IOCtrl::n
				<< IOCtrl::endl;
		return 0;
	}

	channel << "Welcome to Ratscript v0.1 console, please enter a totally "
				 "random SIMPLEXpress model to get started!"
			<< IOCtrl::endl;
	// onestring model = get_input();
	// Simplex simplex(model);
	// channel << "Now let's match something against the model..." << IOCtrl::endl;
	// onestring input = get_input();
	// bool match = simplex.match(input);
	// channel << "Your input " << input
	// 		  << (match ? " matches" : " doesn't match") << " the model "
	// 		  << model << "." << IOCtrl::endl;

	return 0;
}
