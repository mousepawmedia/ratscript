/** System [Ratscript]
  * Version: 0.1
  *
  * Manages data about the system.
  *
  * Author(s): Jason C. McDonald
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

#ifndef R_SYS_HPP
#define R_SYS_HPP

#include <iostream>
#include <string>

#ifdef _WIN64
    const int C_OS = 12;
#elif _WIN32
    const int C_OS = 11;
#elif __APPLE__
    const int C_OS = 21;
#elif __linux
    const int C_OS = 31;
#elif __unix
    const int C_OS = 41;
#elif __posix
    const int C_OS = 61;
#elif __FreeBSD__
    const int C_OS = 51;
#else
    const int C_OS = 0;
#endif

namespace ratscript
{
    class r_sys
    {
        public:
            r_sys();

            enum OSType
            {
                UNKNOWN = 0,
                WINT = 11,      //x32
                WINS = 12,      //x64
                APPLE = 21,
                LINUX = 31,
                UNIX = 41,
                FREEBSD = 51,
                POSIX = 61,
            };

            OSType checkOS();
            static bool useANSI();
        protected:
        private:
            static const OSType os = OSType(C_OS);
    };
}

#endif // R_SYS_H
