
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <initializer_list>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include "Inputstructs.h"

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

	void setEventMask(long);

        void update_screen();


        void updateWMAtoms(const std::initializer_list<std::pair<WMATOMS, Atom>>& l);
        void updateNetAtoms(const std::initializer_list<std::pair<NETATOMS, Atom>>& l);


        XEvent getNextEvent();

	std::optional<XWindowAttributes> getWindowAttributes(Window w);

	void listenforKeys(std::vector<Key>);


	void sendClientAtom(Window w, NETATOMS atom);
	void sendClientAtom(Window w, WMATOMS atom);
	void removeClientAtom(Window w, NETATOMS atom);

	void setClientProp(Window w, NETATOMS atom, std::string val);
	void setClientProp(Window w, NETATOMS atom, std::vector<std::string>& val);
	void setClientProp(Window w, NETATOMS atom, std::vector<Window>& windows);

	std::pair<std::string, std::string> getWindowNameClass(Window w);

	void setAtomString(std::string val);
	void setAtomString(std::vector<std::string>& vals);
	bool isUsable();

	bool checkotherWM();




    private:

        X11Abstraction();









        Display* dpy;


	

        int screen;

	bool _usable;

        size_t screenwidth, screenheight;

	Window _root, _helper;


        std::map<WMATOMS,Atom> wmatoms;

        std::map<NETATOMS, Atom> netatoms;

};


}
