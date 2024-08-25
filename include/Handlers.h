#include "Action.h"
#include <algorithm>



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
	std::string name() override;

	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~ManageRequestAction() override;


};


class UnmanageRequestAction : public Action {

    public:
	std::string name() override;
	void execute() override;

	std::unique_ptr<Action> clone() override;

	bool wantArgument() override;

	void operator()() override;

	~UnmanageRequestAction() override;
};
}
