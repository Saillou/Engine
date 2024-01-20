#pragma once

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

struct ShaderSource {
    ShaderSource() = default;

    static ShaderSource Edit(const ShaderSource& old, const ShaderSource& updated);

    enum Type : unsigned int{
        Vertex   = 0x8B31,
        Geometry = 0x8DD9,
        Fragment = 0x8B30
    };

    ShaderSource& add_cp_layout(const std::string& dir, const std::array<std::string, 3>& n_cores);
    ShaderSource& add_struct(const std::string& structName, const std::vector<std::string>& vars, const std::string& layout_id = "");
    ShaderSource& add_var(const std::string& dir, const std::string& type, const std::string& name, int count = 0, const std::string& value = "");
    ShaderSource& add_func(const std::string& type, const std::string& name, const std::string& args, const std::string& code);

    std::string str() const;

    std::vector<std::string> structs;
    std::vector<std::string> layouts;
    std::vector<std::string> variables;
    std::vector<std::string> functions;

    struct var {
        std::string dir = "";
        std::string type = "";
        std::string name = "";
        std::string value = "";
        int count = 0;
    };
    struct func {
        std::string type = "";
        std::string name = "";
        std::string args = "";
        std::string code = "";
    };

    var getVar(const std::string& name) const;

private:
    std::unordered_map<std::string, var> _vars;
    std::unordered_map<std::string, func> _funcs;
};
