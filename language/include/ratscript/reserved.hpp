/** Reserved [Ratscript]
  * Version: 0.1
  *
  * Manages reserved keywords in Ratscript.
  *
  * Author(s): Michael Parkman, Jason C. McDonald
  */

/* LICENSE
 * Copyright (c) 2016 MousePaw Media.
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

#ifndef R_RESERVED_HPP
#define R_RESERVED_HPP

#include <iostream>
#include <string>

#include "ratscript/utils.hpp"

using namespace std;

class r_reserved
{
    public:
        r_reserved();

        enum ReserveType
        {
            NONE = 0,
            KEYWORD = 1,
            META = 2,
            COMMAND = 3,
        };

        ReserveType checkReserved(string);
    protected:
    private:
        //Keywords open (and close) constructs.
        static const int COUNT_KEYWORD = 5;
        string SYS_KEYWORD[COUNT_KEYWORD] = {"ERROR", "IF", "LOOP", "MAKE", "SWITCH"};
        //Metas modify constructs.
        static const int COUNT_META = 7;
        string SYS_META[COUNT_META] = {"CASE", "DEFAULT", "DO", "ELSE", "END", "FOR", "WHILE"};
        //Commands perform reserved actions, usually on constructs.
        static const int COUNT_COMMAND = 2;
        string SYS_COMMAND[COUNT_COMMAND] = {"BREAK", "CANCEL"};
};

#endif // R_RESERVED_H
