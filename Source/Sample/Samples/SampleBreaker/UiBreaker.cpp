#include "UiBreaker.hpp"

#include <sstream>

void UiBreaker::show() {
    ImGui::Begin("Actions");
    ImGui::Separator();

    ImGui::Checkbox("Show hitboxes", &show_debug);
    ImGui::Checkbox("Stop time", &stop_time);

    if (ImGui::Button("Restart")) {
        want_restart = true;
    }

    ImGui::End();
}
