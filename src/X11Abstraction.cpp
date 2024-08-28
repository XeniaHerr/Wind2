#include "Logger.h"
#include "X11_Abstraction.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <Inputstructs.h>
#include <bits/ranges_algo.h>
#include <optional>
#include <Monitor.h>
#include <Topic.h>
#include <Client.h>
#include <list>


using namespace Wind;


bool _errorvar  = false;
int _xerrorstart(Display* d, XErrorEvent* er) {
    _errorvar = true;
    return -1;}



auto X11Abstraction::sendClientAtom(Window w, NETATOMS atom) -> void {

    if (this->netatoms.contains(atom)) {
	XChangeProperty(this->dpy, w, netatoms[atom], 32, XA_ATOM,PropModeReplace, (unsigned char *)&this->netatoms[atom], 1);

    }
}


auto X11Abstraction::removeClientAtom(Window w, NETATOMS atom ) -> void {

    if (this->netatoms.contains(atom)) {
	XDeleteProperty(this->dpy, w, atom);
    }
}


auto X11Abstraction::getWindowNameClass(Window w) -> std::pair<std::string, std::string> {



    XClassHint h;

    XGetClassHint(this->dpy, w, &h);

    std::pair<std::string, std::string> ret;

    ret.first = h.res_name;

    ret.second = h.res_class;
    if (h.res_name)
	XFree(h.res_name);
    if(h.res_class)
	XFree(h.res_class);


    return ret;
}


auto X11Abstraction::getNextEvent() -> XEvent {
    XEvent event;
    //Maybe syning before?

    XNextEvent(this->dpy, &event);
    return event;
}


//This is "inspired" by the way dwm handles the other wm check
auto X11Abstraction::checkotherWM() -> bool {
    XSetErrorHandler(_xerrorstart);

    XSelectInput(this->dpy, DefaultRootWindow(dpy), SubstructureRedirectMask);

    XSync(dpy, false);

    // Set the real error handler back;
    XSync(dpy, false);
    return _errorvar;
}

auto X11Abstraction::isUsable() -> bool {
    return this->_usable;
}

X11Abstraction::X11Abstraction() {

            if (!(dpy = XOpenDisplay(nullptr))) {
		_usable = false;
	    return;
	    }

	    _usable = true;

            screen = DefaultScreen(dpy);

            screenwidth = DisplayWidth(dpy, screen);

            screenheight = DisplayHeight(dpy, screen);

	    _root = RootWindow(this->dpy, screen);

	    }


X11Abstraction::~X11Abstraction() {

    if (_usable)
    XCloseDisplay(dpy);
}

auto X11Abstraction::setEventMask(long mask) -> void {

    XSetWindowAttributes wa;

    wa.event_mask = mask;

    XChangeWindowAttributes(this->dpy, this->_root, CWCursor|CWEventMask, &wa);

    XSelectInput(this->dpy, this->_root, wa.event_mask);
}

auto X11Abstraction::listenforKeys(std::vector<Key> keys) -> void {

    XUngrabKey(this->dpy, AnyKey, AnyModifier, this->_root);

    Logger::GetInstance().Info("Listening for {} keys", keys.size());

    int start, end, skip;
    XDisplayKeycodes(this->dpy, &start, &end);
    KeySym* syms;

    syms = XGetKeyboardMapping(this->dpy, start, end -start +1, &skip);

    auto containskeysym = [&keys](KeySym s) {
	for(auto& a : keys) {
	    if (a.keysym == s)
		return true;
	}
	    return false;
    };

    for (int k = start; k <= end; k++) {
	for (Key key : keys) 
	if (key.keysym == (syms[(k- start) * skip ]))
	    for ( unsigned mod : { 0, LockMask}) {
		XGrabKey(this->dpy, k, key.modifier|mod, this->_root, true , GrabModeAsync, GrabModeAsync);
		Logger::GetInstance().Info("Listening for Key {} with modifiers {}", k, key.modifier|mod);
	    }

    }



    delete syms;

}



auto X11Abstraction::getWindowAttributes(Window w) -> std::optional<XWindowAttributes> {



    XWindowAttributes wa;

    if (!XGetWindowAttributes(this->dpy, w, &wa))
	return std::nullopt;

    return std::make_optional(wa);

}


auto X11Abstraction::drawMonitor(Monitor& m) -> void {

    auto& Log = Logger::GetInstance();

    for (auto a : m.getCurrent()->getClients()) {

	//XMoveResizeWindow(this->dpy, a->getWindow() , a->getPosition().x,
	//	a->getPosition().y, a->getCurrentDimensions().width, a->getCurrentDimensions().height);
	XWindowChanges wc;
	wc.x = a->getPosition().x;
	wc.y = a->getPosition().y;
	wc.width = a->getCurrentDimensions().width;
	wc.height = a->getCurrentDimensions().height;
	XConfigureWindow(this->dpy, a->getWindow(), CWX|CWY|CWWidth|CWHeight, &wc);
    }


    Log.Info("Done calling XMoveResizeWindow");
    auto c = m.getCurrent()->getClients();
    for(auto a = c.rbegin(); a != c.rend(); a++){

	XRaiseWindow(this->dpy, (*a)->getWindow());
	XMapWindow(this->dpy, (*a)->getWindow());
    }



    XSync(this->dpy, false);

    Log.Info("Synced display");

    this->restack(m);

}


auto X11Abstraction::restack(Monitor&m) -> void {

    auto& Log = Logger::GetInstance();
    Log.Info("Inside restack");

    XWindowChanges wc;

    auto& clients = m.getCurrent()->getClients();
    auto c = clients.begin();

    wc.sibling = (*c)->getWindow();


    while(c != clients.end()) {

	XConfigureWindow(this->dpy, (*c)->getWindow(), CWSibling, &wc);

	wc.sibling = (*c)->getWindow();

	c++;
	Log.Info("Iteration done");


    }

    XSync(this->dpy, false);

    XEvent e;
    while(XCheckMaskEvent(this->dpy, EnterWindowMask, &e));


    Log.Info("Done restacking");





}
