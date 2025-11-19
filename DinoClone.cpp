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

    //All Sprites that will be used

    sf::Texture texture;
    texture.loadFromFile("stylesheet.png");
    if (!texture.loadFromFile("stylesheet.png"))
    {
        return 1;
    }
    //Ground Sprite

    sf::Sprite ground(texture), ground_back(texture);
    ground.setTextureRect(sf::IntRect({ 2, 104 }, { 2440, 26 }));
    ground.setPosition(sf::Vector2f(0.f, 520.f));
    ground_back = ground;

    //Dino sprite and position

    constexpr int height = { 94 };
    constexpr int gap = { 24 };
    sf::Sprite dino(texture);
    dino.setTextureRect(sf::IntRect({ 1678, 2 }, { 88, 94 }));
    dino.setPosition({ 150.f, ground.getPosition().y - (height - gap) });
    constexpr int ground_width = 2440;

    float frame_ground = 0.f;
    float frame_dino = 0.f;

    //floats for the jumping action
    float gravity = ground.getPosition().y - (height - gap);
    float velocity = 0.f;
    float jump = -20;

    float frame_down = 0.f;
    bool crouch = { false };
    bool is_crouching = { false };

    //Open Windows (not the OS lmao)

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>())
            {
                //Key Space is pressed
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space && gravity == ground.getPosition().y - (height - gap))
                {
                    velocity = jump;

                }
                //Key Up is pressed
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up && gravity == ground.getPosition().y - (height - gap))
                {
                    velocity = jump;
                }
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down && !is_crouching && gravity == ground.getPosition().y - (height - gap))
                {
                    crouch = true;
                    is_crouching = true;
                }
                 if (event->is<sf::Event::KeyReleased>())
                    {
                     if (event->getIf<sf::Event::KeyReleased>()->code == sf::Keyboard::Key::Down)
                        {
                            crouch = false;
                            is_crouching = false;
                        }
                    }

        }

        if(!crouch) {
           velocity += 1.f;
           if (velocity < jump) {
           velocity = jump;
           }
        }

        gravity += velocity;
        if (gravity > ground.getPosition().y - (height - gap))
        {
            gravity = ground.getPosition().y - (height - gap);
            velocity = 0.f;
        }
        //Dino is moving
        frame_dino += 0.2f;
		if (frame_dino >= 4.f) {
            frame_dino -= 4.f;
        }

        frame_down += 0.1f;
        if (frame_down > 1.5f) {
            frame_down -= 1.5f;
        }

        //Ground is moving uooollll
        frame_ground -= 8.f;
        if (frame_ground < -ground_width) {
            frame_ground = 0.f;
        }

        //Makes the ground change
        constexpr int ground_width = { 2440 }; //it's a int about the ground's width
        ground.setPosition(sf::Vector2f(frame_ground, ground.getPosition().y));
        ground_back.setPosition(sf::Vector2f(frame_ground + (ground_width - 40), ground.getPosition().y)); //This basically makes a second ground that will substitute the other while moving from the screen, this makes the transition more fluid
        if(crouch) 
        {
       
            
            dino.setTextureRect(sf::IntRect({ 2206 + 188 * static_cast<int>(frame_down), 36 }, { 118, 60}));
            dino.setPosition(sf::Vector2f(dino.getPosition().x, gravity + 30));

        }
        else
        {
            //makes the animation of the dinossaur
     
            dino.setTextureRect(sf::IntRect({ 1678 + 88 * static_cast<int>(frame_dino), 2 }, { 88, 94 }));
            dino.setPosition(sf::Vector2f(dino.getPosition().x, gravity));
        }
        
        //Change the position of dino when the Space key is pressed
        dino.setPosition(sf::Vector2f(dino.getPosition().x, gravity));

        window.clear(sf::Color::White);
        window.draw(ground);
        window.draw(ground_back);
        window.draw(dino);
        window.display();
    }

    return 0;
}