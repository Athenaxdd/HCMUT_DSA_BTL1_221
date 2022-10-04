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
    charArrayList(charArrayList & otherS){
        maxSize = size = otherS.length();
        current = 0;
        list = new char[maxSize];
        for (int i = 0; i < size; ++i){
            list[i] = otherS.getValue();
            otherS.next();
        }
        otherS.toBegin();
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
            list[i] = list[i+1]; //i=1: next to index "i" :smh:
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
    char * getString(){
        return list;
    }
    void reverseChar() {
        for(int i = 0; i < size / 2; ++i){
            swap(list[i], list[size - i - 1]); 
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
        this -> next = nullptr;
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
        ConcatStringList (s);
        s.head = this -> head; // head of the concat is head for the first string;
        head -> nodeData.toBegin();
        s.tail = ptrTail; // tail of the concat is tail of the end concatStringList string (need better wording)
        tail -> next = ptrHead;
        s.listSize = this -> listSize +  otherS.listSize; // tail of the first concatStringList is connected to the head of ther concatStringList
        return s;
    };
    ConcatStringList subString(int from, int to) const{ // idea is take the node contain both index "from" and "to" and all the node between then cut the 2 head and tail   
        if (from >= to) throw logic_error("Invalid range");
        if (from < 0 || to > this -> listSize ) throw out_of_range("Index of string is invalid");
        charALNode * ptr = head;
        ConcatStringList * s = new ConcatStringList();
        s -> listSize = from - to;
        int travel = 0;
        while (from >= (ptr -> nodeData.length() + travel)){ // if "from" index is larger than charALnode length, skip the charALnode 
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
        }
        int headToFormIndex = travel;
        char * temp = new char[ptr -> nodeData.length()+1];
        temp = ptr -> nodeData.getString();
        temp[ptr -> nodeData.length()] = '\0';
        s -> tail = new charALNode(head);
        s -> head = s -> tail;
        s -> listSize = to - from;
        s -> head = new charALNode(ptr); // node has index "from" is stored as head and tail of new node
        s -> tail = s -> head;
        s -> head -> nodeData.toBegin();
        travel += ptr -> nodeData.length();
        ptr = ptr -> next;
        while (to >= (ptr -> nodeData.length() + travel)) { //travel from -> to, if travel throughout a node then add copy that node into a new charALnode, including the node has index "to"
            char * temp = new char[ptr -> nodeData.length()+1];
            temp = ptr -> nodeData.getString();
            temp[ptr -> nodeData.length()] = '\0'; 
            charALNode * newNode = new charALNode(ptr);
            s -> tail -> next = newNode;
            s -> tail = s -> tail -> next;
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
        }
        char * tmp = new char[ptr -> nodeData.length()+1];
        tmp = ptr -> nodeData.getString();
        tmp[ptr -> nodeData.length()] = '\0'; 
        charALNode * newNode =  new charALNode(ptr);
        travel += ptr -> nodeData.length();
        s -> tail -> next =  newNode;
        s -> tail = s -> tail -> next;
        s -> tail -> nodeData.moveToPosition(travel - to);
        while(s -> head -> nodeData.length() > headToFormIndex+1){ // remove from head to "from" index
            s -> head -> nodeData.remove();    
        }
        for (int i = to; i < travel; ++i){ // remove from index "to" to end;
            s -> tail -> nodeData.remove();
        }
        return * s;
    };
    charALNode * reverseList(charALNode * head) const{
        charALNode *current, *next, *prev;
        prev = NULL; 
        current = head;
        while(current != NULL){
                next = current -> next;
                current -> next = prev;
                prev = current;
                current = next;
        }
        head = prev;
        return head;
    };
    ConcatStringList reverse() const{
        ConcatStringList s = new ConcatStringList();
        charALNode * ptr;
        ptr = this -> head;
        s.head = new charALNode(*ptr);
        s.tail = s.head;
        s.tail -> nodeData.reverseChar();
        s.tail -> next = nullptr;
        ptr = ptr -> next;
        while (ptr) {
            charALNode * newNode = new charALNode(*ptr);
            s.tail -> next = newNode;
            s.tail = s.tail -> next;
            s.tail -> nodeData.reverseChar();
            s.tail -> next = nullptr;
            ptr = ptr -> next;
        }
        s.head = reverseList(s.head);
        return s;
    };
    ~ConcatStringList(){
        charALNode * ptr = head;
        while (ptr){
            charALNode * next = head -> next;
            delete ptr;
            ptr = next;
        }
        head = nullptr;
    };

public:
    class refNode{ // same idea as charArrayList and charALnode
        public:
            int referenceNum;
            charALNode * addRefNode;
            refNode * nextRefNode;
            charALNode * getAddRefNode(){
                return addRefNode;
            }
            refNode * getNextRefNode(){
                return nextRefNode;
            }
            refNode(){
                this -> referenceNum = 0;
                addRefNode = nullptr;
                this -> nextRefNode = nullptr;
            }
            refNode(charALNode * otherS){
                this -> referenceNum = 1;
                this -> addRefNode = otherS;
                this -> nextRefNode = nullptr;
            }
    };
    class ReferencesList {
        // TODO: may provide some attributes
    int refListSize = 0;
    refNode * refHead;
    refNode * refTail;
    refNode * refCurrent;
        public:
            ReferencesList(){
                this -> refListSize = 0;
                this -> refHead = new refNode();
                this -> refCurrent = refHead;
                this -> refTail = refHead;
            }
            int size() const{
                return refListSize;
            };
            void spilt(refNode * source, refNode ** front, refNode ** back){ // 2 pointers  
                refNode * slow = source;
                refNode * fast = source -> nextRefNode;
                while (fast != nullptr){
                    fast = fast -> nextRefNode;
                    if (fast != nullptr){
                        fast = fast -> nextRefNode;
                        slow = slow -> nextRefNode;
                    }
                }
                * front = source;
                * back = slow -> nextRefNode;
                slow -> nextRefNode = nullptr;
            }
            refNode * merge (refNode * list1, refNode * list2){
                refNode * result = nullptr;
                if (list1 == nullptr) return list2;
                else if (list2 == nullptr) return list1;
                if (list1 -> referenceNum <= list2 -> referenceNum){
                    result = list1;
                    result -> nextRefNode = merge(list1 -> nextRefNode, list2);
                } else{
                    result = list2;
                    result -> nextRefNode = merge(list1, list2 -> nextRefNode);
                    return result;
                }
            }
            void sortReferenceList(refNode ** refHead){
                refNode * head = * refHead;
                refNode * a, *b;
                if (head == nullptr || head -> nextRefNode == nullptr) return;
                spilt(head, &a, &b);
                sortReferenceList(&a);
                sortReferenceList(&b);
            }
            void increaseReference(charALNode * otherS){
                if (refListSize == 0){
                    refNode * temp = new refNode(otherS);
                    refCurrent = refHead = temp;
                    refHead -> nextRefNode = nullptr;
                    refListSize++;
                    return;
                }
                while (refCurrent -> getAddRefNode() != otherS){
                    if (refCurrent -> nextRefNode == nullptr) break; 
                    if (refCurrent -> addRefNode == otherS){
                        refCurrent -> referenceNum++;
                        sortReferenceList(&refHead);
                        refCurrent = refHead;
                        return;
                    }
                    refCurrent = refCurrent -> nextRefNode;
                }
                refNode * temp = new refNode(otherS);
                refCurrent -> nextRefNode = temp;
                refCurrent = refCurrent -> nextRefNode;
                refCurrent -> nextRefNode = nullptr;
                sortReferenceList(&refHead);
                refCurrent = refHead;

                refCurrent -> nextRefNode = temp;
                refListSize++;
            }
            int refCountAt(int index) const{
                if (index < 0 || index >= refListSize) throw out_of_range("Index of references list is invalid!");
                refNode * temp = refHead;
                for (int i = index; index > 0; --i){
                    temp = temp -> nextRefNode;
                }
                return temp -> referenceNum;
            };
            std::string refCountsString() const{
                string result = "";
                refNode * temp = refHead;
                result.append("RefCounts[");
                while (temp){
                    result.append(to_string(temp -> referenceNum));
                    if (temp -> nextRefNode != nullptr) result.append(",");
                    temp = temp -> nextRefNode;
                }
                result.append("]");
                return result;
            };
    };

    class DeleteStringList {
        // TODO: may provide some attributes

        public:
            int size() const;
            std::string totalRefCountsString() const;
    };
};

#endif // __CONCAT_STRING_LIST_H__