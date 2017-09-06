/* REPLACED BY PAWLIB. REVIEW AND REMOVE */

#ifndef SINGLYLINKEDLIST_HPP_INCLUDED
#define SINGLYLINKEDLIST_HPP_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string.h>
#include "ratscript/utils.hpp"

using std::cout;
using std::endl;

//Node structure using Templates
//contains 4 variables: Pointer to the name, pointer to the type, pointer to the variable, and pointer to the next node
template<typename T>
struct Node{
    public:
        //constructor converts the two std::strings to char* (I was having problems getting it to work with std::strings so i just converted it)
        //initializes pointer to next node to 0
        Node(T *datas, r_utils::DataType types){
            type = types;
            data = datas;
            next = NULL;
        }

        //deconstructor, called when the Node memory is set free
        //frees up the memory for the pointers to the name, the type, and the name
        ~Node(){
            free(data);
        }

        //some getters and setters
        T* getData(){return data;}
        string getName(){return data->getName();}
        r_utils::DataType getType(){return type;}
        Node<T>* getNext(){return next;}
        void setNext(Node<T>* nxt){next = nxt;}

    //private variables
    private:
        T* data;
        r_utils::DataType type;
        Node<T>* next;
};
//

//Class declaration for a template based Linked List
template<class Type> class SinglyLinkedList{
    public:
        //constructor initialized to 0
        SinglyLinkedList(){
            initialization();           //initializes all pointers and sizes to 0
        }
        //deconstructor: frees up the memory of any remaining node in the list
        ~SinglyLinkedList(){
            Node<Type>* prev = NULL;
            Node<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr->getNext()){
                if(prev != NULL){
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
                head = new Node<Type>(data, type);
                size++;
            }else{
                Node<Type>* newNode = new Node<Type>(data, type);
                Node<Type>* temp = head;
                for(int i = 0; i < size - 1; i++, temp = temp -> getNext());
                temp -> setNext(newNode);
                size++;
            }
        }

        //adds new node into list
        //sorts by name
        void addOrdered(Type* data, r_utils::DataType type){
            Node<Type>* newNode = new Node<Type>(data, type);
            if(size == 0){
                head = newNode;
                size++;
            }else{
                Node<Type>* curr = head;
                Node<Type>* prev = NULL;
                for(int i = 0; i < size; i++){
                    if(strcmp(newNode->getName().c_str(), curr->getName().c_str()) < 0){
                        break;
                    }else{
                        prev = curr;
                        curr = curr -> getNext();
                    }
                }
                if(prev == NULL)
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
        Node<Type>* remove(int index){
            if(size == 0 || index < 0 || index > size - 1){
                //TODO index out of bounds exception?
                return NULL;
            }
            else if(index == 0)
            {
                Node<Type>* prev = head;
                head = head -> getNext();
                size--;
                return prev;
            }
            else{
                Node<Type>* prev = head;
                Node<Type>* curr = head;
                for(int i = 0; i < index; i++, prev = curr, curr = curr -> getNext());
                prev->setNext(curr->getNext());
                size--;
                return curr;
            }
        }

        Node<Type>* remove(string name){
            if(size == 0){
                return NULL;
            }else{
                Node<Type>* prev = head;
                Node<Type>* curr = head;
                for(int i = 0; i < size; i++, prev = curr, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        if(i == 0)
                        {
                            head = head -> getNext();
                        }
                        else
                        {
                            prev->setNext(curr->getNext());
                        }
                        size--;
                        return curr;
                    }
                }
                return NULL;
            }
        }

        //removes all the nodes in the list
        void removeAll(){
            if(size != 0){
                Node<Type>* curr = head;
                Node<Type>* prev = head;
                while(size > 1){
                    curr = curr->getNext();
                    head = curr;
                    delete(prev);
                    prev = curr;
                    size--;
                }
                delete curr;
                initialization();
            }
        }

        //finds the Node at the specified index. If the index is not in the list or the list is empty it returns 0
        //otherwise it returns the node at the specified index
        Node<Type>* get(int index){
            if(size == 0 || index > size){
                return NULL;
            }else{
                Node<Type>* curr = head;
                for(int i = 0; i < index; i++, curr = curr -> getNext());
                return curr;
            }
        }

        Node<Type>* get(string name){
            if(size == 0){
                return NULL;
            }else{
                Node<Type>* curr = head;
                for(int i = 0; i < size; i++, curr = curr -> getNext()){
                    if(curr -> getName() == name){
                        return curr;
                    }
                }
            }
            return NULL;
        }

        //returns the index of the Node of the variable name
        int indexOf(string name){
           Node<Type>* curr = head;
            for(int i = 0; i < size; i++, curr = curr -> getNext()){
                if(curr->getName() == name){
                    return i;
                }
            }
            return -1;
        }

        //prints out the list
        void print(){
            Node<Type>* curr = head;
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
        Node<Type>* head;

        //initializes all to 0
        void initialization(){
            head = NULL;
            size = 0;
        }

};

#endif // SINGLYLINKEDLIST_HPP_INCLUDED
