#include "qsdlrenderer.h"
#include "qsdlcontext.h"
#include <QDebug>

QSDLRenderer::QSDLRenderer(QObject *parent) : QObject{parent}, m_textures(*this)
{
  m_context = QSDLContext::require();
  m_fillColor = Qt::white;
  m_size = QSize(800, 600);
  m_window = SDL_CreateWindow("QSDLRenderer",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    m_size.width(), m_size.height(), SDL_WINDOW_HIDDEN);
  if (m_window == nullptr)
    qDebug() << "QSDLRenderer: Window could not be created! SDL_Error:" << SDL_GetError();
  else
  {
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
      qDebug() << "QSDLRenderer: Renderer could not be created! SDL_Error:" << SDL_GetError();
  }
}

QSDLRenderer::~QSDLRenderer()
{
  if (m_renderer)
    SDL_DestroyRenderer(m_renderer);
  if (m_window)
    SDL_DestroyWindow(m_window);
}

void QSDLRenderer::setSize(QSize size)
{
  if (size != m_size)
  {
    SDL_SetWindowSize(m_window, size.width(), size.height());
    m_size = size;
    emit sizeChanged(size);
  }
}

bool QSDLRenderer::isWindowVisible()
{
  if (m_window)
    return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_SHOWN) > 0;
  return false;
}

void QSDLRenderer::setWindowVisible(bool value)
{
  if (m_window)
  {
    value ? SDL_ShowWindow(m_window) : SDL_HideWindow(m_window);
    emit windowVisibilityChanged(value);
  }
}

void QSDLRenderer::render()
{
  SDL_SetRenderDrawColor(m_renderer, m_fillColor.red(), m_fillColor.green(), m_fillColor.blue(), m_fillColor.alpha());
  SDL_RenderClear(m_renderer);
  emit renderStart();
  renderTask();
  emit renderEnd();
  SDL_RenderPresent(m_renderer);
}

void QSDLRenderer::renderTask()
{

}
