#include "ratscript/scope.hpp"

//create a new scope with the given name
r_scope::r_scope(string name)
{
    //create a new catalog for the given scope
    catalog = new r_catalog();
    //create a new map to contain the sub scopes
    subScopes = new map<string,r_scope*>();
    //set the scope name
    scopeName = name;
}

r_scope::~r_scope()
{
    delete catalog;
    for (map<string, r_scope*>::iterator it=subScopes->begin(); it!=subScopes->end();)
    {
        map<string, r_scope*>::iterator temp = it;
        ++it;
        delete (temp->second);
        subScopes->erase(temp);
    }
}

//get the current scopes catalog
r_catalog* r_scope::getCatalog()
{
    return catalog;
}

//create a new variable in this scope
r_var* r_scope::make(string varname, r_utils::DataType dataType)
{
    return catalog->make(varname, dataType);
}

//retrieve the desired variable fromt this scope
r_var* r_scope::retrieve(string name)
{
    return catalog->retrieve(name);
}

//update the passed in variable to the passed in value
void r_scope::update(string name, string newValue)
{
    catalog->update(name, newValue);
}

//get the desired scope
//return NULL if no scope was found matching the given name
r_scope* r_scope::getScope(string scopeName)
{
    //find the desired scope
    map<string, r_scope*>::iterator theScope = subScopes->find(scopeName);
    //return the scope
    return theScope->second;
}

bool r_scope::addScope(string name)
{
    //create the new scope
    r_scope* newScope = new r_scope(name);
    //insert the scope into the map
    std::pair<map<string, r_scope*>::iterator, bool> r = subScopes->emplace(name, newScope);
    //returns false if there is already a scope with the given name
    return std::get<1>(r);
}

//erases the given scope
//returns 1 if the scope was deleted. returns 0 if nothing was deleted
int r_scope::removeScope(string name)
{
    //find the desired scope
    map<string, r_scope*>::iterator theScope = subScopes->find(name);
    //delete the scope. Call the deconstructor
    delete(theScope->second);
    //erase the scope with the given name
    return subScopes->erase(name);
}
