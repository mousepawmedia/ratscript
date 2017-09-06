/** Utilities [Ratscript]
  * Version: 0.1
  *
  * Various utility functions, as well as the various enums and reserved
  * keywords.
  * TODO: May be partially replaced by PawLIB - review.
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

#ifndef R_UTILS_HPP
#define R_UTILS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include <bitset>
#include <vector>

//#include "ratscript/catalog.h"

#include "ratscript/sys.hpp"

using std::bitset;
using std::vector;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

using ratscript::r_sys;

namespace ratscript
{
    class r_utils
    {
        public:
            r_utils();

///TODO HANDLE 0's FOR FLOATS AND DOUBLES
///TODO CREATE GENERIC WRAPPER METHOD THAT CALLS OTHER METHODS. SWITCH BASED ON "AS ____"
//            bool createVariable(string type, string name, string data, r_mem mem);
//            bool createInt(string name, string data, r_mem mem);
//            bool createDouble(string name, string data, r_mem mem);
//            bool createBoolean(string name, string data, r_mem mem);
//            bool createternary(string name, string data, r_mem mem);
//            bool createString(string name, string data, r_mem mem);

            enum DataType
            {
                U = 0,              //Undefined
                B = 1,              //Boolean
                T = 2,              //Ternary
                I = 3,              //Integer
                N = 4,              //Number
                S = 5,              //String
                A = 6,              //Array
                C = 7,              //Catalog
                R = 8,              //RegExp
                F = 9,              //Function
                D = 10,             //Datetime
                X = 11,             //XML
                O = 12,
            };

            //toString for the enum DataType
            static string getDataTypeName(int);

            static DataType getDataType(std::string);
            //static DataType getDataType2(std::string, r_catalog*);

            static int getPrecedence(string);
            ///TODO: Write methods for boolean & ternary. Comment toFloat and toDouble
            ///THEN Write wrapper methods for boolean & ternary.
            ///Then write a generic wrapper method just in case. Pass in varName, data, and "INTEGER" or "BOOLEAN" etc
            static string toBinary(bool b);
            static bool toBool(string);
            static int toTernary(string);
            static string toBinary(int, int numBits = 32);
            static int toInt(string);
            static string toBinary(long, int numBits = 32);
            static long toLong(string);
            static string toBinary(float);
            static float toFloat(string);
            static string toBinary(double);
            static double toDouble(string);
            static string toBinary(char);
            static char toChar(string);
            static string toBinary(string);
            static string binaryToString(string);

            //In the header file because I couldn't make it work in the cpp with templates
            //converts a vector of type T into binary
            template<class theType, class Alloc>
            static string toBinary(vector<theType, Alloc> vec)
            {
                //the stream used for converting the binary values to strings
                stringstream ss;
                for(int i = 0; i < vec.size(); i++)
                {
                    //call the specific types data to binary
                    //then adds it to the end of the current string
                    ss << toBinary(vec.at(i));
                    //Seperate each of the elements with the control code ^? (Unit Seperator)
                    //needed for strings and maybe other vectors?
                    ss << toBinary("^?");
                }
                //returns the whole string
                return ss.str();
            }

            enum ReserveType
            {
                RESERVE_NONE = 0,
                RESERVE_KEYWORD = 1,
                RESERVE_META = 2,
                RESERVE_COMMAND = 3,
            };

            static void broadcast(string, int = 0, int = 0, bool = false);

            static string cleanString(string);

            static int searchArray(string, string[], int, bool=true);
            static int searchArray(int, int[], int, bool=false);
            static int searchArray(string, const string[], int, bool=true);

            //TODO: Migrate from overloaded searchArray to template.
            //However, this doesn't work right now. Aih. -Jason

            ////template <class T>
            ////static int searchArray(T, T[], int, bool = true);

            static ReserveType checkReserved(string);
        protected:
        private:
            //Keywords open (and close) constructs.
            static const int COUNT_KEYWORD = 5;
            static const string SYS_KEYWORD[COUNT_KEYWORD];
            //Metas modify constructs.
            static const int COUNT_META = 8;
            static const string SYS_META[COUNT_META];
            //Commands perform reserved actions, usually on constructs.
            static const int COUNT_COMMAND = 2;
            static const string SYS_COMMAND[COUNT_COMMAND];

            /*TODO: Add regex-like string/character evaluator.
            Based on characters, determines whether it could be
            a certain data type (int, number, bool, string, etc.)*/

    };
}

#endif // R_UTILS_HPP
