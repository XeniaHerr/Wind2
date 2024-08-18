#include "structs.h"
#include <ConfigReader.h>
#include <X11/X.h>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <sys/types.h>
#include <yaml-cpp/node/parse.h>
#include <RuleBuilder.h>


using namespace Wind;

using namespace YAML;

ConfigReader::ConfigReader() : empty(true){}


auto ConfigReader::read(std::string filepath) -> bool {


    


        std::filesystem::path fp = filepath;


        if (!std::filesystem::exists(filepath))
            return false;

    document = LoadFile(filepath);


    if (document.IsNull())
        return false;


    _configs.topicnames.clear();
    _configs.rules.clear();
    _configs.borderwidth = _configs.windowgap = 0;
    _configs.testdim = Dimensions();
    _configs.testtype = Windowtype::ANYTYPE;

    readTopicNames();
    readVariables();
    readRules();

    if (document["test"].IsDefined())

        this->_configs.testdim = readDimensionsfromNode(document["test"]);

    if (document["testtype"].IsDefined())
        this->_configs.testtype = readTypefromNode(document["testtype"]);

    this->empty = false;
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



auto ConfigReader::readRules() -> void {

    Node rulenodes = document["Rules"];

    if (!rulenodes.IsDefined() || !rulenodes.IsSequence())
        return;

    for (auto n : rulenodes) {

        RuleBuilder r;

        if( n["Windowtype"].IsDefined() && n["Windowtype"].IsScalar())
            r.setWindowType(readTypefromNode(n["Windowtype"]));

        if( n["MinDimension"].IsDefined() && n["MinDimension"].IsSequence())
            r.setMinimalDimensions(readDimensionsfromNode(n["MinDimension"]));

        if( n["MaxDimension"].IsDefined() && n["MaxDimension"].IsSequence())
            r.setMaximalDimensions(readDimensionsfromNode(n["MaxDimension"]));

        if (n["Windowclass"].IsDefined() && n["Windowclass"].IsScalar())
            r.setWindowClass(n["Windowclass"].as<std::string>());

        if (n["Windowname"].IsDefined() && n["Windowname"].IsScalar())
            r.setWindowName(n["Windowname"].as<std::string>());

        if (n["Aspectratio"].IsDefined() && n["Aspectratio"].IsScalar())
            r.setAspectratio(n["Aspectratio"].as<double>());

        if (n["Forcefloat"].IsDefined() && n["Forcefloat"].IsScalar()) 
            if (n["Forcefloat"].as<bool>())
                r.setAlwaysFloat();
        if (n["LockAspectRatio"].IsScalar() && n["LockAspectRatio"].IsScalar())
            if (n["LockAspectRatio"].as<bool>())
                r.lockAspectratio();
        if (n["NeverFocus"].IsDefined() && n["NeverFocus"].IsScalar())
            if (n["NeverFocus"].as<bool>())
                r.setNonfocusable();
        if (n["DefaultTopic"].IsDefined() && n["DefaultTopic"].IsScalar()) {
            std::string tn = n["DefaultTopic"].as<std::string>();
            bool contains = false;
            for (auto a : _configs.topicnames)
                if (a == tn)
                    contains = true;
            if(!contains) {
                //TODO: Handle warning emmittion
                continue;
            }
            r.setDefaultTopic(tn);
        }
        _configs.rules.push_back(r.finish());

    }

}
