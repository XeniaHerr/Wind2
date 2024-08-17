#include "AbstractArranger.h"
#include "ConcreteArranger.h"
#include <ArrangerSelector.h>


using namespace Wind;


ArrangerSelector::ArrangerSelector() : _index(0) {
    _arrangers.push_back(  new Monocle());
    _arrangers.push_back( new MasterTile());
}

ArrangerSelector::~ArrangerSelector() {

    for (auto a : _arrangers)
        delete a;
}

auto ArrangerSelector::getArranger() -> AbstractArranger& {

    return *_arrangers[_index];

}



auto ArrangerSelector::next() -> AbstractArranger& {
    _index = ++_index % _arrangers.size();

    return getArranger();
}



auto ArrangerSelector::previous() -> AbstractArranger& {

    _index = --_index % _arrangers.size();

    return getArranger();
}


auto ArrangerSelector::operator++() -> AbstractArranger& {
    return this->next();
}


auto ArrangerSelector::operator--() -> AbstractArranger& {
    return this->previous();
}
