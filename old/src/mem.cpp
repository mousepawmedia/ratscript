/*#include "ratscript/mem.hpp"

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define memSize 64
#else
#define memSize 32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define memSize 64
#else
#define memSize 32
#endif
#endif

template<size_t PageSize>
r_mem<PageSize>::r_mem()
{
    firstPage = new bitset<PageSize>();
}

//each page will be of the same type. l_page points to another l_page, m_page points to m_page

//used to test different functionalities
template<size_t PageSize>
void r_mem<PageSize>::test(bitset<PageSize>* temp)
{
    expandMemory(firstPage, 32, 100);
}

//inserts a new value into the memory
//returns true if a successful insertion
//returns false if there is another variable with that name
template <size_t PageSize>
bool r_mem<PageSize>::newValue(bitset<PageSize>* page, std::string name, int type, std::string binary)
{
    std::string binaryName = r_utils::toBinary(name);
    //set the cursor at the beginning of the memory
    resetCursor();
    while(true)
    {
        //while there is more data to explore and the not currently at the location of a name
        if(state != NAME && state != END_OF_DATA && state != END_OF_MEM)
        {
            nextState(page);
        }
        else if(state == END_OF_DATA)
        {
            //put an end of record in the place of the end of group (end of data)
            //write the new data to this location
            //if not at the beginning of the page
            if(cursor != memSize)
            {
                //move the cursor in front of the end of data seperator
                cursor -= 16;
                toMemory(page, CONTROL_CODE);
                toMemory(page, RECORD_SEPERATOR, cursor + 8);
                cursor += 16;
                //get the binary form of the type
                std::string theType = r_utils::toBinary(type);
                //add the number of bits required to encode the name, type, and data along with 3 seperators which are 16 bits each
                int numBits = binary.length() + theType.length() + binaryName.length() + 16 * 3;
                //shift the number of bits to support the new data
                expandMemory(page, numBits, cursor);
                toMemory(page, binaryName, theType, binary);
            }
        }
        else if(state == END_OF_MEM)
        {
            //if end of memory do something?
            //TODO Figure out what to do here
            std::string memAddress = getValue(page, page->size() - memSize);
            std::string comp(memSize, '0');
            if(memAddress == comp)
            {
                l_page* newPage = new l_page();
                setNextPage(page, newPage);
            }
            else
            {
                l_page* nPage;
                nextPage(page, &nPage);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        //if the names don't match
        else if(binaryName != getValue(page))
        {
            //if the new name should come before this name
            if(compareStrings(binaryName, getValue(page)) < 0)
            {
                //get the binary form of the type
                std::string theType = r_utils::toBinary(type);
                //add the number of bits required to encode the name, type, and data along with 3 seperators which are 16 bits each
                int numBits = binary.length() + theType.length() + name.length() + 16 * 3, tempCursor = cursor;
                //shift the number of bits to support the new data
                expandMemory(page, numBits, cursor);
                groupToMemory(page, binaryName, theType, binary);
            }
            //else if the new name comes after this name
            else
            {
                //move on to the next state
                nextState(page);
            }
            //TODO Create a comparison method for binary strings?
            //Need to check to see if the incoming name comes before this name or not
            //if incoming name is before this name, expand memory enough for name, type, data and 3 seperators
            //then loop through and write the data
        }
        //If the names match return false
        else
        {
            return false;
        }
    }

    return false;
}

template<size_t PageSize>
void r_mem<PageSize>::groupToMemory(bitset<PageSize>* page, std::string name, std::string type, std::string data)
{
    int tempCursor = cursor;
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
    toMemory(page, CONTROL_CODE, tempCursor);
    tempCursor += 8;
    toMemory(page, RECORD_SEPERATOR, tempCursor);
}

//load the given binary string into the memory
template<size_t PageSize>
void r_mem<PageSize>::toMemory(bitset<PageSize>* page, std::string value, int index)
{
    //loop through the string loading it into the correct bitset format starting at the given index
    for(int i = 0; i < value.length(); i++)
    {
        page->set(i + index, '1' == (value[value.length() - 1 - i]));
    }
}

//load the given bitset into the memory
template<size_t PageSize>
template<size_t B>
void r_mem<PageSize>::toMemory(bitset<PageSize>* page, bitset<B> value, int index)
{
    //loop through setting the correct bits starting at the given index
    for(int i = 0; i < value.size(); i++)
    {
        page->set(i + index, value.test(i));
    }
}

//compares the binary values of 2 strings
//returns -1 if string 1 comes before string 2
//returns 0 if the strings are equal
//returns 1 if string 1 comes after string 2
template<size_t PageSize>
int r_mem<PageSize>::compareStrings(std::string string1, std::string string2)
{
    //loop through the strings up to the length of string 1
    for(unsigned int i = 0; i < string1.length(); i++)
    {
        //if the current index is also in the second string
        if(i < string2.length())
        {
            //if the first string has a higher value at the current index
            if(string1[i] == '1' && string2[i] == '0')
            {
                //return that string 2 comes before string 1
                return 1;
            }
            //else if string 2 has a higher value at the index
            else if(string1[i] == '0' && string2[i] == '1')
            {
                //return that string 1 comes before string 2
                return -1;
            }
            //if they are the same value keep looping
        }
        //if the current index is out of the bounds of string 2
        else
        {
            //since the strings should be the same up to this point the shorter string comes first
            //therefore return that string 1 (the longer string) comes after string 2
            return 1;
        }
    }
    //if the strings are not the same length at this point then string 2 is longer than string 1
    //and the strings should be the same up to the length of string 1
    //therefore return that string 1 comes befores string 2 because it is the shorter string
    if(string1.length() != string2.length())
    {
        //return that string 1 comes first
        return -1;
    }
    //otherwise the strings are the same length and the exact same so return 0
    return 0;
}

//Searches for a variable with the given name in order to override it's current data
//return true if successful
//return false if variable doesn't exist
template <size_t PageSize>
bool r_mem<PageSize>::writeValue(bitset<PageSize>* page, std::string name, std::string binary)
{
    //the binary representation of the name
    std::string binaryName = r_utils::toBinary(name);
    //keep looping until the variable is found, it is not in the correct location, the end of memory is reached
    while(true)
    {
        //if the end of the data or memory is reached, return false and quit
        if(state == END_OF_DATA || state == END_OF_MEM)
        {
            return false;
        }
        //if the current state isn't the name
        else if(state != NAME)
        {
            //move to the next state
            nextState(page);
        }
        //if the current state is a name
        else if(state == NAME)
        {
            //compare the current name of the current variable to the name of the variable being searched for
            int comp = compareStrings(binaryName, getValue(page));
            //if the variable name comes after the name at the current index
            if(comp > 0)
            {
                nextState(page);
            }
            //if this is where the variable should have been located
            else if(comp < 0)
            {
                //return false quit now
                return false;
            }
            //if the names match
            else
            {
                //while the state isn't that of the data
                while(state != DATA)
                {
                    //if there is an error with the state
                    if(state == END_OF_DATA || state = END_OF_MEM)
                    {
                        //throw error and break out of this loop
                        r_error e(r_error::TYPE_NULL, "This variable is undefined");
                        break;
                    }
                    //move to the next state
                    nextState(page);
                }
                //now that we are looking at the data for this entry
                //decide the size of the new data and the difference between the new data and the old
                int newSize = binary.size(), difference = valueSize(page) - newSize;
                //if the amount old memory is greater than needed for the new memory
                if(difference > 0)
                {
                    //shink the memory for this data
                    shrinkMemory(page, difference, cursor);
                }
                //if the amount of old memory is less than the amount needed for the new memory
                else if(difference < 0)
                {
                    //shift the memory to create more room for the new variable
                    //multiply the difference by -1 to get the positive value of the number of bits that need to be shifted
                    expandMemory(page, difference * -1, cursor);
                }
                //once the memory is the right size, write the new value to memory
                toMemory(page, binary, cursor);

                //now that everything is finished, return true
                return true;
            }
        }
    }
    //if somehow it reaches this point, return false
    return false;
}

//moves on to the next state
//returns the value of the new state
template <size_t PageSize>
int r_mem<PageSize>::nextState(bitset<PageSize>* page)
{
    //if the cursor is at the end of the page
    if(cursor == PageSize - memSize)
    {
        //throw error and state equals END_OF_MEM
        r_error e(r_error::ACCESS_ERROR, "Memory Out Of Bounds");
        state = END_OF_MEM;
    }
    //else if cursor is on page
    else
    {
        //move to the next value
        nextValue(page);
    }
    //return the state
    return state;
}

//moves on to the next value
//returns nothing
template <size_t PageSize>
void r_mem<PageSize>::nextValue(bitset<PageSize>* page)
{
    //stores each byte as we read through
    bitset<8> currByte;
    //stores the number of bytes in this current state
    int numBytes = valueSize(page);
    //loop through all of the bytes in this state
    for(int i = 0; i < numBytes; i++)
    {
        currByte = readByte(page);
    }
    //move past the control code at the end of the state
    jumpControlCode(page);
}

//reads through the current state
//returns the value
template <size_t PageSize>
std::string r_mem<PageSize>::getValue(bitset<PageSize>* page, int numBytes)
{
    //stores the current byte
    std::string currByte;
    //string stores the value of the state
    //the size is equal to the number of bytes * 8 bits per byte
    std::string returnValue(numBytes, '0');
    //loop through the bytes
    for(int i = 0; i < numBytes / 8; i++)
    {
        currByte = readByte(page).to_string();
        //loop through the bitset
        for(int j = 0; j < 8; j++)
        {
            //set the correct index of the string equal to the correct index of the bitset
            returnValue[returnValue.size() - 1 - i * 8 - j] = currByte[7 - j];
        }
    }
    //reset the cursor to the start of this state
    setCursor(cursor - numBytes);
    //return the state's value
    return returnValue;
}

//returns an 8 bit bitset representation of the current byte
template <size_t PageSize>
bitset<8> r_mem<PageSize>::readByte(bitset<PageSize>* page)
{
    //stores the value of the current byte
    bitset<8> temp;
    //loop through the passed in page starting at the current cursor
    for(int i = 0; i < 8; i++)
    {
        //set the index of the byte equal to the index of the page
        temp.set(i, (*page)[cursor + i]);
    }
    //move the cursor 8 bits forward
    cursor += 8;
    //return the byte
    return temp;
}

//returns the size (number of bits) of the current state
template <size_t PageSize>
int r_mem<PageSize>::valueSize(bitset<PageSize>* page)
{
    //stores the current byte
    bitset<8> currByte;
    //i is the number of bits in the state, prev cursor is the previous location of the cursor
    int i, prevCurs = cursor;
    //keep looping until the end of the state or end of data is reaced
    for(i = 0; true; i++)
    {
        //move to the next byte
        currByte = readByte(page);
        //if the byte is equal to the control code it might signal the end of the state or data
        if(currByte == CONTROL_CODE)
        {
            //get the next byte
            currByte = readByte(page);
            //if the end of the state or data
            if(currByte == RECORD_SEPERATOR || currByte == GROUP_SEPERATOR)
            {
                //break out of the loop
                break;
            }
            //if end of memory and end of state not signaled
            else
            {
                //rewind to the current byte
                //look at it as if it is not part of the control code
                cursor -= 8;
            }
        }
    }
    //set the cursor back to where it was before
    cursor = prevCurs;
    //return the size of the state
    return i;
}

//move on past the control code
//sets the state if end of data is reached else sets it to the next state
template <size_t PageSize>
void r_mem<PageSize>::jumpControlCode(bitset<PageSize>* page)
{
    //read the 2 bit control code into bytes 1 and 2
    bitset<8> b1 = readByte(page), b2 = readByte(page);
    //if the first bit is the control code
    if(b1 == CONTROL_CODE)
    {
        //and the second bit is the end of data
        if(b2 == GROUP_SEPERATOR)
        {
            //the end of the data has been reached
            state = END_OF_DATA;
        }
        //else if second bit is not signaling end of data
        else
        {
            //set the state to the next state
            state = (state + 1) % 3;
        }
    }
}

//Shift the bits from the given index to the right a given amount
//This is an attempt to shrink the memory for the new value to be put in
//usage: if valueSize() < memoryNeeded
//          expandMemory(page, memoryNeeded - valueSize(), cursor);
template <size_t PageSize>
bool r_mem<PageSize>::expandMemory(bitset<PageSize>* binary, int shiftAmount, int index)
{
    if(binary != 0)
    {
        bitset<PageSize>* nPage;
        nextPage(binary, nPage);
        //set the cursor to check the last 2 bytes of data on the page
        cursor = PageSize - memSize - shiftAmount;
        //if there is something stored in the last 2 bytes of data on the page
        bool r = expandMemory(nPage, shiftAmount, index - PageSize);
        if(readByte(binary).any() || readByte(binary).any())
        {
            if(!r)
            {
                setNextPage(binary, nPage);
                setPrevPage(nPage, binary);
                expandMemory(nPage, shiftAmount, index - PageSize);
            }
        }
        bitset<PageSize>* pPage;
        prevPage(binary, pPage);
        //if the index is on some page before this one
        if(index < 0)
        {
            //shift every index
            index = memSize;
        }
        else if(index < memSize)
        {
            r_dev::debug_broadcast("Given index was located in the previous pages pointer", "expand memory", false);
            //EXIT??
        }

        for(int i = PageSize - memSize - 1; i > index - 1; i--)
        {
            //if the index of the desired shift is in the desired subset of the memory
            if(i - shiftAmount > memSize - 1)
            {
                //set the value of the current index to the value at the shifted index
                binary->set(i, binary->test(i - shiftAmount));
            }
            //if there is a previous page
            else if(pPage != 0)
            {
                //get the previous pages value at the shifted index
                binary->set(i, pPage->test(PageSize - memSize  - 1 + (i - shiftAmount)));
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
    else
    {
        return false;
    }
}

//Shift the bits from the given index to the right a given amount
//This is an attempt to shrink the memory for the new value to be put in
//usage: if valueSize() > memoryNeeded
//          expandMemory(page, valueSize() - memoryNeeded, cursor);
template <size_t PageSize>
void r_mem<PageSize>::shrinkMemory(bitset<PageSize>* binary, int shiftAmount, int index)
{
    if(binary != 0)
    {
        bitset<PageSize>* nPage;
        nextPage(binary, nPage);
        if(index > PageSize)
        {
            shrinkMemory(nPage);
            //set index equal to the page size - memSize so that the for loop won't run
            index = PageSize - memSize;
        }
        else if(index > PageSize - memSize - 1)
        {
            r_dev::debug_broadcast("Given index was located in the next pages pointer", "shrink memory", false);
            //EXIT???
        }
        else if(index < 0)
        {
            index = memSize;
        }
        else if(index < memSize)
        {
            r_dev::debug_broadcast("Given index was located in the previous pages pointer", "shrink memory", false);
            //exit?
        }

        for(int i = index; i < PageSize - memSize; i++)
        {
            //if the current index plus the shift amount is in the memory
            if(i + shiftAmount < PageSize - memSize)
            {
                //set the current index equal to the value at the shifted index
                binary->set(i, binary->test(i + shiftAmount));
            }
            //if the shifted index is not in memory
            else if(nPage != 0)
            {
                binary->set(i, nPage->test(memSize + ((i + shiftAmount) - PageSize)));
            }
            else
            {
                //set the value equal to 0
                binary->set(i, 0);
            }
        }
    }
}

//sets the position of the cursor to the current index
template<size_t PageSize>
void r_mem<PageSize>::setCursor(int index)
{
    cursor = index;
}

//sets the position of the cursor to 0
template<size_t PageSize>
void r_mem<PageSize>::resetCursor()
{
    cursor = memSize;
}

template<size_t PageSize>
void r_mem<PageSize>::setNextPage(bitset<PageSize>* currentPage, bitset<PageSize>* nextPage)
{
    stringstream ss;
    ss << nextPage;
    string theAddress = ss.str();
    cout << theAddress << endl;
    int temp = (int)stod(theAddress);
    theAddress = r_utils::toBinary((int)stod(theAddress), memSize);
    cout << theAddress << endl;
    toMemory(currentPage, theAddress, PageSize - memSize - 1);
}

template<size_t PageSize>
void r_mem<PageSize>::setPrevPage(bitset<PageSize>* currentPage, bitset<PageSize>* prevPage)
{
    stringstream ss;
    ss << prevPage;
    string theAddress = ss.str();
    cout << theAddress << endl;
    theAddress = r_utils::toBinary((int)stod(theAddress), memSize);
    cout << theAddress << endl;
    toMemory(currentPage, theAddress, 0);
}

template<size_t PageSize>
void r_mem<PageSize>::nextPage(bitset<PageSize>* currPage, bitset<PageSize>** next)
{
    cursor = PageSize - memSize - 1;
    string memAddress = getValue(currPage, memSize);
    bitset<memSize>* binary = new bitset<memSize>(memAddress);
    *next = reinterpret_cast<bitset<PageSize>*>(binary->to_ulong());
}

template<size_t PageSize>
void r_mem<PageSize>::prevPage(bitset<PageSize>* currPage, bitset<PageSize>** prev)
{
    cursor = 0;
    string memAddress = getValue(currPage, memSize);
    bitset<memSize>* binary = new bitset<memSize>(memAddress);
    *prev = reinterpret_cast<bitset<PageSize>*>(binary->to_ulong());
}
*/
