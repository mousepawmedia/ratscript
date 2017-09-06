#include "r_parser.hpp"

r_parser::r_parser(r_catalog* cat)
{
    catalog = cat;
    status = 0;
}
//MAKE varName AS int
/**parseMake parses the make buffers from the lexer
* and defines the variable (or function).
* @param name: The name of the variable.
* @param command: The variable creation command (usually AS)
* @param type: The variable data type.
* @param val: The default value for the variable.
*/
bool r_parser::parseMake(string name, string command, string type, string val)
{
    //This is only to test output. It won't live here forever.
    cout << name << ", " << command << ", " << type << ", " << val << endl;

    //Define the regex to match the name (alphanumeric only).
    regex regName("^\\w+$");
    /*Define the regex to match the command (alphanumeric).
    We'll validate the command in a later step.*/
    regex regCommand("@\\w+$");
    /*Define the regex to match the type (alphanumeric).
    We'll validate the command in a later step.*/
    regex regType("^\\w+$");

    //As far as regex goes, anything is fine for the value.

    if(std::regex_match(name, regName) == true)
    {
        cout << "NAME: " << name << endl;
    }
    else
    {

    }
    return false;
}

bool r_parser::parseIf(vector<r_conditional>& conditionals)
{
    cout << "Now parsing for IF..." << endl;
    for(int i=0;i<conditionals.size();i++)
    {
        r_conditional c = conditionals[i];
        cout << c.getType() << " " << c.getEval() << endl;
        while(c.queueIsEmpty() == false)
        {
            cout << ":" << c.dequeCode() << endl;
        }
    }
}

void r_parser::setLength(int i)
{
    r_parser::inputLength = i;
}

