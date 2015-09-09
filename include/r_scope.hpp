/**Ratscript Norvegicus: Scope<p>
* A scope management class.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_SCOPE_HPP_INCLUDED
#define R_SCOPE_HPP_INCLUDED

#include <map>
#include <string>

#include "r_catalog.hpp"
#include "r_var.hpp"

using std::map;
using std::string;

class r_scope
{
    public:
        r_scope(string);
        ~r_scope();
        r_scope* getScope(string);
        r_catalog* getCatalog();
        r_var* make(string, r_utils::DataType);
        r_var* retrieve(string);
        void update(string, string);
        bool addScope(string);
        int removeScope(string);
    private:
        string scopeName;
        r_catalog* catalog;
        map<string,r_scope*>* subScopes;
};

#endif // R_SCOPE_HPP_INCLUDED
