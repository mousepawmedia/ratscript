/**Ratscript Norvegicus: Memory<p>
* A memory management class.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_MEM_HPP_INCLUDED
#define R_MEM_HPP_INCLUDED

#include <bitset>
#include <iostream>
#include <fstream>

#include "r_error.hpp"
#include "r_utils.hpp"
#include "r_dev.hpp"

using std::fstream;

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
const int memSize = 64;
#else
const int memSize = 32;
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
const int memSize = 64;
#else
const int memSize = 32;
#endif
#endif

/*The following example code would dynamically allocate a bitset as 1000,
change it to 0101, and then delete it at the end.
    typedef bitset<4> testBitset;
    testBitset* tB = new testBitset("1000");
    tB->reset(3);
    tB->set(0);
    tB->set(2);
    cout << *tB << endl;
    delete tB;
    break;
*/

//64 KB
const int L_PAGE_SIZE = 65536;
//32 KB
const int M_PAGE_SIZE = 32768;
//4 KB
const int S_PAGE_SIZE = 4096;
//A record has a maximum size of 32 B.
const int RECORD_SIZE = 32;

const std::bitset<8> NULL_CHARACTER("00000000");
const std::bitset<8> CONTROL_CODE("01011110");
const std::bitset<8> RECORD_SEPERATOR("01111110");
const std::bitset<8> UNIT_SEPERATOR ("00111111");
const std::bitset<8> DATA_SEPERATOR("01011001");
const std::bitset<8> GROUP_SEPERATOR("01011101");

typedef bitset<L_PAGE_SIZE> l_page;
typedef bitset<M_PAGE_SIZE> m_page;
typedef bitset<S_PAGE_SIZE> s_page;

typedef bitset<RECORD_SIZE> record;

using std::bitset;
using ratscript::r_dev;

template<size_t PageSize>
class r_mem
{
    public:

        typedef bitset<L_PAGE_SIZE> l_page;
        typedef bitset<M_PAGE_SIZE> m_page;
        typedef bitset<S_PAGE_SIZE> s_page;

        typedef bitset<RECORD_SIZE> record;

        //ENTRY: Constructor
        r_mem()
        {
            //create the firstPage
            firstPage = new bitset<PageSize>();
            //make sure all the bits are 0
            firstPage->reset();
            //set the initial size to be the size of the page
            theSize = PageSize;
            //set the default values of both cursors
            pageCursor = memSize;
            memCursor = memSize;
            //on initialization there should be nothing in the memory so we put the seperators in
            //place a Group seperator and a data seperator
            toMemory(firstPage, "1111111111111111111111111111111111111111111111111111111111111111", 0);
            toMemory(firstPage, NULL_CHARACTER, pageCursor);
            toMemory(firstPage, NULL_CHARACTER, pageCursor + 8);
            toMemory(firstPage, CONTROL_CODE, pageCursor + 16);
            toMemory(firstPage, GROUP_SEPERATOR, pageCursor + 24);
            toMemory(firstPage, NULL_CHARACTER, pageCursor + 32);
            toMemory(firstPage, NULL_CHARACTER, pageCursor + 40);
            toMemory(firstPage, CONTROL_CODE, pageCursor + 48);
            toMemory(firstPage, DATA_SEPERATOR, pageCursor + 56);
        }

        //NOTE: These are used to test different functionalities of the memory
        //Testing case. Expands the memory 32 bits starting at index 100
        bool test(bitset<PageSize>* page)
        {
            return expandMemory(page, 32, 100);
        }

        //Testing case. shrinks the memory 32 bits starting at the passed in index
        bool test(bitset<PageSize>* page, int pageIndex)
        {
            shrinkMemory(page, 32, pageIndex);
            return true;
        }

        //adds the new value with the given name, type, data
        bool testNewValue(string name, int type, string data)
        {
            return newValue(name, type, data);
        }

        //NOTE: Used for debugging. Prints out current memory values to a file
        //prints the memory starting at the first page. then prints each new page on a new line
        void printMem()
        {
            bitset<PageSize>* daPage = new bitset<PageSize>(firstPage->to_string());
            int prevCursor = pageCursor;
            print(daPage);
            setCursor(prevCursor);
        }

        //print mems helper. Continues to call itself until the passed in page is null
        void print(bitset<PageSize>* currPage)
        {
            if(currPage != 0)
            {
                string temp = currPage->to_string();
                nextPage(currPage, &currPage);
                print(currPage);
                fstream myfile;
                myfile.open ("ratscriptmemory.txt", std::fstream::in | std::fstream::out | std::fstream::app);
                myfile << temp << endl << endl << endl;
                myfile.close();
            }
        }

        //ENTRY: set next page
        //sets the next pages memory address of current page equal to the memory address of next page
        void setNextPage(bitset<PageSize>* currentPage, bitset<PageSize>* nextPage)
        {
            //converts the memory address of next page to binary
            string theAddress = r_utils::toBinary((long)nextPage, memSize);
            //stores the memory address in the correct part of the current pages memory
            toMemory(currentPage, theAddress, PageSize - memSize, true);
        }

        //ENTRY: Set previous page
        //sets the prev pages memry address of current page equal to the memory address of prev page
        void setPrevPage(bitset<PageSize>* currentPage, bitset<PageSize>* prevPage)
        {
            //converts the memory address of the prev page to binary
            string theAddress = r_utils::toBinary((long)prevPage, memSize);
            //stores the memory address in the correct part of the current pages memory
            toMemory(currentPage, theAddress, 0, true);
        }

        //ENTRY: Get next page
        //gets the next pages memory address from the current page and sets next to point to it
        void nextPage(bitset<PageSize>* currPage, bitset<PageSize>** next)
        {
            //sets the pageCursor to the next pages memory address of the current page
            pageCursor = PageSize - memSize;
            //gets the value of the memory address
            string memAddress = getValue(currPage, pageCursor, memSize, true);
            //converts the memory address from binary to a long
            long memAdd = r_utils::toLong(memAddress);
            //sets next to point to the memory address
            *next = reinterpret_cast<bitset<PageSize>*>(memAdd);
        }

        //ENTRY: Get previous page
        //gets the prev pages memory address from the current page and sets prev to point to it
        void prevPage(bitset<PageSize>* currPage, bitset<PageSize>** prev)
        {
            //sets pageCursor to 0 to get the previous pages memory address
            pageCursor = 0;
            //gets the value of the memory address
            string memAddress = getValue(currPage, 0, memSize, true);
            //converts the memory address from binary to long
            long memAdd = r_utils::toLong(memAddress);
            //sets prev to point to the memory address
            *prev = reinterpret_cast<bitset<PageSize>*>(memAdd);
        }

        //ENTRY: Read Byte
        //returns an 8 bit bitset representation of the current byte
        bitset<8> readByte(bitset<PageSize>* page)
        {
            //stores the value of the current byte
            bitset<8> temp;
            //loop through the passed in page starting at the current cursor
            for(int i = 0; i < 8; i++)
            {
                //set the index of the byte equal to the index of the page
                temp.set(i, page->test(pageCursor + i));
            }
            //move the cursor 8 bits forward
            pageCursor += 8;
            //return the byte
            return temp;
        }

        //ENTRY: Discovers the size of the current state
        //returns the size (number of bits) of the current state
        int valueSize(bitset<PageSize>* page)
        {
            //stores the current byte
            bitset<8> currByte;
            //i is the number of bits in the state, prev cursor is the previous location of the cursor
            int i, prevCurs = pageCursor, currGroupSeparator = 0, currSeperator = 0;

            bool searching = true;
            //keep looping until the end of the state or end of data is reaced
            //800 represents maximum number of bits to read through. This is equal to 800 bytes
            for(i = 0; i < 800 && searching; i++)
            {
                //if the pageCursor is at the end of the current page
                if(pageCursor >= PageSize - memSize)
                {
                    //move on to the next page
                    nextPage(page, &page);
                    //sets the page cursor to the start of the new page
                    resetPageCursor();
                }
                //move to the next byte
                currByte = readByte(page);
                //if the byte is equal to the control code it might signal the end of the state or data

                switch(currSeperator)
                {
                    case 0:
                    {
                        if(currByte == CONTROL_CODE)
                        {
                            currSeperator++;
                        }
                        break;
                    }
                    case 1:
                    {
                        if(currByte == DATA_SEPERATOR || currByte == RECORD_SEPERATOR)
                        {
                            i -= 2;
                            searching = false;
                        }
                        break;
                    }
                }
                if(searching)
                {
                    switch(currGroupSeparator)
                    {
                        case 0:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currGroupSeparator++;
                            }
                            break;
                        }
                        case 1:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currGroupSeparator++;
                            }
                            else
                            {
                                currGroupSeparator = 0;
                                currSeperator = 0;
                                pageCursor -= 8;
                                i -= 1;
                            }
                            break;
                        }
                        case 2:
                        {
                            if(currByte == CONTROL_CODE)
                            {
                                currGroupSeparator++;
                            }
                            else
                            {
                                currGroupSeparator = 0;
                                currSeperator = 0;
                                pageCursor -= 16;
                                i -= 2;
                            }
                            break;
                        }
                        case 3:
                        {
                            if(currByte == GROUP_SEPERATOR)
                            {
                                currGroupSeparator++;
                            }
                            else
                            {
                                currGroupSeparator = 0;
                                currSeperator = 0;
                                pageCursor -= 24;
                                i -= 3;
                            }
                            break;
                        }
                        case 4:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currGroupSeparator++;
                            }
                            else
                            {
                                currGroupSeparator = 0;
                                currSeperator = 0;
                                pageCursor -= 32;
                                i -= 4;
                            }
                            break;
                        }
                        case 5:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                i -= 6;
                                searching = false;
                            }
                            else
                            {
                                currGroupSeparator = 0;
                                currSeperator = 0;
                                pageCursor -= 40;
                                i -= 5;
                            }
                            break;
                        }
                    }
                }
            }
            //set the cursor back to where it was before
            pageCursor = prevCurs;
            //return the size of the state
            return i * 8;
        }

        //move on past the control code
        //sets the state if end of data is reached else sets it to the next state
        void jumpControlCode(bitset<PageSize>** page)
        {
            //stores the number of bytes until the end of the page
            int endOfPage = (PageSize - pageCursor - memSize) / 8;
            //if already at end of page
            if(endOfPage == 0)
            {
                //move on to the next page
                nextPage(*page, page);
                //sets the page cursor to the start of the new page
                resetPageCursor();
            }
            //read the first byte to determine which separator it is
            bitset<8> currByte = readByte(*page);
            //if the first bit is the control code
            if(currByte == CONTROL_CODE)
            {
                if(endOfPage == 1)
                {
                    //move on to the next page
                    nextPage(*page, page);
                    //sets the page cursor to the start of the new page
                    resetPageCursor();
                }
                //read the next byte
                currByte = readByte(*page);
                //and the second bit is not a seperator
                if(currByte != DATA_SEPERATOR && currByte != RECORD_SEPERATOR && currByte != GROUP_SEPERATOR)
                {
                    r_dev::debug_broadcast("Error jumping separator", "jumpControlCode 2 byte separator", false);
                }
                //otherwise the control code and seperator are now jumped
            }
            else if(currByte == NULL_CHARACTER)
            {
                bitset<8> seperator[] = {NULL_CHARACTER, NULL_CHARACTER, CONTROL_CODE, GROUP_SEPERATOR, NULL_CHARACTER, NULL_CHARACTER};
                for(int i = 1; i < 6; i++)
                {
                    if(endOfPage == i)
                    {
                        //move on to the next page
                        nextPage(*page, page);
                        //sets the page cursor to the start of the new page
                        resetPageCursor();
                    }
                    if(readByte(*page) != seperator[i])
                    {
                        r_dev::debug_broadcast("Error jumping separator", "jumpControlCode 6 byte separator", false);
                        break;
                    }
                }
            }
        }

        //sets the position of the cursor to the given index
        void setCursor(int pageIndex)
        {
            pageCursor = pageIndex;
        }

        //sets the position of the cursor to the start of the page
        void resetPageCursor()
        {
            pageCursor = memSize;
        }

        //sets the position of the cursor to the start of the memory
        void resetMemCursor()
        {
            memCursor = memSize;
        }

        //moves on to the next state
        void nextState(bitset<PageSize>** page)
        {
            //if the cursor is at the end of the page
            if(pageCursor == PageSize - memSize)
            {
                nextPage(*page, page);
                resetPageCursor();
                nextState(page);
            }
            //else if cursor is on page
            else
            {
                //move to the next value
                nextValue(page);
            }
        }

        //ENTRY: Get the value at the given index
        //reads through the current state
        //returns the value
        std::string getValue(bitset<PageSize>* page,  int pageIndex, int numBits = PageSize - memSize * 2, bool isMemAddress = false)
        {
            //if there is more than 100 bytes, return an empty value
            if(numBits > 800)
                return "";
            //stores the current byte
            std::string currByte;
            int tempCursor = pageCursor;
            //string stores the value of the state
            std::string returnValue(numBits, '0');
            //loop through the bits
            for(int i = 0; i < numBits / 8; i++)
            {
                //if at the end of the page and it is not a memory address
                if(pageCursor >= PageSize - memSize && !isMemAddress)
                {
                    //move to the next page
                    nextPage(page, &page);
                    //reset the cursor
                    resetPageCursor();
                }
                //read the next byte
                currByte = readByte(page).to_string();
                //loop through the bitset
                for(int j = 0; j < 8; j++)
                {
                    //set the correct index of the string equal to the correct index of the bitset
                    returnValue[returnValue.size() - 1 - i * 8 - j] = currByte[7 - j];
                }
            }
            //reset the cursor to the start of this state
            setCursor(tempCursor);
            //return the state's value
            //cout << page->to_string() << endl;
            return returnValue;
        }

        //moves on to the next value
        //returns nothing
        void nextValue(bitset<PageSize>** page)
        {
            //stores each byte as we read through
            bitset<8> currByte;
            //stores the number of bytes in this current state
            ///print(page);
            int numBits = valueSize(*page);
            //loop through all of the bytes in this state
            for(int i = 0; i < numBits / 8; i++)
            {
                //if at the end of the page
                if(pageCursor >= PageSize - memSize)
                {
                    //move to the next page
                    nextPage(*page, page);
                    //reset the cursor
                    resetPageCursor();
                }
                //read the next byte
                currByte = readByte(*page);
            }
            //move past the control code at the end of the state
            jumpControlCode(page);
        }

        //ENTRY: Write the data of a variable to memory. Includes the binary values of the name, the type and the data as well as the seperators
        //sends the entire group to memory
        //group consists of name, type, and the data
        void groupToMemory(bitset<PageSize>* page, std::string name, std::string type, std::string data)
        {
            int tempCursor = pageCursor;
            //write the name to memory
            toMemory(page, name, tempCursor);
            tempCursor += name.length();
            //write the control code and record seperator to memory
            toMemory(page, CONTROL_CODE, tempCursor);
            tempCursor += 8;
            toMemory(page, RECORD_SEPERATOR, tempCursor);
            tempCursor += 8;
            //write the type to memory
            toMemory(page, type, tempCursor);
            tempCursor += type.length();
            //write the control code and record seperator to memory
            toMemory(page, CONTROL_CODE, tempCursor);
            tempCursor += 8;
            toMemory(page, RECORD_SEPERATOR, tempCursor);
            tempCursor += 8;
            //write the data to memory
            toMemory(page, data, tempCursor);
            tempCursor += data.length();
            //write the control code and record seperator to memory
            toMemory(page, NULL_CHARACTER, tempCursor);
            tempCursor += 8;
            toMemory(page, NULL_CHARACTER, tempCursor);
            tempCursor += 8;
            toMemory(page, CONTROL_CODE, tempCursor);
            tempCursor += 8;
            toMemory(page, GROUP_SEPERATOR, tempCursor);
            tempCursor += 8;
            toMemory(page, NULL_CHARACTER, tempCursor);
            tempCursor += 8;
            toMemory(page, NULL_CHARACTER, tempCursor);
        }

        //ENTRY: string to memory
        //writes the given string value to the memory
        void toMemory(bitset<PageSize>* page, std::string value, int pageIndex, bool isMemAddress = false)
        {
            //loop through the string loading it into the correct bitset format starting at the given index
            for(int i = 0; i < value.length(); i++)
            {
                //if the next index is off the page size and it is not a memory address
                if(i + pageIndex > PageSize - memSize - 1 && !isMemAddress)
                {
                    //set the next page pointer to 0 in case there is no next page
                    bitset<PageSize>* np = 0;
                    //get the next page
                    nextPage(page, &np);
                    //if the next page is null
                    if(np == 0)
                    {
                        //create the next page
                        bitset<PageSize>* np = new bitset<PageSize>();
                        //reset the next pages bits
                        np->reset();
                        //set the next page
                        setNextPage(page, np);
                        //set the prev page of the next page
                        setPrevPage(np, page);
                        //incrememt the size
                        theSize += PageSize;
                    }
                    //set the bits
                    np->set((i + pageIndex + memSize) - (PageSize - memSize), '1' == (value[value.length() - 1 - i]));
                }
                else
                {
                    //set the bits
                    page->set(i + pageIndex, '1' == (value[value.length() - 1 - i]));
                }
            }
        }

        //ENTRY: Bitset to memory
        //writes the given bitset value to memory
        template<size_t B>
        void toMemory(bitset<PageSize>* page, bitset<B> value, int pageIndex, bool isMemAddress = false)
        {
            //loop through setting the correct bits starting at the given index
            for(int i = 0; i < value.size(); i++)
            {
                //if the next index is off the page
                if(i + pageIndex > PageSize - memSize - 1 && !isMemAddress)
                {
                    //set the next page pointer to 0 in case there is no next page
                    bitset<PageSize>* np = 0;
                    //get the next page
                    nextPage(page, &np);
                    //if there is no next page
                    if(np == 0)
                    {
                        //create the next page
                        np = new bitset<PageSize>();
                        //reset the bits of the next page
                        np->reset();
                        //set the next page
                        setNextPage(page, np);
                        //set the prev page of the next page
                        setPrevPage(np, page);
                        //incrememt the size
                        theSize += PageSize;
                    }
                    //set the bits
                    np->set((i + pageIndex + memSize) - (PageSize - memSize), value.test(i));
                }
                else
                {
                    //set the bits
                    page->set(i + pageIndex, value.test(i));
                }
            }
        }

        //ENTRY: String comparison
        //NOTE: Look to rewrite without converting to std::string and using the compare method (start from right, compare bytes maybe?)
        //compares the binary values of 2 strings
        //returns -1 if string 1 comes before string 2
        //returns 0 if the strings are equal
        //returns 1 if string 1 comes after string 2
        int compareStrings(std::string string1, std::string string2)
        {
            //loop through the strings up to the length of string 1
//            int len, string1Offset, string2Offset;
//            //if the first string is longer
//            if(string1.length() > string2.length())
//            {
//                //set the loop length to that of the second string
//                len = string2.length();
//                string1Offset = string1.length() - string2.length();
//                string2Offset = 0;
//            }
//            //else if the second string is longer or they are same length
//            else
//            {
//                //set loop length to that of the first string
//                len = string1.length();
//                string2Offset = string2.length() - string1.length();
//                string1Offset = 0;
//            }
//            //loop through the shorter string
//            for(int i = 0; i < len; i++)
//            {
//                //if the first string has a higher value at the current index
//                if(string1[i + string1Offset] == '1' && string2[i + string2Offset] == '0')
//                {
//                    //return that string 2 comes before string 1
//                    return 1;
//                }
//                //else if string 2 has a higher value at the index
//                else if(string1[i + string1Offset] == '0' && string2[i + string2Offset] == '1')
//                {
//                    //return that string 1 comes before string 2
//                    return -1;
//                }
//                //if they are the same value keep looping
//            }
//
//            if(string1.length() == string2.length())
//                return 0;
//            if(len == string2.length())
//                return 1;
//            return -1;
//            //if the strings are not the same length at this point then string 2 is longer than string 1
//            //and the strings should be the same up to the length of string 1
//            //therefore return that string 1 comes befores string 2 because it is the shorter string
//            if(string1.length() > string2.length())
//            {
//                //return that string 1 comes first
//                return 1;
//            }
//            else if(string1.length() < string2.length())
//            {
//                return -1;
//            }
//            //otherwise the strings are the same length and the exact same so return 0
//            return 0;
            string s1 = r_utils::binaryToString(string1), s2 = r_utils::binaryToString(string2);
            int r = s1.compare(s2);
            if(s1.empty())
                r = 1;
            if(s2 == "")
                r = -1;
            return r;
        }

        /**Store new value in catalog.
        @param name: a string with the name.
        @param type: an enumeration to the datatype.
        @param data: pointer to the raw binary data.
        @return true if successful, false otherwise.*/
        //ENTRY: Create a new variable
        bool newValue(std::string name, int type, std::string binary)
        {
            //the starting index of the current value or the value before or after where the new value should be
            int startIndex;
            //if the binary search finds a variable with this name
            if(binarySearch(name, &startIndex))
            {
                //return false because there is already a variable with this name
                return false;
            }
            //there are no variables yet
            else if(startIndex == -1)
            {
                //move the pageCursor to after the first seperator
                pageCursor = memSize + 48;
                //get the binary form of the type
                std::string theType = r_utils::toBinary(type), binaryName = r_utils::toBinary(name);
                //add the number of bits required to encode the name, type, and data along with 3 seperators which are 16 bits each
                int numBits = binary.length() + theType.length() + binaryName.length() + 16 * 5;
                //shift the number of bits to support the new data
                expandMemory(firstPage, numBits, pageCursor);
                //set the cursor to after the first seperator
                setCursor(memSize + 48);
                //write the new variable to memory
                groupToMemory(firstPage, binaryName, theType, binary);
            }
            //otherwise there is no variable with this name
            else
            {
                //set the current page equal to the first
                currentPage = firstPage;
                //loop until you find the correct page of the given index
                while(startIndex > PageSize)
                {
                    nextPage(currentPage, &currentPage);
                    startIndex -= PageSize;
                }
                //set the pageCursor equal to the first index
                pageCursor = startIndex;
                string nextName = getValue(currentPage, startIndex, valueSize(currentPage));
                //get the binary form of the type
                string theType = r_utils::toBinary(type), binaryName = r_utils::toBinary(name);
                //add the number of bits required to encode the name, type, and data along with 3 seperators which are 16 bits each
                int numBits = binary.length() + theType.length() + binaryName.length() + 16 * 5;
                int prevCursor = pageCursor;
                //if not at end of dataand if the name comes after the next name in the list
                if(!nextName.empty() && compareStrings(binaryName, nextName) > 0)
                {
                    //loop through memory until after this variable
                    for(int i = 0; i < 3; i++)
                        nextState(&currentPage);
                }
                //store the value of the current position
                prevCursor = pageCursor;
                //expand the memory starting at this location
                expandMemory(currentPage, numBits, pageCursor);
                //set the cursor equal to the location before expanding
                setCursor(prevCursor);
                //send the variable to memory
                groupToMemory(currentPage, binaryName, theType, binary);
            }
            return true;
        }

        /**Modify value in catalog.
        @param name: a string with the name.
        @param data: string containing the binary value of the data.
        @return true if successful, false otherwise.*/
        //ENTRY: Overwrite a value
        //Searches for a variable with the given name in order to override it's current data
        //return true if successful
        //return false if variable doesn't exist
        bool writeValue(std::string name, std::string binary)
        {
            //the binary representation of the name
            int pageIndex;
            //if there is a variable with the given name
            if(binarySearch(name, &pageIndex))
            {
                //set current page equal to the first page
                currentPage = firstPage;
                //loop through till the correct page is found
                while(pageIndex > PageSize)
                {
                    nextPage(currentPage, &currentPage);
                    pageIndex -= PageSize;
                }
                //set the pageCursor equal to the given index
                pageCursor = pageIndex;
                //loop through till you get to the data of the variable
                for(int i = 0; i < 2; i++)
                    nextState(&currentPage);
                //calculate the difference in size of the data
                int difference = binary.length() - valueSize(currentPage);
                //stores the current value of the page Cursor
                int prevCursor = pageCursor;
                //if the new data is bigger
                if(difference > 0)
                {
                    //expand the data equal to the difference
                    expandMemory(currentPage, difference, pageCursor);
                }
                //if the data is smaller
                else if(difference < 0)
                {
                    //shrink the data equal to the negative distance
                    shrinkMemory(currentPage, -1 * difference, pageCursor);
                }
                //by this point the data size is the same
                //set the cursor to the last cursor location
                setCursor(prevCursor);
                //write the data to memory
                toMemory(currentPage, binary, pageCursor);
                return true;
            }
            //if there is no variable with that name, return false
            return false;
        }

        //ENTRY: Retrieve variable type from memory
        //can retrieve the type of a variable or the value of the variable
        bool retrieve(string name, string* binaryType, string* binaryValue)
        {
            int pageIndex;
            if(binarySearch(name, &pageIndex))
            {
                currentPage = firstPage;
                while(pageIndex > PageSize)
                {
                    nextPage(currentPage, &currentPage);
                    pageIndex -= PageSize;
                }
                //set the cursor equal to the index
                pageCursor = pageIndex;
                nextState(&currentPage);
                *binaryType = getValue(currentPage, pageCursor, valueSize(currentPage));
                nextState(&currentPage);
                *binaryValue = getValue(currentPage, pageCursor, valueSize(currentPage));
                return true;
            }
            return false;
        }

        //ENTRY: Delete a variable
        /**Delete a value in catalog.
        @param name: a string with the name.
        @return true if successful, false otherwise.*/
        bool deleteValue(std::string name)
        {
            int pageIndex;
            //if there is a variable with the given name
            if(binarySearch(name, &pageIndex))
            {
                //set the current page equal to the first page
                currentPage = firstPage;
                //loop through till you reach the desired page
                while(pageIndex > PageSize)
                {
                    nextPage(currentPage, &currentPage);
                    pageIndex -= PageSize;
                }
                //set the cursor equal to the index
                pageCursor = pageIndex;
                //set numbits to 0
                int numBits = 0;
                //stores the current location of the cursor
                int prevCursor = pageCursor;
                //points to the current page in case the next state moves onto the next page
                bitset<PageSize>* pPage = currentPage;
                //loop through tthis variable to determine the size
                for(int i = 0; i < 3; i++)
                {
                    numBits += valueSize(currentPage);
                    nextState(&currentPage);
                }
                //set the current page to the itself if the next state is on the same page
                //or it sets the current page to the previous page if the next state changes pages
                currentPage = pPage;
                //add the number of bits for the 3 seperators
                numBits += 16 * 5;
                //set the pageCursor back to before the value
                setCursor(prevCursor);
                //shrinks the memory the size of the current variable. thus deleting it from memory
                shrinkMemory(currentPage, numBits, pageCursor);
                //return true
                return true;
            }
            else
            {
                ///Do we want it to return true or false if the variable does not exist?
                return false;
            }
        }

        //ENTRY: Shrink Memory at a certain memory
        //Shift the bits from the given index to the right a given amount
        //This is an attempt to shrink the memory for the new value to be put in
        void shrinkMemory(bitset<PageSize>* binary, int shiftAmount, int pageIndex)
        {
            //if there is a page
            if(binary != 0)
            {
                //stores the pointer to the next page
                bitset<PageSize>* nPage;
                //gest the next page
                nextPage(binary, &nPage);
                //a signed representation of the page size for comparisons
                int pSize = PageSize;
                //if the index is off the page
                if(pageIndex > pSize)
                {
                    //shrink the memory of the next page
                    shrinkMemory(nPage, shiftAmount, pageIndex - PageSize);
                    //set index equal to the page size - memSize so that the for loop won't run
                    pageIndex = PageSize - memSize;
                }
                //otherwise if the the index is located in the memory address of the next page
                else if(pageIndex > pSize - memSize - 1)
                {
                    //throw an error
                    r_dev::debug_broadcast("Given index was located in the next pages pointer", "shrink memory", false);
                    //EXIT???
                }
                //otherwise if the index is on the next page
                else if(pageIndex < 0)
                {
                    //the idex is equal to the memSize
                    pageIndex = memSize;
                }
                //else if the index is located in the memory address of the previous page
                else if(pageIndex < memSize)
                {
                    //throw an error
                    r_dev::debug_broadcast("Given index was located in the previous pages pointer", "shrink memory", false);
                    //exit?
                }

                //starting at the index, loop to the end of the page
                for(int i = pageIndex; i < PageSize - memSize; i++)
                {
                    //if the current index plus the shift amount is in the memory
                    if(i + shiftAmount < PageSize - memSize)
                    {
                        //set the current index equal to the value at the shifted index
                        binary->set(i, binary->test(i + shiftAmount));
                    }
                    //if the shifted index is not in memory and there is another page
                    else if(nPage != 0)
                    {
                        //set bits
                        binary->set(i, nPage->test(i + shiftAmount - PageSize + 2 * memSize));
                    }
                    else
                    {
                        //set the value equal to 0
                        binary->set(i, 0);
                    }
                }
                //shrink the memory of the next page
                shrinkMemory(nPage, shiftAmount, pageIndex - PageSize);
            }
        }

        //checks to see if there is any data that would be shifted onto the next page
        bool anyData(bitset<PageSize>* page, int start)
        {
            //if the start index of the shift is less than 0 you need new page
            if(start < 0)
                return true;
            //loop through the to be shifted data
            for(int i = start; i < PageSize - memSize - 1; i++)
            {
                //if there is some data, return true
                if(page->test(i))
                    return true;
            }
            return false;
        }

        //ENTRY: Expand Memory at a given location
        //Shift the bits from the given index to the right a given amount
        //This is an attempt to shrink the memory for the new value to be put in
        //usage: if valueSize() < memoryNeeded
        //          expandMemory(page, memoryNeeded - valueSize(), cursor);
        bool expandMemory(bitset<PageSize>* binary, int shiftAmount, int pageIndex)
        {
            if(binary != 0)
            {
                //points to the next page
                bitset<PageSize>* nPage;
                //set the pointer
                nextPage(binary, &nPage);
                //r is true if there is a next page
                bool r = expandMemory(nPage, shiftAmount, pageIndex - PageSize);
                //if there is something stored in the data that is to be shifted
                if(anyData(binary, PageSize - memSize - shiftAmount))
                {
                    //if there is not a next page
                    if(!r)
                    {
                        //create a new page
                        nPage = new bitset<PageSize>();
                        //set the bits to 0
                        nPage->reset();
                        //set the next page of the current page
                        setNextPage(binary, nPage);
                        //set the prev page of the next page
                        setPrevPage(nPage, binary);
                        //incrememt the size
                        theSize += PageSize;
                        //expand the memory on the next page
                        expandMemory(nPage, shiftAmount, pageIndex - PageSize);
                    }
                }
                //stores the pointer to the next page
                bitset<PageSize>* pPage;
                //gets the pointer to the next page
                prevPage(binary, &pPage);
                //if the index is on some page before this one
                if(pageIndex < 0)
                {
                    //shift every index
                    pageIndex = memSize;
                }
                //if index is located in the pointer to the previous page
                else if(pageIndex < memSize)
                {
                    //throw error
                    r_dev::debug_broadcast("Given index was located in the previous pages pointer", "expand memory", false);
                    //EXIT??
                }

                //loop through from the end of the memory to the desired index
                for(int i = PageSize - memSize - 1; i > pageIndex - 1; i--)
                {
                    //if the index of the desired shift is in the desired subset of the memory
                    if(i - shiftAmount > pageIndex - 1)
                    {
                        //set the value of the current index to the value at the shifted index
                        binary->set(i, binary->test(i - shiftAmount));
                    }
                    //if there is a previous page
                    else if(i - shiftAmount < memSize && pPage != 0)
                    {
                        //get the previous pages value at the shifted index
                        binary->set(i, pPage->test(PageSize - memSize + (i - memSize - shiftAmount)));
                    }
                    //else if there is no previous page
                    else
                    {
                        //set the value to 0
                        binary->set(i, 0);
                    }
                }
                return true;
            }
            //if this page is null return false
            else
            {
                return false;
            }
        }

        //ENTRY: Binary Search
        //searches the pages for the given name
        //if that name exists return true
        //otherwise return false
        ///pass in a boolean* or an int* and return the result to that
        bool binarySearch(string name, int* finalIndex)
        {
            //if the first page is null, return false
            if(firstPage == 0)
                return false;
            //left part of the binary search, right part of the binary search, and the mid point
            int left = memSize, right = theSize - memSize - 1, mid;
            //reset the cursors
            resetPageCursor();
            resetMemCursor();
            //compName stores the name to compare to, binary name is the binary representation of the passed in name
            string compName, binaryName = r_utils::toBinary(name);
            //start at the first page
            currentPage = firstPage;
            //the middle index
            mid = right / 2 + left;
            //if the middle index is not a factor of 8, set it to a factor of 8
            mid -= mid % 8;
            //memory cursor is equal to the mid point
            memCursor = mid;
            //start the helper method
            //NOTE: Used for debuging the search helper. remove once complete
            bool debug = false;
            //used when the end of memory is reached
            int firstTimeEnd = 0;
            if(name == "a377")
                debug = false;
            compName = searchHelper(mid, right, mid, debug);
            //while there are more names in data and the cursors haven't passed each other
            while(left < right)
            {
                string actualCompName = r_utils::binaryToString(compName);
                //if the name passed back is empty
                if(compName.empty())
                {
                    //there is nothing to the right of the name, so the middle is the new right point
                    right = mid;
                    //the current page is equal to the first again
                    currentPage = firstPage;
                }
                //else if the names are equal || end of data
                else if(binaryName == compName)
                {
                    //set the index of the start of the variable
                    *finalIndex = memCursor;
                    //return true
                    return true;
                }
                //else if end of data
                else if(compName == "^")
                {

                    *finalIndex = memCursor;
                    //NOTE: Was 128 however in case of backwards creation, was not replacing first element. possibly due to this
                    //gets to end of memory then stops. Need to check before end of memory
                    if(right - left < 128 || memCursor > right)
                    {
                        if(firstTimeEnd != 2)
                        {
                            mid = left;
                            firstTimeEnd = 1;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if(memCursor == right)
                    {
                        right -= 48;
                    }
                    else
                    {
                        right = memCursor;
                    }
                }
                //if the passed in name comes before the retrieved value
                else if(compareStrings(binaryName, compName) < 0)
                {
                    //set the index equal to the start of the current variable in case the passed in variable comes before this one
                    *finalIndex = memCursor;
                    //move the right cursor to where the mid point is
                    right = mid;
                }
                //if the passed in name comes after the current name
                else
                {
                    //set the given index equal to the start of the current name in case the passed in variable belongs after the current variable
                    *finalIndex = memCursor;
                    //the left side of the search is equal to the byte after the mid point because the byte at the mid point has already been checked
                    left = mid + 8;
                }
                if(firstTimeEnd != 1)
                {
                    //determine the new mid value
                    mid = (right - left) / 2 + left;
                }
                //make sure the mid point is a factor of 8
                mid -= mid % 8;
                //set the memory cursor equal to the mid
                memCursor = mid;
                //set to the first page again
                currentPage = firstPage;
                //get the current name at the given mid point
                compName = searchHelper(mid, right, mid, debug);
                //increment the end variable
                firstTimeEnd++;
            }
            //if the cursor is equal to first start of the memory set the index equal to a -1 which represents the start of memory
            if(memCursor == memSize)
                *finalIndex = -1;
            //otherwise set the index equal to the current memory cursor
 //           else
//                *finalIndex = memCursor;
            //return if the current name is equal to passed in name or not
            return binaryName == compName;
        }

        //return the total size of the memory
        int getSize(){ return theSize; }

    protected:
    private:
        //the cursors that of the memory
        int pageCursor, memCursor;
        //stores the size of memory
        int theSize;
        //points to the first page of memory
        bitset<PageSize>* firstPage;
        //points to the current page, used for traversing through the memory
        bitset<PageSize>* currentPage;


        //ENTRY: Binary Search Helper Function
        //the search helper memory
        //returns the value of the name around the given index
        //returns an empty string if there is no name in the given section of the memory
        string searchHelper(int pageIndex, int right, int memIndex, bool debug)
        {
            //if the current page is 0, return empty string
            if(currentPage == 0)
                return "";
            //if the idnex is on the next page
            if(pageIndex > PageSize - 1)
            {
                //set current page equal to the next page
                nextPage(currentPage, &currentPage);
                //reset the cursor
                resetPageCursor();
                int newIndex;
                if(pageIndex == PageSize)
                {
                    newIndex = memSize;
                    memIndex += memSize;
                }
                else
                    newIndex = pageIndex - PageSize;
                //return the search helper value of the next page
                return searchHelper(newIndex, right, memIndex, debug);
            }
            //if the index is on the previous page
            else if(pageIndex < 0)
            {
                //set the current page equal to the previous page
                prevPage(currentPage, &currentPage);
                //reset the page cursor
                resetPageCursor();
                //return the search helper value of the previous page
                return searchHelper(pageIndex + PageSize, right, memIndex, debug);
            }
            //if the index is on this page
            else
            {
                //the page cursor equal the index
                if(pageIndex < memSize)
                {
                    memIndex += (memSize - pageIndex);
                    pageCursor = memSize;
                }
                else
                    pageCursor = pageIndex;
                //NOTE: memIndex? Why did I do this haha?
                memCursor = memIndex;
                bool searching = true;
                int currSeperator = 0;
                int theEnd = right + 8 * 20;//(right % PageSize < PageSize - memSize - 1 - 8 * 20 ? right + 8 * 20 : right + (PageSize - right % PageSize) - memSize - 1);
                //loop from after the index to the right side of the search block or until a name is found
                for(int i = pageCursor / 8; currentPage != 0 && (memCursor < theEnd || currSeperator > 3) && searching; i++)
                {
                    //if the cursor reaches the end of the page
                    if(pageCursor > PageSize - memSize - 1)
                    {
                        //move to the next page
                        nextPage(currentPage, &currentPage);
                        if(currentPage == NULL)
                            return "";
                        //set the mem cursor equal to the start of the next page
                        memCursor = (((int)memCursor / PageSize) + 1) * PageSize + memSize;
                        //reset the page cursor
                        resetPageCursor();
                    }
                    else if(pageCursor < memSize)
                    {
                        int backAmount = memSize - pageCursor;
                        prevPage(currentPage, &currentPage);
                        if(currentPage == NULL)
                            return "";
                        memCursor = ((memCursor / PageSize) - 1) * PageSize + PageSize - memSize - backAmount;
                        pageCursor = PageSize - memSize - backAmount;
                    }
                    //read the next byte of data
                    bitset<8> currByte = readByte(currentPage);
                    ///debugging
                    if (debug)
                        cout << currByte.to_string() << endl;
                    //increment the mem cursor
                    memCursor += 8;
                    //if the byte is a group seperator, the next bytes will be a name
                    //NOTE: For Debugging. Remove after
//                    if(byte.any())
//                        cout << byte.to_string() << endl;
                    switch(currSeperator)
                    {
                        case 0:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currSeperator++;
                            }
                            break;
                        }
                        case 1:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currSeperator++;
                            }
                            else
                            {
                                currSeperator = 0;
                                pageCursor -= 8;
                                memCursor -= 8;
                            }
                            break;
                        }
                        case 2:
                        {
                            if(currByte == CONTROL_CODE)
                            {
                                currSeperator++;
                            }
                            else
                            {
                                currSeperator = 0;
                                pageCursor -= 16;
                                memCursor -= 16;
                            }
                            break;
                        }
                        case 3:
                        {
                            if(currByte == GROUP_SEPERATOR)
                            {
                                currSeperator++;
                            }
                            else
                            {
                                currSeperator = 0;
                                pageCursor -= 24;
                                memCursor -= 24;
                            }
                            break;
                        }
                        case 4:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                currSeperator++;
                            }
                            else
                            {
                                currSeperator = 0;
                                pageCursor -= 32;
                                memCursor -= 32;
                            }
                            break;
                        }
                        case 5:
                        {
                            if(currByte == NULL_CHARACTER)
                            {
                                searching = false;
                            }
                            else
                            {
                                currSeperator = 0;
                                pageCursor -= 40;
                                memCursor -= 40;
                            }
                            break;
                        }
                    }
                }
                if(currSeperator == 5)
                {
                    string temp = getValue(currentPage, pageCursor, valueSize(currentPage));
                    if(temp.empty())
                    {
                        return "^";
                    }
                    else
                    {
                        return temp;
                    }
                }
                //if the right end of the search section is reached
                //return empty string
                return "";
            }
        }

};

#endif // R_MEM_HPP_INCLUDED
