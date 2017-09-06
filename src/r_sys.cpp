#include "r_sys.hpp"

namespace ratscript
{
    //Creates an instance of the system class.
    r_sys::r_sys()
    {
        //Constructor
    }

    //Indicates the current operating system (originally determined at runtime).
    r_sys::OSType r_sys::checkOS()
    {
        return os;
    }

    /*Indicates whether the current operating system is ANSI compatible.
    Returns true if system uses ANSI, false otherwise.
    */

    bool r_sys::useANSI()
    {
        bool r;

        //List all systems that don't support ANSI here.
        if(os == WINT || os == WINS)	//Win32 (WINT) or Win64 (WINS)
        {
            r = false;
        }
        else
        {
            r = true;
        }

        return r;
    }
}
