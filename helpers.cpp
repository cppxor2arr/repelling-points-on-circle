#include <SFML/System/Vector2.hpp>
#include <cmath>

float get_touch_angle(const sf::Vector2f &touch_pos,
                      const sf::Vector2f &origin)
{
    const auto rel_pos = touch_pos - origin;
    return std::atan2(rel_pos.y, rel_pos.x);
}
