#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include </home/elisabetta-ferri/projects/myproject/esame2020/epidemic.hpp>


//g++ gui.cpp -o gui -lsfml-graphics -lsfml-window -lsfml-system

class TileMap : public sf::Drawable, public sf::Transformable // classe figlia
{
private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_texture;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_texture;

public:

    bool load(const std::string& texture, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height)
    {                                   // dimensione della piastrella                              diensione della griglia
        // load the tileset texture
        if (!m_texture.loadFromFile(texture))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads); // riorganizza i vertici in quadrati
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];  // questo diventerebbe stato // 1

                // find its position in the tileset texture
                int tu = tileNumber % (m_texture.getSize().x / tileSize.x); // 5
                int tv = tileNumber / (m_texture.getSize().x / tileSize.x); // 3/5

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }


};

int main()
{
    sf::RenderWindow epidemic(sf::VideoMode(800, 600), "My epidemic");
    sf::RenderWindow graph(sf::VideoMode(600, 600), "My graph");
    graph.setVerticalSyncEnabled(true); // call it once, after creating the window


    // change the position of the window (relatively to the desktop)
    epidemic.setPosition(sf::Vector2i(10, 50));
    graph.setPosition(sf::Vector2i(850, 50));

        // immagine su finestra graph
        sf::Texture texture;
        sf::Image image;
        if (!texture.loadFromFile("image.png"))
        {
            return EXIT_FAILURE;;
        }
        texture.setSmooth(true);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        auto n = texture.getSize();
        auto graphSize = graph.getSize();
        float x = (graphSize.x-n.x) / 2;
        float y = (graphSize.y-n.y) / 2;
        sprite.setPosition(sf::Vector2f(x, y)); // absolute position

        std::vector<int> level =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

        TileMap map;

    // run the program as long as the window is open
    while (epidemic.isOpen() || graph.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (epidemic.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                epidemic.close();
                graph.close();
            }
        }
        while (graph.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                graph.close();
            }
        }

        // clear the window with black color
        graph.clear(sf::Color::Black);

        // draw everything here...
        graph.draw(sprite);

        // end the current frame
        graph.display();
        /*Calling display is also mandatory,
        it takes what was drawn since the last
        call to display and displays it on 
        the window. Indeed, things are not drawn
        directly to the window, but to a hidden buffer.
        This buffer is then copied to the window when
        you call display*/


        if (!map.load("tileset.png", sf::Vector2u(32, 32), level, 16, 8))
            return -1;

        // draw the map
        epidemic.clear();
        epidemic.draw(map);
        epidemic.display();
        for (int i =0; i != level.size(); ++i) {
            ++level[i];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));


    }

    return 0;
}