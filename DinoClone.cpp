#include <SFML/Graphics.hpp>

int main()
{
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
    dino.setPosition({ 150.f, ground.getPosition().y - (height - gap) });


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
                    gravity == ground.getPosition().y - (height - gap) && !is_crouching)
                {
                    velocity = jump;
                }
                if (key == sf::Keyboard::Key::Down && !is_crouching &&
                    gravity == ground.getPosition().y - (height - gap))
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
                int runIndex = static_cast<int>(frame_down); // 0..3
                dino.setTextureRect(sf::IntRect({ 2206 + 118 * runIndex, 36 }, { 118, 60 }));
                dino.setPosition({ dino.getPosition().x, gravity + 30 });
            }
            else
            {
                int runIndex = static_cast<int>(frame_dino); // 0..3
                dino.setTextureRect(sf::IntRect({ 1678 + 88 * runIndex, 2 }, { 88, 94 }));
                dino.setPosition({ dino.getPosition().x, gravity });
            }


            // Ground movement
            frame_ground -= 8.f;
            if (frame_ground < -ground_width)
            {
                frame_ground = 0.f;
            }

            ground.setPosition({ frame_ground, ground.getPosition().y });
            ground_back.setPosition({ frame_ground + (ground_width - 40), ground.getPosition().y });

            // Render
            window.clear(sf::Color::White);
            window.draw(ground);
            window.draw(ground_back);
            window.draw(dino);
            window.display();
        }

    
   return 0;
}