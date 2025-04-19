#include "qsdlcontext.h"
#include <QWeakPointer>
#include <SDL2/SDL.h>
#ifdef WITH_SDL_IMAGE
# include <SDL2/SDL_image.h>
#endif

static QWeakPointer<QSDLContext> globalContext;

QSDLContext::QSDLContext()
{
  SDL_InitSubSystem(SDL_INIT_VIDEO);
#ifdef WITH_SDL_IMAGE
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_AVIF);
#endif
}

QSDLContext::~QSDLContext()
{
#ifdef WITH_SDL_IMAGE
  IMG_Quit();
#endif
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

QSharedPointer<QSDLContext> QSDLContext::require()
{
  QSharedPointer<QSDLContext> context = globalContext.lock();

  if (context.isNull())
  {
    context.reset(new QSDLContext);
    globalContext = context;
  }
  return context;
}
