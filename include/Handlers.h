#include "Action.h"



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
}
