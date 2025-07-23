//
// Created by BenN5 on 7/16/2025.
//

#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>


class gui {
public:
    gui(sf::RenderWindow& window, float windowWidth, float windowHeight);
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
};



#endif //GUI_H
