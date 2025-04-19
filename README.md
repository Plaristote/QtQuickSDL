# QtQuickSDL

A simple library designed to help you use SDL to render the content of a QML component.

The library exposes a `QSDLRenderer` class and a `SDLItem` QtQuick component: the SDLItem will display the content of its `renderer` property, which can be initialized to your own implementation of `QSDLRenderer`.

The library also provides a texture manager, attaches to each QSDLRenderer, which can load textures from filesystem, from your [Qt Resource File](https://doc.qt.io/qt-6/resources.html), or even from already loaded instances of QImage.

If the SDL_image package is found when building QtQuickSDL, it will be used for loading images from filesystem. Otherwise, QtQuickSDL falls back to the _QImage to SDL_Texture_ converter.

#### Note

What everyone's hoping for is probably that this library would reach peak performance by allowing SDL to render directly on an OpenGL texture shared between the Qt and SDL contexts.
It's a goal, but I haven't been able to reach it yet. At the moment, we use a QQuickPaintedItem, which implies the following process:
- Copying the pixels from SDL to a QImage,
- Copying the pixels from a QImage to a QPainter
- QPainter renders itself to a QImage (or an OpenGL FrameBuffer, depending on the QQuickPaintedItem setup).

### Setup

The first step to use this library is to initialize a `QSDL` object in your main function.
```c++
#include <QtQuickSDL/qsdl.h>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QSDL sdl; // Registers QML types and cleans up SDL on destruction

  // ...
  return app.exec();
}
```
### Rendering

You should then implement your own renderer. QSDLRenderer acts as a wrapper for your rendering process: it starts each frame by colouring the frame with the fillColor
(which you can set using `setFillColor` or the `fillColor` property), then it triggers the `renderStart` signal, then it calls the `renderTask` method (which you may
overload to implement your own rendering logic), then it calls the `renderEnd` signal, and finally it calls `SDL_RenderPresent`.

You're expected to implement your own logic using either by inheriting QSDLRenderer and overriding the `renderTask` method, or by relying on signals. Here are a couple
of examples to show you what it looks like:

#### Inheritance route

```c++
#include <QtQuickSDL/qsdlrenderer.h>

class MyRenderer : public QSDLRenderer
{
public:
  MyRenderer(QObject* parent = nullptr) : QSDLRenderer(parent)
  {
    setFillColor(Qt::black);
    textures().addFromFile("mypicture", "qrc:/mypicture.png");
  }

  void renderTask() override
  {
    const std::vector<SDL_Vertex> verts =
    {
      { SDL_FPoint{ 400, 150 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 200, 450 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 600, 450 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
    };

    SDL_Renderer* renderer = sdl_renderer();
    SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
  }
};
```

#### Signals route

```c++
#include <QtQuickSDL/qsdl.h>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QSDL sdl; // Registers QML types and cleans up SDL on destruction
  QSDLRenderer* renderer = new QSDLRenderer(&app);

  QObject::connect(renderer, &QSDLRenderer::renderStart, [renderer]()
  {
    const std::vector<SDL_Vertex> verts =
    {
      { SDL_FPoint{ 400, 150 }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 200, 450 }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
      { SDL_FPoint{ 600, 450 }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
    };

    SDL_Renderer* sdl_renderer = renderer->sdl_renderer();
    SDL_RenderGeometry(sdl_renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
  });

  // ...
  engine.rootContext()->setContextProperty("qsdlrenderer", renderer);
  return app.exec();
}
```

### Loading textures

Each renderer comes with its own texture manager, designed to help you use textures from the Qt Resource system or QImage. Let's see how to load textures from the Qt Resource system and render them on screen:

```c++
#include <QtQuickSDL/qsdlrenderer.h>

class MyRenderer : public QSDLRenderer
{
public:
  MyRenderer(QObject* parent = nullptr) : QSDLRenderer(parent)
  {
    textures().addFromFile("mypicture", "qrc:/mypicture.png");
  }

  void renderTask() override
  {
    SDL_Renderer* renderer = sdl_renderer();
    QSDLTexture* texture = textures().get("mypicture");

    if (texture)
    {
      QSize textureSize = texture->size();
      SDL_Rect renderQuad{0, 0, textureSize.width(), textureSize.height()};
      SDL_RenderCopy(renderer, texture->sdl_texture, nullptr, renderQuad);
    }
  }
};
```

The QSDLTexture class also comes with utilities to help you render textures from Qt geometry objects:
```
  void renderTask() override
  {
    SDL_Renderer* renderer = sdl_renderer();
    QSDLTexture* texture = textures().get("mypicture");

    if (texture)
    {
      texture->render(QPoint(0, 0));
      // Or to clip part of the texture:
      texture->renderClip(QPoint(0, 0), QRect(37, 73, 36, 72});
    }
  }
```
