#include "helpers.hpp"
#include "spin.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib> // EXIT_FAILURE

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 450), "Spin");
    window.setFramerateLimit(60);

    if (!window.isOpen()) {
        return EXIT_FAILURE;
    }

    RepellingPointsOnCircle repelling_points_on_circle;
    { // new scope
        const sf::Vector2f window_size(window.getSize());
        const float min_length = std::min(window_size.x, window_size.y);

        repelling_points_on_circle.set_center(0.5f * window_size);
        repelling_points_on_circle.set_radius(0.27f * min_length);
        repelling_points_on_circle.set_point_radius(0.023f * min_length);
    }
    repelling_points_on_circle.set_num_points(6);

    auto &points = repelling_points_on_circle._points;
    auto captured_point = points.end();
    sf::Clock timer;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Q:
                    window.close();
                    break;
                default:;
                }
            case sf::Event::MouseButtonPressed: {
                const auto touch_angle = get_touch_angle(
                    window.mapPixelToCoords(
                        { event.mouseButton.x, event.mouseButton.y }),
                    repelling_points_on_circle.get_center());

                captured_point = std::min_element(
                    points.begin(), points.end(),
                    [touch_angle](const auto &p1, const auto &p2) {
                        return std::abs(p1.angle - touch_angle)
                               < std::abs(p2.angle - touch_angle);
                    });
                captured_point->set_update_enabled(false);
                captured_point->angle = touch_angle;
                break;
            }
            case sf::Event::MouseMoved: {
                if (captured_point == points.end()) {
                    break;
                }
                captured_point->angle = get_touch_angle(
                    window.mapPixelToCoords(
                        { event.mouseMove.x, event.mouseMove.y }),
                    repelling_points_on_circle.get_center());
                break;
            }
            case sf::Event::MouseButtonReleased:
                captured_point->set_update_enabled(true);
                captured_point = points.end();
                break;
            default:
                break;
            }
        }

        window.clear();
        window.draw(repelling_points_on_circle);
        window.display();

        // verlet integration conserves energy as delta time -> 0
        // so trying to reduce delta time
        const float elapsed = timer.restart().asSeconds();
        constexpr unsigned N = 100;
        const float delta = elapsed / N;
        for (unsigned n = 0; n != N; ++n) {
            repelling_points_on_circle.update(delta);
        }
    }
}
