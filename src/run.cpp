#include <X11/X.h>
#include <X11/Xlib.h>
#include <run.h>
#include "Action.h"
#include "Logger.h"
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

	auto& Log = Logger::GetInstance();

	Log.Info("Got next event is of type {}", (e.type == KeyPress ? "Keypressevent" : "other"));

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


Run::~Run() {
    for (auto a: _handlers)
	delete a;
}
