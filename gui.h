//
// Created by BenN5 on 7/16/2025.
//

#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>

#include "routePlanner.h"


class gui {
public:
    gui(sf::RenderWindow& window, float windowWidth, float windowHeight);
    void setRoutePlanner(routePlanner* routePlanner);
    void run();
    // void isButtonPressed(sf::Event::Event& e);
    float windowWidth;
    float windowHeight;

private:
    static void setText(sf::Text &text, float x, float y);
    sf::RenderWindow& window;
    sf::Text text;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text welcome_text;
    std::vector<std::string> ufLocations;
    std::vector<std::string> usLandmarks;
    int startIndex = 0;
    int destIndex = 0;
    bool selectingStart = true;
    routePlanner* routePlannerPtr;
};



#endif //GUI_H
