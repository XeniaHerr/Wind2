#include "structs.h"
#include <Rules.h>
#include <cmath>




namespace Wind {

    class RuleBuilder {


        public:

            RuleBuilder();

            Rule finish();


            RuleBuilder& setWindowClass(string _class);

            RuleBuilder& setWindowName(string _name);

            RuleBuilder& setWindowType(Windowtype ty);

            RuleBuilder& setAspectratio(double_t ratio);

            RuleBuilder& lockAspectratio();

            RuleBuilder& setMinimalDimensions(Dimensions _min);

            RuleBuilder& setMaximalDimensions(Dimensions _max);

            RuleBuilder& setNonfocusable();

            RuleBuilder& setAlwaysFloat();

            RuleBuilder& setDefaultTopic(string topicname);



        private:

            Rule _object;
    };
}
