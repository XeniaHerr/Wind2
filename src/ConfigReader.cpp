#include "structs.h"
#include <ConfigReader.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <algorithm>
#include <cctype>
#include <concepts>
#include <filesystem>
#include <functional>
#include <string>
#include <sys/types.h>
#include <yaml-cpp/node/parse.h>
#include <RuleBuilder.h>
#include <Logger.h>
#include <InputManager.h>
#include <InputBuilder.h>

#define ARL(X) (sizeof(X) / sizeof(X[0]))

using namespace Wind;

using namespace YAML;



ConfigReader::ConfigReader() : empty(true){}

unsigned long _internal_choosen_modifier = Mod4Mask;


static unsigned int StringtoModifier(std::string s) {


    static const struct {std::string str; unsigned long  val;} modifiers[] = {
        {"Mod1", Mod1Mask},
        {"Mod2", Mod2Mask},
        {"Mod3", Mod3Mask},
        {"Mod4", Mod4Mask},
        {"Mod5", Mod5Mask},
        {"Shift", ShiftMask},
        {"Control", ControlMask},
        {"Capslock", LockMask},
        {"Mod", _internal_choosen_modifier}
    };


    for(int i = 0; i < ARL(modifiers); i++) {
        if (modifiers[i].str == s)
            return modifiers[i].val;

    }

    return 0;
}



static Action StringtoFunction(std::string s) {

    Action a;


    static const struct {std::string str; Action val;} functions[] = {
        {"quit", Action([](auto a){ /*do some stuff*/}, 0, false)}
    };

    for (int i = 0; i < ARL(functions); i++) {
        if (s == functions[i].str)
            return functions[i].val;
    }




    return a;
}


static KeySym StringtoKeysym(std::string s) {


    return 0;
}


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
    readKeys();

    if (document["test"].IsDefined())

        this->_configs.testdim = readDimensionsfromNode(document["test"]);

    if (document["testtype"].IsDefined())
        this->_configs.testtype = readTypefromNode(document["testtype"]);

    this->empty = false;
    return true;
}


auto ConfigReader::readTopicNames() -> void {


    std::set<std::string> names;
    Node node_topicnames = document["topicnames"];

    if (node_topicnames.IsDefined() || !node_topicnames.IsSequence()) {
        for (int i = 0; i < node_topicnames.size(); i++) {
            std::string n = node_topicnames[i].as<std::string>();
            if (!names.contains(n)) {
                this->_configs.topicnames.push_back(node_topicnames[i].as<std::string>());
                names.insert(n);
            }
            else {
                Logger::GetInstance().Warn("Topic {} already created. Topicnames must be unique. Skipping.", n);
            }
        }
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

    Node node_modMask = document["Modifier"];

    if (node_modMask.IsDefined() && node_modMask.IsScalar()) {
        unsigned long m = StringtoModifier(node_modMask.as<std::string>());
        if (m < Mod1Mask) {
            Logger::GetInstance().Warn("{} is not a valid value for ModMask, using Mod4 (super)", m);
        }
        else 
            _internal_choosen_modifier = m;
    }
    else {
        Logger::GetInstance().Warn("No default Mod mask defined, using Mod4 (super)");
    }
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
                Logger::GetInstance().Warn("{} is not a valid topic, skipping rule", tn);
                continue;
            }
            r.setDefaultTopic(tn);
        }
        _configs.rules.push_back(r.finish());

    }

}


auto ConfigReader::readKeys() -> void {


    Node keynode = document["Keys"];

    auto & Log = Logger::GetInstance();
    auto & IM = InputManager::GetInstance();


    if (!keynode.IsDefined() || !keynode.IsSequence()) {
        Log.Error("No keybindings specified. Will set Mod4 + q to exit Wind");
        IM.addKey(KeyBuilder().setModMask().setKeySym(100).finish(), Action([](auto a){}, 0, false)); //TODO: Find Key of q and implement exit func;

    }

    for(auto n : keynode) {


        KeyBuilder key;

        Action a;



        if (n["Modifiers"].IsDefined() && n["Modifiers"].IsSequence()) {
            for (auto mod : n["Modifiers"]) {
                switch (StringtoModifier(mod.as<std::string>())) {
                    case ShiftMask: key.setShiftMask();
                                    break;
                    case ControlMask: key.setControlMask();
                                      break;
                    case Mod1Mask:
                    case Mod2Mask:
                    case Mod3Mask:
                    case Mod4Mask:
                    case Mod5Mask: key.setModMask();
                                   break;
                    default:
                                   Log.Info("Modifierfield specified but empty");
                                   break;

                }
            }
        }
        else {
            Log.Warn("No Modifiers for Key set. This might cause problems");
        }

        if (n["Key"].IsDefined() && n["Key"].IsScalar()) {

            std::string s = n["Key"].as<std::string>().c_str();

            KeySym sym = XStringToKeysym(s.c_str());

            if(sym != NoSymbol)
                key.setKeySym(sym);
            else {
            Log.Error("Key {} is not valid. Skipping Key", s);
            continue;
        }
        }
        else {
            Log.Error("Key without Key found. Not the best idea");
            continue;
        }

        if (n["Action"].IsDefined() && n["Action"].IsScalar()) {
            a = StringtoFunction(n["Action"].as<std::string>());

            if (a.wantArgument()) {

                for (std::string s : {"Cmd", "TargetTopic"}) {

                    if (n[s].IsDefined() && n[s].IsScalar()) {
                        a.setArgument(n[s].as<std::string>());
                        goto done;
                    }

                }

                for (std::string s : {"Int", "Offset"}) {
                    if (n[s].IsDefined() && n[s].IsScalar()) {
                        a.setArgument(n[s].as<int>());
                        goto done;
                    }
                }

            }



        }
        else {
            Log.Warn("Found Key with no Action. Key will be useless and skipped");
            continue;
        }

done:



        //IM.addKey(key.finish(), std::move(a));
        localkeys.insert(std::make_pair(key.finish(), a));
    }




}
