//
// Created by BenN5 on 7/16/2025.
//

#include "gui.h"
#include <iostream>
#include <chrono>

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

    // Title
    sf::Text title("Select Start and Destination", font, 32);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    setText(title, windowWidth / 2, 50);

    // Start
    sf::Text startLabel("Start", font, 24);
    startLabel.setFillColor(sf::Color::White);
    setText(startLabel, windowWidth / 4, 150);

    sf::Text startValue(routePlannerPtr->locations[startIndex].name, font, 24);
    setText(startValue, windowWidth / 4, 180);

    // Destination
    sf::Text destLabel("Destination", font, 24);
    destLabel.setFillColor(sf::Color::White);
    setText(destLabel, windowWidth * 3.0f/4.0f, 150);

    sf::Text destValue(routePlannerPtr->locations[destIndex].name, font, 24);
    setText(destValue, windowWidth * 3/4, 180);

    // Algorithm
    sf::Text algorithmLabel("Algorithm", font, 24);
    algorithmLabel.setFillColor(sf::Color::White);
    setText(algorithmLabel, windowWidth / 4, 300);

    sf::Text algorithmValue("Dijkstra", font, 24);
    setText(algorithmValue, windowWidth / 4, 325);

    // Visualize Button
    sf::RectangleShape visualizeButton(sf::Vector2f(200, 50));
    visualizeButton.setFillColor(sf::Color::Blue);
    visualizeButton.setPosition(windowWidth * 3/4 - 100, 300);
    sf::Text buttonText("Visualize", font, 20);
    setText(buttonText, windowWidth * 3/4, 325);

    // Instructions
    sf::Text instr("Instructions: Use TAB to select field; use right and left arrow keys to change", font, 24);
    instr.setFillColor(sf::Color::White);
    setText(instr, windowWidth / 2, 950);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::KeyPressed) {
                //Press tab to switch between Start and Destination
                if (event.key.code == sf::Keyboard::Tab) {
                    fieldSelection = (fieldSelection + 1) % 3; //switching fields
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    if (fieldSelection == 0) {
                        startIndex = (startIndex - 1) % routePlannerPtr->locations.size();
                        while (startIndex == destIndex){startIndex = (startIndex - 1) % routePlannerPtr->locations.size();}
                        startValue.setString(routePlannerPtr->locations[startIndex].name);
                    } else if (fieldSelection == 1) {
                        destIndex = (destIndex - 1) % routePlannerPtr->locations.size();
                        while (startIndex == destIndex){destIndex = (destIndex - 1) % routePlannerPtr->locations.size();}
                        destValue.setString(routePlannerPtr->locations[destIndex].name);
                    } else if (fieldSelection == 2) {
                        useDijkstra = !useDijkstra;
                        if (useDijkstra) {
                            algorithmValue.setString("Dijkstra");
                        } else {
                            algorithmValue.setString("A*");
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    if (fieldSelection == 0) {
                        startIndex = (startIndex + 1) % routePlannerPtr->locations.size();
                        while (startIndex == destIndex){startIndex = (startIndex + 1) % routePlannerPtr->locations.size();}
                        startValue.setString(routePlannerPtr->locations[startIndex].name);
                    } else if (fieldSelection == 1) {
                        destIndex = (destIndex + 1) % routePlannerPtr->locations.size();
                        while (startIndex == destIndex){destIndex = (destIndex + 1) % routePlannerPtr->locations.size();}
                        destValue.setString(routePlannerPtr->locations[destIndex].name);
                    } else if (fieldSelection == 2) {
                        useDijkstra = !useDijkstra;
                        if (useDijkstra) {
                            algorithmValue.setString("Dijkstra");
                        } else {
                            algorithmValue.setString("A*");
                        }
                    }
                }

            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (visualizeButton.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
                        //logic to go to link
                        int source = routePlannerPtr->vertexFromLatLong(routePlannerPtr->locations[startIndex].latitude, routePlannerPtr->locations[startIndex].longitude);
                        int dest = routePlannerPtr->vertexFromLatLong(routePlannerPtr->locations[destIndex].latitude, routePlannerPtr->locations[destIndex].longitude);
                        routePlannerPtr->setSrc(source);
                        routePlannerPtr->setDest(dest);
                        auto clock_start = chrono::high_resolution_clock::now();
                        if (useDijkstra) {
                            cout << "Using Dijkstra's algorithm." << endl;
                            routePlannerPtr->dijkstra();
                        } else {
                            cout << "Using A* algorithm." << endl;
                            routePlannerPtr->aStar();
                        };
                        auto clock_end = chrono::high_resolution_clock::now();
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock_start);
                        std::cout << "Route finding algorithm took " << elapsed.count() << " milliseconds" << std::endl;
                        routePlannerPtr->plotRoute();
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        window.draw(title);
        window.draw(startLabel);
        window.draw(startValue);
        window.draw(destLabel);
        window.draw(destValue);
        window.draw(algorithmLabel);
        window.draw(algorithmValue);
        window.draw(instr);
        window.draw(visualizeButton);
        window.draw(buttonText);
        //To highlight the selected option
        if (fieldSelection == 0) {
            startLabel.setFillColor(sf::Color::Yellow);
            destLabel.setFillColor(sf::Color::White);
            algorithmLabel.setFillColor(sf::Color::White);
        }
        else if (fieldSelection == 1) {
            startLabel.setFillColor(sf::Color::White);
            destLabel.setFillColor(sf::Color::Yellow);
            algorithmLabel.setFillColor(sf::Color::White);
        } else if (fieldSelection == 2) {
            startLabel.setFillColor(sf::Color::White);
            destLabel.setFillColor(sf::Color::White);
            algorithmLabel.setFillColor(sf::Color::Yellow);
        }

        window.display();
    }
}
//helper function to center text
void gui::setText(sf::Text &text, const float x, const float y) {
    const sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
