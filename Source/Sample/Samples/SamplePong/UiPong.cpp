#include "UiPong.hpp"

#include <sstream>

void UiPong::show() {
    ImGui::Begin("Actions");
    ImGui::Separator();

    ImGui::Checkbox("Show hitboxes", &show_debug);
    ImGui::Checkbox("Stop time", &stop_time);

    ImGui::End();
}
