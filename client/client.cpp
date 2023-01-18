#include <iostream>
#include <thread>
#include "GameHub.hpp"  
#include "Network.hpp"

#include <gf/SharedGraphics.h>

int main(int argc, char* argv[]) {

  GameHub hub;
  Network network;
  //hub.getWindow().setResizable(false);  
  //hub.getWindow().setSize(gf::Vector2i(640,420));
  hub.getWindow().toggleFullscreen();

  auto loading = std::thread([&hub, &network]() {
    gf::SharedGraphics glContext(hub.getWindow());
    hub.loadingDownloadAssets();
    hub.loadingAssets(network);
  });

  loading.detach();

  //while (!hub.loadingFinish())
  //{
    /* code */
  //}
  
  hub.pushScene(*hub.download);
  hub.run();
  return EXIT_SUCCESS;
}
