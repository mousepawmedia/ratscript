/** Main [Ratscript Console]
 * Version: 0.1
 *
 * The interactive console for Ratscript.
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

#include <cstring>  // strncmp
#include <iostream>
#include <string>

#include "goldilocks/shell.hpp"
#include "iosqueak/channel.hpp"

#include "ratscript/ratscript.hpp"
#include "ratscript/tests/lexer_test.hpp"

void awesome_test_code_function()
{
	channel << "The current home of awesome test code!" << IOCtrl::endl;

	// Insert awesome test code here. No crappy test code allowed D:<

	// ... okay, okay, we'll allow it if you need to <3
}

int goldilocks_console(int argc, char* argv[])
{
	GoldilocksShell* shell = new GoldilocksShell(">> ");
	shell->register_suite<TestSuite_Lexer>("R-sB00");

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
		channel << "===== Ratscript Console =====" << IOCtrl::n << IOCtrl::n
				<< "Use a filename as an argument to run the interpreter on an "
				   "input file."
				<< IOCtrl::n << IOCtrl::n
				<< "Other available arguments:" << IOCtrl::n
				<< "-g    Launch Goldilocks Shell or pass all following "
				   "arguments to it."
				<< IOCtrl::n << IOCtrl::endl;
		return 0;
	} else if (argc > 2) {
		// If we got multiple arguments without matching known flags
		channel << "Input error." << IOCtrl::n
				<< "Execute a file with: 'console [filename]' or type 'console "
				   "--help' for more options."
				<< IOCtrl::endl;
		return 64;
	} else if (argc == 2) {
		Ratscript rs;
		return rs.run_file(argv[1]);
	} else {
		// awesome_test_code_function();
		// TODO: add control for IOCat/IOVrb verbosity
		Ratscript rs;
		return rs.run_prompt();
	}

	return 0;
}
