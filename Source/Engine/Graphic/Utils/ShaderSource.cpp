#include "ShaderSource.hpp"

#include <iostream>
#include <sstream>

ShaderSource ShaderSource::Edit(const ShaderSource& old, const ShaderSource& partial) {
    ShaderSource new_ss;

    // Note 1: it can't substitue partial layout or struct. (maybe TODO later)
    new_ss.structs = old.structs;
    new_ss.layouts = old.layouts;

    // Note 2: it's assuming it's only about edit not adding or removing. (maybe TODO later)
    for (const auto& p: old._vars) {
        if (partial._vars.find(p.second.name) == partial._vars.cend()) {
            const var& old_v = p.second;
            new_ss.add_var(old_v.dir, old_v.type, old_v.name, old_v.count, old_v.value);
        }
        else {
            const var& new_v = partial._vars.at(p.second.name);
            new_ss.add_var(new_v.dir, new_v.type, new_v.name, new_v.count, new_v.value);
        }
    }

    for (const auto& p : old._funcs) {
        if (partial._funcs.find(p.second.name) == partial._funcs.cend()) {
            const func& old_f = p.second;
            new_ss.add_func(old_f.type, old_f.name, old_f.args, old_f.code);
        }
        else {
            const func& new_f = partial._funcs.at(p.second.name);
            new_ss.add_func(new_f.type, new_f.name, new_f.args, new_f.code);
        }
    }

    return new_ss;
}

// (For compute shader)
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

// (Not used for now)
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

// (For ordinary shaders)
ShaderSource& ShaderSource::add_var(const std::string& dir, const std::string& type, const std::string& name, int count, const std::string& value) {
    std::stringstream ss;
    if (!dir.empty())
        ss << dir << " ";

    ss << type << " " << name;
    if (count > 0) {
        ss << "[" << count << "]";
    }

    if (!value.empty())
        ss << " = " << value;

    ss << ";\n";

    variables.push_back(ss.str());
    _vars[name] = {dir, type, name, value, count };
    return *this;
}

ShaderSource& ShaderSource::add_func(const std::string& type, const std::string& name, const std::string& args, const std::string& code) {
    std::stringstream ss;
    ss << type << " ";
    ss << name << "(" << args << ") { \n";
    ss << code << "\n";
    ss << "} \n";

    functions.push_back(ss.str());
    _funcs[name] = { type , name, args, code };
    return *this;
}

// Helpers
std::string ShaderSource::str() const {
    std::stringstream ss;

    // version
    ss << "#version 400 core\n";

    // structs
    for (const auto& struc : structs)
        ss << struc;

    // layouts
    for (const auto& layout : layouts)
        ss << layout;

    // variables
    for (const auto& variable : variables)
        ss << variable;

    // functions
    for (const auto& func : functions)
        ss << func;

    // ending
    ss << "\n\0";
    return ss.str();
}

ShaderSource::var ShaderSource::getVar(const std::string& name) const {
    if(_vars.find(name) == _vars.cend())
        return {};

    return _vars.at(name);
}
