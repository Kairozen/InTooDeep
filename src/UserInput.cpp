#ifndef USERINPUT_CPP
#define USERINPUT_CPP

#include "UserInput.hpp"
#include <iostream>

UserInput::UserInput() {
    button.left = false;
    button.right = false;
    button.up = false;
    button.down = false;
    button.jump = false;
    button.dash = false;
}

UserInput::Button UserInput::getButton() {
    return button;
}

UserInput::Button UserInput::getLastButton() {
    return lastButton;
}

void UserInput::inputManagement(RenderWindow &window) {
    lastButton = button;
    // Boucle des événements
    while (window.pollEvent(event)) {
        // Gestion des différents cas d'événements
        switch (event.type) {
            // Si on ferme la fenêtre
            case Event::Closed:
                window.close();
                break;

                // Si on appuie sur une touche du clavier
            case Event::KeyPressed:
                // On regarde sur quelle touche on a appuyé
                switch (event.key.code) {
                    case Keyboard::Space:
                        button.jump = true;
                        break;

                    case Keyboard::Z:
                        button.up = true;
                        break;

                    case Keyboard::W:
                        button.up = true;
                        break;

                    case Keyboard::Up:
                        button.up = true;
                        break;

                    case Keyboard::S:
                        button.down = true;
                        break;

                    case Keyboard::Down:
                        button.down = true;
                        break;

                    case Keyboard::Q:
                        button.left = true;
                        break;

                    case Keyboard::A:
                        button.left = true;
                        break;

                    case Keyboard::Left:
                        button.left = true;
                        break;

                    case Keyboard::D:
                        button.right = true;
                        break;

                    case Keyboard::Right:
                        button.right = true;
                        break;

                    case Keyboard::LShift:
                        button.dash = true;
                        break;

                    case Keyboard::RShift:
                        button.dash = true;
                        break;

                    default:
                        break;
                }
                break;

                // Si on relâche une touche du clavier
            case Event::KeyReleased:
                // On regarde quelle touche a été relâchée
                switch (event.key.code) {
                    case Keyboard::Space:
                        button.jump = false;
                        break;

                    case Keyboard::Z:
                        button.up = false;
                        break;

                    case Keyboard::W:
                        button.up = false;
                        break;

                    case Keyboard::Up:
                        button.up = false;
                        break;

                    case Keyboard::S:
                        button.down = false;
                        break;

                    case Keyboard::Down:
                        button.down = false;
                        break;

                    case Keyboard::Q:
                        button.left = false;
                        break;

                    case Keyboard::A:
                        button.left = false;
                        break;

                    case Keyboard::Left:
                        button.left = false;
                        break;

                    case Keyboard::D:
                        button.right = false;
                        break;

                    case Keyboard::Right:
                        button.right = false;
                        break;

                    case Keyboard::LShift:
                        button.dash = false;
                        break;

                    case Keyboard::RShift:
                        button.dash = false;
                        break;

                    default:
                        break;
                }
                break;
                // Pas d'autre événement
            default:
                break;
        }
    }
}

#endif