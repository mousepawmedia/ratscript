/**Ratscript Norvegicus: System<p>
* Manages data about the system.
* @since 2.0
* @author Jason C. McDonald
*/

#ifndef R_SYS_H
#define R_SYS_H

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
