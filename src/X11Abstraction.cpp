#include "Logger.h"
#include "WindowManagerModel.h"
#include "X11_Abstraction.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <Inputstructs.h>
#include <bits/ranges_algo.h>
#include <optional>
#include <Monitor.h>
#include <Topic.h>
#include <Client.h>
#include <list>
#include <string>
#include <utility>
#include <cassert>


using namespace Wind;

static int (*xdefaulterror)(Display *d, XErrorEvent* ev);

bool _errorvar  = false;
int _xerrorstart(Display* d, XErrorEvent* er) {
    _errorvar = true;
    return -1;}


int _xdummyerror(Display* d, XErrorEvent* er) {

    return 0;
}

int _xdefaulerror(Display*d , XErrorEvent*er) {

    auto& Log = Logger::GetInstance();

    Log.Warn("X11 Error occured!");

    return xdefaulterror(d,er);
}


auto X11Abstraction::sendEvent(Window w, ATOMNAME atom) -> bool {


    XEvent e;

    Atom* a;
    int count = 0;

    XGetWMProtocols(this->dpy, w, &a, &count);
    bool has_atom = false;

    for (int i = 0; i < count; i++) {
	if (a[i] == atoms[atom])
	    has_atom = true;
    }

    XFree(a);

    if (has_atom) {
    e.type = ClientMessage;
    e.xclient.window = w;
    e.xclient.data.l[0] = atom;
    e.xclient.data.l[1] = CurrentTime;
    e.xclient.format = 32;
    e.xclient.message_type = this->atoms[ATOMNAME::WMProtocols];


    XSendEvent(this->dpy, w, false, NoEventMask, &e);
    }

    Logger::GetInstance().Info("Event send {}", has_atom ? "Sucsessfully" : "Unsucessfully" );

    return has_atom;

};

auto X11Abstraction::sendClientAtom(Window w, ATOMNAME atom) -> void {

    if (this->atoms.contains(atom)) {
	XChangeProperty(this->dpy, w, this->atoms[atom] , 32, XA_ATOM,PropModeReplace, (unsigned char *)&this->atoms[atom], 1);

    }
}


auto X11Abstraction::setClientState(Window w, ATOMNAME state) -> void {


	XChangeProperty(this->dpy, w, this->atoms[ATOMNAME::WindowState] , 32, XA_ATOM,PropModeReplace, (unsigned char *)&this->atoms[state], 1);
}


auto X11Abstraction::removeClientAtom(Window w, ATOMNAME atom ) -> void {

    if (this->atoms.contains(atom)) {
	XDeleteProperty(this->dpy, w, atom); }
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

	    xdefaulterror = XSetErrorHandler(nullptr);
	    XSetErrorHandler(xdefaulterror);

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

    this->hideTopic(m.getCurrent());

    for (auto a : m.getCurrent()->getStack()) {

	XWindowChanges wc;
	wc.x = a->getPosition().x;
	wc.y = a->getPosition().y;
	wc.width = a->getCurrentDimensions().width;
	wc.height = a->getCurrentDimensions().height;
	XConfigureWindow(this->dpy, a->getWindow(), CWX|CWY|CWWidth|CWHeight, &wc);
    }


    Log.Info("Done calling XMoveResizeWindow");
    auto c = m.getCurrent()->getStack();
    for(auto a = c.rbegin(); a != c.rend(); a++){

	XRaiseWindow(this->dpy, (*a)->getWindow());
	setClientState((*a)->getWindow(), ATOMNAME::WindowNormalState);
	Log.Info("Mapped client {}", (*a)->getWindow());
    }



    XSync(this->dpy, false);

    Log.Info("Synced display");

    this->restack(m);

}


auto X11Abstraction::restack(Monitor&m) -> void {

    auto& Log = Logger::GetInstance();
    Log.Info("Inside restack");

    XWindowChanges wc;

    auto& clients = m.getCurrent()->getStack();

    if (clients.empty()) {
	Log.Info("No clients to restack");
	return;
    }
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




auto X11Abstraction::addAtom(ATOMNAME atom, std::string id) -> void {

    if (!this->atoms.contains(atom))
	this->atoms.insert(std::make_pair(atom,
		    XInternAtom(this->dpy, id.c_str(), false)));
}


auto X11Abstraction::setfocus(Client *c) -> void {

    auto& Log = Logger::GetInstance();
    Log.Info("Inside setfocus of the X11Abstraction");
    if (this->_active != this->_root) {
	// if focusing another client, unfocus it first;
	Log.Info("Another Window is currently focused, removing focus first");
	XSetInputFocus(this->dpy, this->_root, RevertToPointerRoot, CurrentTime);

	this->removeClientAtom(_root, ATOMNAME::NetActiveWindow);
	XSetWindowBorder(this->dpy, this->_active, this->passiveColor.pixel);
    }
    Window w;
    if (c) {
	Log.Info("Found Client to set focus to, sending event");
	w = c->getWindow();	

	this->_active = w;
    this->sendEvent(w, ATOMNAME::WMTakeFocus);
    XSetInputFocus(this->dpy, this->_active, RevertToPointerRoot, CurrentTime);
    XChangeProperty(this->dpy, this->_root,atoms[ATOMNAME::NetActiveWindow],XA_ATOM, 32, PropModeReplace, (const unsigned char*)&w, 1);
	XSetWindowBorder(this->dpy, this->_active, this->activeColor.pixel);
    }
    else {
	this->_active  = this->_root;

    XSetInputFocus(this->dpy, this->_root, RevertToPointerRoot, CurrentTime);
    }
Log.Warn("The currently focused client on X side is {}", this->_active);
}


auto X11Abstraction::initAtoms() -> void {

const Atom supported[] = { atoms[ATOMNAME::NetDesktopNumber],
			    atoms[ATOMNAME::NetClientList],
			    atoms[ATOMNAME::NetDesktopNames],
			    atoms[ATOMNAME::NetCurrentDesktop]};



XDeleteProperty(this->dpy, this->_root, atoms[ATOMNAME::NetDesktopNames]);
    XChangeProperty(this->dpy, this->_root, atoms[ATOMNAME::NetSupported], XA_ATOM, 32, PropModeReplace, (const unsigned char*)supported, sizeof(supported)/ sizeof (supported[0]));

    const unsigned char name[] = "Wind";
    XChangeProperty(this->dpy, this->_root, atoms[ATOMNAME::NetWMName], XA_STRING,8, PropModeReplace, (const unsigned char*) "Wind", 4);

    unsigned long dcount = WindowManagerModel::getInstance().getTopicCount();

    XChangeProperty(this->dpy, this->_root, atoms[ATOMNAME::NetDesktopNumber], XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &dcount, 1);

//	topicNames += (WindowManagerModel::getInstance().getTopic(i)->getName() + "\0");
    std::string topicNames;
    for (int i = 0; i < WindowManagerModel::getInstance().getTopicCount(); i++ )
	XChangeProperty(this->dpy, this->_root,atoms[ATOMNAME::NetDesktopNames],atoms[ATOMNAME::UTF8String], 8, PropModeAppend, (unsigned char*)WindowManagerModel::getInstance().getTopic(i)->getName().c_str(), WindowManagerModel::getInstance().getTopic(i)->getName().length()+1);






}


auto X11Abstraction::prependClientList(Window w) -> void {
    Logger::GetInstance().Info("Inside {} with w = {}" ,__func__, w);

    assert(this->isUsable());
    XChangeProperty(this->dpy, this->_root, atoms[ATOMNAME::NetClientList], XA_WINDOW, 32, PropModeAppend, (unsigned char *)&w, 1);
    Logger::GetInstance().Info("client added to focus list");
}

auto X11Abstraction::updateClientList() -> void {

    auto cl = WindowManagerModel::getInstance().getWindows();

    XChangeProperty(this->dpy, this->_root, atoms[ATOMNAME::NetClientList], XA_WINDOW, 32, PropModeReplace, (unsigned char*) cl.data(), cl.size());



}

auto X11Abstraction::subscribetoWindow(Window w, long flags) -> void {

    XSelectInput(this->dpy, w, flags);
}


auto X11Abstraction::createColor(std::string cc) -> XftColor {



    XftColor c;

    XftColorAllocName(this->dpy, DefaultVisual(this->dpy, this->screen), DefaultColormap(this->dpy, this->screen), cc.c_str(), &c);

    
    return c;
}



void X11Abstraction::setactiveColor(std::string s) {


    this->activeColor = this->createColor(s);

}
void X11Abstraction::setpassiveColor(std::string s) {

    this->passiveColor = this->createColor(s);

}
void X11Abstraction::seturgentColor(std::string s) {

    this->urgentColor = this->createColor(s);

}


auto X11Abstraction::configureClient(Client *c) -> void {

    XConfigureEvent e;


    e.type = ClientMessage;
    e.window = c->getWindow();
    e.send_event = true;
    e.x = c->getPosition().x;
    e.y = c->getPosition().y;
    e.width = c->getCurrentDimensions().width;
    e.height = c->getCurrentDimensions().height;
    e.border_width = WindowManagerModel::getInstance().getBorderwidth();
    

    XSendEvent(this->dpy, c->getWindow(), False, SubstructureNotifyMask, (XEvent*) &e);
}


auto X11Abstraction::ignoreErrors() -> void {


    XSetErrorHandler(_xdummyerror);
}



auto X11Abstraction::acivateErrors() -> void {

    XSetErrorHandler(_xdefaulerror);
}


auto X11Abstraction::hideTopic(Topic* t) -> void {

    for (auto a : t->getClients()) {
	this->setClientState(a->getWindow(), ATOMNAME::WindowIconicState);
	Logger::GetInstance().Info("Hidden client {}", a->getWindow());
	XMoveWindow(this->dpy, a->getWindow(), a->getPosition().x - this->screenwidth, a->getPosition().y - this->screenheight);
    }

}

auto X11Abstraction::hideClient(Client& c) -> void {

	this->setClientState(c.getWindow(), ATOMNAME::WindowIconicState);
	Logger::GetInstance().Info("Hidden client {}", c.getWindow());
	XMoveWindow(this->dpy, c.getWindow(), c.getPosition().x - this->screenwidth, c.getPosition().y - this->screenheight);

}


auto X11Abstraction::showWindow(Window w) -> void {
    this->setClientState(w, ATOMNAME::WindowNormalState);
    XMapWindow(this->dpy, w);
    Logger::GetInstance().Info("Mapped Client {} from showWindow", w);
} 


auto X11Abstraction::getWindowType(Window w) -> Windowtype {

    auto& Log = Logger::GetInstance();
    Atom ret, dummy;

    unsigned long u, u2 ;
    int i;
    

    unsigned char* ret_p = nullptr;


    if (Success != XGetWindowProperty(this->dpy, w, atoms[ATOMNAME::WindowType], 0L, sizeof ret, false, XA_ATOM, &dummy, &i , &u, &u2, &ret_p) || !ret_p)
	return Windowtype::NORMAL;

    Log.Info("Got a type");

    ret = *(Atom*)ret_p;


    if (ret == atoms[ATOMNAME::WindowTypeNormal])
	return Windowtype::NORMAL;
    else if (ret == atoms[ATOMNAME::WindowTypeDialog])
	return Windowtype::DIALOG;
    else if (ret == atoms[ATOMNAME::WindowTypePanel])
	return Windowtype::PANEL;
    else if (ret == atoms[ATOMNAME::WindowTypeDock])
	return Windowtype::DOCK;
    else if (ret == atoms[ATOMNAME::WindowTypeSplash])
	return Windowtype::SPLASH;


    Log.Info("Type fell through");
    return Windowtype::NORMAL;

}


auto X11Abstraction::getAtom(ATOMNAME name) -> Atom {

    if (atoms[name])
	return atoms[name];
    else
	return 0;


}


auto X11Abstraction::drawClient(Client& c) -> void {


    this->hideClient(c);
	XWindowChanges wc;
	wc.x = c.getPosition().x;
	wc.y = c.getPosition().y;
	wc.width = c.getCurrentDimensions().width;
	wc.height = c.getCurrentDimensions().height;
	XConfigureWindow(this->dpy, c.getWindow(), CWX|CWY|CWWidth|CWHeight, &wc);

	if (c.isFloating() || c.isFullscreen())
	    XRaiseWindow(this->dpy, c.getWindow());

	this->showWindow(c.getWindow());
}

