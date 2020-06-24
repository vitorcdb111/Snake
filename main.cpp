#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <list>
#include <cstdlib>

//definindo as constante do tamanho da tela
const int numRows = 20, numCols = 30, blocksize = 25; 

//posicao da cobra  
struct point
{
    int row, col;
};

void draw_field(sf::RenderWindow &window, sf::RectangleShape &fieldBlock)
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

void draw_snake(sf::RenderWindow &window, sf::RectangleShape &snakeBlock, std::list<point> &snake)
{
    for(auto p: snake)
    {
        snakeBlock.setPosition(p.col*blocksize, p.row*blocksize);
        window.draw(snakeBlock);
    }
}

void draw_apple(sf::RenderWindow &window, sf::CircleShape &appleShape, point &apple)
{
    appleShape.setPosition(apple.col * blocksize, apple.row * blocksize);
    window.draw(appleShape);
}

void move_snake(std::list<point> &snake, sf::Keyboard::Key key)
{
    //guarda uma copia do primeiro elemento da cobra
    point newHead = snake.front();

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
    fieldBlock.setOutlineColor(sf::Color::Black);

    //bloco p corpo da conbra
    sf::RectangleShape snakeBlock(sf::Vector2f(blocksize, blocksize));
    snakeBlock.setFillColor(sf::Color(204, 102, 0));
    snakeBlock.setOutlineThickness(1);
    snakeBlock.setOutlineColor(sf::Color::Black);

    std::list<point> snake={{10, 15}, {10, 16}, {10, 17}};

    //desenhar a maca
    sf::CircleShape appleShape(blocksize/2);
    appleShape.setFillColor(sf::Color::Red);

    
    point apple = random_position(snake);
    float speed = 0.3;
    bool gameOver = false;

    sf::Keyboard::Key lastKey = sf::Keyboard::Left;
    

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
            move_snake(snake, lastKey);

            gameOver = isAlive(snake);
            if(gameOver)
            {
                snakeBlock.setFillColor(sf::Color::Red);
            }

            if(snake.front().col == apple.col && snake.front().row == apple.row)
            {
                //aqui q testa o tamanho da maca p muda a velocidade
                apple = random_position(snake);   
            }
            else if(!gameOver)
            {
                snake.pop_back();
            }            

            clock.restart();
        }
        

        // clear the window with black color
        window.clear(sf::Color::Red);

        // draw everything here...
        draw_field(window, fieldBlock);
        draw_snake(window, snakeBlock, snake);
        draw_apple(window, appleShape, apple);

        if(gameOver)
        {
            //mostrar um texto de game over
        }
        // window.draw(...);

        // end the current frame
        window.display();
    }

    return 0;
}