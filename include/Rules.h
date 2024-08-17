#ifndef RULES_H
#define RULES_H


#include <cmath>
#include <structs.h>
#include <string>
#include <sys/types.h>

using std::string;

//TODO: Think about how a rule is being created.

namespace Wind {

    class RuleBuilder;

    class Rule {


        Rule();
        public:


            struct RuleContent {

            string targetTopic;

            bool forceFloat;

            bool neverFocus;


            Dimensions minSize;

            Dimensions maxSize;


            double_t relation;


            bool keepAspectratio;


            };

            RuleContent content;


            u_int isApplicable(string name, string windowclass, Windowtype windowtype );


            friend class RuleBuilder;


        private:

            string windowname;

            string windowclass;


            Windowtype type;

            u_int priority;




    };
}

#endif /*RULES_H*/
