#include <SFML/Graphics.hpp>
#include <sstream> 
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);
const int NUM_OF_BRANCHES = 5;
Sprite branches[NUM_OF_BRANCHES];

enum class side {LEFT, RIGHT, NONE};
side branchPosition[NUM_OF_BRANCHES];

int main()
{
    const float TREE_HORIZONTAL_POSITION = 810;
    const float TREE_VERTICAL_POSITION = 0;

    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber", Style::Fullscreen);
    
    Texture branchTexture;
    branchTexture.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_OF_BRANCHES; i++) {
        branches[i].setTexture(branchTexture);
        //hide it off screen
        branches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    Text messageText;
    messageText.setString("Press Enter to start");
    messageText.setCharacterSize(75);
    messageText.setFillColor(Color::White);
    messageText.setFont(font);

    Text scoreText;
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(75);
    scoreText.setFillColor(Color::White);
    scoreText.setFont(font);

    Text fpsText;
    fpsText.setString("FPS = ");
    fpsText.setCharacterSize(75);
    fpsText.setFillColor(Color::White);
    fpsText.setFont(font);
    fpsText.setPosition(1200, 20);


    RectangleShape scoreTextContainer;
    scoreTextContainer.setFillColor(sf::Color(0, 0, 0, 150));
    scoreTextContainer.setSize(Vector2f(600, 105));
    scoreTextContainer.setPosition(0, 30);

    RectangleShape fpsTextContainer;
    fpsTextContainer.setFillColor(sf::Color(0, 0, 0, 150));
    fpsTextContainer.setSize(Vector2f(600, 105));
    fpsTextContainer.setPosition(0, 30);

    //positioning score on top left and HUD message on exact middle of screen
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f
    );
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    //horizontally centered
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 5.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    backgroundTexture.loadFromFile("graphics/background.png");
    backgroundSprite.setTexture(backgroundTexture);

    Texture treeTexture;
    Sprite treeSprite;

    treeTexture.loadFromFile("graphics/tree.png");
    treeSprite.setTexture(treeTexture);
    treeSprite.setPosition(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION);

    Texture cloudTexture;
    Sprite cloudSprite1;
    Sprite cloudSprite2;
    Sprite cloudSprite3;

    cloudTexture.loadFromFile("graphics/cloud.png");
    cloudSprite1.setTexture(cloudTexture);
    cloudSprite2.setTexture(cloudTexture);
    cloudSprite3.setTexture(cloudTexture);

    cloudSprite1.setPosition(0, 0);
    cloudSprite2.setPosition(0, 250);
    cloudSprite3.setPosition(0, 500);

    bool isCloud1Active = false;
    bool isCloud2Active = false;
    bool isCloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    Texture playerTexture;
    playerTexture.loadFromFile("graphics/player.png");


    const float PLAYER_POSITION_LEFT = 700.0f;
    const float PLAYER_POSITION_RIGHT = 1200.0f;

    Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(-1, 1); // facing left first
    playerSprite.setPosition(PLAYER_POSITION_LEFT, 720);

    //player starts on the left
    side playerSide = side::LEFT;

    Texture graveStoneTexture;
    graveStoneTexture.loadFromFile("graphics/rip.png");

    Sprite graveStoneSprite;
    graveStoneSprite.setTexture(graveStoneTexture);
    graveStoneSprite.setPosition(600, 860);

    Texture axeTexure;
    axeTexure.loadFromFile("graphics/axe.png");

    Sprite axeSprite;
    axeSprite.setTexture(axeTexure);
    axeSprite.setPosition(700, 830);

    //Allign axe with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    Texture logTexture;
    logTexture.loadFromFile("graphics/log.png");

    Sprite logSprite;
    logSprite.setTexture(logTexture);
    logSprite.setPosition(810, 720);

    bool isLogActive = false;
    float logSpeedX = 1000.0f;
    float logSpeedY = -1500.0f;


    Texture beeTexture;
    Sprite beeSprite;

    beeTexture.loadFromFile("graphics/bee.png");
    beeSprite.setTexture(beeTexture);
    beeSprite.setPosition(0, 800);

    bool isBeeActive = false;
    float beeSpeed = 0.0f;

    Clock clock;
    bool isPaused = true;
    bool acceptPlayerInput = false;

    int score = 0;
    //loading texture type to GPU is slow, so don't add in game 
    //loop use sprites to access textures that are already in GPU
    //in loop/runtime

    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chopSound;
    chopSound.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound deathSound;
    deathSound.setBuffer(deathBuffer);

    SoundBuffer outOfTimeBuffer;
    outOfTimeBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTimeSound;
    outOfTimeSound.setBuffer(outOfTimeBuffer);

    int lastDrawn = 0; //to how many loop/frames will only update fps every 100 frames

    updateBranches(2);
    updateBranches(5);
    updateBranches(4);
    updateBranches(3);
    updateBranches(1);



    while (window.isOpen())
    {
        if(Keyboard::isKeyPressed(Keyboard::Enter))
        {
            //reset/new game
            isPaused = false;

            
            score = 0;
            timeRemaining = 5.0f;
            // Make all the branches disappear
            for (int i = 1; i < NUM_OF_BRANCHES; i++)
            {
                branchPosition[i] = side::NONE;
            }
            // Make sure the gravestone is hidden
            graveStoneSprite.setPosition(675, 2000);
            // Move the player into position
            playerSprite.setPosition(580, 720);
            acceptPlayerInput = true;
           

        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            isPaused = true;
        }

        if(Keyboard::isKeyPressed(Keyboard::C))
        {
            window.close();
        }

        Event event;
        while (window.pollEvent(event))
        {
            //check for keyRelease event/when done chopping wood
            if (event.type == Event::KeyReleased && !isPaused)
            {
                acceptPlayerInput = true;
                //hide axe again
                axeSprite.setPosition(2000, axeSprite.getPosition().y);
            }
        }

        if (acceptPlayerInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                //Add to time remaining
                timeRemaining += (2 / score) + 0.15f; //increment gets smaller as score gets higher
                axeSprite.setPosition(AXE_POSITION_RIGHT, axeSprite.getPosition().y);
                
                //face right
                playerSprite.setScale(1,1);
                playerSprite.setPosition(PLAYER_POSITION_RIGHT, playerSprite.getPosition().y);
                updateBranches(score);

                // Set the log flying to the left
                logSprite.setPosition(810, 720);
                logSpeedX = -5000;
                
                chopSound.play();
                isLogActive = true;
              
                acceptPlayerInput = false;

                
            }
            else if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                //Add to time remaining
                timeRemaining += (2 / score) + 0.15f;
                axeSprite.setPosition(AXE_POSITION_LEFT, axeSprite.getPosition().y);
                //face right
                playerSprite.setScale(-1, 1);
                playerSprite.setPosition(PLAYER_POSITION_LEFT, playerSprite.getPosition().y);

                updateBranches(score);

                // Set the log flying to the right
                logSprite.setPosition(810, 720);
                logSpeedX = 5000;

                chopSound.play();
                isLogActive = true;
                acceptPlayerInput = false;
            }
        }

        window.clear();

        if (!isPaused)
        {
            //Updating scene
            //Using time makes the frame rate consistent
            Time dt = clock.restart(); //how much time elapsed since last loop

            timeRemaining -= dt.asSeconds();

            timeBar.setSize(Vector2f(timeBarWidthPerSecond *
                timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                isPaused = true;
                messageText.setString("Out of time");

                //need to recenter messageText cos text is different
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                outOfTimeSound.play();
            }

            lastDrawn++;
            if (lastDrawn == 1000)
            {
                // Update the score text
                std::stringstream ss;
                ss << "Score = " << score;
                scoreText.setString(ss.str());

                // Draw the fps
                std::stringstream ss2;
                ss2 << "FPS = " << 1 / dt.asSeconds();
                fpsText.setString(ss2.str());
                lastDrawn = 0;
            }
            
            // Setup the bee
            if (!isBeeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;//random number between 200-399
                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 700;//random number between 500-999
                beeSprite.setPosition(2000, height);
                isBeeActive = true;
            }
            else
                // Move the bee
            {
                beeSprite.setPosition(
                    beeSprite.getPosition().x -
                    (beeSpeed * dt.asSeconds()),
                    beeSprite.getPosition().y);
                // Has the bee reached the left-hand edge of the screen?
                if (beeSprite.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new bee next frame
                    isBeeActive = false;
                }
            }


            // Manage the clouds
            if (!isCloud1Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                cloudSprite1.setPosition(-200, height);
                isCloud1Active = true;

            }
            else
            {
                cloudSprite1.setPosition(
                    cloudSprite1.getPosition().x +
                    (cloud1Speed * dt.asSeconds()),
                    cloudSprite1.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (cloudSprite1.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    isCloud1Active = false;
                }
            }

            // Manage the clouds
            if (!isCloud2Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 18);
                cloud2Speed = (rand() % 200);

                // How high is the cloud
                srand((int)time(0) * 18);
                float height = (rand() % 150);
                cloudSprite2.setPosition(-200, height);
                isCloud2Active = true;

            }
            else
            {
                cloudSprite2.setPosition(
                    cloudSprite2.getPosition().x +
                    (cloud2Speed * dt.asSeconds()),
                    cloudSprite2.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (cloudSprite2.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    isCloud2Active = false;
                }
            }


            // Manage the clouds
            if (!isCloud3Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 23);
                cloud3Speed = (rand() % 200);

                // How high is the cloud
                srand((int)time(0) * 23);
                float height = (rand() % 150);
                cloudSprite3.setPosition(-200, height);
                isCloud3Active = true;

            }
            else
            {
                cloudSprite3.setPosition(
                    cloudSprite3.getPosition().x +
                    (cloud3Speed * dt.asSeconds()),
                    cloudSprite3.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (cloudSprite3.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    isCloud3Active = false;
                }
            }
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_OF_BRANCHES; i++) 
            {
                float height = i * 150;
                if(branchPosition[i] == side::LEFT)
                {
                // Move the sprite to the left side
                branches[i].setPosition(610, height);
                // Flip the sprite round the other way
                branches[i].setRotation(180);
                }
                else if (branchPosition[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);
                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }
            
            //handle flying log/chopped wood
            if (isLogActive)
            {
                logSprite.setPosition(
                    logSprite.getPosition().x +
                    (logSpeedX * dt.asSeconds()),
                    logSprite.getPosition().y +
                    (logSpeedY * dt.asSeconds())
                );
                //if log is outside of frame
                if (logSprite.getPosition().x < -100 ||
                    logSprite.getPosition().x > 2000)
                {
                    isLogActive = false;
                    logSprite.setPosition(810, 720);
                }
            }

            if (branchPosition[4] == playerSide)
            {
                //player dead
                isPaused = true;
                acceptPlayerInput = false;

                deathSound.play();
                graveStoneSprite.setPosition(525, 760);
                playerSprite.setPosition(2000, 660);
                messageText.setString("YOU DIED");

                //need to recenter message text
               messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f,
                    1080 / 2.0f);
            }
         
        }

        
        
        //order in which they drawn
        window.draw(backgroundSprite);
        window.draw(cloudSprite1);
        window.draw(cloudSprite2);
        window.draw(cloudSprite3);
        
        
       

        for (int i = 0; i < NUM_OF_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(treeSprite);
        window.draw(playerSprite);
        window.draw(axeSprite);
        window.draw(logSprite);
        window.draw(graveStoneSprite);
        window.draw(beeSprite);
        window.draw(scoreTextContainer);
        window.draw(fpsTextContainer);
        window.draw(scoreText);
        window.draw(fpsText);

        window.draw(timeBar);
        

        if (isPaused) {
            window.draw(messageText);
        }

        

        window.display();
    }

    return 0;
}

void updateBranches(int seed)
{

    for (int i = NUM_OF_BRANCHES; i > 0; i--)
    {
        branchPosition[i] = branchPosition[i - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
    case 0:
        branchPosition[0] = side::LEFT;
        break;
    case 1:
        branchPosition[0] = side::RIGHT;
        break;
    default:
        branchPosition[0] = side::NONE;
        break;
    }
}