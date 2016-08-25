#include "Atom.h"

Atom::Atom()
: moveable_(false)
{
    pos_ = std::make_pair(0, 0);
}

void Atom::setMoveable(bool moveable) 
{ 
    this->moveable_ = moveable;
}
bool Atom::isMoveable() const 
{ 
    return moveable_; 
}
