#include <iostream>
#include<string>
#include <cassert>

#include <gf/Packet.h>
#include <gf/Sleep.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Grid.h>
#include <gf/Coordinates.h>

#include "ClientNetwork.hpp"
#include "clientTools.hpp"
#include "model/CBoard.hpp"

int main(int argc, char* argv[]) {
    bool myTurn;

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " [ip] [port] [numJoueur]" << std::endl;
        return -1;
    }

    ClientNetwork network;
    network.connect(argv[1], std::to_string(atoi(argv[2])));
    gf::sleep(gf::milliseconds(500));
    assert(network.isConnected());

    int err;
    TPartieReq req;
    req.idReq = PARTIE;
    req.nomJoueur = argv[3];
    //req.coulPion = 1;

    network.send(req);
    gf::Packet packet;
    network.queue.wait(packet);
    assert(packet.getType() == PartieRep::type);

    auto repPartie = packet.as<PartieRep>();

    //int couleur = initColor(repPartie.validCoulPion, req.coulPion);
    int couleur = repPartie.coulPion;

    if (couleur == 1) {
        myTurn = true;
    }
    else {
        myTurn = false;
    }

    std::cout << "Vous jouez la couleur : " << couleur << std::endl;

    /************** Début de partie ********************/
    Plateau plateau;
    std::string title = "Tempete sur l'echiquier - ";
    title += argv[3];
    CBoard board(gf::Vector2i(gf::Zero), title, couleur);
    gf::Event event;
    gf::Clock clock;
    
    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    actions.addAction(closeWindowAction);

    gf::Action clickAction("select");
    clickAction.addMouseButtonControl(gf::MouseButton::Left);
    clickAction.setInstantaneous();
    actions.addAction(clickAction);

    
    while (board.window.isOpen())
    {
        gf::Time time = clock.restart();
        //board.Update(time);

        actions.reset();
        while (board.window.pollEvent(event)) {
            actions.processEvent(event);
            board.views.processEvent(event);
        }

        if (closeWindowAction.isActive()) {
            board.window.close();
            break;
        }


        board.print();

     
    }
    return 0;
}