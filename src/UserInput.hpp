#ifndef USERINPUT_HPP
#define USERINPUT_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class UserInput {

private:
    struct Button {
        bool up, down, right, left, jump, dash;
    };

    // Variables pour les événements et les bouttons du jeu
    Event event;
    Button button;
    Button lastButton;

public:

    // Constructeur, on initialise toutes les entrées à faux
    UserInput();

    // Accesseur des entrées
    Button getButton();

    // Accesseur des entrées
    Button getLastButton();

    // Pour gérer le clavier
    void inputManagement(RenderWindow &window);

};

#endif