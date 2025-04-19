#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "qsdlrenderer.h"
#include "qsdltexture.h"
#include "qsdl.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QSDL sdl; // Registers QML types and cleans up SDL on destruction
  QSDLRenderer* renderer = new QSDLRenderer(&app);

  QObject::connect(renderer, &QSDLRenderer::renderStart, [renderer]()
  {
    const std::vector<SDL_Vertex> verts =
    {
      { SDL_FPoint{ 400, 150 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 200, 450 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 600, 450 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
    };

    SDL_Renderer* sdl_renderer = renderer->sdl_renderer();
    SDL_RenderGeometry(sdl_renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
  });

  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);
  engine.rootContext()->setContextProperty("qsdlrenderer", renderer);
  engine.loadFromModule("QtQuickSDLTestTriangle", "Main");
  return app.exec();
}
