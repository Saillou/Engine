#include "ShaderSource.hpp"

#include <sstream>

ShaderSource& ShaderSource::add_cp_layout(const std::string& dir, const std::array<std::string,3>& n_cores) {
    char var_name = 'x';
    std::stringstream ss;

    ss << "layout (";
    for (int i = 0; i < 3; i++) {
        ss << "local_size_" << var_name << " = " << n_cores[i];

        if(var_name < 'z') 
            ss << ", ";

        var_name++;
    }
    ss << ") " << dir << "; \n";

    layouts.push_back(ss.str());
    return *this;
}

ShaderSource& ShaderSource::add_struct(const std::string& structName, const std::vector<std::string>& vars, const std::string& layout_id) {
    std::stringstream ss;
    if (!layout_id.empty())
        ss << "layout(binding = " << layout_id  << ") \n";

    ss << structName << " { \n";
    for (const std::string& var : vars) {
        ss << "\t" << var << ";  \n";
    }
    ss << "}; \n";

    structs.push_back(ss.str());
    return *this;
}

ShaderSource& ShaderSource::add_var(const std::string& dir, const std::string& type, const std::string& name) {
    std::stringstream ss;
    ss << dir << " ";
    ss << type << " ";
    ss << name << ";\n";

    layouts.push_back(ss.str());
    return *this;
}

ShaderSource& ShaderSource::add_func(const std::string& type, const std::string& name, const std::string& args, const std::string& code) {
    std::stringstream ss;
    ss << type << " ";
    ss << name << "(" << args << ") { \n";
    ss << code << "\n";
    ss << "} \n";

    functions.push_back(ss.str());
    return *this;
}

std::string ShaderSource::str() const {
    std::stringstream ss;

    // version
    ss << "#version 330 core\n";

    // structs
    for (const auto& struc : structs)
        ss << struc;

    // layouts
    for (const auto& layout : layouts)
        ss << layout;

    // functions
    for (const auto& func : functions)
        ss << func;

    // ending
    ss << "\n\0";
    return ss.str();
}