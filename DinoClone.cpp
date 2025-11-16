#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "Dino Run",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setPosition(sf::Vector2i(100, 100));
    window.setFramerateLimit(60);

    //All Sprites that will be used

    sf::Texture texture;
    texture.loadFromFile("stylesheet.png");
    if (!texture.loadFromFile("stylesheet.png"))
    {
        std::cout << "Error" << std::endl;
    }
    //Ground Sprite

    sf::Sprite ground(texture), ground_back(texture);
    ground.setTextureRect(sf::IntRect({ 2, 104 }, { 2440, 26 }));
    ground.setPosition(sf::Vector2f(0.f, 520.f));
    ground_back = ground;

    //Dnio sprite and position

    constexpr int height = { 94 };
    constexpr int gap = { 24 };
    sf::Sprite dino(texture);
    dino.setTextureRect(sf::IntRect({ 1678, 2 }, { 88, 94 }));
    dino.setPosition({ 150.f,
        ground.getPosition().y - (height - gap) });
    constexpr int ground_width = 2440;
    float frame_ground = 0.f;

    //Open Windows (not the OS lmao)

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        //Ground is moving uooollll
        frame_ground -= 8.f;
        if (frame_ground < -ground_width) {
            frame_ground = 0.f;
        }
        constexpr int ground_width = { 2440 }; //it's a int about the ground's width
        ground.setPosition(sf::Vector2f(frame_ground, ground.getPosition().y));
        ground_back.setPosition(sf::Vector2f(frame_ground + (ground_width - 40), ground.getPosition().y)); //This basically makes a second ground that will substitute the other while moving from the screen, this makes the transition more fluid

        window.clear(sf::Color::White);
        window.draw(ground);
        window.draw(ground_back);
        window.draw(dino);
        window.display();
    }

    return 0;
}