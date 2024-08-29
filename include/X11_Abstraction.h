#ifndef X11_ABSTRACTION_H
#define X11_ABSTRACTION_H
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <initializer_list>
#include <map>
#include <optional>
#include <string>
#include <unistd.h>
#include <vector>
#include "Inputstructs.h"
#include "Monitor.h"

namespace Wind {

    enum ATOMNAME : short {

	WMDelete,
	WMProtocols,
	WMTakeFocus
    };


    class X11Abstraction {


	public:


	    X11Abstraction(X11Abstraction&) = delete;
	    X11Abstraction(X11Abstraction&&) = delete;


	    X11Abstraction& operator=(X11Abstraction& other) = delete;
	    X11Abstraction& operator=(X11Abstraction&& other) = delete;



	    ~X11Abstraction();



	    static X11Abstraction& getInstance() {


		static X11Abstraction s;
		return s;



	    }

	    void setEventMask(long);

	    void update_screen();


	    void addAtom(ATOMNAME name, std::string identifier);
	    void removeAtom(ATOMNAME name);

	    bool sendEvent(Window w, ATOMNAME event);


	    XEvent getNextEvent();

	    std::optional<XWindowAttributes> getWindowAttributes(Window w);

	    void listenforKeys(std::vector<Key>);


	    void sendClientAtom(Window w, ATOMNAME atom);
	    void removeClientAtom(Window w, ATOMNAME atom);

	    void setClientProp(Window w, ATOMNAME atom, std::string val);
	    void setClientProp(Window w, ATOMNAME atom, std::vector<std::string>& val);
	    void setClientProp(Window w, ATOMNAME atom, std::vector<Window>& windows);

	    std::pair<std::string, std::string> getWindowNameClass(Window w);

	    void setAtomString(std::string val);
	    void setAtomString(std::vector<std::string>& vals);
	    bool isUsable();


	    void drawMonitor(Monitor& m);

	    void MapWindow(Window w);

	    inline void Unmapwindow(Window w) {

		//this->sendClientAtom(w, this->wmatoms[ATOMNAME::WMDelete]);

		XUnmapWindow(this->dpy, w);

		XFlush(this->dpy);


	    }



	    void CloseWindow(Window w);

	    bool checkotherWM();


	    void closeConnection() {

		if(this->dpy)
		    close(ConnectionNumber(this->dpy));
	    }




	private:

	    X11Abstraction();


	    void restack(Monitor& m);






	    Display* dpy;




	    int screen;

	    bool _usable;

	    size_t screenwidth, screenheight;

	    Window _root, _helper;

	    std::map<ATOMNAME,Atom> atoms;

    };


}
#endif /*X11_ABSTRACTION_H*/
