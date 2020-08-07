#include "ratscript/dev.hpp"

namespace ratscript
{
    bool r_dev::verbose = false;

    void r_dev::debug_broadcast(string msg, string source, bool panic)
    {
        cout << "INTERNAL ERROR @ " << source << ": " << msg << endl;
        //If we received the panic signal...
        if(panic)
        {
            //Release the Panic.
            abort();
        }
    }

    void r_dev::verbose_broadcast(string msg)
    {
        if(verbose)
        {
            cout << msg << endl;
        }
    }
}
