#pragma once

namespace mini_engine {
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void update() = 0;
    };
}
