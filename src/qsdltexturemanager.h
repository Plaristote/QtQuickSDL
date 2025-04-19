#ifndef QSDLTEXTUREMANAGER_H
#define QSDLTEXTUREMANAGER_H

#include "QSDL_global.h"
#include <QMap>
#include <QSharedPointer>
#include <QUrl>

class QSDLRenderer;
class QSDLTexture;

class QSDL_EXPORT QSDLTextureManager
{
public:
  QSDLTextureManager(QSDLRenderer&);

  QSDLTexture* get(const QString& key);
  void addFromFile(const QUrl& path) { addFromFile(path.toString(), path); }
  void addFromFile(const QString& key, const QUrl& path);
  void addFromImage(const QString& key, const QImage&);

private:
  QSDLRenderer& m_parent;
  QMap<QString, QSharedPointer<QSDLTexture>> m_textures;
};

#endif // QSDLTEXTUREMANAGER_H
