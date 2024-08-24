
#include "Client.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace Wind {

enum WMATOMS : short;
enum NETATOMS : short;


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


        void update_screen();


        void updateWMAtoms(const std::initializer_list<std::pair<WMATOMS, Atom>>& l);
        void updateNetAtoms(const std::initializer_list<std::pair<NETATOMS, Atom>>& l);


        XEvent getNextEvent();


	void sendClientAtom(Client c, NETATOMS atom);
	void sendClientAtom(Client c, WMATOMS atom);
	void removeClientAtom(Client c, NETATOMS atom);

	void setClientProp(Client c, NETATOMS atom, std::string val);
	void setClientProp(Client c, NETATOMS atom, std::vector<std::string>& val);
	void setClientProp(Client c, NETATOMS atom, std::vector<Window>& windows);

	std::pair<std::string, std::string> getWindowNameClass(Window w);

	void setAtomString(std::string val);
	void setAtomString(std::vector<std::string>& vals);
	bool isUsable();

	bool checkotherWM();




    private:

        X11Abstraction() {


            if (!(dpy = XOpenDisplay(nullptr))) {
		_usable = false;
	    return;
            }

	    _usable = true;

            screen = DefaultScreen(dpy);

            screenwidth = DisplayWidth(dpy, screen);

            screenheight = DisplayHeight(dpy, screen);


        }





        Display* dpy;


	

        int screen;

	bool _usable;

        size_t screenwidth, screenheight;

	Window _root, _helper;


        std::map<WMATOMS,Atom> wmatoms;

        std::map<NETATOMS, Atom> netatoms;

};


}
