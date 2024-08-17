#include <ConfigReader.h>
#include <sys/types.h>
#include <yaml-cpp/node/parse.h>


using namespace Wind;


ConfigReader::ConfigReader(): _parser() {}


auto ConfigReader::read(std::string filepath) -> bool {


    using namespace YAML;
    


    Node document =LoadFile(filepath);


    if (document.IsNull())
        return false;

    Node node_borderwidth = document["borderwidth"];

    if (node_borderwidth.IsNull())
       this->_configs.borderwidth = 0;
    else
        this->_configs.borderwidth = node_borderwidth.as<u_int16_t>();

    return true;
}
