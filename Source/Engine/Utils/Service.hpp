#pragma once

#include <cassert>

template<typename T>
class Service {
    Service() = delete;

public:
    template<typename... Args>
    static void build(Args&&... args) {
        provide(new T(std::forward<Args>(args)...));
    }

    static void provide(T* ptr) {
        auto& service = storage();
        service = ptr;
    }

    static T& get() {
        auto& service = storage();
        return *service;
    }

    static void destroy() {
        auto& service = storage();
        delete service;
        service = nullptr;
    }

    static void clear() {
        auto& service = storage();
        service = nullptr;
    }

private:
    static T*& storage() {
        static T* instance = nullptr;
        return instance;
    }
};

template<typename T>
class Provider {
public:
    explicit Provider(T* pointer) {
        Service<T>::provide(pointer);
    }
    ~Provider() {
        Service<T>::clear();
    }
};

template<typename T>
static T& Get() {
    return Service<T>::get();
}