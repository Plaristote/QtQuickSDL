#include "sdlitem.h"
#include <QPainter>

SDLItem::SDLItem(QObject* object)
{
  connect(this, &QQuickItem::heightChanged, this, &SDLItem::onSizeChanged);
  connect(this, &QQuickItem::widthChanged, this, &SDLItem::onSizeChanged);
  connect(this, &SDLItem::rendererChanged, this, &SDLItem::onSizeChanged);
}

void SDLItem::setRenderer(QSDLRenderer* renderer)
{
  if (m_renderer)
    disconnect(m_renderer, &QSDLRenderer::renderEnd, this, &SDLItem::updateImage);
  if (renderer)
    connect(renderer, &QSDLRenderer::renderEnd, this, &SDLItem::updateImage);
  m_renderer = renderer;
  emit rendererChanged();
}

void SDLItem::onSizeChanged()
{
  QSize size(width(), height());

  m_image = QImage(size, QImage::Format_ARGB32);
  if (m_renderer)
    m_renderer->setSize(size);
}

void SDLItem::updateImage()
{
  if (m_renderer)
  {
    SDL_Renderer* renderer = m_renderer->sdl_renderer();
    SDL_Rect rect = { 0, 0, m_image.width(), m_image.height() };

    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_BGRA32,
      m_image.bits(), m_image.bytesPerLine());
    update();
  }
}

void SDLItem::paint(QPainter* painter)
{
  painter->drawImage(0, 0, m_image);
}
