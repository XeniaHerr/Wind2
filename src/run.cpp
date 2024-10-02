#include <X11/X.h>
#include <X11/Xlib.h>
#include <run.h>
#include "Action.h"
#include "Logger.h"
#include "X11_Abstraction.h"


using namespace Wind;


/**
 * What events should i handle
 *---------------------------
 * I already have
 * --------------------------
 *  KeyPress: When one of the buttons i am listening for is pressed
 *  MapRequest: A Window wants to be mapped(on creation)
 *  EnterNotify: We are entering a window, change focus?
 *  UnmapNotify: Window stops being mapped, ensure window gets destroyed
 *  DestroyNotify: Window is destroyed, stop bookkeeping
 *
 *  ------------------------
 *  What i should implement
 *  ------------------------
 *  ButtonPress: I want mouse support
 *  ClientMessage: Clients should be able to give me commands
 *  FocusIn: Update Focus (reset it)
 *  MappingNotify: Changes in the Keyboard. Rerun grabkeys if nececcary
 *  ConfigureRequest: Clients may sometimes govern their own appearance
 *  ConfigureNotify: When the Configuration (Mostly Monitors) changes
 *
 *  -----------------------
 *  What i have no idea about
 *  -----------------------
 *  PropertyNotify: No idea what that does
 *  Exposure: Should not be needed, as i am not drawing myself
 *  
 * */

std::string MasktoString(long mask) {

    switch(mask) {

	case KeyPress:
	    return "Keypress";
	case UnmapNotify:
	    return "UnmapNotify";
	case MappingNotify:
	    return "MappingNotify";
	case DestroyNotify:
	    return "DestroyNotify";
	case ClientMessage:
	    return "ClientMessage";
	case MapRequest:
	    return "MapRequest";
	case EnterNotify:
	    return "EnterNotify";
	case ConfigureNotify:
	    return "ConfigureNotify";
	case ConfigureRequest:
	    return "ConfigureRequest";
	case FocusIn:
	    return "FocusIn";
	case PropertyNotify:
	    return "PropertyNotify";
	case LeaveNotify:
	    return "LeaveNotify";
    default:
	    return "Other";
    }
}

/*auto Run::getInstance() -> Run& {
    static Run r;
    return r;
}
*/


auto Run::stop() -> void {
    this->_is_running = false;
}


auto Run::startloop() -> void {

    while(RunningIndicator::getInstance()._var) {

	XEvent e = X11Abstraction::getInstance().getNextEvent();

	auto& Log = Logger::GetInstance();

	Log.Info("Got next event is of type {}", MasktoString(e.type));

	if (_handlers[e.type]) {
	    Log.Info("Found handler");

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
