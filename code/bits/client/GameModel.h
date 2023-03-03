#ifndef TSL_GAME_MODEL_H
#define TSL_GAME_MODEL_H

#include <gf/Log.h>
#include <fstream>
#include <map>

#include "Singletons.h"
#include "../common/GamePhase.h"
#include "../common/Deck.h"

namespace tsl {

    namespace {
        std::array<std::string, 3> languages = { "English", "French", "Spanish" };
        static constexpr int nbCards = 5;
    }

    struct GameModel {
        GameModel()
        : language("English")
        , theme(1)
        , music(0)
        , sound(0)
        , chessStatus(ChessStatus::NO_STARTED)
        , chessColor(ChessColor::NONE)
        , selectedCard(-1)
        {
            readSettings();
            readDictionnary();
            Deck d = Deck();
            for(Card c: d.getDeck()) {
                deck.insert({c.m_num, c});
            }
        }

        void writeSettings() {
            gf::Log::info("Write settings game\n");
            std::ofstream file("../config/Settings.csv");

            if(!file) {
                gf::Log::error("Failed to open file.\n");
            }
            else {
                file << "language;" << language << "\n";
                file << "theme;" << theme << "\n";
                file << "sound;" << sound << "\n";;
                file << "music;" << music;
                file.close();
            }
        }   

        void readSettings() {
            gf::Log::info("Read settings game\n");
            std::ifstream file("../config/Settings.csv");

            if(!file) {
                gf::Log::error("Failed to open file.\n");
            }
            else {
                std::string line;
                
                if(std::getline(file, line)) {
                    language = line.substr(9).c_str();
                }

                if(std::getline(file, line)) {
                    theme = atoi(line.substr(6).c_str());
                }

                if(std::getline(file, line)) {
                    sound = atoi(line.substr(6).c_str());
                    FxsVolume = sound;
                }
                
                if(std::getline(file, line)) {
                    music = atoi(line.substr(6).c_str());
                    BackgroundAmbiantVolume = (FxsVolume == 0.f) ? 0.f : music;
                    gBackgroundMusic.setVolume(BackgroundAmbiantVolume);
                }
        
                file.close();
            }
        }

        void readDictionnary() {
            gf::Log::info("Read dictionary game\n");
            
            std::ifstream file("../data/Dictionary.csv");

            if(!file) {
                gf::Log::error("Failed to open file.\n");
            }
            else {
                std::string line;

                while (std::getline(file, line)) {
                    std::array<std::string, languages.size()> tab;
                    for(std::size_t i=0 ; i<tab.size() ; i++) {
                
                        std::size_t index = line.find(";");
                        tab[i] = line.substr(0, index);
                        
                        line = line.substr(index+1);
                    }
                    
                    // English
                    dictionary[languages[0]].insert(std::make_pair(tab[0], tab[0]));
                    // French
                    dictionary[languages[1]].insert(std::make_pair(tab[0], tab[1]));
                    // Spanish
                    dictionary[languages[2]].insert(std::make_pair(tab[0], tab[2]));
                    
                    for(auto a : dictionary) {
                        //gf::Log::info("%s\n     ", a.first.c_str());
                        for(auto aa : a.second) {
                            //gf::Log::info("aa%s , \n", aa.second.c_str());
                        }
                        //gf::Log::info("\n");
                    }
                }
                file.close();
            }
        }

        std::string getWord(std::string word) {
            return dictionary[language][word];
        }

        std::string language;
        int theme;
        int music;
        int sound;
        std::map<std::string, std::map<std::string, std::string>> dictionary;

        GamePhase gamePhase;
        ChessStatus chessStatus;
        ChessColor chessColor;
            
        int selectedCard;
        std::array<int, nbCards> cards;
        std::map<int, Card> deck;
    };
}

#endif // TSL_GAME_MODEL_H