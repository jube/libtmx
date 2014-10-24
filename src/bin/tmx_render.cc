#include <cstdio>
#include <cstdlib>

#include <boost/filesystem.hpp>

#include <QPainter>
#include <QCache>

#include <tmx/Map.h>
#include <tmx/TileLayer.h>
#include <tmx/ObjectLayer.h>

namespace fs = boost::filesystem;

class LayerRenderer : public tmx::LayerVisitor {
public:
  LayerRenderer()
  : map(nullptr), painter(), tilewidth(0), tileheight(0), width(0), height(0) { }

  void renderMap(const fs::path& map_path) {

    map = tmx::Map::parseFile(map_path);

    if (!map) {
      return;
    }

    if (map->getOrientation() != tmx::Orientation::ORTHOGONAL) {
      std::printf("Can render only orthogonal maps. Exiting.\n");
      return;
    }

    tilewidth = map->getTileWidth();
    assert(tilewidth);

    tileheight = map->getTileHeight();
    assert(tileheight);

    width = map->getWidth();
    assert(width);

    height = map->getHeight();
    assert(height);

    // create surface

    QImage image(width * tilewidth, height * tileheight, QImage::Format_ARGB32);
    painter.begin(&image);
    map->visitLayers(*this);
    painter.end();

    std::printf("Saving image...\n");
    image.save("map.png");
  }

private:
  std::unique_ptr<tmx::Map> map;

  QPainter painter;

  unsigned tilewidth;
  unsigned tileheight;
  unsigned width;
  unsigned height;

  QCache<QString, QImage> cache;

  const QImage getTexture(const fs::path& path) {
    QString str(path.string().c_str());
    QImage *img = cache.object(str);

    if (img != nullptr) {
      return *img;
    }

    img = new QImage(str);
    assert(!img->isNull());

    cache.insert(str, img);
    return *img;
  }

  enum class Alignment {
    TOP_LEFT,
    BOTTOM_LEFT,
  };

  void drawGID(const QPoint& origin, unsigned gid, Alignment align) {
    auto tileset = map->getTileSetFromGID(gid);
    assert(tileset);
    gid = gid - tileset->getFirstGID();

    if (tileset->hasImage()) {

      auto image = tileset->getImage();
      assert(image);

      const QImage texture = getTexture(image->getSource());

      tmx::Size size;

      if (image->hasSize()) {
        size = image->getSize();
      } else {
        QSize texture_size = texture.size();
        assert(texture_size.width() >= 0);
        assert(texture_size.height() >= 0);
        size.width = texture_size.width();
        size.height = texture_size.height();
      }

      tmx::Rect rect = tileset->getCoords(gid, size);
      QPoint offset;

      if (align == Alignment::BOTTOM_LEFT) {
        offset.ry() -= rect.height;
      }

      painter.drawImage(origin + offset, texture, QRect(rect.x, rect.y, rect.width, rect.height));

    } else {

      auto tile = tileset->getTile(gid);
      assert(tile);
      assert(tile->hasImage());

      auto image = tile->getImage();
      assert(image);

      const QImage texture = getTexture(image->getSource());
      painter.drawImage(origin, texture);

    }
  }

public:
  virtual void visitTileLayer(tmx::TileLayer& layer) override {
    if (!layer.isVisible()) {
      return;
    }

    std::printf("Rendering tile layer '%s'.\n", layer.getName().c_str());

    unsigned k = 0;
    for (auto cell : layer) {
      unsigned i = k % width;
      unsigned j = k / width;
      assert(j < height);

      QPoint origin(i * tilewidth, j * tileheight);

      unsigned gid = cell.getGID();

      if (gid != 0) {
        drawGID(origin, gid, Alignment::TOP_LEFT);
      }

      k++;
    }

  }

  virtual void visitObjectLayer(tmx::ObjectLayer &layer) override {
    if (!layer.isVisible()) {
      return;
    }

    std::printf("Rendering object layer '%s'.\n", layer.getName().c_str());

    for (auto& obj : layer) {
      if (!obj->isTile()) {
        continue;
      }

      auto tile = static_cast<tmx::TileObject *>(obj.get());

      QPoint origin(tile->getX(), tile->getY());

      unsigned gid = tile->getGID();
      assert(gid != 0);

      drawGID(origin, gid, Alignment::BOTTOM_LEFT);
    }

  }


};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::printf("Usage: tmx_render <file.tmx>\n");
    return 1;
  }

  fs::path map_path(argv[1]);

  LayerRenderer renderer;
  renderer.renderMap(map_path);

  return 0;
}
