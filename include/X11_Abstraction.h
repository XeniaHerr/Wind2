
#include <X11/X.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <initializer_list>
#include <map>


enum WMATOMS : short;
enum NETATOMS : short;


class X11_Abstraction {


    public:


        X11_Abstraction(X11_Abstraction&) = delete;



        ~X11_Abstraction();



        static X11_Abstraction& get_instance() {


            static X11_Abstraction s;
            return s;



        }


        void update_screen();


        void updateWMAtoms(const std::initializer_list<std::pair<WMATOMS, Atom>>& l);
        void updateNetAtoms(const std::initializer_list<std::pair<NETATOMS, Atom>>& l);



    private:

        X11_Abstraction() {


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
