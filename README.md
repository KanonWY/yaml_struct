## yaml_struct
基于visit_struct/1.0静态反射实现的yaml与struct的快速序列化接口。

```bash
// 安装依赖 
conan install .
// 
cmake -S . -B build
cmake --build build
```

简单使用说明：

```c++
struct Person
{
    int age{};
    std::string name;
};

// 使用宏注册
YCS_ADD_STRUCT(Person, age, name);

Person p;
p.age = 200;
p.name = "dlpaldp";
//struct 转换为yaml string.
auto [node_str, e] = yaml_cpp_struct::to_yaml(ac);
//写入yaml文件
if(node_str.have_value())
{
    yaml_cpp_struct::save_string_to_file(node_str.value(),"out.yaml");
}
```

