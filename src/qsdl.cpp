#include "qsdl.h"
#include "qsdlrenderer.h"
#include "sdlitem.h"
#include <SDL2/SDL.h>
#include <QtQml>

QSDL::QSDL()
{
  qmlRegisterType<QSDLRenderer>("SDL", 1,0, "SDLRenderer");
  qmlRegisterType<SDLItem>     ("SDL", 1,0, "SDLItem");
}

QSDL::~QSDL()
{
  SDL_Quit();
}
