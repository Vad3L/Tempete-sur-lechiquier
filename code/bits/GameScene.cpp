#include <cstddef>
#include <cstdlib>
#include <cstdio>

#include <string>
#include <iostream>

#include <gf/Clock.h>
#include <gf/Monitor.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Event.h>
#include <gf/Grid.h>
#include <gf/Shapes.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <gf/RenderWindow.h>


namespace game() {
  auto monitorsList = gf::Monitor::getAvailableMonitors();
  gf::VideoMode currentMode;
  for (auto& monitor : monitorsList) {

    currentMode = monitor.getCurrentVideoMode();
  }

  static gf::Vector2i ScreenSize(currentMode.size.width-100, currentMode.size.height-100);
  constexpr gf::Vector2i GridSize = { 24, 15 };
  constexpr float HexagonRadius = 32.0f;

  

  gf::Window window("lotr", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ViewContainer views;

  gf::ScreenView screenView;
  views.addView(screenView);
  views.setInitialFramebufferSize(ScreenSize);

  gf::Grid grids = gf::Grid::createHexagonal(GridSize, HexagonRadius, gf::CellAxis::X, gf::CellIndex::Even); 

  grids.setColor(gf::Color::fromRgba32(0xCDC1B4FF));
  grids.setSelectedColor(gf::Color::fromRgba32(0xCFB53BFF));

  renderer.clear(gf::Color::White);
  renderer.setView(screenView);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        case gf::EventType::MouseMoved:
          grids.hover(renderer.mapPixelToCoords(event.mouseCursor.coords));
          break;

        default:
          break;
      }

      views.processEvent(event);
    }

    gf::Grid& grid = grids;

    gf::Coordinates coordinates(renderer);
    grid.setPosition(coordinates.getCenter());
    grid.setAnchor(gf::Anchor::Center);
    grid.setLineWidth(1.5f);

    renderer.clear();
    renderer.draw(grid);
    renderer.display();
  }

  return 0;
}
