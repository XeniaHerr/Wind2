#ifndef ACTION_H
#define ACTION_H

#include <X11/Xlib.h>
#include <functional>
#include <variant>
#include <string>



namespace Wind {
class Action {

    public:
    using Argument = std::variant<std::string, unsigned long, int, XEvent*>;

    private:
    Argument Arg;
        std::function<void(decltype(Arg))> _f;

        bool needs_argument;




    public:


        Action() : Arg(0), _f(), needs_argument(false) {}
        Action(decltype(_f) f, decltype(Arg) a, bool n ) : _f(f), Arg(a), needs_argument(n) {}
        void operator()();

        void setAction(decltype(_f));

        void setArgument(decltype(Arg));


        bool wantArgument();


        std::size_t index() {
            return Arg.index();
        }





};
}
#endif /*ACTION_H*/
