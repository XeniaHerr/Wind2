#include "structs.h"
#include <ConfigReader.h>
#include <X11/X.h>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <sys/types.h>
#include <yaml-cpp/node/parse.h>


using namespace Wind;

using namespace YAML;

ConfigReader::ConfigReader(){}


auto ConfigReader::read(std::string filepath) -> bool {


    


        std::filesystem::path fp = filepath;


        if (!std::filesystem::exists(filepath))
            return false;

    document = LoadFile(filepath);


    if (document.IsNull())
        return false;

    readTopicNames();
    readVariables();

    if (document["test"].IsDefined())

        this->_configs.testdim = readDimensionsfromNode(document["test"]);

    if (document["testtype"].IsDefined())
        this->_configs.testtype = readTypefromNode(document["testtype"]);

    return true;
}


auto ConfigReader::readTopicNames() -> void {

    Node node_topicnames = document["topicnames"];

    if (node_topicnames.IsDefined() || !node_topicnames.IsSequence()) {
        for (int i = 0; i < node_topicnames.size(); i++)
            this->_configs.topicnames.push_back(node_topicnames[i].as<std::string>());
    }
}


auto ConfigReader::readVariables() -> void {



    Node node_borderwidth = document["borderwidth"];

    if (!node_borderwidth.IsDefined() || !node_borderwidth.IsScalar())
       this->_configs.borderwidth = 0;
    else
        this->_configs.borderwidth = node_borderwidth.as<u_int16_t>();

    
    Node node_windowgap = document["windowgap"];

    if(!node_windowgap.IsDefined() || !node_windowgap.IsScalar())
        this->_configs.windowgap = 0;
    else
        this->_configs.windowgap = node_windowgap.as<u_int16_t>();
}


auto ConfigReader::readTypefromNode( Node n) -> Windowtype {

    if (!n.IsDefined() || !n.IsScalar())
        return Windowtype::ANYTYPE;


    std::string str = n.as<std::string>();
    std::transform(str.begin(), str.end(),str.begin(),::toupper);


    if (str == "WIDGET")
        return Windowtype::WIDGET;
    if(str == "DOCK")
        return Windowtype::DOCK;
    if(str == "BAR")
        return Windowtype::BAR;
    if(str == "SPLASH")
        return Windowtype::SPLASH;
    if (str == "DIALOG")
        return Windowtype::DIALOG;


    return Windowtype::ANYTYPE;
}


auto ConfigReader::readDimensionsfromNode( Node n) -> Dimensions {

    Dimensions r;


    if (n.IsDefined() && n.IsSequence()) {

        r.width = n[0].as<u_int32_t>();
        r.height= n[1].as<u_int32_t>();
    }


    return r;
}
