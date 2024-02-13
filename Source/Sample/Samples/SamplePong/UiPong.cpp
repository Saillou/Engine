#include "UiPong.hpp"

#include <sstream>

void UiPong::show() {
    ImGui::Begin("Actions");
    ImGui::Separator();

    ImGui::Checkbox("Show debug", &show_debug);
    ImGui::Checkbox("Stop time", &stop_time);

    if (ImGui::Button("Restart")) {
        want_restart = true;
    }

    ImGui::End();
}
