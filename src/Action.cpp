#include <Action.h>


using namespace Wind;


auto Action::setArgument(decltype(Arg) a) -> void {
    Arg = a;

}


auto Action::setAction(decltype(_f) f) -> void {
    _f = f;
}


void Action::operator()() {
    if(_f) {
        _f(Arg);
    }
}


auto Action::wantArgument() -> bool {
    return this->needs_argument;
}
