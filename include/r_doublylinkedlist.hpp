/**Ratscript Norvegicus: DoublyLinkedList<p>
* A doubly linked list class.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_DOUBLYLINKEDLIST_HPP_INCLUDED
#define R_DOUBLYLINKEDLIST_HPP_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string.h>
#include "r_utils.hpp"

using std::cout;
using std::endl;

//DoubleNode structure using Templates
//contains 4 variables: Pointer to the name, pointer to the type, pointer to the variable, and pointer to the next DoubleNode
template<typename T>
struct r_doublenode{
    public:
        //constructor converts the two std::strings to char* (I was having problems getting it to work with std::strings so i just converted it)
        //initializes pointer to next DoubleNode to 0
        r_doublenode(T *datas, r_utils::DataType types){
            type = types;
            data = datas;
            next = NULL;
            prev = NULL;
        }

        //deconstructor, called when the DoubleNode memory is set free
        //frees up the memory for the pointers to the name, the type, and the name
        ~r_doublenode(){
            free(data);
        }

        //some getters and setters
        T* getData(){return data;}
        string getName(){return data->getName();}
        r_utils::DataType getType(){return type;}
        r_doublenode<T>* getNext(){return next;}
        void setNext(r_doublenode<T>* nxt){next = nxt;}
        r_doublenode<T>* getPrev(){return prev;}
        void setPrev(r_doublenode<T>* prv){prev = prv;}

    //private variables
    private:
        T* data;
        r_utils::DataType type;
        r_doublenode<T>* next;
        r_doublenode<T>* prev;
};
//

//Class declaration for a template based Linked List
template<class Type> class r_doublylinkedlist{
    public:
        //constructor initialized to 0
        r_doublylinkedlist(){
            initialization();           //initializes all pointers and sizes to 0
        }
        //deconstructor: frees up the memory of any remaining DoubleNode in the list
        ~r_doublylinkedlist(){
            r_doublenode<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr->getNext()){
                if(curr->getPrev() != NULL){
                    delete(curr->getPrev());
                }
            }
            delete(curr);
        }

        //adds a DoubleNode to the list
        //gets passed in variable name, variable type, and the actual variable
        void  add(Type* data, r_utils::DataType type){
            if(size == 0){
                head = new r_doublenode<Type>(data, type);
                size++;
            }else{
                r_doublenode<Type>* newDoubleNode = new r_doublenode<Type>(data, type);
                r_doublenode<Type>* temp = head;
                for(int i = 0; i < size - 1; i++, temp = temp -> getNext());
                temp -> setNext(newDoubleNode);
                newDoubleNode -> setPrev(temp);
                size++;
            }
        }

        //adds new DoubleNode into list
        //sorts by name
        void addOrdered(Type* data, r_utils::DataType type)
        {
            r_doublenode<Type>* newDoubleNode = new r_doublenode<Type>(data, type);
            if(size == 0)
            {
                head = newDoubleNode;
                size++;
            }
            else
            {
                r_doublenode<Type>* curr = head;
                for(int i = 0; i < size; i++)
                {
                    if(strcmp(newDoubleNode->getName().c_str(), curr->getName().c_str()) < 0)
                    {
                        if(curr == head)
                        {
                            curr->setPrev(newDoubleNode);
                            newDoubleNode->setNext(curr);
                            head = newDoubleNode;
                            size++;
                        }
                        else
                        {
                            curr->getPrev() -> setNext(newDoubleNode);
                            newDoubleNode->setPrev(curr->getPrev());
                            curr->setPrev(newDoubleNode);
                            newDoubleNode->setNext(curr);
                            size++;
                        }
                        break;
                    }
                    else if(curr->getNext() != NULL)
                    {
                        curr = curr -> getNext();
                    }
                    else
                    {
                        newDoubleNode->setPrev(curr);
                        curr->setNext(newDoubleNode);
                        size++;
                        break;
                    }
                }
            }
        }

        //removes the DoubleNode at the specified index
        //if the index is not in the list or there are no items in the list it returns 0
        //otherwise it returns the DoubleNode that was removed
        r_doublenode<Type>* remove(int index){
            if(size == 0 || index < 0 || index > size - 1){
                return (Type) 0;
            }
            else if(index == 0)
            {
                r_doublenode<Type>* temp = head;
                head = head -> getNext();
                size--;
                return temp;
            }
            else
            {
                r_doublenode<Type>* curr = head;
                for(int i = 0; i < index; i++, curr = curr -> getNext());
                curr->prev->setNext(curr->getNext());
                size--;
                return curr;
            }
        }

        r_doublenode<Type>* remove(string name){
            if(size == 0){
                return NULL;
            }
            else
            {
                r_doublenode<Type>* curr = head;
                for(int i = 0; i < size; i++, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        if(i == 0)
                        {
                            head = head -> getNext();
                        }
                        else
                        {
                            curr->prev->setNext(curr->getNext());
                        }
                        size--;
                        return curr;
                    }
                }
                return NULL;
            }
        }

        //removes all the DoubleNodes in the list
        void removeAll(){
            if(size != 0){
                r_doublenode<Type>* curr = head;
                while(size > 1){
                    curr = curr->getNext();
                    head = curr;
                    delete(curr->prev);
                    size--;
                }
                delete(curr);
                initialization();
            }
        }

        //finds the DoubleNode at the specified index. If the index is not in the list or the list is empty it returns 0
        //otherwise it returns the DoubleNode at the specified index
        r_doublenode<Type>* get(int index){
            if(size == 0 || index > size){
                return 0;
            }else{
                r_doublenode<Type>* curr = head;
                for(int i = 0; i < index; i++, curr = curr -> getNext());
                return curr;
            }
        }

        r_doublenode<Type>* get(string name){
            if(size == 0){
                return 0;
            }else{
                r_doublenode<Type>* curr = head;
                for(int i = 0; i < size; i++, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        return curr;
                    }
                }
            }
            return 0;
        }

        //returns the index of the DoubleNode of the variable name
        int indexOf(string name){
           r_doublenode<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr -> getNext()){
                if(curr->getName() == name){
                    return i;
                }
            }
            return -1;
        }

        //prints out the list
        void print(){
            r_doublenode<Type>* curr = head;
            for(int i = 0; i < size; i++){
                cout << curr -> getData() -> getName() << endl;
                curr = curr -> getNext();
            }
        }

        //returns size of the list
        int getSize(){
            return size;
        }

    //private variables
    private:
        int size;
        r_doublenode<Type>* head;

        //initializes all to 0
        void initialization(){
            head = NULL;
            size = 0;
        }

};

#endif // R_DOUBLYLINKEDLIST_HPP_INCLUDED
