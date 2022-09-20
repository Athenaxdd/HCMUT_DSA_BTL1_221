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
    void remove(){
        for (int i = current; i < size - 1; i++){
            list[i] = list[i++];
        }
        size--;
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
    char& getValue(){
        return list[current];
    }
    void reverse(char* str) {
        int len = sizeof(str) / sizeof(str[0]);
        for(size_t i=0; i<len/2; i++){
            swap(str[i], str[len-i-1]);
        }
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
    charALNode(const charALNode * other){
        this -> nodeData = other -> nodeData;
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
        while (travel + ptr -> nodeData.length() <= index){ // using travel in case the index position is larger than the length of a node
            travel += ptr -> nodeData.length();
            ptr = ptr ->next;
        }
        ptr ->nodeData.moveToPosition(index - travel); // index - travel = position 
        return ptr -> nodeData.getValue();
    };
    int indexOf(char c) const{
        charALNode * ptr = head;
        int position = 0; // counter for position
        while(ptr){
            for (ptr -> nodeData.toBegin(); ptr -> nodeData.currentPosition() < ptr -> nodeData.length(); ptr -> nodeData.next()){ // move from head of charALnode to end
                if(ptr -> nodeData.getValue() == c) return position;
                else position++;
            }
            ptr -> nodeData.toBegin(); // reset ptr position to charALnode head
            ptr = ptr -> next;
        }
        return -1;  
    };
    std::string toString() const{
        string ans = "";
        charALNode * ptr = head;
        while(ptr){
            for (ptr -> nodeData.toBegin(); ptr -> nodeData.currentPosition() < ptr -> nodeData.length(); ptr -> nodeData.next()){
                ans += ptr -> nodeData.getValue(); // push each character into ans;
            }
            ptr -> nodeData.toBegin(); // reset ptr position to charALnode head
            ptr = ptr -> next;
        }
        return ans;
    };
    ConcatStringList concat(const ConcatStringList & otherS) const{
        charALNode * ptrHead = otherS.head; 
        charALNode * ptrTail = otherS.tail;
        ConcatStringList(s);
        s.head = head; // head of the concat is head for the first string;
        s.tail = ptrTail; // tail of the concat is tail of the end concatStringList string (need better wording)
        tail -> next = ptrHead; // tail of the first concatStringList is connected to the head of ther concatStringList
        return s;
    };
    ConcatStringList subString(int from, int to) const{
        if (from >= to) throw logic_error("Invalid range");
        if (from < 0 || to < 0 ) throw out_of_range("Index of string is invalid");
        charALNode * ptr = head;
        ConcatStringList * s = new ConcatStringList();
        s -> listSize = to - from;
        int travel = 0;
        while (from >= (ptr -> nodeData.length() + travel)){ // if "from" index is larger than charALnode length, skip the charALnode 
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
        }
        s -> head = new charALNode(ptr); // node has index "from" is stored as head and tail of new node
        s -> tail = s -> head;
        s -> head -> nodeData.toBegin();
        ptr = ptr -> next;
        travel += ptr -> nodeData.length();
        while (to >= (ptr -> nodeData.length() + travel)) { //travel from -> to, if travel throughout a node then add copy that node into a new charALnode, including the node has index "to"
            charALNode * newNode = new charALNode(ptr);
            s -> tail -> next = newNode;
            s -> tail = s -> tail -> next;
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
            delete newNode;
        }
        charALNode * newNode =  new charALNode(ptr);
        s -> tail -> next =  newNode;
        s -> tail = s -> tail -> next;
        cout << travel << endl;
        s -> tail -> nodeData.moveToPosition(to - travel);
        for (int i = 0; i <= from; ++i){ // remove from head to index "from"
            s -> head -> nodeData.remove();
        }
        for (int i = to; i < travel; ++i){ // remove from index "to" to end;
            s -> tail -> nodeData.remove();
        }
        delete newNode;
        return s;
    };
    ConcatStringList reverse() const{
        charALNode * nodeCurrent, * nodeNext, * nodePrev, * ptr;
        nodeCurrent = tail;
        nodePrev = nullptr;
        while (nodeCurrent){
            ptr -> nodeData.reverse();
            nodeNext = head -> next;
            nodeCurrent -> next = nodePrev;
            nodePrev = nodeCurrent;
            nodeCurrent = nodeNext;
        }
    };
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