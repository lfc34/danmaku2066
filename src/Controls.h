//makes life easier
#ifndef CONTROLS_H
#define CONTROLS_H

#include <SFML/Window/Keyboard.hpp>

namespace Controls {
    constexpr sf::Keyboard::Key UP = sf::Keyboard::Key::Up;
    constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Key::Down;
    constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Key::Left;
    constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Key::Right;
    constexpr sf::Keyboard::Key Z_K = sf::Keyboard::Key::Z;
    constexpr sf::Keyboard::Key X_K = sf::Keyboard::Key::X;
    constexpr sf::Keyboard::Key ESC = sf::Keyboard::Key::Escape;
}

#endif