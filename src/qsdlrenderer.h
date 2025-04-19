#ifndef QSDLRENDERER_H
#define QSDLRENDERER_H

#include <QObject>
#include <QSize>
#include <SDL2/SDL.h>
#include "qsdltexturemanager.h"

class QSDLContext;

class QSDL_EXPORT QSDLRenderer : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
  Q_PROPERTY(bool visibleWindow READ isWindowVisible WRITE setWindowVisible NOTIFY windowVisibilityChanged)
public:
  explicit QSDLRenderer(QObject *parent = nullptr);
  virtual ~QSDLRenderer();

  SDL_Renderer* sdl_renderer() const { return m_renderer; }
  QSDLTextureManager& textures() { return m_textures; }
  void setSize(QSize);
  QSize size() const { return m_size; }
  bool isWindowVisible();
  void setWindowVisible(bool);

public slots:
  void render();

signals:
  void sizeChanged();
  void renderStart();
  void renderEnd();
  void windowVisibilityChanged();

protected:
  virtual void renderTask();

private:
  QSharedPointer<QSDLContext> m_context;
  QSDLTextureManager m_textures;
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  QSize m_size;
};

#endif // QSDLRENDERER_H
