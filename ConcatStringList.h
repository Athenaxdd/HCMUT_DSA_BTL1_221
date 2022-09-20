#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

//g++ -o main main.cpp ConcatStringList.cpp -I . -std=c++11

class charArrayList {
private:
    int maxSize;
    int size;
    int current;
    char * list;

public:
    charArrayList(int initSize = 256){
        maxSize = size;
        size = current = 0;
        list = new char[maxSize];
    }
    ~charArrayList(){
        delete [] list;
    }
    void clear(){
        delete [] list;
        size = current = 0;
        list = new char[maxSize];
    }
    void toBegin(){
        current = 0;
    }
    void toEnd(){ 
        current = size;
    }
    void next(){
        if (current < size) current++;
    }
    void prev(){
        if(current != 0) current--;
    }
    int length() const{
        return size;
    }
    void append(const char& s){
        list[size++] = s;
    }
    void resize(int size){
        maxSize = size;
        list = new char[maxSize];
    }
    void insert (const char& s){
        for (int i = size; i > current; --i){
            list[i] = list[i-1];
        }
        list[current] = s;
        size++;
    }
    int currentPosition() const{
        return current;
    }
    void moveToPosition(int pos){
        if ((pos <= 0) && (pos >= size)) {
            return;
        }
        else current = pos;
    }
    char& getValue() {
        return list[current];
    }
};

class charALNode{
public:
    charArrayList nodeData;
    charALNode * next;
    charALNode(const char *s) {
        int size = 0;
        while(s[size] != '\0') size++;
        for (int i = 0; i < size; ++i){
            nodeData.append(s[i]);
        }
        next = nullptr;
    }
};

class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration

public:
    static ReferencesList refList;
    static DeleteStringList delStrList;

    // TODO: may provide some attributes
    charALNode* head = nullptr;
    charALNode* tail = nullptr;
    int listSize = 0;

public:
    ConcatStringList(){
        head = nullptr;
        tail = nullptr;
    }
    ConcatStringList(const ConcatStringList* other){
        this -> head = other -> head;
        this -> tail = other -> tail;
        this -> listSize = other -> listSize;
    }
    ConcatStringList(const char *s){
        head = nullptr;
        tail = nullptr;
        head = new charALNode(s);
        tail = head;
        listSize += head -> nodeData.length();
    };
    int length() const{
        return listSize;
    };
    char get(int index) const{
        if (index > listSize || index < 0) throw out_of_range("Index of string is invalid!");
        charALNode * ptr = head;
        int travel = 0;
        while (travel + ptr -> nodeData.length() <= index){
            travel += ptr -> nodeData.length();
            ptr = ptr ->next;
        }
        ptr ->nodeData.moveToPosition(index - travel);
        return ptr -> nodeData.getValue();
    };
    int indexOf(char c) const{
        charALNode * ptr = head;
        int position = 0;
        ptr -> nodeData.toBegin();
        while(ptr){
            for (ptr -> nodeData.toBegin(); ptr -> nodeData.currentPosition() < ptr -> nodeData.length(); ptr -> nodeData.next()){
                if(ptr -> nodeData.getValue() == c) return position;
                else position++;
            }
            ptr -> nodeData.toBegin();
            ptr = ptr -> next;
        }
        return -1;  
    };
    std::string toString() const{
        string ans = "";
        charALNode * ptr = head;
        while(ptr){
            for (ptr -> nodeData.toBegin(); ptr -> nodeData.currentPosition() < ptr -> nodeData.length(); ptr -> nodeData.next()){
                ans += ptr -> nodeData.getValue();
            }
            ptr -> nodeData.toBegin();
            ptr = ptr -> next;
        }
        return ans;
    };
    ConcatStringList concat(const ConcatStringList & otherS) const{
        charALNode * ptrHead = otherS.head; 
        charALNode * ptrTail = otherS.tail;
        ConcatStringList(s);
        s.head = head;
        s.tail = ptrTail;
        tail -> next = ptrHead;
        return s;
    };
    ConcatStringList subString(int from, int to) const{
        
    };
    // ConcatStringList reverse() const;
    // ~ConcatStringList();

public:
    class ReferencesList {
        // TODO: may provide some attributes

        public:
            int size() const;
            int refCountAt(int index) const;
            std::string refCountsString() const;
    };

    class DeleteStringList {
        // TODO: may provide some attributes

        public:
            int size() const;
            std::string totalRefCountsString() const;
    };
};

#endif // __CONCAT_STRING_LIST_H__