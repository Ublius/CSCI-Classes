/* CSCI 200: Lab 6A - SFML: Bob Ross
 *
 * Author: Tristen Erpelding
 *
 * The opportunity to play with the power that SFML (a special framework called Simple and Fast Multimedia Library) offers.
 * Your job is to draw something in SFML.
 * 
*/

#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
using namespace std;

int main()
{
    // create a window
    RenderWindow window(VideoMode(1920, 1080), "Star Across The Sky");

    // create shapes
    ConvexShape star;
    star.setPointCount(10);
    star.setPoint(0, Vector2f(100, 0));
    star.setPoint(1, Vector2f(150, 150));
    star.setPoint(2, Vector2f(0, 60));
    star.setPoint(3, Vector2f(200, 60));
    star.setPoint(4, Vector2f(50, 150));
    star.setPoint(5, Vector2f(100, 300));
    star.setPoint(6, Vector2f(150, 150));
    star.setPoint(7, Vector2f(100, 0));
    star.setFillColor(Color::Yellow);
    star.setPosition(25, 50);

    // create variables for movement
    float speed = 250.f;
    Clock clock;

    // while the window is open
    while (window.isOpen())
    {
        window.clear();
        // calculate elapsed time since last frame
        float deltaTime = clock.restart().asSeconds();

        // create a rectangle shape for the house
        RectangleShape house;
        house.setSize(Vector2f(200, 200));
        house.setFillColor(Color::Red);
        house.setPosition(1000, 430);

        // create a triangle shape for the roof
        ConvexShape roof;
        roof.setPointCount(3);
        roof.setPoint(0, Vector2f(1000, 430));
        roof.setPoint(1, Vector2f(1200, 430));
        roof.setPoint(2, Vector2f(1100, 350));;
        roof.setFillColor(Color::Blue);
        
        // create a rectangle shape for the door
        RectangleShape door;
        door.setSize(Vector2f(50, 100));
        door.setFillColor(Color::White);
        door.setPosition(1075, 530);

        // Set the Sky background
        RectangleShape sky;
        sky.setSize(Vector2f(1920, 1080));
        sky.setPosition(0, 0);
        sky.setFillColor(Color(12, 20, 69));
        window.draw(sky);
        
        // Set the Leafs of the tree
        CircleShape treeLeafs;
        treeLeafs.setPosition(680, 350);
        treeLeafs.setRadius(100);
        treeLeafs.setFillColor(Color::Green);
        window.draw(treeLeafs);
        
        // Set the trunk of the tree
        RectangleShape treeTrunk;
        treeTrunk.setSize(Vector2f(45, 150));
        treeTrunk.setPosition(760, 500);
        treeTrunk.setFillColor(Color(165, 42, 42));
        window.draw(treeTrunk);
        
        // Set floor Background
        RectangleShape floorBackground;
        floorBackground.setSize(Vector2f(1920, 500));
        floorBackground.setPosition(0, 1080-450);
        floorBackground.setFillColor(Color(31, 100, 32));
        window.draw(floorBackground);

        // handle events
        Event event;
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // move the star
        star.move(speed * deltaTime, 0);

        // wrap the star around the window
        if (star.getPosition().x > window.getSize().x + 30)
            star.setPosition(25, 50);

        // draw the star
        window.draw(star);
        // draw the house
        window.draw(house);
        window.draw(roof);
        window.draw(door);

        // display the window
        window.display();
    }

    return 0;
}