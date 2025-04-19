#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "qsdlrenderer.h"
#include "qsdltexture.h"
#include "qsdl.h"
#include "tst_tilemap.h"

// Tile dimensions
const int TILE_WIDTH = 72;
const int TILE_HEIGHT = 36;

class Tileset
{
public:
  static constexpr const int spacing = 1;
  Tileset(QSDLTexture* tex, int tileW, int tileH, int totalTiles, int firstId)
    : texture(tex), tileWidth(tileW), tileHeight(tileH),
    totalTiles(totalTiles), firstTileId(firstId),
    columns(tex->width() / (tileW + spacing))
  {}

  QSDLTexture* getTexture() const { return texture; }

  SDL_Rect getTileRect(int tileId) const
  {
    // Calculate the relative tile ID within this tileset
    int localId = tileId - firstTileId;

    // Return invalid rect if the tile isn't in this tileset
    if (localId < 0 || localId >= totalTiles) {
      return {0, 0, 0, 0};
    }

    // Calculate position in the texture
    int x = (localId % columns) * (tileWidth + spacing);
    int y = (localId / columns) * (tileHeight + spacing);

    return {x, y, tileWidth, tileHeight};
  }

  bool containsTile(int tileId) const
  {
    return tileId >= firstTileId && tileId < firstTileId + totalTiles;
  }

private:
  QSDLTexture* texture;
  int tileWidth;
  int tileHeight;
  int totalTiles;
  int firstTileId;
  int columns;
};

class RenderableObject
{
public:
  RenderableObject(QSDLTexture* tex, SDL_Rect clip, int offsetX = 0, int offsetY = 0)
    : texture(tex), clipRect(clip), offsetX(offsetX), offsetY(offsetY) {}

  void render(SDL_Renderer* renderer, int tileX, int tileY) const
  {
    int centerX = tileX + TILE_WIDTH / 2 - clipRect.w / 2 + offsetX;
    int bottomY = tileY + TILE_HEIGHT - clipRect.h + offsetY;
    texture->renderClip(centerX, bottomY, clipRect);
  }

private:
  QSDLTexture* texture;
  SDL_Rect clipRect;
  int offsetX;
  int offsetY;
};

class Layer
{
public:
  Layer(int width, int height) : width(width), height(height)
  {
      tileIds.resize(width * height, 0);
  }

  void setTileId(int x, int y, int tileId)
  {
    if (x >= 0 && x < width && y >= 0 && y < height) {
      tileIds[y * width + x] = tileId;
    }
  }

  int getTileId(int x, int y) const
  {
    if (x >= 0 && x < width && y >= 0 && y < height) {
      return tileIds[y * width + x];
    }
    return 0;  // Default to empty tile
  }

  void render(SDL_Renderer* renderer, const std::vector<Tileset>& tilesets, int offsetX, int offsetY)
  {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        int tileId = getTileId(x, y);

        // Skip empty tiles (tileId 0)
        if (tileId <= 0) continue;

        // Find the right tileset
        for (const auto& tileset : tilesets) {
          if (tileset.containsTile(tileId)) {
            SDL_Rect tileRect = tileset.getTileRect(tileId);

            // Calculate isometric position
            int isoX = (x - y) * (TILE_WIDTH / 2) + offsetX;
            int isoY = (x + y) * (TILE_HEIGHT / 2) + offsetY;
            tileset.getTexture()->renderClip(isoX, isoY, tileRect);
            break ;
          }
        }
      }
    }
  }

private:
  int width;
  int height;
  std::vector<int> tileIds;
};

class TileMap
{
public:
  TileMap(int width, int height) : width(width), height(height)
  {}

  void addLayer()
  {
    layers.push_back(Layer(width, height));
  }

  Layer& getLayer(int index)
  {
    return layers[index];
  }

  void addTileset(QSDLTexture* texture, int tileWidth, int tileHeight, int totalTiles)
  {
    int firstId = 1;  // First tileset starts at 1

    if (!tilesets.empty())
    {
      // Calculate the first ID based on previous tilesets
      const auto& lastTileset = tilesets.back();
      firstId = lastTileset.containsTile(1) ? 1 + totalTiles : 1;

      for (const auto& ts : tilesets) {
        if (ts.containsTile(firstId))
          firstId += totalTiles;
      }
    }
    tilesets.emplace_back(texture, tileWidth, tileHeight, totalTiles, firstId);
  }

  void addObject(int x, int y, RenderableObject obj)
  {
    objects.push_back({x, y, obj});
  }

  void render(SDL_Renderer* renderer, int offsetX, int offsetY)
  {
    // Render all layers
    for (auto& layer : layers) {
      layer.render(renderer, tilesets, offsetX, offsetY);
    }

    // Sort objects based on their isometric order (bottom-left to top-right)
    std::sort(objects.begin(), objects.end(),
      [](const ObjectInstance& a, const ObjectInstance& b)
      {
        return (a.x + a.y) < (b.x + b.y);
      });

    // Render all objects
    for (const auto& obj : objects)
    {
      // Calculate isometric position
      int isoX = (obj.x - obj.y) * (TILE_WIDTH / 2) + offsetX;
      int isoY = (obj.x + obj.y) * (TILE_HEIGHT / 2) + offsetY;
      obj.object.render(renderer, isoX, isoY);
    }
  }

private:
  struct ObjectInstance
  {
    int x;
    int y;
    RenderableObject object;
  };

  int width;
  int height;
  std::vector<Layer> layers;
  std::vector<Tileset> tilesets;
  std::vector<ObjectInstance> objects;
};

Game::Game(QObject* parent) : QObject(parent), renderer(nullptr), isRunning(true),
    cameraOffsetX(0), cameraOffsetY(0), map(nullptr)
{}

Game::~Game()
{
  if (map)
    delete map;
}

bool Game::init()
{
  renderer = new QSDLRenderer();
  QObject::connect(renderer, &QSDLRenderer::renderStart, [this]() { startRender(); });
  return true;
}

void Game::loadTestMap()
{
  // Create a sample tilemap
  map = new TileMap(10, 10);

  // Load a texture
  const char* texturePath = "qrc:/tileset.png";
  renderer->textures().addFromFile("tileset", QUrl(texturePath));
  auto* tilesetTexture = renderer->textures().get("tileset");
  if (!tilesetTexture)
  {
    std::cerr << "Failed to load tileset texture!" << std::endl;
    return;
  }

  // Add tileset to the map
  map->addTileset(tilesetTexture, TILE_WIDTH, TILE_HEIGHT, 152);

  // Add a layer
  map->addLayer();

  // Fill the layer with tiles
  for (int y = 0; y < 10; ++y)
  {
    for (int x = 0; x < 10; ++x)
    {
      // Just a simple pattern for demonstration
      int tileId = 1 + ((x + y) % 8);
      map->getLayer(0).setTileId(x, y, tileId);
    }
  }

  // Add another layer for some variation
  map->addLayer();

  // Add some objects
  renderer->textures().addFromFile("objects", QUrl("qrc:/objects.png"));
  auto* objectTexture = renderer->textures().get("objects");
  if (objectTexture)
  {
    // Add tree at position 2,3
    map->addObject(2, 3, RenderableObject(objectTexture, {0, 0, 50, 70}));
    // Add rock at position 5,7
    map->addObject(5, 7, RenderableObject(objectTexture, {50, 0, 40, 30}));
  }

  // Center the map in the view
  cameraOffsetX = renderer->size().width() / 2 - (10 * TILE_WIDTH / 4);
  cameraOffsetY = renderer->size().height() / 4;
}

void Game::startRender()
{
  if (map)
    map->render(renderer->sdl_renderer(), cameraOffsetX, cameraOffsetY);
}

void Game::render()
{
  if (renderer)
    renderer->render();
}

void Game::close()
{
  delete renderer;
}

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  QSDL sdl;
  Game game(&app);

  if (!game.init()) {
    std::cerr << "Failed to initialize!" << std::endl;
    return -1;
  }
  game.loadTestMap();

  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit(-1); },
    Qt::QueuedConnection);

  engine.rootContext()->setContextProperty("game", &game);
  engine.loadFromModule("QtQuickSDLTest", "Main");

  return app.exec();
}
