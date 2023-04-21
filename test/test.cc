#include <iostream>
#include <fstream>
#include "../include/struct_yaml.h"

struct AccountInfo
{
    bool flag;
    std::string name{"kodkoad"};
    std::vector<std::string> family;
    std::unordered_map<std::string, int> address;
    std::optional<std::string> num;
    std::chrono::milliseconds msec;
    std::tuple<std::string, uint8_t> tuple;
    std::unordered_map<std::string, std::tuple<std::string, int32_t>> map_tuple;
};
YCS_ADD_STRUCT(AccountInfo, flag, family, name, address, num, msec, tuple, map_tuple)

struct Person
{
    int age{};
    std::string name;
};

YCS_ADD_STRUCT(Person, age, name);

int main()
{
    AccountInfo ac;
    ac.flag = false;
    ac.name = "kanon";
    ac.address["key1"] = 200;
    ac.address["key2"] = 3000;
    ac.family.emplace_back("hello");
    ac.family.emplace_back("kdoakod");
    ac.msec = std::chrono::milliseconds(200);
    std::get<0>(ac.tuple) = "kdokaod";
    std::get<1>(ac.tuple) = 200;

    auto [node_str, e] = yaml_cpp_struct::to_yaml(ac);
    if (node_str.has_value()) {
        yaml_cpp_struct::save_string_to_file(node_str.value(),"Test.yaml");
    }

    return 0;
}