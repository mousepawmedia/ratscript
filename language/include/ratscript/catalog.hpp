/** Catalog [Ratscript]
  * Version: 0.1
  *
  * The catalog stores all the variables and functions defined within Ratscript.
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



#ifndef R_CATALOG_HPP_INCLUDED
#define R_CATALOG_HPP_INCLUDED

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "ratscript/utils.hpp"
#include "ratscript/var.hpp"
#include "ratscript/mem.hpp"
//#include "ratscript/errors.h"
#include "pawlib/singly_linked_list.hpp"

using ratscript::r_utils;

class r_catalog
{
    public:
        r_catalog();		//Constructor
        ~r_catalog();		//Destructor

        //r_errors errors;	//Error system.

        //Function definitions. See r_catalog.cpp
        bool exists(string);
        r_utils::DataType getType(string);
        r_pointer lookup(string, r_utils::DataType);
        r_var* make(string, r_utils::DataType);
        void print();

        r_var* retrieve(string);
        void update(string, string);
        r_boolean* retrieve_B(int);
        r_boolean* retrieve_B(r_pointer);

        r_ternary* retrieve_T(int);
        r_ternary* retrieve_T(r_pointer);

        r_integer* retrieve_I(int);
        r_integer* retrieve_I(r_pointer);

        r_number* retrieve_N(int);
        r_number* retrieve_N(r_pointer);

        r_string* retrieve_S(int);
        r_string* retrieve_S(r_pointer);

        r_array* retrieve_A(int);
        r_array* retrieve_A(r_pointer);

        r_struct* retrieve_C(int);
        r_struct* retrieve_C(r_pointer);

        r_regex* retrieve_R(int);
        r_regex* retrieve_R(r_pointer);

        r_function* retrieve_F(int);
        r_function* retrieve_F(r_pointer);

        r_datetime* retrieve_D(int);
        r_datetime* retrieve_D(r_pointer);

        r_xml* retrieve_X(int);
        r_xml* retrieve_X(r_pointer);
    protected:

    private:
        //Singly Linked List to store names types. Maybe variables?
        ///SinglyLinkedList<r_var>* list;
        r_mem<65536>* memory;
        //TODO: Add scope to variables.

};

#endif // R_CATALOG_HPP_INCLUDED
