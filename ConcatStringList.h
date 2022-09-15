#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"

class charArrayList {
private:
    int maxSize;
    int currentSize;
    int currentElement;
    string * list;

public:
    charArrayList(int size = 256){
        maxSize = size;
        currentSize = currentElement = 0;
        list = new string[maxSize];
    }
    ~charArrayList(){
        delete [] list;
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

    int currentNode;
    int maxSize;
    int listSize;


public:

    ConcatStringList(const char *){
        
    };
    int length() const{
        return listSize;
    };
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