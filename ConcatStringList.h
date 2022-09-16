#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

class charArrayList {
private:
    int maxSize;
    int size;
    int currentElement;
    string * list;

public:
    charArrayList(int size = 256){
        maxSize = size;
        size = currentElement = 0;
        list = new string[maxSize];
    }
    ~charArrayList(){
        delete [] list;
    }
    void clear(){
        delete [] list;
        size = currentElement = 0;
        list = new string[maxSize];
    }
    void toStart(){
        currentElement = 0;
    }
    void toEnd(){ 
        currentElement = size;
    }
    void next(){
        if (currentElement < size) currentElement++;
    }
    void prev(){
        if(currentElement != 0) currentElement--;
    }
    int length() const{
        return size;
    }
    int currentPosition() const{
        return currentElement;
    }
    void moveToPosition(int pos){
        if ((pos >= 0) && (pos <= size)) {
            cout << "Out of range";
            return;
        }
        else currentElement = pos;
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

public:

    ConcatStringList(const char *){
        
    };
    // int length() const{
    //     return listSize;
    // };
    int get(int index) const;
    int indexOf(char c) const;
    std::string toString() const;
    ConcatStringList concat(const ConcatStringList & otherS) const;
    ConcatStringList subString(int from, int to) const;
    ConcatStringList reverse() const;
    ~ConcatStringList();

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