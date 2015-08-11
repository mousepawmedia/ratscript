#ifndef R_DEV_H
#define R_DEV_H

#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace ratscript
{
    class r_dev
    {
        public:
            r_dev(){}

            /**Broadcast a debug message, and abort program if
            something goes horribly wrong.
            @param msg: the message to broadcast
            @param panic: whether we should abort*/
            static void debug_broadcast(string, string = "", bool = false);

            /**Broadcast a verbose message. Only runs if VERBOSE is true.
            @param msg: the message to broadcast*/
            static void verbose_broadcast(string);

            ///Supress verbose messages.
            static void shutup(){verbose = false;}

            ///Permit verbose messages.
            static void speakup(){verbose = true;}
        protected:
            ///If false, verbose_broadcast will be supressed.
            static bool verbose;
        private:
    };
}
#endif // R_DEV_H
