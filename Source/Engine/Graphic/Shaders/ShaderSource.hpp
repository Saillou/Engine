#pragma once

#include <string>
#include <vector>
#include <array>

struct ShaderSource {
    ShaderSource() = default;

    ShaderSource& add_cp_layout(const std::string& dir, const std::array<std::string, 3>& n_cores);
    ShaderSource& add_struct(const std::string& structName, const std::vector<std::string>& vars, const std::string& layout_id = "");
    ShaderSource& add_var(const std::string& dir, const std::string& type, const std::string& name, const std::string& value = "");
    ShaderSource& add_func(const std::string& type, const std::string& name, const std::string& args, const std::string& code);

    std::string str() const;

    std::vector<std::string> structs;
    std::vector<std::string> layouts;
    std::vector<std::string> functions;
};
