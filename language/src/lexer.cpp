#include "ratscript/lexer.hpp"

//Define the ruleset for MAKE statements (the sentinel characters, in order).
//We're commenting this out and using literals instead.
//const char r_lexer::makeRuleSet[MAKE_RULE_SIZE] = {' ', ' ', ' ', '='};
//Define the ruleset for IF statements.
//const char r_lexer::ifRuleSet[IF_RULE_SIZE] = {'(', ')'};

/**The constructor creates an instance of the lexer.
* @param status: The status type (enum) for the lexer.
* @param check: If true, the lexer will only check syntax, not compile.
* @param catalog: A pointer to the catalog the lexer (and its parser) is to use.
*/
r_lexer::r_lexer(int initialStatus, bool check, r_catalog* catalog)
{
    //Mark the checkOnly flag based on user input.
    checkOnly = check;
    //Initialize the status at normal.
    status = NORMAL;
    //Initialize the comment flag at false.
    possComment = false;
    //Initialize the end comment flag at false.
    possEndComment = false;
    //TODO: Figure out what this is.
    flagSpace = false;
    //Define a parser to go with the lexer, and link it to the input catalog.
    parser = new r_parser(catalog);
    //Initialize the input length as 0.
    inputLength = 0;
}

/**lex performs initial interpretation of Ratscript code.
* @param input: The string (line) of code to lex.
*/
void r_lexer::lex(string input)
{
    //Store the length of the string, so we know when we're done lexing.
    inputLength = input.length();

    //I'm commenting out the next two lines of code because I'm not sure
    //they are needed anymore.
    //parser->setLength(inputLength);
    //prepExecute();

    //If the code is subordinated...
    if (input[0] == ':')
    {
        //If we were waiting for subordinated code...
        if (expectingSubordinated == true)
        {
            //Perform a colonectomy (remove the colon from the beginning).
            input.erase(0, 1);
            //If we have a conditional object (the array thereof is not empty)...
            if(!conditionals.empty())
            {
                //Push it to the latest conditional object and move on.
                conditionals.back().queueCode(input);
            }
            //Otherwise, do nothing.
        }
        //Otherwise, if we weren't waiting for subordinated code...
        else
        {
            //Throw an error.
            r_error e(r_error::UTOK_SUBORD,
                      "Subordinated code must exist under a header.");
            e.throwError();
        }
    }
    //Otherwise, if the code is NOT subordinated...
    else
    {
        //For each character in the string...
        for (int i = 0; i < inputLength; i++)
        {
            //Lex the string, and if there is an error (returned false)...
            if (lex_char(input[i], i) == false)
            {
                //We encountered an error. Fallback to prior status.
                fallback();
                //Get out of Dodge.
                break;
            }
        }
    }
    //Clear the lexer for the next line of code, because no one likes baggage.
    flush();
}

/**Analyses each character based on position and existing flags.
* @param ch: the character to analyze
* @param i: the current index in the string
* @return Returns false if there is an error.
*/
bool r_lexer::lex_char(char ch, int i)
{
    //type is an integer that holds the statement type enum.
    int type;
    //r (return variable) is true unless there is an error.
    bool r = true;

    //Add the input character ch to the buffer (the processed code string).
    buffer = buffer + ch;

    //If we encounter a lone '>' (possComment is not yet raised)...
    if (ch == '>' && !possComment)
    {
        //Raise possComment, as we MIGHT have a comment.
        possComment = true;
        //We still need one more '>'.
    }
    //Else if possComment is raised (the last character was a '>')...
    else if (possComment)
    {
        //If the next character is also a '>'...
        if (ch == '>')
        {
            //We have a single line comment.
            //Set the new status to SLEEP for the rest of the line.
            //In other words, don't lex anything until we call flush().
            switchStatus(SLEEP);

            //We don't need to do anything else here, it'll wait for a new line.
        }
        //Else if the next character is a '*'...
        else if (ch == '*')
        {
            //We have a multiline comment.
            //Switch the lexer to (multiline) comment mode.
            switchStatus(COMMENT);
            //This means we sleep until we get the closing comment marker.

            /*return so we don't continue lexing, as we're in a comment.
            Otherwise, it'll reflag the * as the start of the closing comment
            in the next part of the lexer.*/
            return r;
        }
        else
        {
            //Else, we don't do anything - it must not be a comment.
            r_error e(r_error::ETOK_INCOMPLETE_COMM,
                      "Comments start with either a >> or a >*");
            e.throwError();
        }

        //We handled the second character, it is no longer a possible comment.
        //Reset the possComment flag accordingly.
        possComment = false;
    }

    //Switch functionality based on the lexer status (enum).
    switch (status)
    {
        //If our status is NORMAL...
        case NORMAL:
            /*We removed this because we'll actually pass code to the evaluator
            by default.*/
            /*if(ch == '@' && i == 0)
            {
                cout << "About to call a variable." << endl;
            }
            else*/

            //Start checking for a reserved keyword, saving it to 'type'.
            //We're allowing build_keyword to use the default global variable
            //"keyword" for persistant storage.
            if (type = build_keyword(ch, i, &keyword), type > 0)
            {
                //If we found a reserved keyword...
                if (type == utils.RESERVE_KEYWORD)
                {
                    //Open proper construct type.

                    //If we have "IF", we have an if statement...
                    if (keyword == "IF")
                    {
                        //Prepare the lexer for an IF construct.
                        r_lexer::prepIf();
                        //Switch the status to IF.
                        switchStatus(IF);
                    }
                    //If we have "SWITCH", we have a switch statement...
                    else if (keyword == "SWITCH")
                    {
                        //TODO: Build the switch statement lexer.
                        cout << "This is a switch statement." << endl;
                    }
                    //If we have "LOOP", we have some sort of loop statement...
                    else if (keyword == "LOOP")
                    {
                        //TODO: Build the loop statement lexer.
                        cout << "This is a loop statement." << endl;
                    }
                    /*If we have "MAKE", we are about to define a variable,
                    constant, function, or the like.*/
                    else if (keyword == "MAKE")
                    {
                        //Prepare the lexer for a MAKE statment.
                        r_lexer::prepMake();
                        //Switch the status to MAKE.
                        switchStatus(MAKE);
                    }
                    else if (keyword == "TRY")
                    {
                        //TODO: Build the try statement (as in TRY/CATCH) lexer.
                        cout << "This is an try/catch statement." << endl;
                    }
                }
                //Else if we found a reserved meta keyword...
                else if (type == utils.RESERVE_META)
                {
                    //We can't accept meta keywords without a preceding keyword.
                    //Throw an error.
                    r_error e(r_error::UKEY_CONTEXT,
                              "Cannot use " + keyword + " out of context.");
                    e.throwError();
                }
                //Else if we found a reserved command...
                else if (type == utils.RESERVE_COMMAND)
                {
                    //Run the command.
                    //TODO: Write the command lexer.
                    cout << "Run command " << keyword << "." << endl;
                }
            }
            //If it isn't any of the above...
            else
            {
                //TODO: Pass it through the evaluator instead.
            }
            break;
        //If our status is COMMENT...
        case COMMENT:
            //We're in a multiline comment.

            //If we encounter a '*'...
            if (ch == '*')
            {
                //We might be about to end the comment. Raise possEndComment.
                possEndComment = true;
            }
            /*Else if the last character was a '*',
            (and, by the logic, this one wasn't)...
            */
            else if (possEndComment)
            {
                //If we encounter a '<'...
                if (ch == '<')
                {
                    //We're done with the multiline comment.
                    //Fallback
                    fallback();

                    //(Why did we originally always set it back to normal?)
                    //status = NORMAL;
                }

                //We dealt with the second character, so unflag possEndComment.
                possEndComment = false;
            }
            break;
        //If our status is IF...
        case IF:
            //Pass the char to the IF lexing function, and mirror the return.
            r = lexIf(ch, i);
            break;
        //If our status is SWITCH...
        case SWITCH:
            //TODO: Redirect to switch lexing function, and mirror the return.
            break;
        case LOOP:
            //TODO: Redirect to loop lexing function, and mirror the return.
            break;
        case MAKE:
            //Pass the char to the MAKE lexing function, and mirror the return.
            r = lexMake(ch, i);
            break;
            //TODO: If our status is TRY...
            //(We discontinued ERROR, see above.)
            /*case ERROR:
                //Redirect to error handler.
                break;*/
    }
    //Return r (false if error, true otherwise).
    return r;
}

///prepMake initializes variables for the MAKE lexing.
void r_lexer::prepMake()
{
    //Set the rule to 0 (first token).
    rule = 0;
    //Clear the variable name (string).
    MAKE_varName = "";
    //Clear the variable data type (string).
    MAKE_varType = "";
    //Clear the variable command (string).
    MAKE_varCommand = "";
    //Clear the variable value (string).
    MAKE_varVal = "";
}

/**Lexes the given character following the MAKE statement rules.
* @param ch: The character to lex.
* @param i: The current (integer) position in the string of
* code being lexed.
*/
bool r_lexer::lexMake(char ch, int i)
{
    //Basic model: MAKE foo AS type = "initial_value"

    //r (return) is true unless there is an error.
    bool r = true;

    //Switch based on the current rule.
    switch (rule)
    {
        case 0:
            //We're waiting for the first space after MAKE.

            /*We used to use a constant character array for the ruleset, but
            //we decided to switch to literals.*/
            //if (ch == makeRuleSet[MAKE_rule])

            //If we encounter a space...
            if (ch == ' ')
            {
                //Move on to the next rule.
                rule++;
            }
            //If we didn't get the space we wanted...
            else
            {
                //Throw an error to complain.
                r_error e(r_error::ETOK_MISSING_SPACE, "Expected space after MAKE.");
                e.throwError();
                r = false;
            }
            break;
        case 1:
            //We're waiting for another space after the variable name
            //If we encounter a space, and we have a variable name...
            if (ch == ' ' && MAKE_varName != "")
            {
                //Move on to the next rule.
                rule++;
            }
            //If the character isn't a space...
            else if (ch != ' ')
            {
                //Add the character to the variable name buffer.
                MAKE_varName += ch;
            }
            break;
        case 2:
            //We're waiting for another space after the variable command.
            //If the character is a space, and we have a command...
            if (ch == ' ' && MAKE_varCommand != "")
            {
                //Move on to the next rule.
                rule++;
            }
            //If the character isn't a space...
            else if (ch != ' ')
            {
                //Add the character to the variable command buffer.
                MAKE_varCommand += ch;
                //This should probably be "AS".
            }
            break;
        case 3:
            //We're waiting for an assignment operator ('=')
            //If we get one...
            if (ch == '=')
            {
                //Move on to the next rule.
                rule++;
            }
            //If the character is anything but '='...
            else
            {
                //Add the character to the data type string.
                MAKE_varType += ch;
            }
            break;
        case 4:
            //Everything following the '=' is part of the default value.
            //Add the character to the value string.
            MAKE_varVal += ch;
            break;
    }

    //If we're at the end of the string.
    if (i == inputLength - 1)
    {
        //Clean and parse the data.
        finalizeMake();
    }

    //Return false if error, otherwise true.
    /*At the moment, this will always be true, and error handling will be
    handled by the parser instead.*/
    return r;
}

///finalizeMake cleans the data strings and sends them to the parser.
void r_lexer::finalizeMake()
{
    //Clean all four data strings for MAKE (name, command, type, and value).
    MAKE_varName = utils.cleanString(MAKE_varName);
    MAKE_varCommand = utils.cleanString(MAKE_varCommand);
    MAKE_varType = utils.cleanString(MAKE_varType);
    MAKE_varVal = utils.cleanString(MAKE_varVal);

    //Send the strings to the parser.
    parser->parseMake(MAKE_varName, MAKE_varCommand, MAKE_varType, MAKE_varVal);

    //We're done with the MAKE statement. Return to NORMAL.
    status = NORMAL;

    /*TODO: What if the code is to MAKE a function, which is multiline?
    Shouldn't we wait for an END MAKE in that case, or reset to normal if the
    next line does not have subordinated code?*/
}

///prepIf initializes variables for IF lexing.
void r_lexer::prepIf()
{
    //Set the header to false.
    header = false;
    //Set the endConstruct flag to false.
    endConstruct = false;
    //Set the rule to 0.
    rule = 0;
    //Set the open parenthesis count to 0.
    openParen = 0;
    //Clear the eval expression buffer.
    eval_buffer = "";
    //Clear the meta keyword buffer.
    meta_buffer = "";
    //Clear the vector (array) of conditionals.
    conditionals.clear();

    //Create the header conditional object...
    r_conditional c("IF", "IF");
    //...and add it to the vector of conditionals.
    conditionals.push_back(c);

    //We are probably removing these.
    /*
    //Set the IF meta keyword empty.
    IF_meta = "";
    //Set the submeta buffer empty.
    IF_submeta = "";
    //Set the IF eval to empty.
    IF_eval = "";
    */
}

/**lexMake lexes the given character following the MAKE statement rules.
* @param ch: The character to lex.
* @param i: The current (integer) position in the string of code being lexed.*/
bool r_lexer::lexIf(char ch, int i)
{
    //Basic model: IF (condition)
    //or: <META> IF (condition)
    //or: END IF

    //r (return) is true unless there is an error.
    bool r = true;

    //If we already have the header, and the meta_buffer is not ELSE IF...
    if(header == true && meta_buffer != "ELSE IF")
    {
        int type;
        if(meta_buffer == "ELSE")
        {
            if(rule == 0 && ch == ' ')
            {
                rule++;
            }
            else if(rule == 1 && ch == 'I')
            {
                rule++;
            }
            else if(rule == 2 && ch == 'F')
            {
                //We must be on an ELSE IF conditional. Record that fact...
                meta_buffer = "ELSE IF";
                //Set the rule to 0, so we can continue in the next part.
                rule = 0;
                //Create the conditional object, in prep for the eval string.
                r_conditional c("IF", "ELSE IF");
                conditionals.push_back(c);
            }
            else
            {
                /*TODO: Throw an error. Either we get ELSE IF,
                or stop at ELSE altogether.*/
                cout << "There was a problem." << endl;
            }
        }
        else if(meta_buffer == "END")
        {
            if(rule == 0 && ch == ' ')
            {
                rule++;
            }
            else if(rule == 1 && ch == 'I')
            {
                rule++;
            }
            else if(rule == 2 && ch == 'F')
            {
                endConstruct = true;
            }
            else
            {
                r_error e(r_error::EKEY_END, "Expected \"END IF\".");
                e.throwError();
                //Clear out meta. We'll give them another chance.
                meta_buffer = "";
                r = true;
                return r;
            }
        }
        else if (type = build_keyword(ch, i, &meta_buffer), type > 0 &&
            type == utils.RESERVE_META)
        {
            //Ignore reserved keywords.
            //If we found a valid reserved meta keyword...
            if (meta_buffer == "ELSE" || meta_buffer == "END")
            {
                //NOTE: What was this even for?
            }
            //Else if we found a reserved command...
            else if (type == utils.RESERVE_COMMAND)
            {
                //Run the command.
                //TODO: Run the command (BREAK, CANCEL)
            }
        }
    }

    /*Else if we haven't yet compiled the header
    or are dealing with an ELSE IF...*/
    //(Combining these just saves time and code.)
    else if (header == false || meta_buffer == "ELSE IF")
    {
        //Do so now. Switch based on rule.
        switch (rule)
        {
            case 0:
                //If we hit the opening parenthesis...
                if (ch == '(')
                {
                    //Progress the rule count.
                    rule++;
                }
                else
                {
                    //Throw an error...
                    r_error e(r_error::ETOK_OPEN_PAREN, "");
                    e.throwError();
                    //...and return false.
                    return false;
                }
                break;
            case 1:
                //If we hit the final closing parenthesis, based on the char '('
                //and the open parenthesis count being 0...
                if (ch == ')' && openParen == 0)
                {
                    //Progress the rule count.
                    rule++;
                }
                //If we hit the last character, and we didn't get a final ')'
                else if(i == inputLength - 1 && (ch != ')' || openParen != 0))
                {
                    //Throw an error...
                    r_error e(r_error::ETOK_CLOSE_PAREN, "");
                    e.throwError();
                    //...and return false.
                    return false;
                }
                //Otherwise...
                else
                {
                    //We need to push the character to the eval string.
                    eval_buffer += ch;
                    //If we open another parenthesis...
                    if (ch == '(')
                    {
                        //Increase our count of open parenthesis.
                        openParen++;
                    }
                    //If we close a parenthesis...
                    else if (ch == ')')
                    {
                        //Decrease our count of open parenthesis.
                        openParen--;
                    }
                }
                break;
        }
    }

    if (i == inputLength - 1)
    {
        endLineIf();
    }
    return r;
}

///endLineIf clears intermediate data at the end of each line lexing.
void r_lexer::endLineIf()
{
    //If we didn't have the header before, we obviously do now.
    header == false ? header = true : header;

    /*If we have a conditional object (the array thereof is not empty)
    and the eval_buffer is not an empty string.*/
    if(!conditionals.empty() && eval_buffer != "")
    {
        //Write the eval to the latest conditional object.
        conditionals.back().setEval(eval_buffer);
    }
    /*Otherwise, if we have an ELSE (there should be no eval), create the
    conditional object and move on.*/
    else if(meta_buffer == "ELSE")
    {
        //Create a new ELSE conditional and store it.
        r_conditional c("IF", "ELSE");
        conditionals.push_back(c);
    }

    //Clear meta_buffer.
    meta_buffer = "";

    //Clear the eval buffer in preparation for the next part, if any.
    eval_buffer = "";

    //We should be getting subordinated code now.
    expectingSubordinated = true;

    //Set the rule to 0.
    rule = 0;
    //Set the open parenthesis count to 0.
    openParen = 0;

    if(endConstruct == true)
    {
        finalizeIf();
    }
}

///Finalize IF lexing, sending the line of code to the parser.
void r_lexer::finalizeIf()
{
    //Send the conditionals array to the parser.
    parser->parseIf(conditionals);

    /*Switching to normal is fine (instead of fallback), since we never lex
    nested code in the same lexer object.*/
    switchStatus(NORMAL);

    //No more subordinated code, please! We're done.
    expectingSubordinated = false;
}

//I don't think we'll be keeping this stuff.
/*void r_lexer::prepExecute()
{
    EXECUTE_command = "";
    EXECUTE_keyword = "";
    EXECUTE_rule = 0;
}

//lexExecute -> lexecute
bool r_lexer::lexecute(char ch, int i)
{
    bool r = true;
    switch (EXECUTE_rule)
    {
    case 0:
        if (ch == ':')
        {
            EXECUTE_rule++;
        }
        else if (ch != ' ')
        {
            EXECUTE_keyword += ch;
            if (EXECUTE_keyword == "ENDIF")
            {
                status = NORMAL;
            }
            else if (EXECUTE_keyword.size() || i == inputLength - 1)
            {
                errors.throwError(errors.SYNTAX_ERROR, "Need a colon to start execution line");
                r = false;
            }
        }
        break;
    case 1:
        if (ch == ' ' && EXECUTE_command != "")
        {
            EXECUTE_rule++;
        }
        else if (ch != ' ')
        {
            EXECUTE_command += ch;
        }
        break;
    }
    if (EXECUTE_command == "PRINT")
    {
        cout << "Print Command" << endl;
    }
    return r;
}*/


/**build_keyword collects characters to attempt a keyword build, and
then returns the match type, if any.
ch: the character to analyze
i: the current index in the string
saveTo: a pointer to the string to save to. (Default keyword)
Returns the keyword type
*/
int r_lexer::build_keyword(char ch, int i, string* saveTo)
{
    //Store and return the reserve type (enum)
    int type;

    //All characters should be treated as uppercase in keywords.
    ch = toupper(
             ch);

    //Add the character to the buffer.
    keybuffer = keybuffer + ch;

    //Check the reservation type (if any) of the buffer so far, and store the
    //reservation type.
    type = utils.checkReserved(keybuffer);

    //If there is a reservation (type is not RESERVE_NONE)
    if (type != utils.RESERVE_NONE)
    {
        //Save the buffer to the indicated location.
        *saveTo = keybuffer;
        //Clear the buffer.
        keybuffer = "";
    }

    //Return the type.
    return type;
}

///flush resets the lexer's status and global flags.
void r_lexer::flush()
{
    //If the lexer is sleeping...
    if (status == SLEEP)
    {
        //WAKE UP! (Return to prior status)
        fallback();
    }

    //Unflag possible comment...
    possComment = false;
    //..and possible end comment flags.
    possEndComment = false;

    //Clear buffer.
    buffer = "";
    //Clear key buffer.
    keybuffer = "";
    //Clear keyword.
    keyword = "";
}

/**checkStatus returns lexer status code.
* @return the lexer's status code.
*/
r_lexer::LexerStatus r_lexer::checkStatus()
{
    //Return lexer's stored status code.
    return status;
}

/**switchStatus switches the lexer to a new status.
* Should be called EVERY TIME we lex a new line, for
* fallback to work properly.
* @param newStatus: The status to switch to.
*/
void r_lexer::switchStatus(r_lexer::LexerStatus newStatus)
{
    //Store current status as fallbackStatus, in case of error.
    fallbackStatus = status;
    //Set the new status.
    status = newStatus;
}

/**fallback returns to the last status if there is
an error.*/
void r_lexer::fallback()
{
    //Set the status to the fallbackStatus.
    status = fallbackStatus;
    //Set the fallback to NORMAL, just in case.
    fallbackStatus = NORMAL;
}

//Status should NOT be directly modifiable.
//It needs to rely on commands (i.e. BREAK[]) and proper syntax (END MAKE).
