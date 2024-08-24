#include <X11/X.h>
#include <run.h>
#include "Action.h"
#include "X11_Abstraction.h"


using namespace Wind;

auto Run::getInstance() -> Run& {
    static Run r;
    return r;
}


auto Run::stop() -> void {
    this->_is_running = false;
}


auto Run::startloop() -> void {

    while(_is_running) {

	XEvent e = X11Abstraction::getInstance().getNextEvent();

	if (_handlers[e.type]) {

	    _handlers[e.type]->setArgument(&e);

	    _handlers[e.type]->execute();

	}

    }
}


auto Run::setHandler(unsigned index, Action* handler) -> void {

    if (index < LASTEvent)
	_handlers[index] = handler;
}
