#ifndef SDLITEM_H
#define SDLITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include "qsdlrenderer.h"

class QSDL_EXPORT SDLItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SDLItem)
    Q_PROPERTY(QSDLRenderer* renderer READ renderer WRITE setRenderer NOTIFY rendererChanged)
public:
    SDLItem(QObject* parent = nullptr);

    QSDLRenderer* renderer() const { return m_renderer; }
    void setRenderer(QSDLRenderer*);

signals:
    void rendererChanged();

private slots:
    void onSizeChanged();
    void updateImage();

private:
    void paint(QPainter* painter) override;

    QSDLRenderer* m_renderer = nullptr;
    QImage m_image;
};

#endif // SDLITEM_H
