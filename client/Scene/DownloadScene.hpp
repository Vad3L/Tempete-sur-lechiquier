#ifndef DOWNLOAD_SCENE_H
#define DOWNLOAD_SCENE_H

#include <gf/ResourceManager.h>
#include <gf/Scene.h>

struct GameHub;

class DownloadScene : public gf::Scene {

    public :
        DownloadScene(GameHub& game);

    private :
        void loadAnimation();
        void doHandleActions(gf::Window& window) override;
        void doUpdate(gf::Time time) override;

        gf::Action m_escapeAction;

        GameHub& m_game;
    
};

#endif // DOWNLOAD_SCENE_H