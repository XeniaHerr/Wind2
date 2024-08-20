#include "Rules.h"
#include "structs.h"
#include <string>
#include <sys/types.h>
#include <vector>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <Action.h>
#include <Inputstructs.h>



namespace Wind {

    struct Config;


    class ConfigReader {


        public:

            struct Config {

                std::vector<std::string> topicnames;

                u_int16_t windowgap;

                u_int16_t borderwidth;


                std::vector<Rule> rules;


                Dimensions testdim;

                Windowtype testtype;
            };
            static ConfigReader& getInstance() {

                static ConfigReader s;
                return s;
            }

            Config _configs;

            bool empty;

            ConfigReader(ConfigReader& other) = delete;
            ConfigReader(ConfigReader&& other) = delete;

            ConfigReader& operator=(ConfigReader& other) = delete;
            ConfigReader& operator=(ConfigReader&& other) = delete;


            bool read(std::string filepath);


            std::vector<std::string> getTopicNames() const;

            u_int16_t getWindowGap() const;

            u_int16_t getBorderWidth() const;


            std::map<Key, Action> localkeys;



        private:

            ConfigReader();

            YAML::Node document;



            void readTopicNames();

            void readVariables();

            void readRules();


            void readKeys();

            Windowtype readTypefromNode( YAML::Node n);


            Dimensions readDimensionsfromNode( YAML::Node n);

            Position readPositionfromNode( YAML::Node n);
    };

} /*namespace Wind*/




//namespace YAML {
//
//    template<>
//        struct convert<Wind::Action::Argument> {
//
//
//            static Node encode(const Wind::Action::Argument& rhs) {
//                Node node;
//
//                switch (rhs.index()) {
//                    case 1:
//                        std::string s = rhs;
//                        node.push_back(static_cast<unsinged long>(rhs));
//                }
//            }
//
//        };
//}
