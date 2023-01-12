#ifndef SPIN_HPP
#define SPIN_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
class RepellingPointsOnCircle : public sf::Drawable {
private:
    class Point {
    public:
        Point(const float magnitude, const float angle);

        void update(const float dt, const float force); // dt: seconds
        void set_update_enabled(const bool enabled);
        bool is_update_enabled() const;

        sf::Vector2f get_position() const;

        float magnitude;
        float angle;
        float vel = 0;
        float acc = 0;

    private:
        float applied_forces(const float force);

        bool m_update_enabled = true;
    };

public:
    using PointVector = std::vector<Point>;

    void update(const float dt); // seconds

    float get_radius() const;
    const sf::Vector2f &get_center() const;
    float get_point_radius() const;
    PointVector::size_type get_num_points() const;

    void set_radius(const float radius);
    void set_center(const sf::Vector2f &center);
    void set_point_radius(const float point_radius);
    void set_num_points(const PointVector::size_type num_points);

    PointVector _points;

private:
    virtual void draw(sf::RenderTarget &target,
                      const sf::RenderStates states) const;

    float m_radius;
    sf::Vector2f m_center;
    float m_point_radius;

    PointVector::size_type m_num_points;
};
#pragma clang diagnostic pop

#endif
