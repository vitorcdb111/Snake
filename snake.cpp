#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/AlResource.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <sstream>
#include <iostream>
#include <list>
#include <cstdlib>
#include <unistd.h>

//definindo as constante do tamanho da tela
const int numRows = 20, numCols = 30, blocksize = 40; 

//posicao da cobra  
struct point
{
    int row, col;
};

void draw_field(sf::RenderWindow &window, sf::RectangleShape &fieldBlock)
{
    sf::Texture grass;

    if(!grass.loadFromFile("Assets/Sprites/grass_00.png", sf::IntRect(40, 40, 40, 40)))
    {
        for(int i=0; i < numRows; i++)
        {
            for(int j = 0; j < numCols; j++)
            {
                fieldBlock.setPosition(j*blocksize, i*blocksize);
                window.draw(fieldBlock);
            }
        }
    }

    sf::Sprite spr_grass;
    spr_grass.setTexture(grass);

    for(int i=0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            spr_grass.setPosition(j*blocksize, i*blocksize);
            window.draw(spr_grass);
        }
    }
}

void draw_snake(sf::RenderWindow &window, sf::RectangleShape &snakeBlock, std::list<point> &snake)
{
    sf::Texture snake_tex;

    if(!snake_tex.loadFromFile("Assets/Sprites/snake.jpg", sf::IntRect(40, 40, 40, 40)))
    {
        for(auto p: snake)
        {
            snakeBlock.setPosition(p.col*blocksize, p.row*blocksize);
            window.draw(snakeBlock);
        }
    }

    sf::Sprite spr_snake;
    spr_snake.setTexture(snake_tex);
    
    for(auto p: snake)
    {
        spr_snake.setPosition(p.col*blocksize, p.row*blocksize);
        window.draw(spr_snake);
    }
}

void draw_apple(sf::RenderWindow &window, sf::CircleShape &appleShape, point &apple)
{
    sf::Texture apple_tex;

    if(!apple_tex.loadFromFile("Assets/Sprites/apple_00.png", sf::IntRect(10, 10, 38, 38)))
    {
        appleShape.setPosition(apple.col * blocksize, apple.row * blocksize);
        window.draw(appleShape);
    }

    sf::Sprite spr_apple;
    spr_apple.setTexture(apple_tex);
    spr_apple.setPosition(apple.col * blocksize, apple.row * blocksize);
    window.draw(spr_apple);
}

void move_snake(std::list<point> &snake, sf::Keyboard::Key key, sf::Clock clock)
{
    //guarda uma copia do primeiro elemento da cobra
    point newHead = snake.front();
    bool KeyPressed = true;

    if(key == sf::Keyboard::Left || key == sf::Keyboard::A)
    {
        newHead.col--;
    }
    else if(key == sf::Keyboard::Right || key == sf::Keyboard::D)
    {
        newHead.col++;
    }
    else if(key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        newHead.row--;
    }
    else if(key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        newHead.row++;
    }
    else if(key == sf::Keyboard::Escape)
    {
        clock.restart();
        //ENDGAME
    }
    else if(key == sf::Keyboard::F2)
    {
        clock.getElapsedTime();
        //RESTART
    }
    else
    {
        newHead.col--;
    }

    snake.push_front(newHead);
}

point random_position(std::list<point> &snake)
{
    point p;
    while(true)
    {
        p.row = rand() % numRows;
        p.col = rand() % numCols;

        bool inSnake = false;

        for(auto b: snake)
        {
            if(p.row == b.row && p.col == b.col)
            {
                inSnake = true;
                break;
            }
        }

        if(!inSnake)
        {
            return p;
        }
    }      
}

bool isAlive(std::list<point> &snake)
{
    point head = snake.front();

    if(head.row >= numRows || head.col >= numCols || head.row < 0 || head.col < 0)
    {
        return true;
    }

    auto it = snake.begin();
    it++;
    while(it != snake.end())
    {
        if(head.row == it->row && head.col == it->col)
        {
            return true;
        }
        ++it;
    }

    return false;
}

int main()
{
    sf::Clock clock;

    //inicializa os valores aleatorios // pega o relogio do computador e utiliza os micro segundos
    srand(clock.getElapsedTime().asMicroseconds());

    // create the window
    sf::RenderWindow window(sf::VideoMode(numCols * blocksize, numRows * blocksize), "Snake");
    //criando o tamanho do campo do game
    sf::RectangleShape fieldBlock(sf::Vector2f(blocksize, blocksize));
    //definindo o tamanho do quadradinho do campo
    fieldBlock.setFillColor(sf::Color(84, 139, 84));
    //definindo o tamanho da borda do quadradinho
    fieldBlock.setOutlineThickness(1);
    //definindo a cor da borda
    fieldBlock.setOutlineColor(sf::Color(57, 46, 29));

    //bloco p corpo da cobra
    sf::RectangleShape snakeBlock(sf::Vector2f(blocksize, blocksize));
    snakeBlock.setFillColor(sf::Color::Blue);
    snakeBlock.setOutlineThickness(1);
    snakeBlock.setOutlineColor(sf::Color::Black);

    std::list<point> snake={{10, 15}, {10, 16}, {10, 17}};

    //desenhar a maca
    sf::CircleShape appleShape(blocksize/2);
    appleShape.setFillColor(sf::Color::Red);
    
    point apple = random_position(snake);
    float speed = 0.09;
    bool gameOver = false;
    int count_apple = 0, auxcount = 0;

    sf::Keyboard::Key lastKey = sf::Keyboard::Left;
    sf::Music music;
    if(!music.openFromFile("Assets/Sounds/Song.wav"))
    {
        //error
    }
    sf::Font font;
    if(!font.loadFromFile("Assets/Font/helvetica-1.otf"))
    {
        //erro
    }

    sf::Text text_win;
    text_win.setFont(font);
    text_win.setString("YOU WIN!!");
    text_win.setCharacterSize(100);
    text_win.setStyle(sf::Text::Bold);
    text_win.setPosition(210, 280);

    sf::Text text_score;
    text_score.setFont(font);
    text_score.setString("Score = 0");
    text_score.setCharacterSize(40);
    text_score.setFillColor(sf::Color::White);
    text_score.setPosition(450, 450);

    sf::Text text_go;
    text_go.setFont(font);
    text_go.setString("GAME OVER!!");
    text_go.setCharacterSize(100);
    text_go.setStyle(sf::Text::Bold);
    text_go.setPosition(210, 280);

    sf::Text text_f2;
    text_f2.setFont(font);
    text_f2.setString("F2 - Reiniciar");
    text_f2.setCharacterSize(40);
    text_f2.setPosition(80, 30);

    sf::Text text_esc;
    text_esc.setFont(font);
    text_esc.setString("ESC - Sair");
    text_esc.setCharacterSize(40);
    text_esc.setPosition(880, 30);

    music.play();
    

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;

        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            else if(event.type == sf::Event::KeyPressed)
            {                
                lastKey = event.key.code;
            }
        }

        if(!gameOver && clock.getElapsedTime().asSeconds() > speed)
        {
            move_snake(snake, lastKey, clock);

            gameOver = isAlive(snake);

            if(snake.size() == (numCols * numRows))
            {
                //voce venceu!
                //sf::RenderWindow win(sf::VideoMode(300, 300), "WIN");

                window.clear(sf::Color::Black);

                window.draw(text_win);

                window.display();

                sleep(5);

                window.close();

                return EXIT_SUCCESS;

            }

            if(snake.front().col == apple.col && snake.front().row == apple.row)
            {
                //aqui q testa o tamanho da maca p muda a velocidade
                count_apple++;
                auxcount++;
                if(auxcount == 5)
                {
                    speed -= 0.01;
                    auxcount = 0;
                }
                apple = random_position(snake);       
            }
            else if(!gameOver)
            {
                snake.pop_back();
            }            

            clock.restart();
        }
        

        // clear the window with black color

        window.clear(sf::Color::Black);

        // draw everything here...
        draw_field(window, fieldBlock);
        draw_snake(window, snakeBlock, snake);
        draw_apple(window, appleShape, apple);

        if(gameOver)
        {
            //mostrar um texto de game over
            snakeBlock.setFillColor(sf::Color::Red);

            window.clear(sf::Color::Black);

            std::stringstream ss;
            ss << "Score = " << count_apple;
            text_score.setString(ss.str());

            window.draw(text_go);
            window.draw(text_score);
            window.draw(text_f2);
            window.draw(text_esc);

            window.display();

            sleep(5);

            window.close();

            return EXIT_SUCCESS;

        }
        // window.draw(...);

        // end the current frame
        window.display();
    }

    return 0;
}