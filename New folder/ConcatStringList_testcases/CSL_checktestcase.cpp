/*#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

/*#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG*/

using namespace std;

const string dir = "C:\myCode\initial\New folder\ConcatStringList_testcases\\input\\";
const string dir2 = "C:\myCode\initial\New folder\ConcatStringList_testcases\\result\\";
// Replace '_' with your ConcatStringList_testcases's directory

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
        refList.increaseReferenceList(this -> head);
        refList.increaseReferenceList(this -> tail);
    }
    ConcatStringList(const char *s){
        head = nullptr;
        tail = nullptr;
        head = new charALNode(s);
        tail = head;
        listSize += head -> nodeData.length();
        refList.increaseReferenceList(this -> head);
        refList.increaseReferenceList(this -> tail);
    };
    char * createNewChar (charALNode * node){
        char * temp = new char[node -> nodeData.length()+1];
        temp = node -> nodeData.getString();
        temp[node -> nodeData.length()] = '\0';
        return temp;
    }
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
        ConcatStringList s;
        s.listSize = this -> listSize +  otherS.listSize; // tail of the first concatStringList is connected to the head of ther concatStringList
        s.head = this -> head; // head of the concat is head for the first string;
        s.tail = otherS.tail; // tail of the concat is tail of the end concatStringList string (need better wording)
        tail -> next = otherS.head;
        head -> nodeData.toBegin();
        refList.increaseReferenceList(s.head);
        refList.increaseReferenceList(s.tail);
        return s;
    };
    ConcatStringList subString(int from, int to) const{ // idea is take the node contain both index "from" and "to" and all the node between then cut the 2 head and tail   
        if (from >= to) throw logic_error("Invalid range");
        if (from < 0 || to > this -> listSize ) throw out_of_range("Index of string is invalid");
        charALNode * ptr = head;
        ConcatStringList s;
        int travel = 0;
        while (from >= (ptr -> nodeData.length() + travel)){ // if "from" index is larger than charALnode length, skip the charALnode 
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
        }
        int headToFormIndex = travel;
        char * temp = new char[ptr -> nodeData.length()+1];
        temp = ptr -> nodeData.getString();
        temp[ptr -> nodeData.length()] = '\0';
        s.head = s.tail = new charALNode(temp);
        s.listSize = to - from; // node has index "from" is stored as head and tail of new node
        s.head -> nodeData.toBegin();
        travel += ptr -> nodeData.length();
        ptr = ptr -> next;
        while (to >= (ptr -> nodeData.length() + travel)) { //travel from -> to, if travel throughout a node then add copy that node into a new charALnode, including the node has index "to"
            char * temp = new char[ptr -> nodeData.length()+1];
            temp = ptr -> nodeData.getString();
            temp[ptr -> nodeData.length()] = '\0'; 
            charALNode * newNode = new charALNode(temp);
            s.tail -> next = newNode;
            s.tail = s.tail -> next;
            travel += ptr -> nodeData.length();
            ptr = ptr -> next;
        }
        char * tmp = new char[ptr -> nodeData.length()+1];
        tmp = ptr -> nodeData.getString();
        tmp[ptr -> nodeData.length()] = '\0'; 
        charALNode * newNode =  new charALNode(tmp);
        travel += ptr -> nodeData.length();
        s.tail -> next =  newNode;
        s.tail = s.tail -> next;
        s.tail -> nodeData.moveToPosition(travel - to);
        while(from - headToFormIndex > 0){ // remove from head to "from" index
            s.head -> nodeData.remove();
            headToFormIndex++;    
        }
        for (int i = to; i < travel; ++i){ // remove from index "to" to end;
            s.tail -> nodeData.remove();
        }
        refList.increaseReferenceList(s.head);
        refList.increaseReferenceList(s.tail);
        return s;
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
        ConcatStringList s;
        charALNode * ptr;
        ptr = this -> head;
        char * temp = new char[ptr -> nodeData.length()+1];
        temp = ptr -> nodeData.getString();
        temp[ptr -> nodeData.length()] = '\0';
        s.head = s.tail = new charALNode(temp);
        s.tail -> next = nullptr;
        s.tail -> nodeData.reverseChar();
        ptr = ptr -> next;
        while (ptr) {
            charALNode * newNode = new charALNode(*ptr);
            s.tail -> next = newNode;
            s.tail = s.tail -> next;
            s.tail -> nodeData.reverseChar();
            s.tail -> next = nullptr;
            ptr = ptr -> next;
        }
        charALNode * tempHead = s.head;
        s.head = reverseList(s.head);
        s.tail = tempHead;
        refList.increaseReferenceList(s.head);
        refList.increaseReferenceList(s.tail);
        cout << s.head << " " << s.tail << endl;
        return s;
    };
    ~ConcatStringList(){
        refList.deleteRefNode(this -> head, this -> tail);
        if (delStrList.deleteStringListCheck()){
            cout << "loop called" << endl;
            while (head){
                charALNode * temp = head;
                head = head -> next;
                delete temp;
            }
        }
        head = nullptr;
        tail = nullptr;
        cout << ConcatStringList::refList.refCountsString() << endl;
        cout << ConcatStringList::delStrList.totalRefCountsString() << endl;
        system("pause");
        refList.checkEmpty();
    };

public:
    class refNode{ // same idea as charALnode
        public:
            int referenceNum;
            charALNode * addRefNode;
            refNode * nextRefNode;  
            refNode(){  
                this -> referenceNum = 0;
                this -> addRefNode = nullptr;
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
    refNode * headReferenceNode;
    refNode * tailReferenceNode;
    refNode * currentReferenceNode;
        public:
            ReferencesList(){
                this -> refListSize = 0;
                this -> headReferenceNode = nullptr;
                this -> currentReferenceNode = nullptr;
                this -> tailReferenceNode = nullptr;
            }
            int size() const{
                return refListSize;
            };
            void spilt(refNode * source, refNode ** front, refNode ** back){ // 2 pointers | tortoise & hare 
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
                }
                return result;
            }
            void sortReferenceList(refNode ** headReferenceNode){
                refNode * head = * headReferenceNode;
                refNode * a, *b;
                if (head == nullptr || head -> nextRefNode == nullptr) return;
                spilt(head, &a, &b);
                sortReferenceList(&a);
                sortReferenceList(&b);
                *headReferenceNode = merge(a, b);
            }
            // void pushZeroes(){
            //     if (currentReferenceNode == nullptr) return;
            //     refNode * tempHead = headReferenceNode;
            //     while (tempHead -> nextRefNode -> referenceNum != 0){
            //         refNode * temp = tempHead;
            //         tailReferenceNode -> nextRefNode = temp;
            //         temp -> nextRefNode = nullptr;
            //         tailReferenceNode = temp;
            //         headReferenceNode = temp -> nextRefNode; //?? memo leak?
            //         tempHead = tempHead -> nextRefNode;
            //     }
            //     if (tempHead -> referenceNum == 0){
            //         refNode * temp = tempHead;
            //         tailReferenceNode -> nextRefNode = temp;
            //         temp -> nextRefNode = nullptr;
            //         tailReferenceNode = temp;
            //     }
            // }
            void pushZeroes(){

            };
            void increaseReferenceList(charALNode * otherS){
                if (refListSize == 0){ // create new list
                    refNode * temp = new refNode(otherS);
                    currentReferenceNode = headReferenceNode = temp;
                    headReferenceNode -> nextRefNode = nullptr;
                    tailReferenceNode = headReferenceNode;
                    refListSize++;
                    return;
                }
                while (1){
                    if (currentReferenceNode -> addRefNode == otherS){
                        currentReferenceNode -> referenceNum += 1;
                        sortReferenceList(&headReferenceNode);
                        currentReferenceNode = headReferenceNode;
                        return;
                    }
                    if (currentReferenceNode -> nextRefNode == nullptr) break;  
                    else currentReferenceNode = currentReferenceNode -> nextRefNode;
                }
                refNode * temp = new refNode(otherS);
                currentReferenceNode -> nextRefNode = temp;
                currentReferenceNode = currentReferenceNode -> nextRefNode;
                currentReferenceNode -> nextRefNode = nullptr;
                sortReferenceList(&headReferenceNode);
                currentReferenceNode = headReferenceNode;
                pushZeroes();
                currentReferenceNode = headReferenceNode;
                refListSize++;
            }
            void addToTail(refNode * node){
                refNode * tmp = tailReferenceNode;
                tmp -> nextRefNode = node;
                node -> nextRefNode = nullptr;
                tailReferenceNode = node;
            }
            void deleteRefNode(charALNode * head, charALNode * tail){
                refNode * deleteRefHead;
                refNode * deleteRefTail;
                currentReferenceNode = headReferenceNode;
                while (currentReferenceNode){
                    if (head == currentReferenceNode -> addRefNode){
                        deleteRefHead = currentReferenceNode;
                        currentReferenceNode -> referenceNum--;
                    }
                    if (tail == currentReferenceNode -> addRefNode){
                        deleteRefTail = currentReferenceNode;
                        currentReferenceNode -> referenceNum--;
                    }
                    currentReferenceNode = currentReferenceNode -> nextRefNode;
                    
                }
                delStrList.increaseDeleteList(deleteRefHead, deleteRefTail);
                sortReferenceList(&headReferenceNode);
                currentReferenceNode = headReferenceNode;
                // cout << headReferenceNode << endl;
                pushZeroes();
                currentReferenceNode = headReferenceNode;
            }
            void clearRefList(int size){
                if (size == 0){
                    currentReferenceNode = headReferenceNode;
                    while (currentReferenceNode){
                        refNode * temp = currentReferenceNode -> nextRefNode;
                        delete currentReferenceNode;
                        currentReferenceNode = temp;
                        refListSize--;
                    }
                    headReferenceNode = nullptr;
                }
                return;
            }
            void checkEmpty(){
                currentReferenceNode = headReferenceNode;
                while (currentReferenceNode)
                {
                    if (currentReferenceNode -> referenceNum != 0) return;
                    currentReferenceNode = currentReferenceNode -> nextRefNode;
                }
                currentReferenceNode = headReferenceNode;
                while(currentReferenceNode) 
                {
                    refNode* next = currentReferenceNode -> nextRefNode;
                    delete currentReferenceNode;
                    currentReferenceNode = next;
                    refListSize--;
                }          
                headReferenceNode = nullptr;
            }
            int refCountAt(int index) const{
                if (index < 0 || index >= refListSize) throw out_of_range("Index of references list is invalid!");
                refNode * temp = headReferenceNode;
                for (int i = index; index > 0; --i){
                    temp = temp -> nextRefNode;
                }
                return temp -> referenceNum;
            };
            std::string refCountsString() const{
                string result = "";
                refNode * temp = headReferenceNode;
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
    class deleteStringNode{
        public:
        refNode * headDeleteStringNode;
        refNode * tailDeleteStringNode;
        deleteStringNode * nextDeleteStringNode;
        deleteStringNode(refNode * head, refNode * tail){
            this -> headDeleteStringNode = head;
            this -> tailDeleteStringNode = tail;
            this -> nextDeleteStringNode= nullptr;
        }
    };
    class DeleteStringList {
        // TODO: may provide some attributes
        int deleteStringListSize = 0;
        deleteStringNode * headDeleteNode;
        deleteStringNode * tailDeleteNode;
        public:
        DeleteStringList(){
            deleteStringListSize = 0;
            headDeleteNode = nullptr;
            tailDeleteNode = nullptr;
        }
        public:
            int size() const{
                return deleteStringListSize;
            };
            void increaseDeleteList(refNode * head, refNode * tail){
                if (deleteStringListSize == 0){
                    deleteStringNode * tempDelete = new deleteStringNode(head, tail);
                    tailDeleteNode = headDeleteNode = tempDelete;
                    headDeleteNode -> nextDeleteStringNode = nullptr;
                    deleteStringListSize++;
                }
                deleteStringNode * tempDelete = new deleteStringNode(head, tail);
                tailDeleteNode -> nextDeleteStringNode = tempDelete;
                tailDeleteNode = tailDeleteNode -> nextDeleteStringNode;
                tailDeleteNode -> nextDeleteStringNode = nullptr;
                deleteStringListSize++;
            }
            bool deleteStringListCheck(){
                deleteStringNode * current = headDeleteNode;
                deleteStringNode * prev = current;
                bool flag = false;
                while (current){
                    if (current -> headDeleteStringNode -> referenceNum == 0 && current -> tailDeleteStringNode -> referenceNum == 0){
                        deleteStringListSize--;
                        flag = true;
                        if (deleteStringListSize == 0){
                            headDeleteNode = tailDeleteNode = nullptr;
                            return true;
                        }
                        if (current == headDeleteNode){
                            deleteStringNode * temp = headDeleteNode;
                            prev = current = headDeleteNode -> nextDeleteStringNode;
                            headDeleteNode = headDeleteNode -> nextDeleteStringNode;
                            temp -> nextDeleteStringNode = nullptr;
                            delete temp;
                            continue;
                        }
                        if (current == tailDeleteNode){
                            tailDeleteNode = prev;
                            tailDeleteNode -> nextDeleteStringNode = nullptr;
                            current -> nextDeleteStringNode = nullptr;
                            delete current;
                            return true;
                        }
                        prev -> nextDeleteStringNode = current -> nextDeleteStringNode;
                        current -> nextDeleteStringNode = nullptr;
                        delete current;
                        current = prev -> nextDeleteStringNode;
                    }
                    prev = current;
                    current = current -> nextDeleteStringNode;
                }
                return flag;
            }
            std::string totalRefCountsString() const{
                if (headDeleteNode == nullptr) return "TotalRefCounts[]";
                string result = "";
                deleteStringNode * temp = headDeleteNode;
                result.append("TotalRefCounts[");
                while (temp){
                    if (temp -> headDeleteStringNode -> addRefNode != temp -> tailDeleteStringNode -> addRefNode){
                        result.append(to_string(temp -> headDeleteStringNode -> referenceNum + temp -> tailDeleteStringNode -> referenceNum));
                    } else result.append(to_string(temp -> headDeleteStringNode -> referenceNum));
                    if (temp -> nextDeleteStringNode != nullptr) result.append(",");
                    temp = temp -> nextDeleteStringNode;
                }
                result.append("]");
                return result;
            }; 
    };
};

ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();

void myAssert(bool val, string msg)
{
    if (val == false)
    {
        cout << msg;
        abort();
    }
}

int main()
{
    /*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);*/
    //_CrtMemState s1, s2, s3;
    vector<ConcatStringList*> CSLVector(100, nullptr);
    //_CrtMemCheckpoint(&s1);
    for (int i = 1; i <= 100; i++)
    {
        cout << "Testcase #" << i << ": ";
        fstream inp(dir + "test" + to_string(i) + ".txt", ios::in);
        fstream out(dir2 + "test" + to_string(i) + ".txt", ios::in);
        string line;
        while (getline(inp, line))
        {
            string res_line = "";
            string s = "";
            if (line == "refCountsString")
            {
                getline(out, res_line);
                s = ConcatStringList::refList.refCountsString();
            }
            else if (line == "totalRefCountsString")
            {
                getline(out, res_line);
                s = ConcatStringList::delStrList.totalRefCountsString();
            }
            else if (line == "CheckMemoryLeaks")
            {
                break;
            }
            else
            {
                string key, variable, instruction;
                int space_index = line.find(' ');
                key = line.substr(0, space_index);
                if (key == "toString")
                {
                    getline(out, res_line);
                    variable = line.substr(space_index + 1);
                    int index = stoi(variable);
                    myAssert((index >= 0 && index < 100), "Index is out of bound!");
                    if (CSLVector[index] != nullptr)
                    {
                        s = CSLVector[index]->toString();
                    }
                }
                else if (key == "length")
                {
                    getline(out, res_line);
                    variable = line.substr(space_index + 1);
                    if (variable == "refList")
                    {
                        s = to_string(ConcatStringList::refList.size());
                    }
                    else if (variable == "delStrList")
                    {
                        s = to_string(ConcatStringList::delStrList.size());
                    }
                    else
                    {
                        int index = stoi(variable);
                        myAssert((index >= 0 && index < 100), "Index is out of bound!");
                        if (CSLVector[index] != nullptr)
                        {
                            s = to_string(CSLVector[index]->length());
                        }
                    }
                }
                else if (key == "refCountAt")
                {
                    getline(out, res_line);
                    variable = line.substr(space_index + 1);
                    int idx = stoi(variable);
                    try
                    {
                        s = to_string(ConcatStringList::refList.refCountAt(idx));
                    }
                    catch (exception& e)
                    {
                        s = string(e.what());
                    }
                }
                else if (key == "get")
                {
                    getline(out, res_line);
                    int space_index2 = line.find(' ', space_index + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1);
                    int index = stoi(variable);
                    int idx = stoi(instruction);
                    myAssert((index >= 0 && index < 100), "Index is out of bound!");
                    if (CSLVector[index] != nullptr)
                    {
                        try
                        {
                            s += CSLVector[index]->get(idx);
                        }
                        catch (exception& e)
                        {
                            s = string(e.what());
                        }
                    }
                }
                else if (key == "indexOf")
                {
                    getline(out, res_line);
                    int space_index2 = line.find(' ', space_index + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1);
                    int index = stoi(variable);
                    myAssert((index >= 0 && index < 100), "Index is out of bound!");
                    if (CSLVector[index] != nullptr)
                    {
                        s = to_string(CSLVector[index]->indexOf(instruction[0]));
                    }
                }
                else if (key == "subString")
                {
                    getline(out, res_line);
                    int space_index2 = line.find(' ', space_index + 1);
                    int space_index3 = line.find(' ', space_index2 + 1);
                    int space_index4 = line.find(' ', space_index3 + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1, space_index3 - space_index2 - 1);
                    string from = line.substr(space_index3 + 1, space_index4 - space_index3 - 1);
                    string to = line.substr(space_index4 + 1);
                    int index1 = stoi(variable);
                    int index2 = stoi(instruction);
                    myAssert((index1 >= 0 && index1 < 100), "Index is out of bound!");
                    myAssert((index2 >= 0 && index2 < 100), "Index is out of bound!");
                    myAssert(CSLVector[index1] == nullptr, "Cannot overwrite the object!");
                    if (CSLVector[index2] != nullptr)
                    {
                        try
                        {
                            CSLVector[index1] = new ConcatStringList(CSLVector[index2]->subString(stoi(from), stoi(to)));
                            s = (CSLVector[index1] == nullptr ? "Failed to allocate memory" : "Success");
                        }
                        catch (exception& e)
                        {
                            s = string(e.what());
                        }
                    }
                }
                else if (key == "reverse")
                {
                    getline(out, res_line);
                    int space_index2 = line.find(' ', space_index + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1);
                    int index1 = stoi(variable);
                    int index2 = stoi(instruction);
                    myAssert((index1 >= 0 && index1 < 100), "Index is out of bound!");
                    myAssert((index2 >= 0 && index2 < 100), "Index is out of bound!");
                    myAssert(CSLVector[index1] == nullptr, "Cannot overwrite the object!");
                    if (CSLVector[index2] != nullptr)
                    {
                        CSLVector[index1] = new ConcatStringList(CSLVector[index2]->reverse());
                        s = (CSLVector[index1] == nullptr ? "Failed to allocate memory" : "Success");
                    }
                }
                else if (key == "single_concat")
                {
                    getline(out, res_line);
                    string safe_object;
                    int space_index2 = line.find(' ', space_index + 1);
                    int space_index3 = line.find(' ', space_index2 + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1, space_index3 - space_index2 - 1);
                    safe_object = line.substr(space_index3 + 1);
                    myAssert(instruction != safe_object, "An object cannot appear twice in function concat() !");
                    int index1 = stoi(variable);
                    int index2 = stoi(instruction);
                    int index3 = stoi(safe_object);
                    myAssert((index1 >= 0 && index1 < 100), "Index is out of bound!");
                    myAssert((index2 >= 0 && index2 < 100), "Index is out of bound!");
                    myAssert((index3 >= 0 && index3 < 100), "Index is out of bound!");
                    myAssert((CSLVector[index2] != nullptr) || (CSLVector[index3] != nullptr), "Object is not initialize!");
                    myAssert((variable != safe_object) || (variable != instruction), "Memory will be leaked!");
                    myAssert(CSLVector[index1] == nullptr, "Cannot overwrite the object!");
                    CSLVector[index1] = new ConcatStringList(CSLVector[index2]->concat(*CSLVector[index3]));
                    s = (CSLVector[index1] == nullptr ? "Failed to allocate memory" : "Success");
                }
                else if (key == "init")
                {
                    getline(out, res_line);
                    int space_index2 = line.find(' ', space_index + 1);
                    variable = line.substr(space_index + 1, space_index2 - space_index - 1);
                    instruction = line.substr(space_index2 + 1);
                    int index1 = stoi(variable);
                    myAssert((index1 >= 0 && index1 < 100), "Index is out of bound!");
                    myAssert(CSLVector[index1] == nullptr, "Cannot overwrite the object!");
                    if (instruction == "\"empty_string\"")
                    {
                        CSLVector[index1] = new ConcatStringList("");
                    }
                    else
                    {
                        CSLVector[index1] = new ConcatStringList(instruction.c_str());
                    }
                    s = (CSLVector[index1] == nullptr ? "Failed to allocate memory!" : "Success");
                }
                else if (key == "delete")
                {
                    variable = line.substr(space_index + 1);
                    int index1 = stoi(variable);
                    delete CSLVector[index1];
                    CSLVector[index1] = nullptr;
                }
                else
                {
                    cout << "Invalid instruction: " + line;
                    break;
                }
            }
            myAssert(s == res_line, "Wrong answer! Expected " + res_line + ", got " + s);
        }
        cout << "Correct!\n";
    }
    /*_CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
    {
        _CrtMemDumpStatistics(&s3);
    }*/
}
