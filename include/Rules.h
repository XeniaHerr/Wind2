#ifndef RULES_H
#define RULES_H


#include <cmath>
#include <structs.h>
#include <string>
#include <sys/types.h>

using std::string;

//TODO: Think about how a rule is being created.

namespace Wind {

    class Rule {

        Rule(string _name, string _class, Windowtype _type);

        public:

            struct RuleContent {

            string targetTopic;

            bool forceFloat;

            bool neverFocus;


            Dimensions minSize;


            double_t relation;


            };


            int isApplicable(string name, string windowclass, Windowtype windowtype );




        private:

            string windowname;

            string windowclass;


            Windowtype type;

            u_int priority;




    };
}

#endif /*RULES_H*/
