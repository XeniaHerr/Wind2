#include <Action.h>


using namespace Wind;


auto Action::wantArgument() -> bool {
    return this->needs_argument;
}


auto Action::setArgument(Argument a) -> void {
    this->Arg = a;
}



auto Action::name() -> std::string {
    return "";
}

Action::~Action() {}


auto Action::operator()() -> void {

}

auto Action::clone() -> std::unique_ptr<Action> {

    return nullptr;
}
