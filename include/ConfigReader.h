#include "Rules.h"
#include "structs.h"
#include <string>
#include <sys/types.h>
#include <vector>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>



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

            ConfigReader(ConfigReader& other) = delete;
            ConfigReader(ConfigReader&& other) = delete;

            ConfigReader& operator=(ConfigReader& other) = delete;
            ConfigReader& operator=(ConfigReader&& other) = delete;


            bool read(std::string filepath);


            std::vector<std::string> getTopicNames() const;

            u_int16_t getWindowGap() const;

            u_int16_t getBorderWidth() const;



        private:

            ConfigReader();

            YAML::Node document;



            void readTopicNames();

            void readVariables();

            void readRules();

            Windowtype readTypefromNode( YAML::Node n);


            Dimensions readDimensionsfromNode( YAML::Node n);

            Position readPositionfromNode( YAML::Node n);
    };

} /*namespace Wind*/
