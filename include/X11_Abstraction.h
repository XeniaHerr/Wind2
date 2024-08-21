
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <initializer_list>
#include <map>


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






    private:

        X11Abstraction() {


            if (!(dpy = XOpenDisplay(nullptr))) {
                exit(1);
            }


            screen = DefaultScreen(dpy);

            screenwidth = DisplayWidth(dpy, screen);

            screenheight = DisplayHeight(dpy, screen);


        }


        Display* dpy;

        int screen;

        size_t screenwidth, screenheight;


        std::map<WMATOMS,Atom> wmatoms;

        std::map<NETATOMS, Atom> netatoms;

};
