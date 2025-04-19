#pragma once
#include <memory>
#include "qsdlrenderer.h"
#include "qsdltexture.h"

class TileMap;

class Game : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int cameraOffsetX MEMBER cameraOffsetX NOTIFY cameraMoved)
  Q_PROPERTY(int cameraOffsetY MEMBER cameraOffsetY NOTIFY cameraMoved)
  Q_PROPERTY(QSDLRenderer* renderer READ qsdlRenderer CONSTANT)
public:
  Game(QObject* parent = nullptr);
  ~Game();

  bool init();
  void loadTestMap();
  void startRender();
  void render();
  void close();
  bool running() const { return isRunning; }
  QSDLRenderer* qsdlRenderer() { return renderer; }

signals:
  void cameraMoved();

private:
  QSDLRenderer* renderer;
  bool isRunning;
  int cameraOffsetX;
  int cameraOffsetY;
  TileMap* map;
};
