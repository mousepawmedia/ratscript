# Top Level Makefile (MousePaw Media Build System)
# Version: 2.1.0
# Tailored For: Ratscript
#
# Author(s): Jason C. McDonald

# LICENSE
# Copyright (c) 2018 MousePaw Media.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
# CONTRIBUTING
# See https://www.mousepawmedia.com/developers for information
# on how to contribute to our projects.

MK_DIR = @cmake -E make_directory
CH_DIR = @cmake -E chdir
CP = @cmake -E copy
CP_DIR = @cmake -E copy_directory
RM = @cmake -E remove
RM_DIR = @cmake -E remove_directory
ECHO = @cmake -E echo
LN = @cmake -E create_symlink

none: help

help:
	$(ECHO) "=== Ratscript 2.0 ==="
	$(ECHO) "Select a build target:"
	$(ECHO) "  make ready              Build Ratscript and bundles it for distribution."
	$(ECHO) "  make clean              Clean up Ratscript."
	$(ECHO) "  make cleandebug         Clean up Ratscript Debug."
	$(ECHO) "  make cleanrelease       Clean up Ratscript Release."
	$(ECHO) "  make docs               Generate HTML docs."
	$(ECHO) "  make docs_pdf           Generate PDF docs."
	$(ECHO) "  make ratscript          Build Ratscript as release."
	$(ECHO) "  make ratscript_debug    Build Ratscript as debug."
	$(ECHO) "  make console            Build Ratscript as release."
	$(ECHO) "  make console_debug      Build Ratscript as debug."
	$(ECHO) "  make all                Build everything."
	$(ECHO) "  make allfresh           Clean and rebuild everything."
	$(ECHO)
	$(ECHO) "Clang Sanitizers (requires Debug build and Clang.)"
	$(ECHO) "  SAN=address     Use AddressSanitizer"
	$(ECHO) "  SAN=leak        Use LeakSanitizer w/o AddressSanitizer (Linux only)"
	$(ECHO) "  SAN=memory      Use MemorySanitizer"
	$(ECHO) "  SAN=thread      Use ThreadSanitizer"
	$(ECHO) "  SAN=undefined   Use UndefiniedBehaviorSanitizer"
	$(ECHO)
	$(ECHO) "Optional Architecture"
	$(ECHO) "  ARCH=32         Make x86 build (-m32)"
	$(ECHO) "  ARCH=64         Make x64 build (-m64)"
	$(ECHO)
	$(ECHO) "Use Configuration File"
	$(ECHO) "  CONFIG=foo      Uses the configuration file 'foo.config'"
	$(ECHO) "                  in the root of this repository."
	$(ECHO) "  When unspecified, default.config will be used."
	$(ECHO)
	$(ECHO) "For other build options, see the 'make' command in 'docs/' and 'language/'."

clean:
	$(MAKE) clean -C language
	$(MAKE) clean -C console
	$(RM) rsconsole
	$(RM) rsconsole_debug

cleanall: clean
	$(MAKE) clean -C docs

cleandebug:
	$(MAKE) cleandebug -C language
	$(MAKE) cleandebug -C console
	$(RM) rsconsole_debug

cleanrelease:
	$(MAKE) cleanrelease -C language
	$(MAKE) cleanrelease -C console
	$(RM) rsconsole

docs:
	$(RM_DIR) docs/build/html
	$(MAKE) html -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/html/index.html'."
	$(ECHO) "-------------"

docs_pdf:
	$(MAKE) latexpdf -C docs
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "View docs at 'docs/build/latex/Ratscript.pdf'."
	$(ECHO) "-------------"

ratscript:
	$(MAKE) release -C language
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Ratscript is in 'language/bin/Release'."
	$(ECHO) "-------------"

ratscript_debug:
	$(MAKE) debug -C language
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO)  on "Ratscript is in 'language/bin/Debug'."
	$(ECHO) "-------------"

console: ratscript
	$(MAKE) release -C console
	$(RM) rsconsole
	$(LN) console/bin/Release/rsconsole rsconsole
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Ratscript Console is in 'console/bin/Release'."
	$(ECHO) "The link './rsconsole' has been created for convenience."
	$(ECHO) "-------------"

console_debug: ratscript_debug
	$(MAKE) debug -C console
	$(RM) rsconsole_debug
	$(LN) console/bin/Debug/rsconsole rsconsole_debug
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "Ratscript Console is in 'console/bin/Debug'."
	$(ECHO) "The link './rsconsole_debug' has been created for convenience."
	$(ECHO) "-------------"

ready: console
	$(RM_DIR) ratscript
	$(ECHO) "Creating file structure..."
	$(MK_DIR) ratscript/lib
	$(ECHO) "Copying Ratscript language library..."
	$(CP_DIR) language/include/ ratscript/include/
	$(CP) language/lib/Release/libratscript.a ratscript/lib/libratscript.a
	$(ECHO) "Copying Ratscript interactive console..."
	$(CP) console/bin/Release/rsconsole ratscript/rsconsole
	$(ECHO) "Copying README and LICENSE..."
	$(CP) README.md ratscript/README.md
	$(CP) LICENSE.md ratscript/LICENSE.md
	$(ECHO) "-------------"
	$(ECHO) "<<<<<<< FINISHED >>>>>>>"
	$(ECHO) "The library is in 'ratscript'."
	$(ECHO) "-------------"

all: docs ratscript

allfresh: cleanall all

.PHONY: all allfresh clean cleanall cleandebug cleanrelease console console_debug docs docs_pdf ratscript ratscript_debug ready
