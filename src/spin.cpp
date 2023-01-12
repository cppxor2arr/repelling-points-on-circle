#include "spin.hpp"
#include "constants.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

void RepellingPointsOnCircle::update(const float dt) // seconds
{
    std::vector<float> angles;
    for (const Point &point : _points) {
        angles.push_back(point.angle);
    }

    for (decltype(_points)::size_type i = 0; i != _points.size(); ++i) {
        if (!_points[i].is_update_enabled()) {
            continue;
        }

        float total_force = 0;

        for (decltype(angles)::size_type i2 = 0; i2 != angles.size(); ++i2) {
            if (i2 == i) {
                continue;
            }
            const float alpha = angles[i] - angles[i2];
            const float distance_sqrd = 2 * (1 - std::cos(alpha));
            const float force = std::copysign(1.f, alpha) * 1 / distance_sqrd;
            total_force += force * std::cos(alpha / 2);
        }
        total_force = std::clamp(total_force, -50.f, 50.f);
        _points[i].update(dt, total_force);
    }

    for (Point &point : _points) {
        point.angle = std::fmod(point.angle + 2 * pi, 2 * pi);
    }
}

float RepellingPointsOnCircle::get_radius() const
{
    return m_radius;
}

const sf::Vector2f &RepellingPointsOnCircle::get_center() const
{
    return m_center;
}

float RepellingPointsOnCircle::get_point_radius() const
{
    return m_point_radius;
}

RepellingPointsOnCircle::PointVector::size_type
RepellingPointsOnCircle::get_num_points() const
{
    return m_num_points;
}

void RepellingPointsOnCircle::set_radius(const float radius)
{
    m_radius = radius;
}

void RepellingPointsOnCircle::set_center(const sf::Vector2f &center)
{
    m_center = center;
}

void RepellingPointsOnCircle::set_point_radius(const float point_radius)
{
    m_point_radius = point_radius;
}

void RepellingPointsOnCircle::set_num_points(
    const PointVector::size_type num_points)
{
    m_num_points = num_points;
    const float delta = 2 * pi / static_cast<float>(num_points);

    _points.clear();
    for (PointVector::size_type n = 0; n != num_points; ++n) {
        const float angle = static_cast<float>(n) * delta;
        _points.emplace_back(m_radius, angle);
    }
}

void RepellingPointsOnCircle::draw(sf::RenderTarget &target,
                                   const sf::RenderStates states) const
{
    static sf::CircleShape circle;
    circle.setRadius(m_point_radius);
    circle.setOrigin(m_point_radius, m_point_radius);
    circle.setPointCount(static_cast<std::size_t>(2 * pi * m_point_radius));

    for (const Point &point : _points) {
        circle.setPosition(m_center + point.get_position());
        target.draw(circle, states);
    }
}

RepellingPointsOnCircle::Point::Point(const float magnitude, const float angle)
    : magnitude(magnitude)
    , angle(angle)
{
}

void RepellingPointsOnCircle::Point::update(const float dt,
                                            const float force) // dt: seconds
{
    angle += vel * dt + 0.5f * acc * dt * dt;
    const float new_acc = applied_forces(force);
    vel += 0.5f * (acc + new_acc) * dt;
    acc = new_acc;
}

void RepellingPointsOnCircle::Point::set_update_enabled(const bool enabled)
{
    m_update_enabled = enabled;

    if (!enabled) {
        vel = 0;
        acc = 0;
    }
}

bool RepellingPointsOnCircle::Point::is_update_enabled() const
{
    return m_update_enabled;
}

sf::Vector2f RepellingPointsOnCircle::Point::get_position() const
{
    return magnitude * sf::Vector2f(std::cos(angle), std::sin(angle));
}

float RepellingPointsOnCircle::Point::applied_forces(const float force)
{
    constexpr float drag = 0.5f;
    const float drag_force = 0.5f * drag * vel * std::abs(vel);
    return force - drag_force;
}
