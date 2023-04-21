#include "yaml-cpp/yaml.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <cassert>
#include <string>
#include <vector>

void parseYamlNode2(const YAML::Node &node,
                    std::map<std::string, std::string> &result,
                    const std::string &prefix)
{
    if (node.IsScalar()) {
        // 如果是叶子节点，添加键值对到结果中
        result[prefix] = node.as<std::string>();
    } else if (node.IsMap()) {
        // 如果是映射节点，递归解析每个子节点
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix;
            if (prefix.empty()) {
                newPrefix = key;
            } else {
                newPrefix = prefix + "/" + key;
            }
            parseYamlNode2(it->second, result, newPrefix);
        }
    } else if (node.IsSequence()) {
        // 如果是序列节点，递归解析每个子节点并添加编号前缀
        int i = 0;
        for (auto it = node.begin(); it != node.end(); ++it) {
            std::string newPrefix = prefix + "[" + std::to_string(i++) + "]";
            parseYamlNode2(*it, result, newPrefix);
        }
    }
}

void testprase(YAML::Node &node, std::map<std::string, std::string> &res,
               const std::string &prefix)
{
    if (node.IsScalar() || node.IsSequence() || node.IsNull()) {
        if (node.IsNull()) {
            res[prefix] = {};
        } else {
            std::stringstream ss;
            ss << node;
            res[prefix] = ss.str();
        }
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix;
            newPrefix = prefix.empty() ? key : (prefix + "/" + key);
            testprase(it->second, res, newPrefix);
        }
    }
}

void parseYamlNodetest(const YAML::Node &node,
                       std::map<std::string, YAML::Node> &result,
                       const std::string &prefix)
{
    if (node.IsScalar() || node.IsSequence() || node.IsNull()) {
        if (node.IsNull()) {
            result[prefix] = YAML::Node();
        } else {
            result[prefix] = node;
        }
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix;
            newPrefix = prefix.empty() ? key : (prefix + "/" + key);
            parseYamlNodetest(it->second, result, newPrefix);
        }
    }
}

int main22()
{
    // YAML::Node config =
    // YAML::LoadFile("/home/kanon/code/yaml_show/example.yaml");

    YAML::Node config = YAML::Load("dad");
    std::cout << "config Type = " << config.Type() << std::endl;
    auto res = config.as<std::string>();
    std::stringstream ss;
    ss << config;
    std::cout << ss.str() << std::endl;

    return 0;
}

void parseYamlNode(const YAML::Node &node, std::map<std::string, YAML::Node> &result, const std::string &prefix)
{
    if (node.IsScalar() || node.IsSequence() || node.IsNull()) {
        if (node.IsNull()) {
            result[prefix] = YAML::Node();
        } else {
            result[prefix] = node;
        }
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix;
            newPrefix = prefix.empty() ? key : (prefix + "/" + key);
            parseYamlNode(it->second, result, newPrefix);
        }
    }
}

//notice, this only can delete the lowest key
void deleteGiveNameNode(YAML::Node rootNode, YAML::Node &parent,
                        const std::string &name, const std::string &prefix = "",
                        const std::string &curName = "")

{
    if (rootNode.IsScalar() || rootNode.IsSequence()) {
        if (!strcmp(prefix.c_str(), name.c_str())) {
            parent.remove(curName);
            return;
        }
    } else if (rootNode.IsMap()) {
        for (auto it = rootNode.begin(); it != rootNode.end(); ++it) {
            //1.get map key
            auto key = it->first.as<std::string>();
            std::string newPrefixName = prefix.empty() ? key : (prefix + "/" + key);
            deleteGiveNameNode(it->second, rootNode, name, newPrefixName, key);
        }
    }
}

void modifyGiveNameNodeT(YAML::Node &node, YAML::Node &parent,
                         const std::string &name, const YAML::Node &newNode,
                         const std::string &prefix,
                         const std::string &curName)
{
    if (node.IsScalar() || node.IsSequence() || node.IsNull()) {
        if (!strcmp(prefix.c_str(), name.c_str())) {
            for (auto j = node.begin(); j != node.end(); ++j) {
                if (!strcmp(j->first.as<std::string>().c_str(), curName.c_str())) {
                    j->second = newNode;
                    return;
                }
            }
        }
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix = prefix.empty() ? key : (prefix + "/" + key);
            modifyGiveNameNodeT(it->second, node, name, newNode, newPrefix, key);
        }
    } else {
        std::cout << "teswt" << std::endl;
    }
}

/**
 *  make sure input node IsMap!
*/
void modifyGiveNameNode(YAML::Node &node, YAML::Node &parent,
                        const std::string &modifyName, const YAML::Node &newNode,
                        const std::string &preName = "", const std::string &curNodeName = "")
{
    if (node.IsScalar() || node.IsSequence()) {
        //notice, first time never enter!
        if (!strcmp(preName.c_str(), modifyName.c_str())) {
            parent[curNodeName] = newNode;
            return;
        }
    } else if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            auto key = it->first.as<std::string>();
            std::string newPrefix = preName.empty() ? key : (preName + "/" + key);
            modifyGiveNameNode(it->second, node, modifyName, newNode, newPrefix, key);
        }
    }
}

/**
 *          create a key:value
 * before call, make sure node is a map
*/

void addNameNode(YAML::Node &node, const std::string &key, YAML::Node &newNode)
{
    auto pos = key.find('/');
    if (pos == std::string::npos) {

        node[key] = newNode;
    } else {
        auto nextNode = node[key.substr(0, pos)];
        addNameNode(nextNode, key.substr(pos + 1), newNode);
    }
}

void printYAMLNode(const YAML::Node &node)
{
    std::stringstream ss;
    ss << node;
    std::cout << ss.str() << std::endl;
}

int main2333()
{
    YAML::Node config = YAML::LoadFile("/home/kanon/code/yaml_show/test.yaml");
    printYAMLNode(config);
    YAML::Node ep_node;
    YAML::Node new_node = YAML::Load("[1, 2, 3]");
    // if use push_back, node will become a seq.
    // new_node.push_back(1);
    // new_node.push_back(2);
    // if use [] =, node will become a map.
    // new_node["key"] = "kdoakod";

    std::cout
        << new_node.Type() << std::endl;
    assert(config.IsMap());
    modifyGiveNameNode(config, ep_node, "key3/in_lidar_topic/value", new_node);
    printYAMLNode(config);
    return 0;
}


int main()
{
    return 0;
}