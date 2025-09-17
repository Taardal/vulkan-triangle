#include "keyboard.h"

namespace Game {
    Keyboard::Keyboard(const KeyboardConfig& config) : config(config),
        keys(createKeys()),
        keysByName(createKeysByName()),
        namesByKey(createNamesByKey()) {
    }

    const std::vector<Key>& Keyboard::getKeys() const {
        return keys;
    }

    const std::unordered_map<std::string, Key>& Keyboard::getKeysByName() const {
        return keysByName;
    }

    std::unordered_map<Key, std::string> Keyboard::getNamesByKey() const {
        return namesByKey;
    }

    bool Keyboard::isPressed(Key key) const {
        // return config.window->isKeyPressed((uint16_t) key);
        return false;
    }

    bool Keyboard::isPressed(uint16_t key) const {
        // return config.window->isKeyPressed(key);
        return false;
    }

    bool Keyboard::isPressed(const std::string& keyName) const {
        Key key = keysByName.at(keyName);
        // return config.window->isKeyPressed((uint16_t) key);
        return false;
    }

    std::string Keyboard::getName(Key key) const {
        return namesByKey.at(key);
    }

    std::string Keyboard::getName(uint16_t key) const {
        return getName((Key) key);
    }

    std::string Keyboard::getKeyName(uint16_t key) {
        return getKeyName((Key) key);
    }

    std::string Keyboard::getKeyName(Key key) {
        switch (key) {
            case Key::A:
                return "A";
            case Key::B:
                return "B";
            case Key::C:
                return "C";
            case Key::D:
                return "D";
            case Key::E:
                return "E";
            case Key::F:
                return "F";
            case Key::G:
                return "G";
            case Key::H:
                return "H";
            case Key::I:
                return "I";
            case Key::J:
                return "J";
            case Key::K:
                return "K";
            case Key::L:
                return "L";
            case Key::M:
                return "M";
            case Key::N:
                return "N";
            case Key::O:
                return "O";
            case Key::P:
                return "P";
            case Key::Q:
                return "Q";
            case Key::R:
                return "R";
            case Key::S:
                return "S";
            case Key::T:
                return "T";
            case Key::U:
                return "U";
            case Key::V:
                return "V";
            case Key::W:
                return "W";
            case Key::X:
                return "X";
            case Key::Y:
                return "Y";
            case Key::Z:
                return "Z";
            case Key::Num_0:
                return "Num_0";
            case Key::Num_1:
                return "Num_1";
            case Key::Num_2:
                return "Num_2";
            case Key::Num_3:
                return "Num_3";
            case Key::Num_4:
                return "Num_4";
            case Key::Num_5:
                return "Num_5";
            case Key::Num_6:
                return "Num_6";
            case Key::Num_7:
                return "Num_7";
            case Key::Num_8:
                return "Num_8";
            case Key::Num_9:
                return "Num_9";
            case Key::Numpad_0:
                return "Numpad_0";
            case Key::Numpad_1:
                return "Numpad_1";
            case Key::Numpad_2:
                return "Numpad_2";
            case Key::Numpad_3:
                return "Numpad_3";
            case Key::Numpad_4:
                return "Numpad_4";
            case Key::Numpad_5:
                return "Numpad_5";
            case Key::Numpad_6:
                return "Numpad_6";
            case Key::Numpad_7:
                return "Numpad_7";
            case Key::Numpad_8:
                return "Numpad_8";
            case Key::Numpad_9:
                return "Numpad_9";
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            case Key::F5:
                return "F5";
            case Key::F6:
                return "F6";
            case Key::F7:
                return "F7";
            case Key::F8:
                return "F8";
            case Key::F9:
                return "F9";
            case Key::F10:
                return "F10";
            case Key::F11:
                return "F11";
            case Key::F12:
                return "F12";
            case Key::Space:
                return "Space";
            case Key::Escape:
                return "Escape";
            case Key::Enter:
                return "Enter";
            case Key::Tab:
                return "Tab";
            case Key::Backspace:
                return "Backspace";
            case Key::Up:
                return "Up";
            case Key::Down:
                return "Down";
            case Key::Left:
                return "Left";
            case Key::Right:
                return "Right";
            default:
                return "";
        }
    }

    std::vector<Key> Keyboard::createKeys() {
        return {
            Key::A,
            Key::B,
            Key::C,
            Key::D,
            Key::E,
            Key::F,
            Key::G,
            Key::H,
            Key::I,
            Key::J,
            Key::K,
            Key::L,
            Key::M,
            Key::N,
            Key::O,
            Key::P,
            Key::Q,
            Key::R,
            Key::S,
            Key::T,
            Key::U,
            Key::V,
            Key::W,
            Key::X,
            Key::Y,
            Key::Z,
            Key::Num_0,
            Key::Num_1,
            Key::Num_2,
            Key::Num_3,
            Key::Num_4,
            Key::Num_5,
            Key::Num_6,
            Key::Num_7,
            Key::Num_8,
            Key::Num_9,
            Key::Numpad_0,
            Key::Numpad_1,
            Key::Numpad_2,
            Key::Numpad_3,
            Key::Numpad_4,
            Key::Numpad_5,
            Key::Numpad_6,
            Key::Numpad_7,
            Key::Numpad_8,
            Key::Numpad_9,
            Key::F1,
            Key::F2,
            Key::F3,
            Key::F4,
            Key::F5,
            Key::F6,
            Key::F7,
            Key::F8,
            Key::F9,
            Key::F10,
            Key::F11,
            Key::F12,
            Key::Space,
            Key::Escape,
            Key::Enter,
            Key::Tab,
            Key::Backspace,
            Key::Up,
            Key::Down,
            Key::Left,
            Key::Right,
        };
    }

    std::unordered_map<std::string, Key> Keyboard::createKeysByName() {
        return {
            { "A", Key::A },
            { "B", Key::B },
            { "C", Key::C },
            { "D", Key::D },
            { "E", Key::E },
            { "F", Key::F },
            { "G", Key::G },
            { "H", Key::H },
            { "I", Key::I },
            { "J", Key::J },
            { "K", Key::K },
            { "L", Key::L },
            { "M", Key::M },
            { "N", Key::N },
            { "O", Key::O },
            { "P", Key::P },
            { "Q", Key::Q },
            { "R", Key::R },
            { "S", Key::S },
            { "T", Key::T },
            { "U", Key::U },
            { "V", Key::V },
            { "W", Key::W },
            { "X", Key::X },
            { "Y", Key::Y },
            { "Z", Key::Z },
            { "Num_0", Key::Num_0 },
            { "Num_1", Key::Num_1 },
            { "Num_2", Key::Num_2 },
            { "Num_3", Key::Num_3 },
            { "Num_4", Key::Num_4 },
            { "Num_5", Key::Num_5 },
            { "Num_6", Key::Num_6 },
            { "Num_7", Key::Num_7 },
            { "Num_8", Key::Num_8 },
            { "Num_9", Key::Num_9 },
            { "Numpad_0", Key::Numpad_0 },
            { "Numpad_1", Key::Numpad_1 },
            { "Numpad_2", Key::Numpad_2 },
            { "Numpad_3", Key::Numpad_3 },
            { "Numpad_4", Key::Numpad_4 },
            { "Numpad_5", Key::Numpad_5 },
            { "Numpad_6", Key::Numpad_6 },
            { "Numpad_7", Key::Numpad_7 },
            { "Numpad_8", Key::Numpad_8 },
            { "Numpad_9", Key::Numpad_9 },
            { "F1", Key::F1 },
            { "F2", Key::F2 },
            { "F3", Key::F3 },
            { "F4", Key::F4 },
            { "F5", Key::F5 },
            { "F6", Key::F6 },
            { "F7", Key::F7 },
            { "F8", Key::F8 },
            { "F9", Key::F9 },
            { "F10", Key::F10 },
            { "F11", Key::F11 },
            { "F12", Key::F12 },
            { "Space", Key::Space },
            { "Escape", Key::Escape },
            { "Enter", Key::Enter },
            { "Tab", Key::Tab },
            { "Backspace", Key::Backspace },
            { "Up", Key::Up },
            { "Down", Key::Down },
            { "Left", Key::Left },
            { "Right", Key::Right },
        };
    }

    std::unordered_map<Key, std::string> Keyboard::createNamesByKey() {
        return {
            { Key::A, "A" },
            { Key::B, "B" },
            { Key::C, "C" },
            { Key::D, "D" },
            { Key::E, "E" },
            { Key::F, "F" },
            { Key::G, "G" },
            { Key::H, "H" },
            { Key::I, "I" },
            { Key::J, "J" },
            { Key::K, "K" },
            { Key::L, "L" },
            { Key::M, "M" },
            { Key::N, "N" },
            { Key::O, "O" },
            { Key::P, "P" },
            { Key::Q, "Q" },
            { Key::R, "R" },
            { Key::S, "S" },
            { Key::T, "T" },
            { Key::U, "U" },
            { Key::V, "V" },
            { Key::W, "W" },
            { Key::X, "X" },
            { Key::Y, "Y" },
            { Key::Z, "Z" },
            { Key::Num_0, "Num_0" },
            { Key::Num_1, "Num_1" },
            { Key::Num_2, "Num_2" },
            { Key::Num_3, "Num_3" },
            { Key::Num_4, "Num_4" },
            { Key::Num_5, "Num_5" },
            { Key::Num_6, "Num_6" },
            { Key::Num_7, "Num_7" },
            { Key::Num_8, "Num_8" },
            { Key::Num_9, "Num_9" },
            { Key::Numpad_0, "Numpad_0" },
            { Key::Numpad_1, "Numpad_1" },
            { Key::Numpad_2, "Numpad_2" },
            { Key::Numpad_3, "Numpad_3" },
            { Key::Numpad_4, "Numpad_4" },
            { Key::Numpad_5, "Numpad_5" },
            { Key::Numpad_6, "Numpad_6" },
            { Key::Numpad_7, "Numpad_7" },
            { Key::Numpad_8, "Numpad_8" },
            { Key::Numpad_9, "Numpad_9" },
            { Key::F1, "F1" },
            { Key::F2, "F2" },
            { Key::F3, "F3" },
            { Key::F4, "F4" },
            { Key::F5, "F5" },
            { Key::F6, "F6" },
            { Key::F7, "F7" },
            { Key::F8, "F8" },
            { Key::F9, "F9" },
            { Key::F10, "F10" },
            { Key::F11, "F11" },
            { Key::F12, "F12" },
            { Key::Space, "Space" },
            { Key::Escape, "Escape" },
            { Key::Enter, "Enter" },
            { Key::Tab, "Tab" },
            { Key::Backspace, "Backspace" },
            { Key::Up, "Up" },
            { Key::Down, "Down" },
            { Key::Left, "Left" },
            { Key::Right, "Right" },
        };
    }
}
