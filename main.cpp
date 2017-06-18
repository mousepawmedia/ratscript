#include <cctype>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

//Ratscript classes
#include "r_catalog.hpp"
#include "r_dev.h"
#include "r_scope.hpp"
#include "r_sys.h"
#include "r_lexer.h"
//#include "r_lexer_fsm.h"
#include "r_var.hpp"
#include "r_evaluator_postfix.hpp"
#include "r_utils.h"
#include "r_mem.hpp"

using std::iostream;
using ratscript::r_dev;
using ratscript::r_utils;
using ratscript::r_sys;

//Function prototypes.
//void broadcast(string, int = 0, int = 0);
void runTest();
void unitTest(int);
int interpret(string);
void startup();

//Ratscript class declarations
//r_errors errors;
r_catalog* catalog;
r_scope* scope;
r_lexer* lexer;

//Variables
string buffer;
//NOTE: Used for randomized testing. remove later
std::vector<unsigned int> sequence;
std::vector<unsigned int> currRun;

//Main function
int main()
{
    //Start Ratscript and initialize its objects.
    startup();

    //Do while command is not the pseudocommand #EXIT (or #exit)...
    do
    {
        //Get the user input.
        getline(cin, buffer);

        /*NOTE: The #TEST pseudocommand and its redirect should be removed
        for the final version.*/

        //If we get the pseudocommand #TEST (or #test)...
        if(buffer.substr(0,5) == "#TEST" || buffer.substr(0,5) == "#test")
        {
            //Redirect to our testing function.
            runTest();
        }
        //Turn off VERBOSE with pseudocommand #SHUTUP.
        else if(buffer == "#SHUTUP" || buffer == "#shutup")
        {
            r_dev::shutup();
        }
        //Turn on VERBOSE with pseudocommand #SPEAKUP.
        else if(buffer == "#SPEAKUP" || buffer == "#speakup")
        {
            r_dev::speakup();
        }
        //Otherwise...
        else
        {
            //Pass to the lexer normally.
            lexer->lex(buffer);
        }
    }
    while(buffer != "#EXIT" && buffer != "#exit");

    return 0;
}

//ADD UNIT TESTS TO THE FUNCTION AT THE BOTTOM OF MAIN.CPP!

///Runs one or more unit tests (input in command line).
void runTest()
{
    /*Commenting this out, because we shouldn't reinitilize catalog
    when we call #TEST, as that will mess with some unit tests and
    erase existing variables.*/
    ////catalog = new r_catalog();

    //Broadcast beginning of test.

    //Pseudoparse.
    if(buffer.length() > 6 && buffer.substr(5,1) == " ")
    {
        //Get "arguments" for the #TEST command.
        std::stringstream tests(buffer.substr(6));
        //Temporary string for splitting by commas.
        string segment;
        //Vector of split strings.
        std::vector<string> testlist;
        //Test number (derived from segments)
        int test;

        //Get the string of arguments, parsing by the comma.
        while(std::getline(tests, segment, ','))
        {
            //If we find a space.
            if(segment.find(" ") != string::npos)
            {
                //Complain...
                r_utils::broadcast("Separate test numbers by commas, no spaces.", 32, 1);
                r_utils::broadcast("Unit tests cancelled.", 32, 1);
                //...and exit.
                return;
            }
            //Else if we find a parenthesis...
            else if(segment.find("(") != string::npos || \
                segment.find(")") != string::npos)
            {
                //Complain...
                r_utils::broadcast("List test numbers by commas. Parenthesis not required.", 32, 1);
                r_utils::broadcast("Unit tests cancelled.", 32, 1);
                //...and exit.
                return;
            }
            //Otherwise...
            else
            {
                //It is probably all right. Add the segment to the list.
                testlist.push_back(segment);
                //We'll do further checking in the next step.
            }
        }

        //For each of those segments we stored...
        for(unsigned int i = 0; i < testlist.size(); i++)
        {
            //Move each to a stringstream...
            stringstream convert(testlist[i]);
            //Attempt to convert to an integer. If we have a problem...
            if(!(convert >> test))
            {
                //Complain...
                r_utils::broadcast("Invalid test number " + testlist[i] + ".", 32, 1);
                r_utils::broadcast("Skipping...", 32, 1);
                test = -1;
                //...and move on.
                continue;
            }
            //Otherwise, if we have no trouble...
            else
            {
                //Announce that we're about to run the test.
                r_utils::broadcast("Running test " + testlist[i] + ".", 32, 1);
                //Try to run it.
                unitTest(test);
                //And then declare that we're finished.
                r_utils::broadcast("Test " + testlist[i] + " completed.\n", 32, 1);
            }
        }

        //Declare our completion.
        r_utils::broadcast("Unit tester complete! Resuming normal activity.\n", 32, 1);
    }
    //We're missing the arguments (or a leading space after #TEST)...
    else
    {
        //Display help.
        r_utils::broadcast("To run unit tests, follow #TEST with your test numbers,", 32, 1);
        r_utils::broadcast("separated by commas, no spaces. For example...", 32, 1);
        r_utils::broadcast("'#TEST 1,2,3'", 32, 1);
        r_utils::broadcast("...runs unit tests 1, 2, and 3.\n", 32, 1);
    }
}


/**Run startup functions and display version information.
*
*/
void startup()
{
    //Broadcast version.
    r_utils::broadcast("Ratscript 2.0 \"Norvegicus\"");
    //Broadcast copyright notice.
    r_utils::broadcast("Copyright (C) 2014 MousePaw Media. All Rights Reserved.");

    //If the system allows ANSI formatting...
    if(r_sys::useANSI())
    {
        //Broadcast ANSI message.
        r_utils::broadcast("ANSI formatting activated.", 34, 1);
    }
    //Otherwise, if the system does not allow ANSI formatting...
    else
    {
        //Broadcast ANSI unsupported message.
        r_utils::broadcast("ANSI formatting unsupported.");
    }

    //Print a blank line.
    cout << endl;

    //Initizalize the catalog.
    catalog = new r_catalog();
    //Create the main scope of the program
    scope = new r_scope("main");
    //Initialize the base lexer.
    lexer = new r_lexer(0, false, catalog);

}

unsigned int rollDice(unsigned int minimum, unsigned int maximum)
{
    unsigned int r = rand() % maximum + minimum;

    //Validate random number.
    r > maximum ? r = maximum : r;
    r < minimum ? r = minimum : r;

    return r;
}

void sequenceReset(unsigned int size)
{
    sequence.clear();
    for(unsigned int i=1;i<=size;i++)
    {
        sequence.push_back(i);
    }
}

int sequenceNext()
{
    int r = 0;
    if(sequence.size() != 0)
    {
        int index = (rollDice(1, sequence.size())) - 1;
        r = sequence[index];
        sequence.erase(sequence.begin() + index);
    }
    return r;
}

bool sequenceEmpty()
{
    return (sequence.size() <= 0);
}

/*------------------ UNIT TESTER ------------------*/

/**Run the indicated unit test.
@param test: The test ID number to run.
**/
void unitTest(int test)
{
    cout << ".........................................." << endl;
    switch(test)
    {
        //TEST 0: Unit Tester Alive?
        //AUTHOR: Jason C. McDonald
        case 0:
        {
            cout << "The unit tester is alive!" << endl;
            break;
        }
        /*ADD EACH ADDITIONAL UNIT TEST AS A CASE HERE.
        Be sure to keep track of ID numbers and log them on Phriction.*/

        //TEST 1:
        //AUTHOR:
        case 1:
        {
            //NOTE: Was the old evaluator test case however the old class was removed so this test needed to be removed as well
        }
        //TEST 2: Postfix Evaluator Checker
        //Author: Michael Parkman
        case 2:
        {
            r_integer* int1 = (r_integer*) scope->make("int1", r_utils::I);
            int1 -> setValue(8);
            scope->update("int1", r_utils::toBinary(int1->getValue()));

            r_integer* int2 = (r_integer*) scope->make("int2", r_utils::I);
            int2 -> setValue(2);
            scope->update("int2", r_utils::toBinary(int2->getValue()));

            r_number* num1 = (r_number*)scope->make("num1", r_utils::N);
            num1 -> setValue(5);
            scope->update("num1", r_utils::toBinary(num1->getValue()));

            r_string* string1 = (r_string*) scope->make("string1", r_utils::S);
            string1 -> setValue("Mike");
            scope->update("string1", r_utils::toBinary(string1->convert()));

            r_string* string2 = (r_string*)scope->make("string2", r_utils::S);
            string2 -> setValue("Sam I am");
            scope->update("string2", r_utils::toBinary(string2->convert()));

            string test = "(@r:c{int1} - @r:c{int2} > @r:c{num1} && @r:c{int1} * (@r:c{int2} + @r:c{num1}) == 56) && @r:c{string1} == \"Mike\"";
            //string test = "r:c{int1} = @r:c{int2} + 6";
            //above statement throws segmentation fault without the c
            r_evaluator_postfix* eval = new r_evaluator_postfix(scope);
            eval->getEvaluation(test);
            break;
        }
        //TEST 3: Scope testing
        //Author Michael Parkman
        case 3:
        {
            //create variables in the new scope
            r_integer* int1 = (r_integer*) scope->make("int1", r_utils::I);
            int1->setValue(8);
            scope->update("int1", r_utils::toBinary(int1->getValue()));

            r_integer* int2 = (r_integer*) scope->make("int2", r_utils::I);
            int2->setValue(2);
            scope->update("int2", r_utils::toBinary(int1->getValue()));
            //create a new scope and add it to the main scope
            if(scope->addScope("Temp Scope"))
            {
                //create variables in the new scope
                r_integer* int3 = (r_integer*) scope->getScope("Temp Scope")->make("int3", r_utils::I);
                int3->setValue(34);
                scope->getScope("Temp Scope")->update("int3", r_utils::toBinary(int1->getValue()));
            }
            else
            {
                cout << "ERROR CREATING SCOPE" << endl;
            }
            //use variables from both scopes
            string test = "@r:s{Temp Scope}:c{int3} / @r:c{int1} * @r:c{int2}";
            r_evaluator_postfix* eval = new r_evaluator_postfix(scope);
            eval->getEvaluation(test);
            scope->removeScope("Temp Scope");
            break;
        }
        //TEST 4: FSM Lexer Tester
        //Author: Jason C. McDonald
        case 4:
        {
            cout << ".........................................." << endl;
            //r_lexer_fsm fsmLexer;
            cout << "=======SUBTEST 1=========" << endl;
            //fsmLexer.lexString("DEFINE foo AS string =\"blah\"");
            break;
        }
        //TEST 5: Expanding and shrinking memory
        //Author: Michael Parkman
        case 5:
        {
            bitset<192>* testing = new bitset<192>();
            for(unsigned int i = 64; i < testing->size() - 64; i++)
            {
                    testing->set(i, true);
            }
            r_mem<192> myMem;
            cout << testing->to_string() << endl;
            bool b1 = myMem.test(testing);
            //cout << testing->to_string() << endl;
            for(int i = 100; i < 132; i++)
                testing->set(i, false);
            cout << testing->to_string() << endl;
            myMem.nextPage(testing, &testing);
            cout << testing->to_string() << endl;
            myMem.prevPage(testing, &testing);
            bool b2 = myMem.test(testing, 100);
            cout << "=====================================================================================================================================" << endl;
            cout << testing->to_string() << endl;
            myMem.nextPage(testing, &testing);
            cout << testing->to_string() << endl;
            if(b1 && b2)
                cout << "It works!" << endl;
            break;
        }
        //Test 6: Adding new variables and overriding their values and then deleting some
        //Author: Michael Parkman
        case 6:
        {
        ///CREATE SUPER INTENSIVE TESTING FOR MEMORY
        ///LOOK UP Dynamic dispatch
            stringstream ss;
            string bin;
            r_mem<65536> myMem;
            bool allGood = true;
            ///sometimes can not find variables. 377 is max page. 378 creates new page
            ///error retrieving a78 when creating 771 variables


            ///problem retreiving variables on the second page. Look into binary search problems for that second page

            cout << "Enter total number of variables to test" << endl;
            int i, total;
            cin >> total;
            for(i = 0; i < total && allGood; i++)
            {
                ss << 'a' << i;
                allGood = myMem.newValue(ss.str(), r_utils::DataType::S, r_utils::toBinary("a"));
                //allGood = myMem.newValue(ss.str(), r_utils::DataType::I, r_utils::toBinary(i));
                if(!(allGood = myMem.retrieve(ss.str(), NULL, &bin)))
                {
                    cout << "name is: " << ss.str() << endl;
//                        myMem.printMem();
//                        fstream myfile;
//                        myfile.open ("runsequence.txt", std::fstream::in | std::fstream::out | std::fstream::app);
//                        for(int i = 0; i < currRun.size(); i++)
//                        {
//                            myfile << currRun[i] << endl;
//                        }
                    break;
                }
                ss.str("");
            }
            if(!allGood)
            {
                cout << "============================================================ ERROR CREATING " << i - 1 <<  " ZOMG ======================================================" << endl;
            }
            else
            {
                cout << "============================================================ Variable Creation Complete! ======================================================" << endl;
            }
            //myMem.printMem();
            myMem.printMem();
//            for(i = 0; i < total && allGood; i++)
//            {
//                if(i % 5 == 0)
//                {
//                    ss << i;
//                    allGood = myMem.deleteValue(ss.str());
//                    ss.str("");
//                }
//            }
//            if(!allGood)
//            {
//                cout << "============================================================ ERROR DELETING " << i - 1 <<  " ZOMG ======================================================" << endl;
//            }
//            else
//            {
//                cout << "============================================================ Variable Deletion Complete! ======================================================" << endl;
//            }
            for(i = 0; i < total; i++)
            {
                ss << 'a' << i;
                if(i == 23)
                    cout << "";
                allGood = myMem.retrieve(ss.str(), NULL, &bin);
                if(!allGood)
                    cout << "name is: " << ss.str() << endl;//<< " and value is: " << bin << endl;
                ss.str("");
            }

            //myMem.printMem();
//            bool b1 = myMem.testNewValue("b", r_utils::DataType::I, r_utils::toBinary(6541));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            bool b2 = myMem.testNewValue("y", r_utils::DataType::I, r_utils::toBinary(4098));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            bool b3 = myMem.testNewValue("m", r_utils::DataType::I, r_utils::toBinary(1304));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            if(b1 && b2 && b3)
//                cout << "Total size is: " << myMem.getSize() << endl;
//            cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
//            if(myMem.writeValue("b", r_utils::toBinary(65535)))
//            {
//                cout << "Successfull Override!" << endl;
//                cout << "=====================================================================================================================================" << endl;
//                myMem.printMem();
//            }
//            else
//                cout << "Failed To Override" << endl;
//
//            if(myMem.deleteValue("b"))
//            {
//                cout << "Delete Successful!" << endl;
//                cout << "=====================================================================================================================================" << endl;
//                myMem.printMem();
//            }
            break;
        }
        case 7:
        {
            ///CREATE SUPER INTENSIVE TESTING FOR MEMORY
        ///LOOK UP Dynamic dispatch
            stringstream ss;
            string bin;
            r_mem<65536> myMem;
            bool allGood = true;
            ///sometimes can not find variables. 377 is max page. 378 creates new page
            ///Reverse looping has problems with finding variable a2284 but can create up to 10,000 variables and only have trouble finding that one variable

            cout << "Enter total number of variables to test" << endl;
            int i, total;
            cin >> total;
            for(i = total - 1; i > -1 && allGood; i--)
            {
                ss << 'a' << i;
                myMem.printMem();
                allGood = myMem.newValue(ss.str(), r_utils::DataType::S, r_utils::toBinary("a"));
                //allGood = myMem.newValue(ss.str(), r_utils::DataType::I, r_utils::toBinary(i));
                if(!(allGood = myMem.retrieve(ss.str(), NULL, &bin)))
                {
                    cout << "name is: " << ss.str() << endl;
//                        myMem.printMem();
//                        fstream myfile;
//                        myfile.open ("runsequence.txt", std::fstream::in | std::fstream::out | std::fstream::app);
//                        for(int i = 0; i < currRun.size(); i++)
//                        {
//                            myfile << currRun[i] << endl;
//                        }
                    break;
                }
                ss.str("");
            }
            if(!allGood)
            {
                cout << "============================================================ ERROR CREATING " << i - 1 <<  " ZOMG ======================================================" << endl;
            }
            else
            {
                cout << "============================================================ Variable Creation Complete! ======================================================" << endl;
            }
            //myMem.printMem();
            myMem.printMem();
            for(i = 0; i < total; i++)
            {
                ss << 'a' << i;
                //if(i == 337)
                //    cout << "";
                allGood = myMem.retrieve(ss.str(), NULL, &bin);
                if(!allGood)
                    cout << "name is: " << ss.str() << endl;//<< " and value is: " << bin << endl;
                ss.str("");
            }

            //myMem.printMem();
//            bool b1 = myMem.testNewValue("b", r_utils::DataType::I, r_utils::toBinary(6541));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            bool b2 = myMem.testNewValue("y", r_utils::DataType::I, r_utils::toBinary(4098));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            bool b3 = myMem.testNewValue("m", r_utils::DataType::I, r_utils::toBinary(1304));
//            cout << "=====================================================================================================================================" << endl;
//            myMem.printMem();
//            if(b1 && b2 && b3)
//                cout << "Total size is: " << myMem.getSize() << endl;
//            cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
//            if(myMem.writeValue("b", r_utils::toBinary(65535)))
//            {
//                cout << "Successfull Override!" << endl;
//                cout << "=====================================================================================================================================" << endl;
//                myMem.printMem();
//            }
//            else
//                cout << "Failed To Override" << endl;
//
//            if(myMem.deleteValue("b"))
//            {
//                cout << "Delete Successful!" << endl;
//                cout << "=====================================================================================================================================" << endl;
//                myMem.printMem();
//            }
            break;
        }
        ///run multiple times check for cominalities
        case 8:
        {
            cout << "Enter total number of variables to test" << endl;
            int total;
            cin >> total;
            for(int i = 0; i < total; i++)
            {
                stringstream ss;
                r_mem<65536> myMem;
                bool allGood = true;
                int currNum = 0;
                sequenceReset(total);
                string bin;
                while(!sequenceEmpty())
                {
                    currNum = sequenceNext();
                    currRun.push_back(currNum);
                    ss << currNum;
                    //NOTE: Used to test string values isntead of integer values
                    //allGood = myMem.newValue(ss.str(), r_utils::DataType::S, r_utils::toBinary(ss.str()));
                    myMem.newValue(ss.str(), r_utils::DataType::I, r_utils::toBinary(currNum));
                    if(!(allGood = myMem.retrieve(ss.str(), NULL, &bin)))
                    {
                        cout << "name is: " << ss.str() << endl;
                        myMem.printMem();
                        fstream myfile;
                        myfile.open ("runsequence.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                        for(int i = 0; i < currRun.size(); i++)
                        {
                            myfile << currRun[i] << endl;
                        }
                        break;
                    }
                    ss.str("");
                }
                if(!allGood)
                    break;
                sequenceReset(total);
                while(!sequenceEmpty())
                {
                    currNum = sequenceNext();
                    ss << currNum;
                    allGood = myMem.retrieve(ss.str(), NULL, &bin);
                    if(!allGood)
                    {
                        cout << "name is: " << ss.str() << endl;
                        myMem.printMem();
                        fstream myfile;
                        myfile.open ("runsequence.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                        for(int i = 0; i < currRun.size(); i++)
                        {
                            myfile << currRun[i] << endl;
                        }
                        break;
                    }
                    ss.str("");
                }
                currRun.clear();
            }
            break;
        }
        case 9:
        {
            cout << "Enter total number of variables to test" << endl;
            int total;
            cin >> total;
            cout << "Enter number to stop at" << endl;
            int stopNum;
            cin >> stopNum;
            r_mem<65536> myMem;
            fstream myfile;
            stringstream ss;
            myfile.open ("runsequence.txt", std::fstream::in | std::fstream::out | std::fstream::app);
            int currNum;
            for(int i = 0; i < total; i++)
            {
                myfile >> currNum;
                ss << currNum;
                if(currNum == stopNum)
                {
                    cout << "";
                }
                //NOTE: Used to test string values isntead of integer values
                //allGood = myMem.newValue(ss.str(), r_utils::DataType::S, r_utils::toBinary(ss.str()));
                myMem.newValue(ss.str(), r_utils::DataType::I, r_utils::toBinary(currNum));
                ss.str("");
            }
            myMem.printMem();
            break;
        }
        case 10:
        {
            float myFloat = 0.0;
            string binaryFloat = r_utils::toBinary(myFloat);
            myFloat = r_utils::toFloat(binaryFloat);
            cout << myFloat << endl;

            cout << "============= DUBBLE ============" << endl;
            double dubble = 0.0;
            string binaryDubble= r_utils::toBinary(dubble);
            dubble = r_utils::toDouble(binaryDubble);
            cout << dubble << endl;
            break;
        }
        default:
        {
            cout << "Test does not exist." << endl;
        }
    }
    cout << ".........................................." << endl;
}
