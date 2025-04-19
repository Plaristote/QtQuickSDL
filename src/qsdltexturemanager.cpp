#include "qsdltexturemanager.h"
#include "qsdlrenderer.h"
#include "qsdltexture.h"
#include <QDebug>

static QImage imageFromRC(const QUrl& url)
{
  const QString path = ":/" + url.path();
  const QImage image(path);
  const auto format = QImage::Format_ARGB32;

  return image.format() == format ? image : image.convertToFormat(format);
}

QSDLTextureManager::QSDLTextureManager(QSDLRenderer& parent) : m_parent(parent)
{
}

void QSDLTextureManager::addFromFile(const QString& key, const QUrl& url)
{
  if (m_textures.count(key) == 0)
  {
    QSharedPointer<QSDLTexture> texture(new QSDLTexture(m_parent));
    const QString scheme = url.scheme();

    if (scheme == "qrc")
      texture->loadFromImage(imageFromRC(url));
    else if (scheme.isEmpty() || scheme == "file")
      texture->loadFromFile(url.path());
    else
      qDebug() << "QSDLTextureManager: could not load" << url << ": unsupported scheme";
    if (texture->sdl_texture())
      m_textures[key] = texture;
  }
  else
    qDebug() << "QSDLTextureManager: texture" << key << "is already loaded.";
}

void QSDLTextureManager::addFromImage(const QString& key, const QImage& image)
{
  if (m_textures.count(key) == 0)
  {
    QSharedPointer<QSDLTexture> texture(new QSDLTexture(m_parent));

    texture->loadFromImage(image);
    m_textures[key] = texture;
  }
  else
    qDebug() << "QSDLTextureManager: texture" << key << "is already loaded.";
}

QSDLTexture* QSDLTextureManager::get(const QString& key)
{
  auto it = m_textures.find(key);
  return it != m_textures.end() ? it.value().get() : nullptr;
}
