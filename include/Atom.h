#ifndef _ATOM_H_
#define _ATOM_H_

#include <utility>

class Atom
{
public:
    Atom();
    void setMoveable(bool moveable); 
    bool isMoveable() const; 
private:
    std::pair<int, int> pos_;
    std::pair<int, int> dimension_;
    bool moveable_;
};

#endif
