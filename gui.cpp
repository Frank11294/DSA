//
// Created by BenN5 on 7/16/2025.
//

#include "gui.h"
#include <iostream>

gui::gui(sf::RenderWindow& window, float windowWidth, float windowHeight):
window(window), windowWidth(windowWidth), windowHeight(windowHeight) {
}

void gui::run() {
    //load font
    if (!font.loadFromFile("files/bahnschrift.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    //background image
    if (!backgroundTexture.loadFromFile("files/images/statue_of_liberty.png")) {
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
    // Start label
    sf::Text start("Start", font, 24);
    start.setFillColor(sf::Color::White);
    setText(start, windowWidth / 4, 150);

    // Destination label
    sf::Text end("Destination", font, 24);
    end.setFillColor(sf::Color::White);
    setText(end, windowWidth * 3.0f/4.0f, 150);

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

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    std::cout << "Start selected\n";
                    // You can set internal state here
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    std::cout << "Destination selected\n";
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    std::cout << "Visualize triggered\n";
                    // You can call BRIDGES logic later from here
                    window.close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (button.getGlobalBounds().contains(static_cast<float>(position.x), static_cast<float>(position.y))) {
                        //logic to go to link
                        std::cout << "Visualize triggered" << std::endl;
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        window.draw(title);
        window.draw(start);
        window.draw(end);
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
