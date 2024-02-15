#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Framework/Service.hpp>

#include "AppMenu.hpp"
#include "App.hpp"

// -- Entry point --
int main_() {
    // Setup
    Service<Window>::build(1600, 900, "Sample scene");
    Service<App>::build();
    Service<AppMenu>::build();

    // Loop
    do {
        // Check stuff
        if (Service<App>::get().wantQuit())
        {
            Service<Window>::get().close();
        }

        // States, Physics ..
        {
            Event::Emit(CommonEvents::StateUpdated());
        }
    } while (Service<Window>::get().update());

    // Cleanup
    Service<AppMenu>::destroy();
    Service<App>::destroy();
    Service<Window>::destroy();
    return 0;
}

// --------------------------- test ---------------------------
#include <iterator>
#include <cstddef>

struct _Mesh {
    _Mesh(float v) : value(v) {};
    float value = 0.0f;
};

class _Model
{
public:
    struct _Node {
        float coeff = 1.0f;
        std::vector<std::shared_ptr<_Mesh>> meshes = {};
        std::vector<std::shared_ptr<_Node>> children = {};
    };

    _Model() : _root(std::make_shared<_Node>()) {
        // > Root
        _root->coeff = 1.0f;
        _root->meshes = { {std::make_shared<_Mesh>(1.0f), std::make_shared<_Mesh>(2.0f), std::make_shared<_Mesh>(3.0f)} };
        
        // > Root > Child 1
        _root->children.push_back(std::make_shared<_Node>());
        auto& child_1 = _root->children.back();
        child_1->coeff = 2.0f;
        child_1->meshes = { {std::make_shared<_Mesh>(1.0f), std::make_shared<_Mesh>(2.0f), std::make_shared<_Mesh>(3.0f), std::make_shared<_Mesh>(4.0f)} };

        // > Root > Child 1 > Child 11
        child_1->children.push_back(std::make_shared<_Node>());
        auto& child_11 = child_1->children.back();
        child_11->coeff = 3.0f;
        child_11->meshes = { {std::make_shared<_Mesh>(1.0f), std::make_shared<_Mesh>(2.0f)} };

        // > Root > Child 2
        _root->children.push_back(std::make_shared<_Node>());
        auto& child_2 = _root->children.back();
        child_2->coeff = 1.5f;
        child_2->meshes = { {std::make_shared<_Mesh>(1.0f)} };

        // -- Expectations --
        /*
            | > 1 2 3
            | --> 2 4 6 8
            | ----> 6 12
            | --> 1.5
        */
    }

public:
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = int*;
        using reference = int&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };

    //Iterator begin() { return Iterator(&m_data[0]); }
    //Iterator end() { return Iterator(&m_data[10]); }

//private:
    std::shared_ptr<_Node> _root;
};

void _goThrought(_Model& model) {
    std::stack<const std::shared_ptr<_Model::_Node>*> st;
    st.push(&model._root);

    while (!st.empty()) {
        // Get next in line
        const auto currNode = st.top();
        st.pop();

        // Draw
        for (const auto& mesh : (*currNode)->meshes) {
            std::cerr << mesh->value * (*currNode)->coeff << " ";
        }
        std::cerr << std::endl;

        // Add children
        for (size_t i = 0; i < (*currNode)->children.size(); i++) {
            st.push(&(*currNode)->children[i]);
        }
    }
}

int main() {
    _Model model;
    std::cerr << "- model go -" << std::endl;
    _goThrought(model);
    std::cerr << "--" << std::endl;

    //for (auto it = model.begin(), end = model.end(); it != end; ++it) {
    //    const auto i = *it;
    //    std::cout << i << "\n";
    //}
    return 0;
}