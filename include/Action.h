#ifndef ACTION_H
#define ACTION_H

#include <X11/Xlib.h>
#include <memory>
#include <variant>
#include <string>



namespace Wind {
class Action {

    public:
    using Argument = std::variant<std::string, unsigned long, int, XEvent*>;

    private:

        bool needs_argument;




    public:

    Argument Arg;

        virtual void operator()();


        void setArgument(decltype(Arg));


        virtual void execute() = 0;


        virtual std::unique_ptr<Action> clone();


        virtual bool wantArgument();


        std::size_t index() {
            return Arg.index();
        }

        virtual std::string name();

        virtual ~Action();







};
}
#endif /*ACTION_H*/
