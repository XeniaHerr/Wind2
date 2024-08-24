#include "X11_Abstraction.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>


using namespace Wind;


auto X11Abstraction::sendClientAtom(Client c, NETATOMS atom) -> void {

    if (this->netatoms.contains(atom)) {
	XChangeProperty(this->dpy, c.getWindow(), netatoms[atom], 32, XA_ATOM,PropModeReplace, (unsigned char *)&this->netatoms[atom], 1);

    }
}


auto X11Abstraction::removeClientAtom(Client c, NETATOMS atom ) -> void {

    if (this->netatoms.contains(atom)) {
	XDeleteProperty(this->dpy, c.getWindow(), atom);
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

