#include "UiTrain.hpp"

#include <glm/gtx/string_cast.hpp>
#include <sstream>

void UiTrain::show() {
    ImGui::Begin("Info");
    ImGui::Separator();

    for (const auto& row : _datas) {
        const std::string& row_name = row.first;

        ImGui::Text(row_name.c_str());

        for (const auto& article : row.second) {
            const std::string& article_name = article.first;
            const std::string& article_content = article.second;

            ImGui::Text(std::string(article_name + ": " + article_content).c_str());
        }

        ImGui::Separator();
    }

    ImGui::End();
}

void UiTrain::reset() {
    _datas.clear();
}

void UiTrain::createSection(const std::string& sectionName) {
    _datas[sectionName] = _row{};
}

void UiTrain::addContent(const std::string& sectionName, const std::string& articleName, const std::string& value) {
    _datas[sectionName][articleName] = value;
}

void UiTrain::addContent(const std::string& sectionName, const std::string& articleName, const glm::vec3& value) {
    addContent(sectionName, articleName, glm::to_string(value));
}

void UiTrain::addContent(const std::string& sectionName, const std::string& articleName, const glm::vec2& value) {
    addContent(sectionName, articleName, glm::to_string(value));
}

void UiTrain::addContent(const std::string& sectionName, const std::string& articleName, const float& value) {
    addContent(sectionName, articleName, std::to_string(value));
}

void UiTrain::addContent(const std::string& sectionName, const std::string& articleName, const int& value) {
    addContent(sectionName, articleName, std::to_string(value));
}
