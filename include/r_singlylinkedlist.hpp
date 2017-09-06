/**Ratscript Norvegicus: SinglyLinkedList<p>
* A singly linked list class.
* @since 2.0
* @author Michael Parkman
*/

#ifndef R_SINGLYLINKEDLIST_HPP_INCLUDED
#define R_SINGLYLINKEDLIST_HPP_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string.h>
#include "r_utils.hpp"

using std::cout;
using std::endl;

//Node structure using Templates
//contains 4 variables: Pointer to the name, pointer to the type, pointer to the variable, and pointer to the next node
template<typename T>
struct r_node{
    public:
        //constructor converts the two std::strings to char* (I was having problems getting it to work with std::strings so i just converted it)
        //initializes pointer to next node to 0
        r_node(T *datas, r_utils::DataType types){
            type = types;
            data = datas;
            next = 0;
        }

        //deconstructor, called when the Node memory is set free
        //frees up the memory for the pointers to the name, the type, and the name
        ~r_node(){
            free(data);
        }

        //some getters and setters
        T* getData(){return data;}
        string getName(){return data->getName();}
        r_utils::DataType getType(){return type;}
        r_node<T>* getNext(){return next;}
        void setNext(r_node<T>* nxt){next = nxt;}

    //private variables
    private:
        T* data;
        r_utils::DataType type;
        r_node<T>* next;
};
//

//Class declaration for a template based Linked List
template<class Type> class r_singlylinkedlist{
    public:
        //constructor initialized to 0
        r_singlylinkedlist(){
            initialization();           //initializes all pointers and sizes to 0
        }
        //deconstructor: frees up the memory of any remaining node in the list
        ~r_singlylinkedlist(){
            r_node<Type>* prev = 0;
            r_node<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr->getNext()){
                if(prev != 0){
                    delete(prev);
                    prev = curr;
                }else{
                    prev = curr;
                }
            }
            delete(prev);
        }

        //adds a node to the list
        //gets passed in variable name, variable type, and the actual variable
        void  add(Type* data, r_utils::DataType type){
            if(size == 0){
                head = new r_node<Type>(data, type);
                size++;
            }else{
                r_node<Type>* newNode = new r_node<Type>(data, type);
                r_node<Type>* temp = head;
                for(int i = 0; i < size - 1; i++, temp = temp -> getNext());
                temp -> setNext(newNode);
                size++;
            }
        }

        //adds new node into list
        //sorts by name
        void addOrdered(Type* data, r_utils::DataType type){
            r_node<Type>* newNode = new r_node<Type>(data, type);
            if(size == 0){
                head = newNode;
                size++;
            }else{
                r_node<Type>* curr = head;
                r_node<Type>* prev = 0;
                for(int i = 0; i < size; i++){
                    if(strcmp(newNode->getName().c_str(), curr->getName().c_str()) < 0){
                        break;
                    }else{
                        prev = curr;
                        curr = curr -> getNext();
                    }
                }
                if(prev == 0)
                {
                    head = newNode;
                }
                else
                {
                   prev -> setNext(newNode);
                }
                newNode->setNext(curr);
                size++;
            }
        }

        //removes the node at the specified index
        //if the index is not in the list or there are no items in the list it returns 0
        //otherwise it returns the node that was removed
        r_node<Type>* remove(int index){
            if(size == 0 || index < 0 || index > size - 1){
                return (Type) 0;
            }else{
                r_node<Type>* prev = head;
                r_node<Type>* curr = head;
                for(int i = 0; i < index; i++, prev = curr, curr = curr -> getNext());
                prev->setNext(curr->getNext());
                size--;
                return curr;
            }
        }

        r_node<Type>* remove(string name){
            if(size == 0){
                return (Type) 0;
            }else{
                r_node<Type>* prev = head;
                r_node<Type>* curr = head;
                for(int i = 0; i < size; i++, prev = curr, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        prev->setNext(curr->getNext());
                        size--;
                        return curr;
                    }
                }
                return (Type) 0;
            }
        }

        //removes all the nodes in the list
        void removeAll(){
            if(size != 0){
                r_node<Type>* curr = head;
                r_node<Type>* prev = head;
                while(curr != 0){
                    curr = curr->getNext();
                    head = curr;
                    delete(prev);
                    prev = curr;
                    size--;
                }
                initialization();
            }
        }

        //finds the Node at the specified index. If the index is not in the list or the list is empty it returns 0
        //otherwise it returns the node at the specified index
        r_node<Type>* get(int index){
            if(size == 0 || index > size){
                return 0;
            }else{
                r_node<Type>* curr = head;
                for(int i = 0; i < index; i++, curr = curr -> getNext());
                return curr;
            }
        }

        r_node<Type>* get(string name){
            if(size == 0){
                return 0;
            }else{
                r_node<Type>* curr = head;
                for(int i = 0; i < size; i++, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        return curr;
                    }
                }
            }
            return 0;
        }

        //returns the index of the Node of the variable name
        int indexOf(string name){
           r_node<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr -> getNext()){
                if(curr->getName() == name){
                    return i;
                }
            }
            return -1;
        }

        //prints out the list
        void print(){
            r_node<Type>* curr = head;
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
        r_node<Type>* head;

        //initializes all to 0
        void initialization(){
            head = 0;
            size = 0;
        }

};

#endif // R_SINGLYLINKEDLIST_HPP_INCLUDED
