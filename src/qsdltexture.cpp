#include "qsdltexture.h"
#include "qsdlrenderer.h"
#ifdef WITH_SDL_IMAGE
# include <SDL2/SDL_image.h>
#endif
#include <QDebug>

QSDLTexture::QSDLTexture(QSDLRenderer& parent) : m_parent(parent)
{
}

QSDLTexture::~QSDLTexture()
{
  free();
}

void QSDLTexture::loadFromFile(const QString& path)
{
#ifdef WITH_SDL_IMAGE
  SDL_Surface* surface = IMG_Load(path.toUtf8().constData());

  free();
  if (surface == nullptr)
  {
    qDebug() << "QSDLTexture: failed to load" << path << ": " << IMG_GetError();
    return ;
  }
  m_texture = SDL_CreateTextureFromSurface(m_parent.sdl_renderer(), surface);
  if (m_texture == nullptr)
    qDebug() << "QSDLTexture: failed to create texture from" << path << SDL_GetError();
  m_size = QSize(surface->w, surface->h);
  SDL_FreeSurface(surface);
#else
  loadFromImage(QImage(path).convertToFormat(QImage::Format_ARGB32));
#endif
}

void QSDLTexture::loadFromImage(const QImage& image)
{
  SDL_Surface* surface;
  const void* bytes = static_cast<const void*>(image.constBits());

  free();
  if (image.format() != QImage::Format_ARGB32)
  {
    qDebug() << "QSDLTexture: failed to load QImage, format was not QImage::Format_ARGB32";
    return ;
  }
  surface = SDL_CreateRGBSurfaceFrom(
    const_cast<void*>(bytes), // pixel data
    image.width(),            // width
    image.height(),           // height
    32,                       // depth (bits per pixel)
    image.bytesPerLine(),     // pitch (bytes per line)
    0x00FF0000,               // red mask
    0x0000FF00,               // green mask
    0x000000FF,               // blue mask
    0xFF000000                // alpha mask
  );
  m_texture = SDL_CreateTextureFromSurface(m_parent.sdl_renderer(), surface);
  if (m_texture == nullptr)
      qDebug() << "QSDLTexture: failed to create texture from image:" << SDL_GetError();
  m_size = QSize(surface->w, surface->h);
  SDL_FreeSurface(surface);
}

void QSDLTexture::render(SDL_Rect renderQuad)
{
  SDL_RenderCopy(m_parent.sdl_renderer(), m_texture, nullptr, &renderQuad);
}

void QSDLTexture::renderClip(int x, int y, SDL_Rect clip)
{
  SDL_Rect renderQuad{x, y, clip.w, clip.h};
  SDL_RenderCopy(m_parent.sdl_renderer(), m_texture, &clip, &renderQuad);
}

void QSDLTexture::free()
{
  if (m_texture)
  {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    m_size = QSize();
  }
}
