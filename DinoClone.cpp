#include <SFML/Graphics.hpp>

int main()
{

    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode({ 1280, 720 }),
        "Dino Run",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setPosition(sf::Vector2i(100, 100));
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("stylesheet.png"))
    {
        return 1;
    }


    // Setup sprites

    // Ground
    sf::Sprite ground(texture), ground_back(texture);
    ground.setTextureRect(sf::IntRect({ 2, 104 }, { 2440, 26 }));
    ground.setPosition({ 0.f, 520.f });
    ground_back = ground;

    // height of dino sprite and gap from ground
    constexpr int height = 94;
    constexpr int gap = 24;
    constexpr int ground_width = 2440;

    // Dino
    sf::Sprite dino(texture);
    dino.setTextureRect(sf::IntRect({ 1678, 2 }, { 88, 94 }));
    dino.setPosition({ 150.f, ground.getPosition().y - (dino.getTextureRect().size.y - gap)});

    // Objects
    std::vector<sf::IntRect> objs = {
        sf::IntRect({260, 14}, {92, 68}), //bird (w/ 2 animation frames)
        sf::IntRect({446, 2}, {68, 70}), // 2 small cactus
        sf::IntRect({752, 2 }, { 50, 96 }), // 1 large cactus
        sf::IntRect({848, 2}, {104, 98}), // 3 large cactus

    };
    std::vector<sf::Sprite> sprites = {};
    int count = { 0 };
    float frame_bird = 0.f; 
    
    float frame_ground = 0.f;
    float frame_dino = 0.f;

    float gravity = ground.getPosition().y - (height - gap);
    float velocity = 0.f;
    const float jump = -20.f;

    float frame_down = 0.f;
    bool crouching = false;
    bool is_crouching = false;

    while (window.isOpen())
    {
        // Process the events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>()->code;
                if ((key == sf::Keyboard::Key::Space || key == sf::Keyboard::Key::Up) &&
                    gravity == ground.getPosition().y - (dino.getTextureRect().size.y - gap) && !is_crouching)
                {
                    velocity = jump;
                }
                if (key == sf::Keyboard::Key::Down && !is_crouching &&
                    gravity == ground.getPosition().y - (dino.getTextureRect().size.y - gap))
                {
                    crouching = true;
                    is_crouching = true;
                }
            }

          
            if (event->is<sf::Event::KeyReleased>())
            {
                auto key = event->getIf<sf::Event::KeyReleased>()->code;
                if (key == sf::Keyboard::Key::Down && is_crouching)
                {
                    crouching = false;
                    is_crouching = false;
                }
            }
        }

        int rand_distance = std::rand() % 2 + 1;
        if (count % (50 * rand_distance) == 0)
        {
            int rand_obj = std::rand() % objs.size();
            sf::Sprite sprite(texture);
            sprite.setTextureRect(objs[rand_obj]);

            if (sprite.getTextureRect().size.y == 68)
            {
                sprite.setPosition(
                    { window.getSize().x, 410 }
                );
            }
            else
            {
                sprite.setPosition(
                    { window.getSize().x, 520 - sprite.getTextureRect().size.y + gap }
                );
            }

            sprites.push_back(sprite);
        };
        


            // Physics
             velocity += 1.f;
            if (velocity < jump)
            {
                velocity = jump;
            }

            // Apply gravity
            gravity += velocity;
            const float floorY = ground.getPosition().y - (height - gap);
            if (gravity > floorY)
            {
                gravity = floorY;
                velocity = 0.f;
            }

            // Dino animation
            frame_dino += 0.2f;
            if (frame_dino >= 4.f)
            {
                frame_dino -= 4.f;
            }

            

            // Dino crouching
            frame_down += 0.1f;
            if (frame_down > 1.5f)
            {
                frame_down -= 1.5f;
            }

            if (crouching)
            {
                //int runIndex = static_cast<int>(frame_down); // 0..3
                dino.setTextureRect(sf::IntRect({ 2206 + 118 * static_cast<int>(frame_down), 36 }, { 118, 60 }));
                dino.setPosition({ dino.getPosition().x, gravity + 30 });
            }
            else
            {
                //int runIndex = static_cast<int>(frame_dino); // 0..3
                dino.setTextureRect(sf::IntRect({ 1678 + 88 * static_cast<int>(frame_dino), 2 }, { 88, 94 }));
                dino.setPosition({ dino.getPosition().x, gravity });
            }


            // Ground movement
            frame_ground -= 8.f;
            if (frame_ground < -ground_width)
            {
                frame_ground = 0.f;
            }

            for (size_t i{}; i < sprites.size(); ++i)
            {
                if (sprites[i].getTextureRect().size.y == 68)
                {
                    frame_bird += 0.08f;

                    if (frame_bird > 1.5f)
                    {
                        frame_bird -= 1.5f;
                    }
                    sprites[i].setTextureRect(sf::IntRect({ 260 + 92 * static_cast<int>(frame_bird), 14 }, { 92, 67 }));
                }
                sprites[i].move({ -10.f, 0 });
                if (sprites[i].getPosition().x < -sprites[i].getTextureRect().size.x)
                {
                    sprites.erase(sprites.begin() + i);
                }
            }

            for (size_t i {}; i < sprites.size(); ++i)
            {
                window.draw(sprites[i]);
            }

            ground.setPosition({ frame_ground, ground.getPosition().y });
            ground_back.setPosition({ frame_ground + (ground_width - 40), ground.getPosition().y });

            // Render
            window.clear(sf::Color::White);
            window.draw(ground);
            window.draw(ground_back);
            window.draw(dino);

            ++count;
            if (count >= 99999)
            {
                count = 99999;
            }

            window.display();
        }

    
   return 0;
}