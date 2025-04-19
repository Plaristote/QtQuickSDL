#ifndef QSDLTEXTURE_H
#define QSDLTEXTURE_H

#include "QSDL_global.h"
#include <QRect>
#include <QImage>
#include <SDL2/SDL.h>

class QSDLRenderer;

class QSDL_EXPORT QSDLTexture
{
public:
  QSDLTexture(QSDLRenderer&);
  ~QSDLTexture();

  void loadFromFile(const QString& path);
  void loadFromImage(const QImage&);
  void render(QPoint value) { render(value.x(), value.y()); }
  void render(QRect value) { render(SDL_Rect{value.x(), value.y(), value.width(), value.height()}); }
  void render(int x, int y) { render(SDL_Rect{x, y, m_size.width(), m_size.height()}); }
  void render(SDL_Rect);
  void renderClip(QPoint value, QRect clip) { renderClip(value.x(), value.y(), SDL_Rect{clip.x(), clip.y(), clip.width(), clip.height()}); }
  void renderClip(QPoint value, SDL_Rect clip) { renderClip(value.x(), value.y(), clip); }
  void renderClip(int x, int y, SDL_Rect clip);

  SDL_Texture* sdl_texture() const { return m_texture; }
  QSize size() const { return m_size; }
  int width() const { return m_size.width(); }
  int height() const { return m_size.height(); }

private:
  void free();

  QSDLRenderer& m_parent;
  SDL_Texture* m_texture;
  QSize m_size;
};

#endif // QSDLTEXTURE_H
