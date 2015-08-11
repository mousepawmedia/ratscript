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
    ///list = new SinglyLinkedList<r_var>();
    memory = new r_mem<65536>();
}

r_catalog::~r_catalog()
{
    //Destructor. This will release dynamically allocated memory.
    delete(memory);
}

//Returns true or false if variable exists.
bool r_catalog::exists(string name)
{
   int i;				//Holds the index for testing.
   return memory->binarySearch(name, &i);
}

void r_catalog::print(){
    memory->printMem();
}

//Looks up a variable and returns its type.
r_utils::DataType r_catalog::getType(string name)
{
    string binaryVal;
    if(memory->retrieve(name, &binaryVal, NULL))
    {
        return r_utils::DataType(r_utils::toInt(binaryVal));
    }
    else
    {
        return r_utils::DataType::U;
    }
}

//Looks up a variable by name and type.
///NOTE: Do we want to throw an error if the type doesn't match, or do we want to just remove type from the lookup
r_pointer r_catalog::lookup(string name, r_utils::DataType type)
{
    int index;
    if(memory->binarySearch(name, &index))
    {

    }
    r_pointer ptr(type, index);         //Create the Ratscript pointer.
    return ptr;                     //Return the Ratscript pointer.
}

//Define a new variable in the catalog.
r_var* r_catalog::make(string name, r_utils::DataType type)
{
    //NOTE: Don't need type?
    int index;
    bool tmp = memory->binarySearch(name, &index);
    if(!tmp)	//If the variable does not yet exist...
    {
        switch(type)				        //Switch based on type.
        {
            case r_utils::B:			    //If {B}...
            {
                r_boolean *newB = new r_boolean(name);	//Define a new {B} variable object.
                memory->newValue(name, type, r_utils::toBinary(newB->getBooleanValue()));
                return newB;
            }
            case r_utils::T:			    //If {T}...
            {
                r_ternary *newT = new r_ternary(name);	//Same process.
                memory->newValue(name, type, r_utils::toBinary(newT->getTernaryValue()));
                return newT;
            }
            case r_utils::I:
            {
                r_integer *newI = new r_integer(name);
                memory->newValue(name, type, r_utils::toBinary(newI->getValue()));
                return newI;
            }
            case r_utils::N:
            {
                r_number *newN = new r_number(name);
                memory->newValue(name, type, r_utils::toBinary(newN->getValue()));
                return newN;
            }
            case r_utils::S:
            {
                r_string *newS = new r_string(name);
                memory->newValue(name, type, r_utils::toBinary(newS->convert()));
                return newS;
            }
            case r_utils::A:
            {
                r_array *newA = new r_array(name);
                //NOTE: What to do for array
                //memory->newValue()name, type, r_utils::toBinary());
                return newA;
            }
            case r_utils::C:
            {
                r_struct *newC = new r_struct(name);
                //NOTE: What to do for struct?
                //memory->newValue(name, type, r_utils::toBinary(newC->toString()));
                return newC;
            }
            case r_utils::R:
            {
                r_regex *newR = new r_regex(name);
                //NOTE: What to do for regex?
                //memory->newValue(name, type, r_utils::toBinary(newR->toString()));
                return newR;
            }
            case r_utils::F:
            {
                r_function *newF = new r_function(name);
                //NOTE: WHat to do for function?
                //memory->newValue(name, type, r_utils::toBinary(newF->toString()));
                return newF;
            }
            case r_utils::D:
            {
                r_datetime *newD = new r_datetime(name);
                //NOTE: IS this what we want to do for DateTime?
                //memory->newValue(name, type, r_utils::toBinary(newD->getDateString()));
                return newD;
            }
            case r_utils::X:
            {
                r_xml *newX = new r_xml(name);
                //NOTE: What to do for XML
                //memory->newValue(name, type, nr_utils::toBinary(ewX->toString()));
                return newX;
            }
        }
    }
}

r_var* r_catalog::retrieve(string name){
    string binaryType, binaryValue;
    if(memory->retrieve(name, &binaryType, &binaryValue))
    {
        switch(r_utils::toInt(binaryType))
        {
            case r_utils::DataType::U:
            {
                ///BROADCAST ERROR?
                break;
            }
            case r_utils::DataType::B:
            {
                return new r_boolean(name, r_utils::toBool(binaryValue));
            }
            case r_utils::DataType::T:
            {
                return new r_ternary(name, r_utils::toInt(binaryValue));
            }
            case r_utils::DataType::I:
            {
                return new r_integer(name, r_utils::toTernary(binaryValue));
            }
            case r_utils::DataType::N:
            {
                return new r_number(name, r_utils::toDouble(binaryValue));
            }
            case r_utils::DataType::S:
            {
                return new r_string(name, r_utils::binaryToString(binaryValue));
            }
            //TODO: Figure these out
            case r_utils::DataType::A:
            case r_utils::DataType::C:
            case r_utils::DataType::R:
            case r_utils::DataType::F:
            case r_utils::DataType::D:
            case r_utils::DataType::X:
            case r_utils::DataType::O:
            {
                return NULL;
            }
        }
    }
    return NULL; //list->get(name)->getData();
}

//updates the passed in variable to the passed in value
void r_catalog::update(string name, string newValue)
{
    if(!memory->writeValue(name, newValue))
    {
        r_dev::debug_broadcast("Error updating the value", "update:r_catalog", false);
    }
}

/*
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
r_ternary* r_catalog::retrieve_T(int index)
{
    r_ternary* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_ternary*>(parent)){
        temp = (r_ternary*) parent;
    }
    return temp;
}

r_ternary* r_catalog::retrieve_T(r_pointer pnt)
{
    int index = pnt.getIndex();
    r_ternary* temp = 0;
    r_var* parent= list->get(index)->getData();
    if(dynamic_cast<r_ternary*>(parent)){
        temp = (r_ternary*) parent;
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
*/
