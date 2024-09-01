#include "Action.h"
#include <X11/X.h>



namespace Wind {

class keyHandlerAction : public Action {

    public: 
	std::string name() override;

    void execute() override;

    std::unique_ptr<Action> clone() override;



    bool wantArgument() override;


    void operator()() override;


    ~keyHandlerAction() override {}
};


class ManageRequestAction : public Action {

    public:

	void execute() override;




	~ManageRequestAction() override {};


};


class UnmanageRequestAction : public Action {

    public:
	void execute() override;


	~UnmanageRequestAction() override {};
};


class EnterNotifyAction : public Action {

    public:
	void execute() override;

	~EnterNotifyAction() {}
};


class DestroyNotifyAction : public Action {

    public:
	void execute() override;

	~DestroyNotifyAction() {}
};


class MappingNotifyAction : public Action {

    public:
	void execute() override;

	~MappingNotifyAction() {}
};


class ConfigureRequestAction: public Action {

    public:
	void execute() override;

	~ConfigureRequestAction() {}
};
}
