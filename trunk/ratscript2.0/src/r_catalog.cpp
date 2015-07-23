/*RATSCRIPT 2.X NORVEGICUS
CATALOG

The catalog stores all the variables and functions defined within Ratscript.
These functions allow for the creation, search, and removal of items from the
catalog.
*/

#include "r_catalog.hpp"

r_catalog::r_catalog()
{
    //Constructor
    list = new SinglyLinkedList<r_var>();
}

r_catalog::~r_catalog()
{
    //Destructor. This will release dynamically allocated memory.
    delete(list);
}

//Returns true or false if variable exists.
bool r_catalog::exists(string name)
{
   bool r;				//Return true/false.
   int i;				//Holds the index for testing.

   i = list->indexOf(name);	//Get the master index of varaible 'name'.
   if(i == -1)				//If index is -1 (does not exist)
   {
      r = false;			//Return false.
   }
   else					    //Else if index is not -1 (exists)
   {
      r = true;				//Return true.
   }

   return r;
}

void r_catalog::print(){
    list->print();
}

//Looks up a variable and returns its type.
r_utils::DataType r_catalog::getType(string name)
{
    int i = list->indexOf(name);				            //Holds the index.
    r_utils::DataType r;		    //Holds the data type for return.

    if(i == -1)				        //If the index is -1 (does not exist)
    {
	//Throw an error.
        //errors.throwError(errors.REFERENCE_ERROR, "The variable " + name + "' is not accessible or was not declared.");
        r = r_utils::U;			    //Return unknown type.
    }
    else				            //If the index is not -1 (it exists).
    {
        r = list->get(i)->getType();		    //Return stored type.
    }

    return r;
}

//Looks up a variable by name and type.
r_pointer r_catalog::lookup(string name, r_utils::DataType type)
{
    int i = list->indexOf(name);

    if(i == -1)     //If the index is -1, the variable was not found.
    {
        //Throw an error.
        //errors.throwError(errors.REFERENCE_ERROR, "The variable '" + name + "' is not accessible or was not declared.");
    }
    r_pointer ptr(type, i);         //Create the Ratscript pointer.
    return ptr;                     //Return the Ratscript pointer.
}

//Define a new variable in the catalog.
r_var* r_catalog::make(string name, r_utils::DataType type)
{
    if(list->indexOf(name) == -1)	//If the variable does not yet exist...
    {
        switch(type)				        //Switch based on type.
        {
            case r_utils::B:			    //If {B}...
            {
                r_boolean *newB = new r_boolean(name);	//Define a new {B} variable object.
                list->addOrdered(newB, type);
                return newB;
            }
            case r_utils::T:			    //If {T}...
            {
                r_trilean *newT = new r_trilean(name);	//Same process.
                list->addOrdered(newT, type);
                return newT;
            }
            case r_utils::I:
            {
                r_integer *newI = new r_integer(name);
                list->addOrdered(newI, type);
                return newI;
            }
            case r_utils::N:
            {
                r_number *newN = new r_number(name);
                list->addOrdered(newN, type);
                return newN;
            }
            case r_utils::S:
            {
                r_string *newS = new r_string(name);
                list->addOrdered(newS, type);
                return newS;
            }
            case r_utils::A:
            {
                r_array *newA = new r_array(name);
                list->addOrdered(newA, type);
                return newA;
            }
            case r_utils::C:
            {
                r_struct *newC = new r_struct(name);
                list->addOrdered(newC, type);
                return newC;
            }
            case r_utils::R:
            {
                r_regex *newR = new r_regex(name);
                list->addOrdered(newR, type);
                return newR;
            }
            case r_utils::F:
            {
                r_function *newF = new r_function(name);
                list->addOrdered(newF, type);
                return newF;
            }
            case r_utils::D:
            {
                r_datetime *newD = new r_datetime(name);
                list->addOrdered(newD, type);
                return newD;
            }
            case r_utils::X:
            {
                r_xml *newX = new r_xml(name);
                list->addOrdered(newX, type);
                return newX;
            }
        }
    }
}

r_var* r_catalog::retrieve(string name){
    return list->get(name)->getData();
}

//Retrieve variable object (pointer) from catalog using its index.
r_boolean* r_catalog::retrieve_B(int index)
{
    //return cat_B[index];	//Return variable object from {B} catalog using index.
    r_boolean* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_boolean*>(parent)){
        temp = (r_boolean*) parent;
    }
    return temp;
}

//Retrieve variable object (pointer) from catalog using Ratscript pointer.
r_boolean* r_catalog::retrieve_B(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_boolean* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_boolean*>(parent)){
        temp = (r_boolean*) parent;
    }
    return temp;	//Return variable object from {B} catalog using Ratscript pointer index.
}

//Same for all following...
r_trilean* r_catalog::retrieve_T(int index)
{
    r_trilean* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_trilean*>(parent)){
        temp = (r_trilean*) parent;
    }
    return temp;
}

r_trilean* r_catalog::retrieve_T(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_trilean* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_trilean*>(parent)){
        temp = (r_trilean*) parent;
    }
    return temp;
}

r_integer* r_catalog::retrieve_I(int index)
{
    r_integer* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_integer*>(parent)){
        temp = (r_integer*) parent;
    }
    return temp;
}

r_integer* r_catalog::retrieve_I(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_integer* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_integer*>(parent)){
        temp = (r_integer*) parent;
    }
    return temp;
}

r_number* r_catalog::retrieve_N(int index)
{
    r_number* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_number*>(parent)){
        temp = (r_number*) parent;
    }
    return temp;
}

r_number* r_catalog::retrieve_N(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_number* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_number*>(parent)){
        temp = (r_number*) parent;
    }
    return temp;
}

r_string* r_catalog::retrieve_S(int index)
{
    r_string* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_string*>(parent)){
        temp = (r_string*) parent;
    }
    return temp;
}

r_string* r_catalog::retrieve_S(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_string* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_string*>(parent)){
        temp = (r_string*) parent;
    }
    return temp;
}

r_array* r_catalog::retrieve_A(int index)
{
    r_array* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_array*>(parent)){
        temp = (r_array*) parent;
    }
    return temp;
}

r_array* r_catalog::retrieve_A(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_array* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_array*>(parent)){
        temp = (r_array*) parent;
    }
    return temp;
}

r_struct* r_catalog::retrieve_C(int index)
{
    r_struct* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_struct*>(parent)){
        temp = (r_struct*) parent;
    }
    return temp;
}

r_struct* r_catalog::retrieve_C(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_struct* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_struct*>(parent)){
        temp = (r_struct*) parent;
    }
    return temp;
}

r_regex* r_catalog::retrieve_R(int index)
{
    r_regex* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_regex*>(parent)){
        temp = (r_regex*) parent;
    }
    return temp;
}

r_regex* r_catalog::retrieve_R(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_regex* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_regex*>(parent)){
        temp = (r_regex*) parent;
    }
    return temp;
}

r_function* r_catalog::retrieve_F(int index)
{
    r_function* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_function*>(parent)){
        temp = (r_function*) parent;
    }
    return temp;
}

r_function* r_catalog::retrieve_F(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_function* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_function*>(parent)){
        temp = (r_function*) parent;
    }
    return temp;
}

r_datetime* r_catalog::retrieve_D(int index)
{
    r_datetime* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_datetime*>(parent)){
        temp = (r_datetime*) parent;
    }
    return temp;
}

r_datetime* r_catalog::retrieve_D(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_datetime* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_datetime*>(parent)){
        temp = (r_datetime*) parent;
    }
    return temp;
}

r_xml* r_catalog::retrieve_X(int index)
{
    r_xml* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_xml*>(parent)){
        temp = (r_xml*) parent;
    }
    return temp;
}

r_xml* r_catalog::retrieve_X(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_xml* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_xml*>(parent)){
        temp = (r_xml*) parent;
    }
    return temp;
}
