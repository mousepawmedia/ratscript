none: help

help:
	@echo "=== Ratscript 2.0 ==="
	@echo "Select a build target:"
	@echo "  make ready              Build Ratscript and bundles it for distribution."
	@echo "  make clean              Clean up Ratscript."
	@echo "  make cleandebug         Clean up Ratscript Debug."
	@echo "  make cleanrelease       Clean up Ratscript Release."
	@echo "  make docs               Generate HTML docs."
	@echo "  make docs_pdf           Generate PDF docs."
	@echo "  make ratscript          Build Ratscript as release."
	@echo "  make ratscript_debug    Build Ratscript as debug."
	@echo "  make console            Build Ratscript as release."
	@echo "  make console_debug      Build Ratscript as debug."
	@echo "  make all                Build everything."
	@echo "  make allfresh           Clean and rebuild everything."
	@echo
	@echo "Clang Sanitizers (requires Debug build and Clang.)"
	@echo "  SAN=address     Use AddressSanitizer"
	@echo "  SAN=leak        Use LeakSanitizer w/o AddressSanitizer (Linux only)"
	@echo "  SAN=memory      Use MemorySanitizer"
	@echo "  SAN=thread      Use ThreadSanitizer"
	@echo "  SAN=undefined   Use UndefiniedBehaviorSanitizer"
	@echo
	@echo "Optional Architecture"
	@echo "  ARCH=32         Make x86 build (-m32)"
	@echo "  ARCH=64         Make x64 build (-m64)"
	@echo
	@echo "Use Configuration File"
	@echo "  CONFIG=foo      Uses the configuration file 'foo.config'"
	@echo "                  in the root of this repository."
	@echo "  When unspecified, default.config will be used."
	@echo
	@echo "For other build options, see the 'make' command in 'docs/' and 'language/'."

clean:
	$(MAKE) clean -C language
	$(MAKE) clean -C console
	@rm -f rsconsole
	@rm -f rsconsole_debug

cleanall: clean
	$(MAKE) clean -C docs

cleandebug:
	$(MAKE) cleandebug -C language
	$(MAKE) cleandebug -C console
	@rm -f rsconsole_debug

cleanrelease:
	$(MAKE) cleanrelease -C language
	$(MAKE) cleanrelease -C console
	@rm -f rsconsole

docs:
	@rm -rf docs/build/html
	$(MAKE) html -C docs
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "View docs at 'docs/build/html/index.html'."
	@echo "-------------"

docs_pdf:
	$(MAKE) latexpdf -C docs
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "View docs at 'docs/build/latex/Ratscript.pdf'."
	@echo "-------------"

ratscript:
	$(MAKE) release ARCH=$(ARCH) CONFIG=$(CONFIG) -C language
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "Ratscript is in 'language/bin/Release'."
	@echo "-------------"

ratscript_debug:
	$(MAKE) debug ARCH=$(ARCH) CONFIG=$(CONFIG) SAN=$(SAN) -C language
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo  on "Ratscript is in 'language/bin/Debug'."
	@echo "-------------"

console: ratscript
	$(MAKE) release ARCH=$(ARCH) CONFIG=$(CONFIG) -C console
	@rm -f rsconsole
	@ln -s console/bin/Release/rsconsole rsconsole
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "Ratscript Console is in 'console/bin/Release'."
	@echo "The link './rsconsole' has been created for convenience."
	@echo "-------------"

console_debug: ratscript_debug
	$(MAKE) debug ARCH=$(ARCH) CONFIG=$(CONFIG) -C console
	@rm -f rsconsole_debug
	@ln -s console/bin/Debug/rsconsole rsconsole_debug
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "Ratscript Console is in 'console/bin/Debug'."
	@echo "The link './rsconsole_debug' has been created for convenience."
	@echo "-------------"

ready: console
	@rm -rf ratscript
	@echo "Creating file structure..."
	@mkdir -p ratscript/lib
	@echo "Copying Ratscript language library..."
	@cp -r language/include ratscript/
	@cp language/lib/Release/libratscript.a ratscript/lib/libratscript.a
	@echo "Copying Ratscript interactive console..."
	@cp console/bin/Release/rsconsole ratscript/rsconsole
	@echo "Copying README and LICENSE..."
	@cp README.md ratscript/README.md
	@cp LICENSE.md ratscript/LICENSE.md
	@echo "-------------"
	@echo "<<<<<<< FINISHED >>>>>>>"
	@echo "The library is in 'ratscript'."
	@echo "-------------"

all: docs ratscript

allfresh: cleanall all

.PHONY: all allfresh clean cleanall cleandebug cleanrelease console console_debug docs docs_pdf ratscript ratscript_debug ready
