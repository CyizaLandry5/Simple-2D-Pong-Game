#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 20.0f;
const float PADDLE_HEIGHT = 100.0f;
const float PADDLE_SPEED = 500.0f;
const float BALL_SIZE = 15.0f;
const float BALL_SPEED_INITIAL = 400.0f;
const float BALL_SPEED_INCREMENT = 25.0f;
const int MAX_SCORE = 10;

// Paddle class
class Paddle {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    int score;

    Paddle(float x, float y) {
        shape.setSize(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(PADDLE_WIDTH / 2, PADDLE_HEIGHT / 2);
        velocity.y = 0;
        score = 0;
    }

    void update(float deltaTime) {
        // Move paddle
        shape.move(velocity * deltaTime);

        // Keep paddle within window bounds
        if (shape.getPosition().y - PADDLE_HEIGHT / 2 < 0) {
            shape.setPosition(shape.getPosition().x, PADDLE_HEIGHT / 2);
        }
        if (shape.getPosition().y + PADDLE_HEIGHT / 2 > WINDOW_HEIGHT) {
            shape.setPosition(shape.getPosition().x, WINDOW_HEIGHT - PADDLE_HEIGHT / 2);
        }
    }

    void moveUp() {
        velocity.y = -PADDLE_SPEED;
    }

    void moveDown() {
        velocity.y = PADDLE_SPEED;
    }

    void stop() {
        velocity.y = 0;
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};

// Ball class
class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball() {
        shape.setRadius(BALL_SIZE);
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(BALL_SIZE, BALL_SIZE);
        
        // Initial random direction
        float angle = (std::rand() % 120 - 60) * 3.14159f / 180.0f;
        velocity.x = BALL_SPEED_INITIAL * std::cos(angle);
        velocity.y = BALL_SPEED_INITIAL * std::sin(angle);
        
        // Randomly choose left or right direction
        if (std::rand() % 2 == 0) {
            velocity.x *= -1;
        }
    }

    void update(float deltaTime) {
        shape.move(velocity * deltaTime);
    }

    void reset() {
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        
        // New random direction
        float angle = (std::rand() % 120 - 60) * 3.14159f / 180.0f;
        velocity.x = BALL_SPEED_INITIAL * std::cos(angle);
        velocity.y = BALL_SPEED_INITIAL * std::sin(angle);
        
        // Randomly choose left or right direction
        if (std::rand() % 2 == 0) {
            velocity.x *= -1;
        }
    }

    void increaseSpeed() {
        // Increase speed while maintaining direction
        float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        float directionX = velocity.x / speed;
        float directionY = velocity.y / speed;
        
        speed += BALL_SPEED_INCREMENT;
        velocity.x = directionX * speed;
        velocity.y = directionY * speed;
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};

// Game class
class PongGame {
private:
    sf::RenderWindow window;
    Paddle player1;
    Paddle player2;
    Ball ball;
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text restartText;
    bool gameOver;
    bool isPaused;

public:
    PongGame() : 
        window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Simple Pong Game"),
        player1(50.0f, WINDOW_HEIGHT / 2),
        player2(WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT / 2),
        gameOver(false),
        isPaused(false)
    {
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            // If font file not found, use a default system font
            font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
        }
        
        // Setup score text
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(WINDOW_WIDTH / 2 - 50, 20);
        
        // Setup game over text
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::Yellow);
        gameOverText.setString("Game Over!");
        gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2, 
                                WINDOW_HEIGHT / 2 - 60);
        
        // Setup restart text
        restartText.setFont(font);
        restartText.setCharacterSize(30);
        restartText.setFillColor(sf::Color::Green);
        restartText.setString("Press R to restart or ESC to exit");
        restartText.setPosition(WINDOW_WIDTH / 2 - restartText.getLocalBounds().width / 2, 
                               WINDOW_HEIGHT / 2 + 20);
        
        updateScoreText();
    }

    void run() {
        sf::Clock clock;
        
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            processEvents();
            if (!gameOver && !isPaused) {
                update(deltaTime);
            }
            render();
        }
    }

private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            
            if (event.type == sf::Event::KeyReleased) {
                handleKeyRelease(event.key.code);
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Escape) {
            window.close();
        }
        
        if (gameOver) {
            if (key == sf::Keyboard::R) {
                resetGame();
            }
            return;
        }
        
        if (key == sf::Keyboard::P) {
            isPaused = !isPaused;
        }
        
        if (isPaused) return;
        
        // Player 1 controls (W, S)
        if (key == sf::Keyboard::W) {
            player1.moveUp();
        }
        if (key == sf::Keyboard::S) {
            player1.moveDown();
        }
        
        // Player 2 controls (Up, Down)
        if (key == sf::Keyboard::Up) {
            player2.moveUp();
        }
        if (key == sf::Keyboard::Down) {
            player2.moveDown();
        }
    }

    void handleKeyRelease(sf::Keyboard::Key key) {
        if (gameOver || isPaused) return;
        
        // Player 1 controls
        if (key == sf::Keyboard::W && player1.velocity.y < 0) {
            player1.stop();
        }
        if (key == sf::Keyboard::S && player1.velocity.y > 0) {
            player1.stop();
        }
        
        // Player 2 controls
        if (key == sf::Keyboard::Up && player2.velocity.y < 0) {
            player2.stop();
        }
        if (key == sf::Keyboard::Down && player2.velocity.y > 0) {
            player2.stop();
        }
    }

    void update(float deltaTime) {
        // Update paddles
        player1.update(deltaTime);
        player2.update(deltaTime);
        
        // Update ball
        ball.update(deltaTime);
        
        // Ball collision with top and bottom
        if (ball.shape.getPosition().y - BALL_SIZE < 0) {
            ball.shape.setPosition(ball.shape.getPosition().x, BALL_SIZE);
            ball.velocity.y = -ball.velocity.y;
        }
        if (ball.shape.getPosition().y + BALL_SIZE > WINDOW_HEIGHT) {
            ball.shape.setPosition(ball.shape.getPosition().x, WINDOW_HEIGHT - BALL_SIZE);
            ball.velocity.y = -ball.velocity.y;
        }
        
        // Ball collision with paddles
        if (ball.getBounds().intersects(player1.getBounds())) {
            handlePaddleCollision(player1);
        }
        if (ball.getBounds().intersects(player2.getBounds())) {
            handlePaddleCollision(player2);
        }
        
        // Ball out of bounds (scoring)
        if (ball.shape.getPosition().x < 0) {
            // Player 2 scores
            player2.score++;
            updateScoreText();
            ball.reset();
            
            if (player2.score >= MAX_SCORE) {
                gameOver = true;
                gameOverText.setString("Player 2 Wins!");
                gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2, 
                                        WINDOW_HEIGHT / 2 - 60);
            }
        }
        if (ball.shape.getPosition().x > WINDOW_WIDTH) {
            // Player 1 scores
            player1.score++;
            updateScoreText();
            ball.reset();
            
            if (player1.score >= MAX_SCORE) {
                gameOver = true;
                gameOverText.setString("Player 1 Wins!");
                gameOverText.setPosition(WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2, 
                                        WINDOW_HEIGHT / 2 - 60);
            }
        }
    }

    void handlePaddleCollision(Paddle& paddle) {
        // Move ball outside the paddle to prevent sticking
        if (ball.velocity.x > 0) {
            ball.shape.setPosition(paddle.shape.getPosition().x - PADDLE_WIDTH / 2 - BALL_SIZE, 
                                  ball.shape.getPosition().y);
        } else {
            ball.shape.setPosition(paddle.shape.getPosition().x + PADDLE_WIDTH / 2 + BALL_SIZE, 
                                  ball.shape.getPosition().y);
        }
        
        // Reverse X direction
        ball.velocity.x = -ball.velocity.x;
        
        // Adjust Y direction based on where the ball hit the paddle
        float hitPosition = (ball.shape.getPosition().y - paddle.shape.getPosition().y) / (PADDLE_HEIGHT / 2);
        ball.velocity.y = hitPosition * BALL_SPEED_INITIAL;
        
        // Increase ball speed
        ball.increaseSpeed();
    }

    void updateScoreText() {
        std::stringstream ss;
        ss << player1.score << " - " << player2.score;
        scoreText.setString(ss.str());
        scoreText.setPosition(WINDOW_WIDTH / 2 - scoreText.getLocalBounds().width / 2, 20);
    }

    void resetGame() {
        player1.score = 0;
        player2.score = 0;
        player1.shape.setPosition(50.0f, WINDOW_HEIGHT / 2);
        player2.shape.setPosition(WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT / 2);
        player1.stop();
        player2.stop();
        ball.reset();
        gameOver = false;
        isPaused = false;
        updateScoreText();
    }

    void render() {
        window.clear(sf::Color::Black);
        
        // Draw center line
        sf::RectangleShape centerLine(sf::Vector2f(2, WINDOW_HEIGHT));
        centerLine.setPosition(WINDOW_WIDTH / 2, 0);
        centerLine.setFillColor(sf::Color(100, 100, 100));
        window.draw(centerLine);
        
        // Draw game objects
        window.draw(player1.shape);
        window.draw(player2.shape);
        window.draw(ball.shape);
        
        // Draw score
        window.draw(scoreText);
        
        // Draw game over screen
        if (gameOver) {
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);
            window.draw(gameOverText);
            window.draw(restartText);
        }
        
        // Draw pause screen
        if (isPaused && !gameOver) {
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);
            
            sf::Text pauseText;
            pauseText.setFont(font);
            pauseText.setCharacterSize(50);
            pauseText.setFillColor(sf::Color::Yellow);
            pauseText.setString("Game Paused");
            pauseText.setPosition(WINDOW_WIDTH / 2 - pauseText.getLocalBounds().width / 2, 
                                 WINDOW_HEIGHT / 2 - 60);
            window.draw(pauseText);
            
            sf::Text continueText;
            continueText.setFont(font);
            continueText.setCharacterSize(30);
            continueText.setFillColor(sf::Color::Green);
            continueText.setString("Press P to continue");
            continueText.setPosition(WINDOW_WIDTH / 2 - continueText.getLocalBounds().width / 2, 
                                   WINDOW_HEIGHT / 2 + 20);
            window.draw(continueText);
        }
        
        window.display();
    }
};

int main() {
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    try {
        PongGame game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
