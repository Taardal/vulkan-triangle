#pragma once

#include "window.h"

namespace dd {
    enum class Key {
        None = 0,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        Num_0 = GLFW_KEY_0,
        Num_1 = GLFW_KEY_1,
        Num_2 = GLFW_KEY_2,
        Num_3 = GLFW_KEY_3,
        Num_4 = GLFW_KEY_4,
        Num_5 = GLFW_KEY_5,
        Num_6 = GLFW_KEY_6,
        Num_7 = GLFW_KEY_7,
        Num_8 = GLFW_KEY_8,
        Num_9 = GLFW_KEY_9,
        Numpad_0 = GLFW_KEY_KP_0,
        Numpad_1 = GLFW_KEY_KP_1,
        Numpad_2 = GLFW_KEY_KP_2,
        Numpad_3 = GLFW_KEY_KP_3,
        Numpad_4 = GLFW_KEY_KP_4,
        Numpad_5 = GLFW_KEY_KP_5,
        Numpad_6 = GLFW_KEY_KP_6,
        Numpad_7 = GLFW_KEY_KP_7,
        Numpad_8 = GLFW_KEY_KP_8,
        Numpad_9 = GLFW_KEY_KP_9,
        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,
        Space = GLFW_KEY_SPACE,
        Escape = GLFW_KEY_ESCAPE,
        Enter = GLFW_KEY_ENTER,
        Tab = GLFW_KEY_TAB,
        Backspace = GLFW_KEY_BACKSPACE,
        Up = GLFW_KEY_UP,
        Down = GLFW_KEY_DOWN,
        Left = GLFW_KEY_LEFT,
        Right = GLFW_KEY_RIGHT,
    };

    struct KeyboardConfig {
        window* window = nullptr;
    };

    class Keyboard {
    private:
        KeyboardConfig config;
        std::vector<Key> keys;
        std::unordered_map<std::string, Key> keysByName;
        std::unordered_map<Key, std::string> namesByKey;

    public:
        explicit Keyboard(const KeyboardConfig& config);

        const std::vector<Key>& getKeys() const;

        const std::unordered_map<std::string, Key>& getKeysByName() const;

        std::unordered_map<Key, std::string> getNamesByKey() const;

        bool isPressed(Key key) const;

        bool isPressed(uint16_t key) const;

        bool isPressed(const std::string& keyName) const;

        std::string getName(Key key) const;

        std::string getName(uint16_t key) const;

        static std::string getKeyName(Key key);

        static std::string getKeyName(uint16_t key);

    private:
        static std::vector<Key> createKeys();

        static std::unordered_map<std::string, Key> createKeysByName();

        static std::unordered_map<Key, std::string> createNamesByKey();
    };
}
