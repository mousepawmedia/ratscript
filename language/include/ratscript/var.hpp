/** Variables [Ratscript]
  * Version: 0.1
  *
  * Defines the Ratscript variable/data type classes. Functions are also
  * defined herein, as they are created and stored in the same way.
  *
  * Author(s): Michael Parkman, Jason C. McDonald, Hunter Gilroy
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

#ifndef R_VAR_HPP_INCLUDED
#define R_VAR_HPP_INCLUDED

#include <cctype>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "ratscript/utils.hpp"

using ratscript::r_utils;
using std::vector;

//r_var is the base class from which all other data types are derived.
class r_var
{
    public:
        r_var();
        virtual ~r_var();
        virtual bool getBooleanValue();
        r_var(string);
        virtual bool setValue(string);
        virtual string toString();
        virtual int compareTo(r_var*);
        //TODO: Add virtual function for casting to boolean.
        string getName();
        r_utils::DataType getType();
    protected:
        string refName;     //The variable name.
        r_utils::DataType type;
    private:
};

/*TODO: Add stack, queue, linkedlist, etc data types, all as aliases of
Michael's linked list class.*/

//r_pointer points to the variable's location in the catalog.
class r_pointer
{
    public:
        r_pointer(r_utils::DataType, int);
        int getIndex();
        r_utils::DataType getDatatype();
    protected:
    private:
        r_utils::DataType dataType;
        int catalogIndex;
};

//CLASSES DERIVED FROM VAR

class r_operator:public r_var
{
    public:
        r_operator(string);

        string getOperator();
        int getPrecedence();
    private:
        int setPrecedence();
        string theOperator;
        int precedence;
};

//Boolean {B} can store either a true or false.
class r_boolean:public r_var
{
    public:
        r_boolean(string);
        r_boolean(string, bool);

        enum BinaryState
        {
            FALSE = 0,
            TRUE = 1,
        };

        bool convert();
        int compareTo(r_var*);
        bool getBooleanValue();
        string toString();
        bool setValue(bool);
        bool setValue(string);
        bool setValue(BinaryState);
    protected:
        bool value;
    private:
};

//ternary {T} can store true, false, or maybe (indeterminate).
class r_ternary:public r_var
{
    public:
        r_ternary(string);
        r_ternary(string, int);

        enum TernaryState
        {
            FALSE = 0,
            TRUE = 1,
            MAYBE = 2,
        };

        int convert();
        int compareTo(r_var*);
        bool getBooleanValue();
        int getTernaryValue();
        string toString();
        bool setValue(int);
        bool setValue(string);
        bool setValue(TernaryState);
    protected:
        TernaryState value;
    private:
};

//Integer {I} can store any positive or negative integer.
class r_integer:public r_var
{
    public:
        r_integer(string);
        r_integer(string, int);
        int compareTo(r_var*);
        bool getBooleanValue();
        string toString();
        bool setValue(int val);
        bool setValue(string);
        int getValue();
    protected:
        int value;
    private:
};

//Number {N} can store any positive or negative number.
class r_number:public r_var
{
    public:
        r_number(string);
        r_number(string, double);
        int compareTo(r_var*);
        bool getBooleanValue();
        string toString();
        bool setValue(double val);
        bool setValue(string);
        double getValue();
    protected:
        double value;
    private:
};

//String {S} can store a string.
//We built this class from the ground up to ensure maximum control.
class r_string:public r_var
{
    public:
        r_string(string, string = "");
        r_string(r_string &);   //Copy constructor

        int compareTo(r_var*);
        bool getBooleanValue();
        string toString();

        bool append(string);
        string convert();
        int length();
        bool setValue(string);
        string toLower();
        string toUpper();

        void operator=(const string &);
        void operator=(r_string &);
        void operator+=(const string &);
        void operator+=(r_string &);
    protected:
        vector<char> value;
    private:
};

//Array {A} can store an array.
//This utilizes vector.
class r_array:public r_var
{
    public:
        r_array(string);
    protected:
    private:
};

//Catalog {C} (referred to in code as "s" or struct)
//is essentially a miniature implementation of catalog.
class r_struct:public r_var
{
    public:
        r_struct(string);
    protected:
    private:
};

//Regex (R) holds a regular expression.
class r_regex:public r_var
{
    public:
        r_regex(string);
    protected:
    private:
};

//Function {F} is treated as a variable here, though
//it is a more complex data type with arguments,
//return, and code.
class r_function:public r_var
{
    public:
        r_function(string);
    protected:
    private:
};

class r_datetime:public r_var
{
    public:
        r_datetime(string);

        enum DateData
        {
            Space= 0,
            Comma= 1,
            Dash= 2,
            Year= 3,
            Month= 4,
            MonthName= 5,
            DayOfWeek= 6,
            DayOfWeekName= 7,
            Day= 8,
            Hour= 9,
            HourMil= 10,
            Minute= 11,
            Second= 12,
            Period= 13,
        };

        int getYear();
        int getMonth();
        int getDay();
        int getDayOfWeek();
        int getHour();
        int getHourMil();
        int getMinute();
        int getSecond();

        bool getPeriod();
        bool getLeapYear();

        string getMonthName();
        string getDayOfWeekName();

        void setYear(int);
        void setMonth(int);
        void setDay(int);
        void setDayOfWeek(int);
        void setHour(int);
        void setHourMil(int);
        void setMinute(int);
        void setSecond(int);
        void setPeriod(bool);

        void addDataToFormat(DateData);
        void clearFormat();

        string getDateString();



    protected:
        int year, month, day, dayOfWeek, hour, minute, second, period;
        string monthName, dayOfWeekName;
        vector<DateData> format;                   //Will hold the format codes for the output.
    private:
};

class r_xml:public r_var
{
    public:
        r_xml(string);
    protected:
    private:
};

#endif // R_VAR_HPP_INCLUDED
