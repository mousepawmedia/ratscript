/*RATSCRIPT 2.X NORVEGICUS
Variables

Defines the Ratscript variable/data type classes.*/

#include "r_var.hpp"

//FUNCTIONS FOR VAR (EMPTY)

r_var::r_var()
{
    //constructor
}

r_var::r_var(string name)
{
    refName = name;
}

r_var::~r_var(){

}

int r_var::compareTo(r_var* otherVariable)
{
    return -1;
}

bool r_var::getBooleanValue()
{
    return false;
}

bool r_var::setValue(string statement)
{
    return false;
}

string r_var::toString()
{
    return NULL;
}

string r_var::getName()
{
    return refName;
}

r_utils::DataType r_var::getType()
{
    return type;
}

//FUNCTIONS FOR POINTER
r_pointer::r_pointer(r_utils::DataType type, int pageIndex)
{
    dataType = type;
    catalogIndex = pageIndex;
}

int r_pointer::getIndex()
{
    return catalogIndex;
}

r_utils::DataType r_pointer::getDatatype()
{
    return dataType;
}

//Functions for r_operator
r_operator::r_operator(string theOperator)
{
    this->theOperator = theOperator;
    this->precedence = setPrecedence();
    r_var::type = r_utils::O;
}

string r_operator::getOperator()
{
    return this->theOperator;
}

int r_operator::getPrecedence()
{
    return this->precedence;
}


//Need to handle commas
 //precedence levels. Top is highest precedence

    //(                     1
    // ++ --                2
    // ^  root log          3
    // * / \\ %             4
    // + -                  5
    // > < >= <=            6
    // == !=                7
    // && &| ||             8
    // =                    9
    // ,                    10

//the value that is returned represents the precedence of the the given operator
//though this is returning a value, it is being used for the initialization of the
//variable precedence. Also there is already a getter called getPrecedence, thus it is set instead of get
int r_operator::setPrecedence()
{
    int r = 0;
    switch(theOperator.at(0))
    {
        //if one of these characters there is a possibility for there to be a second character in the operator
        case '+':
        case '-':
        {
            if(theOperator.size() == 1)
            {
                //if the operator is + or -
                r = 5;
            }
            else
            {
                //if the operator is ++ or --
                r = 2;
            }
            break;
        }
        case '^':
        {
            r = 3;
            break;
        }
        case '*':
        case '/':
        case '\\':
        case '%':
        {
            r = 4;
            break;
        }
        case '<':
        case '>':
        {
            //the operators > >= < <= have same precedence
            r = 6;
            break;
        }
        case '&':
        case '|':
        {
            //the operators &&, ||, or &| have the same precedence
            r = 8;
            break;
        }
        case '(':
        {
            r = 1;
            break;
        }
        case '=':
        {
            if(theOperator.size() == 1)
            {
                //the operator is =
                r = 9;
            }
            else
            {
                //the operator is ==
                r = 7;
            }
            break;
        }
        case '!':
        {
            //the operator is !=
            r = 7;
            break;
        }
        case ',':
        {
            r = 10;
            break;
        }
        //by default this character set is not an operator so return 0
        default:
            r = 0;
    }
    return r;
}

//FUNCTIONS FOR BOOLEAN

r_boolean::r_boolean(string name)
{
    r_var::type = r_utils::B;
    refName = name;
}

r_boolean::r_boolean(string name, bool b)
{
    r_var::type = r_utils::B;
    refName = name;
    value = b;
}

bool r_boolean::convert()
{
    bool r = value;
    return r;
}

//virtual function. Returns the "boolean value" of the variable
bool r_boolean::getBooleanValue(){
    bool r = value;
    return r;
}

string r_boolean::toString()
{
    if(value == TRUE)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

bool r_boolean::setValue(bool initialValue)
{
    bool r;
    switch(initialValue)
    {
        case 0:
            r = setValue(FALSE);
        break;
        case 1:
            r = setValue(TRUE);
        break;
        default:
            r = false;
            //TODO: Error. Out of Range.
        break;
    }
    return r;
}

bool r_boolean::setValue(r_boolean::BinaryState initialValue)
{
    value = initialValue;
    return true;
}

bool r_boolean::setValue(string statement)
{
    string cleanString = r_utils::cleanString(statement);
    if(cleanString == "true" || cleanString == "TRUE")
    {
        value = TRUE;
    }
    else if(cleanString == "false" || cleanString == "FALSE")
    {
        value = FALSE;
    }
    else
    {
        return false;
    }
    return true;
}

//compares two booleans. Returns 0 if they are equal
//returns -1 if they are not equal
//throws error if passed in variable is not a boolean
int r_boolean::compareTo(r_var* otherVariable)
{
    if(dynamic_cast<r_boolean*>(otherVariable))
    {
        if(this->convert() == ((r_boolean*) otherVariable)->convert())
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        //TODO add error checking into r_var
        //errors.throwError(r_errors::TYPE_ERROR, "Second argument was not a boolean");
    }
}

//FUNCTIONS FOR TRILEAN

r_trilean::r_trilean(string name)
{
    r_var::type = r_utils::T;
    refName = name;
}

r_trilean::r_trilean(string name, int val)
{
    r_var::type == r_utils::T;
    refName = name;
    setValue(val);
}

int r_trilean::convert()
{
    int r = value;
    return r;
}

//Virtual function, returns the value of the current variable
//return true if the value is true, otherwise return false
bool r_trilean::getBooleanValue()
{
    if(value == r_trilean::FALSE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

string r_trilean::toString()
{
    if(value == TRUE)
    {
        return "true";
    }
    else if(value == FALSE)
    {
        return "false";
    }
    else
    {
        return "maybe";
    }
}

bool r_trilean::setValue(int initialValue)
{
    switch(initialValue)
    {
        case 0:
            setValue(FALSE);
        break;
        case 1:
            setValue(TRUE);
        break;
        case 2:
            setValue(MAYBE);
        break;
        default:
            return false;
            //TODO: Error. Out of Range.
        break;
    }
    return true;
}

bool r_trilean::setValue(string statement)
{
    string cleanString = r_utils::cleanString(statement);
    if(cleanString == "true" || cleanString == "TRUE")
    {
        value = TRUE;
    }
    else if(cleanString == "false" || cleanString == "FALSE")
    {
        value = FALSE;
    }
    else if(cleanString == "maybe" || cleanString == "MAYBE")
    {
        value = MAYBE;
    }
    else
    {
        return false;
    }
    return true;
}

//checks to see if the two trilean values are equal. Returns 0 if they are
//returns -1 if they are not equal
//throws error if passed in variable is not a trilean
int r_trilean::compareTo(r_var* otherVariable)
{
    //checks to see if the passed in variable is an trilean
    if(dynamic_cast<r_trilean*>(otherVariable))
    {
        //if it is, cast it to a trilean and then check to see if they two values are equal
        if(this->convert() == ((r_trilean*) otherVariable)->convert())
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        //TODO throw error if not trilean
        //errors.throwError(r_errors::TYPE_ERROR, "Second argument was not a trilean");
    }
}

bool r_trilean::setValue(r_trilean::TrileanState initialValue)
{
    value = initialValue;
    return true;
}

//FUNCTIONS FOR INTEGER

r_integer::r_integer(string name)
{
    r_var::type = r_utils::I;
    refName = name;
}

r_integer::r_integer(string name, int val)
{
    r_var::type = r_utils::I;
    refName = name;
    r_integer::value = val;
}

string r_integer::toString()
{
    stringstream ss;
    ss << value;
    return ss.str();
}

bool r_integer::setValue(int val)
{
    //WARNING: Segfault is occuring here.
    r_integer::value = val;
    return true;
}

bool r_integer::setValue(string statement)
{
    try
    {
        int temp = stoi(r_utils::cleanString(statement));
        value = temp;
        return true;
    }
    catch(const std::invalid_argument& ia)
    {
        return false;
    }
}

int r_integer::getValue(){
    return r_integer::value;
}

//virtual function, return the boolean value of this variable
//if the value is a 0 return false, otherwise return true
bool r_integer::getBooleanValue()
{
    if(value == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//compares two integers, or a number and an integer
//returns < 0 if this integer comes before the passed in number or integer
//returns 0 if the two values are the same
//returns > 0 if this integer comes after the passed in number or integer
//throws error if passed in variable is not a number or an integer
int r_integer::compareTo(r_var* otherVariable)
{
    //checks to see if the passed in variable is an integer
    if(dynamic_cast<r_integer*>(otherVariable))
    {
        //if it is, cast the value to an integer
        r_integer* temp = (r_integer*)otherVariable;
        //compare the values
        if(this->getValue() == temp->getValue())
        {
            return 0;
        }
        else if(this->getValue() < temp->getValue())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    //checks to see if the passed in variable is a number
    else if(dynamic_cast<r_number*>(otherVariable))
    {
        //if it is, cast the value to a number
        r_number* temp = (r_number*)otherVariable;
        //compare the values
        if(this->getValue() == temp->getValue())
        {
            return 0;
        }
        else if(this->getValue() < temp->getValue())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        //TODO throw error if not integer or number
        //errors.throwError(r_errors::TYPE_ERROR, "Second argument was not an integer or a number");
    }
}

//FUNCTIONS FOR NUMBER

r_number::r_number(string name)
{
    r_var::type = r_utils::N;
    refName = name;
}

r_number::r_number(string name, double val)
{
    r_var::type = r_utils::N;
    refName = name;
    r_number::value = val;
}

string r_number::toString()
{
    stringstream ss;
    ss << value;
    return ss.str();
}

bool r_number::setValue(double val){
    r_number::value = val;
    return true;
}

bool r_number::setValue(string statement)
{
    try
    {
        double temp = stod(r_utils::cleanString(statement));
        value = temp;
        return true;
    }
    catch(const std::invalid_argument& ia)
    {
        return false;
    }
}

double r_number::getValue(){
    return r_number::value;
}

//virtual function, returns boolean value of the variable
//return false if the value is a 0, otherwise return false
bool r_number::getBooleanValue()
{
    if(value == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//compares two numbers, or a number and an integer
//returns < 0 if this number comes before the passed in number or integer
//returns 0 if the two values are the same
//returns > 0 if this number comes after the passed in number or integer
//throws error if passed in variable is not a number or an integer
int r_number::compareTo(r_var* otherVariable)
{
    //checks to see if the passed in variable is a number
    if(dynamic_cast<r_number*>(otherVariable))
    {
        //if it is, cast it to a number
        r_number* temp = (r_number*)otherVariable;
        //compare the values
        if(this->getValue() == temp->getValue())
        {
            return 0;
        }
        else if(this->getValue() < temp->getValue())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    //checks to see if the passed in variable is an integer
    else if(dynamic_cast<r_integer*>(otherVariable))
    {
        //if it is, cast it to an integer
        r_integer* temp = (r_integer*)otherVariable;
        //compare the values
        if(this->getValue() == temp->getValue())
        {
            return 0;
        }
        else if(this->getValue() < temp->getValue())
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        //throw error if not number or integer
        //errors.throwError(r_errors::TYPE_ERROR, "Second argument was not a number or an integer");
    }
}

//FUNCTIONS FOR STRING

r_string::r_string(string name, string initialValue)
{
    r_var::type = r_utils::S;
    refName = name;
    setValue(initialValue);
}

r_string::r_string(r_string &initialValue)
{
    setValue(initialValue.convert());
}

//compares two strings
//returns < 0 if this string comes before the passed in string alphabetically
//returns 0 if the two strings are equal to each other
//returns > 0 if this string comes after the passed in string alphabetically
//throws error if passed in variable is not a string
int r_string::compareTo(r_var* variable)
{
    //checks to see if the passed in variable is an string
    if(dynamic_cast<r_string*> (variable))
    {
        //if it is, cast it to a string
        r_string* temp = (r_string*)variable;
        //compare the values
        return convert().compare(temp->convert());
    }
    else
    {
        //TODO throw error if not string
        //errors.throwError(r_errors::TYPE_ERROR, "Second argument was not a string");
    }
}

string r_string::toString()
{
    return convert();
}

bool r_string::getBooleanValue()
{
    if(value.size() == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool r_string::append(string input)
{
    bool r = true;
    int i = 0;
    int len = input.length();

    for(i=0;i<len;i++)
    {
        value.push_back(input[i]);
    }

    return r;
}

string r_string::convert()
{
    string r;

    int i = 0;
    int len = value.size();

    for(i=0;i<len;i++)
    {
        r += value[i];
    }

    return r;
}

int r_string::length()
{
    return value.size();
}

string r_string::toLower()
{
    string r;

    int i = 0;
    int len = value.size();

    for(i=0;i<len;i++)
    {
        r += tolower(value[i]);
    }

    return r;
}

string r_string::toUpper()
{
    string r;

    int i = 0;
    int len = value.size();

    for(i=0;i<len;i++)
    {
        r += toupper(value[i]);
    }

    return r;
}

bool r_string::setValue(string input)
{
    bool r = true;
    int i = 0;
    int len = input.length();

    value.clear();

    for(i=0;i<len;i++)
    {
        value.push_back(input[i]);
    }

    return r;
}


void r_string::operator=(const string &right)
{
    setValue(right);
}

void r_string::operator=(r_string &right)
{
    string str = right.convert();
    setValue(str);
}

void r_string::operator+=(const string &right)
{
    append(right);
}

void r_string::operator+=(r_string &right)
{
    string str = right.convert();
    append(str);
}

//FUNCTIONS FOR ARRAY

r_array::r_array(string name)
{
    r_var::type = r_utils::A;
    refName = name;
}

//FUNCTIONS FOR CATALOG

r_struct::r_struct(string name)
{
    r_var::type = r_utils::C;
    refName = name;
}

//FUNCTIONS FOR REGULAR EXPRESSION

r_regex::r_regex(string name)
{
    r_var::type = r_utils::R;
    refName = name;
}

//FUNCTIONS FOR FUNCTION

r_function::r_function(string name)
{
    r_var::type = r_utils::F;
    refName = name;
}

//FUNCTIONS FOR DATETIME

r_datetime::r_datetime(string name)
{
    r_var::type = r_utils::D;
    refName = name;
}

//Function to return the day of the month.
int r_datetime::getDay()
{

}

//Function to return the day of the week.
string r_datetime::getDayOfWeekName()
{

}

//Function to return the hour.
int r_datetime::getHour()
{

}

//Function to return the hour in military time.
int r_datetime::getHourMil()
{

}

//Function to return the minute.
int r_datetime::getMinute()
{

}

//Function to return the second.
int r_datetime::getSecond()
{

}

//Function to return whether the time is A.M. or P.M.
bool r_datetime::getPeriod()
{

}

//Function to return whether the current year is a leap year.
bool r_datetime::getLeapYear()
{

}

//Function to return the name of the current month.
string r_datetime::getMonthName()
{

}

//Function to return the current year.
int r_datetime::getYear()
{

}

//Function to set the current year
void r_datetime::setYear(int initialValue)
{
    time_t t = time(0); //Current time

    struct tm * now = localtime(&t);    //Struct to hold the year to point to.

    int year = now->tm_year; //Variable to hold the current year.
}

//Function to set the current month
void r_datetime::setMonth(int initialValue)
{
    time_t t = time(0);

    struct tm * now = localtime(&t);

    int month = now->tm_mon;    //Variable to hold the current month.
}

//Function to set the current day
void r_datetime::setDay(int initialValue)
{
    time_t t = time(0);

    struct tm * now = localtime(&t);

    int day = now->tm_mday;    //Variable to hold the current day.
}

//Function to set the current day of the week
void r_datetime::setDayOfWeek(int initialValue)
{
    time_t t = time(0);

    struct tm * now = localtime(&t);

    int dayOfWeek = now->tm_wday;    //variable to hold the number day of the week.
    string dayOfWeekName;              //String to hold the name of the weekday.

    //Switch statement to set the name of the weekday.
    switch(dayOfWeek)
    {
    case 1:
        dayOfWeekName = "Sunday";
        break;
    case 2:
        dayOfWeekName = "Monday";
        break;
    case 3:
        dayOfWeekName = "Tuesday";
        break;
    case 4:
        dayOfWeekName = "Wednesday";
        break;
    case 5:
        dayOfWeekName = "Thursday";
        break;
    case 6:
        dayOfWeekName = "Friday";
        break;
    case 7:
        dayOfWeekName = "Saturday";
        break;
    }
}

//Function to set the current hour
void r_datetime::setHour(int initialValue)
{
     time_t t = time(0);

     struct tm * now = localtime(&t);

     int hour = now->tm_hour;   //Variable to hold the current hour.
}

//Function to set the current hour in military time
void r_datetime::setHourMil(int initialValue)
{
     time_t t = time(0);

     struct tm * now = localtime(&t);

     int hourMil = now->tm_hour;   //set the hour in military time.
}

//Function to set the current minute
void r_datetime::setMinute(int initialValue)
{
     time_t t = time(0);

     struct tm * now = localtime(&t);

     int minute = now->tm_min;  //Variable to hold the current minute.
}

//Function to set the current second
void r_datetime::setSecond(int initialValue)
{
     time_t t = time(0);

     struct tm * now = localtime(&t);

     int second = now->tm_sec;  //Variable to hold the current second.
}

//Function to set the current period
void r_datetime::setPeriod(bool initialValue)
{
    string period2;     //String to hold either AM or PM.

    time_t t = time(0);

    struct tm * now = localtime(&t);

    int period = now->tm_hour;

    if (period <= 1200) //Loop to determine the period based on military time
    {
        period2 = "AM";
    }
    else
    {
        period2 = "PM";
    }
}

//Function to set the current format
void r_datetime::addDataToFormat(r_datetime::DateData)
{

}
//FUNCTIONS FOR ARRAY

r_xml::r_xml(string name)
{
    r_var::type = r_utils::X;
    refName = name;
}
