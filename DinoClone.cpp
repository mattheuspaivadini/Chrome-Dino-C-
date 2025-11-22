#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iomanip>
#include <sstream>

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

    //Count
    sf::Sprite hi(texture);
	hi.setTextureRect(sf::IntRect({ 1494, 2 }, { 38, 21 }));
    hi.setPosition({955, 167});
    float hi_start_x = 1020.f;
    int num_hi = 0;
    float spacing = 20.f;
    constexpr int total = {5};

    //count system
    std::unordered_map<char, sf::IntRect> digits = 
    {
        { '0', sf::IntRect({ 1294, 2 }, { 18, 21 }) },
        { '1', sf::IntRect({ 1316, 2 }, { 18, 21 }) },
        { '2', sf::IntRect({ 1334, 2 }, { 18, 21 }) },
        { '3', sf::IntRect({ 1354, 2 }, { 18, 21 }) },
        { '4', sf::IntRect({ 1374, 2 }, { 18, 21 }) },
        { '5', sf::IntRect({ 1394, 2 }, { 18, 21 }) },
        { '6', sf::IntRect({ 1414, 2 }, { 18, 21 }) },
        { '7', sf::IntRect({ 1434, 2 }, { 18, 21 }) },
        { '8', sf::IntRect({ 1454, 2 }, { 18, 21 }) },
        { '9', sf::IntRect({ 1474, 2 }, { 18, 21 }) }
    };
    float start_x = 1140.f;
    float start_y = hi.getPosition().y;

    //colision

    sf::RectangleShape rect_dino(sf::Vector2f(40.f, 70.f));
    rect_dino.setFillColor(sf::Color::Transparent);
    std::vector<sf::RectangleShape> rect_objs = {};
    bool gamover = { false };

    //Game over sprites & logic
    sf::Sprite sprite_gameover(texture);
    sprite_gameover.setTextureRect(sf::IntRect({1295, 29},{380, 21}));
    sprite_gameover.setPosition(
        sf::Vector2f(
            window.getSize().x / 2.f - sprite_gameover.getTextureRect().size.x / 2.f,
            window.getSize().y / 2.f - sprite_gameover.getTextureRect().size.y / 2.f
        )
    );
    //Restart
    sf::Sprite icon_restart(texture);
    icon_restart.setTextureRect(sf::IntRect({506, 130},{72, 64}));
    icon_restart.setPosition(
        sf::Vector2f(
            window.getSize().x / 2.f - icon_restart.getTextureRect().size.x / 2.f,
            window.getSize().y / 2.f - icon_restart.getTextureRect().size.y / 2.f + 70.f
        )
    );
    
    // Ground
    sf::Sprite ground(texture), ground_back(texture);
    ground.setTextureRect(sf::IntRect({ 2, 104 }, { 2440, 26 }));
    ground.setPosition({ 0.f, 520.f });
    ground_back = ground;

    // height of dino sprite and gap from ground
    constexpr int height = 94; // even with dino.gettexturerect().size.y, DONT REMOVE THIS OR THE DINO WILL BE BUGGED AND I DONT FUCKIN KNOW WHY
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
        // Keys events
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

        if (!gamover)
        {
            //spawning objects system
            int min_distance = 600;
            if (sprites.empty() || (window.getSize().x - sprites.back().getPosition().x) > (min_distance + (std::rand() % 300)))
            {
                int rand_obj = std::rand() % objs.size();
                sf::Sprite sprite(texture);
                sprite.setTextureRect(objs[rand_obj]);

                if (sprite.getTextureRect().size.y == 68)
                {
                    sprite.setPosition(
                        { static_cast<float>(window.getSize().x), 410.f }
                    );
                }
                else
                {
                    sprite.setPosition(
                        { static_cast<float>(window.getSize().x), 520.f - sprite.getTextureRect().size.y + gap }
                    );
                }

                sf::RectangleShape rect_obj(sf::Vector2f(
                    sprite.getTextureRect().size.x,
                    sprite.getTextureRect().size.y
                ));
                rect_obj.setFillColor(sf::Color::Transparent);
                rect_obj.setPosition(sprite.getPosition());
                sprites.push_back(sprite);
                rect_objs.push_back(rect_obj);
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

            // crouch and stand system
            if (crouching)
            {
                dino.setTextureRect(sf::IntRect({ 2206 + 118 * static_cast<int>(frame_down), 36 }, { 118, 60 }));
                dino.setPosition({ dino.getPosition().x, gravity + 30 });
                rect_dino.setSize(sf::Vector2f(100.f, 40.f));
                rect_dino.setPosition(sf::Vector2f(dino.getPosition().x + 20.f, gravity + 40.f));
            }
            else
            {
                dino.setTextureRect(sf::IntRect({ 1678 + 88 * static_cast<int>(frame_dino), 2 }, { 88, 94 }));
                dino.setPosition({ dino.getPosition().x, gravity });
                rect_dino.setSize(sf::Vector2f(40.f, 70.f));
                rect_dino.setPosition(sf::Vector2f(dino.getPosition().x + 20.f, gravity + 10.f));
            }

            // Ground movement
            frame_ground -= 8.f;
            if (frame_ground < -ground_width)
            {
                frame_ground = 0.f;
            }

            // Bird animation
            frame_bird += 0.08f;
            if (frame_bird >= 1.5f)
            {
                frame_bird -= 1.5f;
            }

            for (size_t i{}; i < sprites.size(); ++i)
            {
                if (sprites[i].getTextureRect().size.y == 68)
                {
                    sprites[i].setTextureRect(sf::IntRect({ 260 + 92 * static_cast<int>(frame_bird), 14 }, { 92, 68 }));
                }
                sprites[i].move({ -10.f, 0 });
                rect_objs[i].move({ -10.f, 0 });

                if (rect_dino.getGlobalBounds().findIntersection(rect_objs[i].getGlobalBounds()))
                {
                    gamover = true;
                }

                if (sprites[i].getPosition().x < -sprites[i].getTextureRect().size.x)
                {
                    sprites.erase(sprites.begin() + i);
                    rect_objs.erase(rect_objs.begin() + i);
                }
            }

            // ground position
            ground.setPosition({ frame_ground, ground.getPosition().y });
            ground_back.setPosition({ frame_ground + (ground_width - 40), ground.getPosition().y });

        }

        
            // Render
            window.clear(sf::Color::White);
            window.draw(ground);
            window.draw(ground_back);
            //spawn of objects (birds, cactus)
            for (size_t i{}; i < sprites.size(); ++i)
            {
				window.draw(rect_objs[i]);
                window.draw(sprites[i]);
            }
			window.draw(rect_dino);
            window.draw(dino);
            
            //count
            std::stringstream ss;
            ss << std::setw(total) << std::setfill('0') << count;
            std::string count_str = ss.str();
            float x = start_x;
            for (char digit : count_str)
            {
                sf::Sprite sprite(texture);
                sprite.setTextureRect(digits[digit]);
                sprite.setPosition({ x, start_y });
                window.draw(sprite);
                x += spacing;

            };
			window.draw(hi);

            std::stringstream hi_ss;
            hi_ss << std::setw(total) << std::setfill('0') << num_hi;
            std::string hi_str = hi_ss.str();
            float pos_hi_x = hi_start_x;
            for (char digit : hi_str)
            {
                sf::Sprite sprite(texture);
                sprite.setTextureRect(digits[digit]);
                sprite.setPosition({ pos_hi_x, start_y });
                window.draw(sprite);
                pos_hi_x += spacing;

            };

            if (gamover == true)
            {
                window.draw(sprite_gameover);
                window.draw(icon_restart);
                dino.setTextureRect(sf::IntRect({ 2122, 6 }, { 80, 86 }));
				if (count > num_hi)
                {
                    num_hi = count;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                {
                    count = 0;
                    rect_objs.clear();
                    sprites.clear();
                    gravity = ground.getPosition().y - (height - gap);
                    velocity = 0.f;
                    gamover = false;  
                }
            }
            else
            {
                ++count;
                if (count >= 99999)
                {
                    count = 99999;
                }
            }

            

            window.display();
        }

   return 0;
}