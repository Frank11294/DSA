//
// Created by BenN5 on 7/16/2025.
//

#include "gui.h"
#include <iostream>

gui::gui(sf::RenderWindow& window, float windowWidth, float windowHeight):
window(window), windowWidth(windowWidth), windowHeight(windowHeight) {
}

void gui::setRoutePlanner(routePlanner *routePlanner) {
    routePlannerPtr = routePlanner;
}

void gui::run() {
    window.setFramerateLimit(60);

    //load font
    if (!font.loadFromFile("../libraries/files/bahnschrift.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    //background image
    if (!backgroundTexture.loadFromFile("../libraries/files/images/statue_of_liberty.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return;
    }

    backgroundSprite.setTexture(backgroundTexture);
    float factor_x = windowWidth / static_cast<float>(backgroundTexture.getSize().x);
    float factor_y = windowHeight / static_cast<float>(backgroundTexture.getSize().y);
    backgroundSprite.setScale(factor_x, factor_y);

    //locations
    ufLocations = {"Reitz Union", "Marston Library", "Ben Hill Griffin Stadium", "Lib West", "MacDinton's"};
    usLandmarks = {"Statue of Liberty", "Golden Gate Bridge", "Grand Canyon", "Mount Rushmore"};

    // Title
    sf::Text title("Select Start and Destination", font, 32);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    setText(title, windowWidth / 2, 50);

    // Start label
    sf::Text start("Start", font, 24);
    start.setFillColor(sf::Color::White);
    setText(start, windowWidth / 4, 150);

    sf::Text startValue(ufLocations[startIndex], font, 24);
    setText(startValue, windowWidth / 4, 180);

    // Destination label
    sf::Text end("Destination", font, 24);
    end.setFillColor(sf::Color::White);
    setText(end, windowWidth * 3.0f/4.0f, 150);

    sf::Text destValue(usLandmarks[destIndex], font, 24);
    setText(destValue, windowWidth * 3/4, 180);

    // Visualize Button
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(windowWidth / 2 - 100, 300);
    sf::Text buttonText("Visualize", font, 20);
    setText(buttonText, windowWidth / 2, 325);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::KeyPressed) {
                //Press tab to switch between Start and Destination
                if (event.key.code == sf::Keyboard::Tab) {
                    selectingStart = !selectingStart; //switching between start and dest
                    if (selectingStart) {
                        start.setFillColor(sf::Color::Yellow);
                        end.setFillColor(sf::Color::White);
                    }
                    else {
                        start.setFillColor(sf::Color::White);
                        end.setFillColor(sf::Color::Yellow);
                    }
                    // You can set internal state here
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    if (selectingStart) {
                        startIndex = (startIndex -1 + ufLocations.size()) % usLandmarks.size();
                        startValue.setString(ufLocations[startIndex]);
                    } else {
                        destIndex = (destIndex - 1 + usLandmarks.size()) % usLandmarks.size();
                        destValue.setString(usLandmarks[destIndex]);
                    }
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    if (selectingStart) {
                        startIndex = (startIndex + 1) % ufLocations.size();
                        startValue.setString(ufLocations[startIndex]);
                    } else {
                        destIndex = (destIndex + 1) % usLandmarks.size();
                        destValue.setString(usLandmarks[destIndex]);
                    }
                }

                // if (event.key.code == sf::Keyboard::Num2) {
                //     std::cout << "Destination selected\n";
                // }
                // if (event.key.code == sf::Keyboard::Enter) {
                //     std::cout << "Visualize triggered\n";
                //     // You can call BRIDGES logic later from here
                //     window.close();
                // }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (button.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
                        //logic to go to link
                        std::cout << "Visualize triggered" << std::endl;
                        // using some arbitrary latitude and longitude points to demonstrate
                        // todo: need to provide appropriate lat, long from ui selection
                        int source = routePlannerPtr->vertexFromLatLong(40.719,-74.000);
                        int dest = routePlannerPtr->vertexFromLatLong(40.793, -73.953);
                        routePlannerPtr->setSrc(source);
                        routePlannerPtr->setDest(dest);
                        routePlannerPtr->dijkstra();
                        routePlannerPtr->plotRoute();
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        window.draw(title);
        window.draw(start);
        window.draw(startValue);
        window.draw(end);
        window.draw(destValue);
        window.draw(button);
        window.draw(buttonText);

        window.display();
    }
}

void gui::setText(sf::Text &text, const float x, const float y) {
    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
