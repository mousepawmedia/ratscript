/**Ratscript Norvegicus: Catalog
* The catalog stores all the variables and functions defined within Ratscript.
* @since 2.0
* @author Michael Parkman
* @author Jason C. McDonald
*/

#ifndef R_CATALOG_HPP_INCLUDED
#define R_CATALOG_HPP_INCLUDED

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "r_utils.hpp"
#include "r_var.hpp"
#include "r_mem.hpp"
//#include "r_errors.h"
#include "SinglyLinkedList.hpp"

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
