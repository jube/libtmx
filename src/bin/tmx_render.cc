#include <cstdio>
#include <cstdlib>

#include <boost/filesystem.hpp>

#include <QPainter>
#include <QCache>

#include <tmx/TMX.h>
#include <tmx/TileLayer.h>

namespace fs = boost::filesystem;

class LayerRenderer : public tmx::LayerVisitor {
public:
  LayerRenderer()
  : map(nullptr), painter(), tilewidth(0), tileheight(0), width(0), height(0) { }

  void renderMap(const fs::path& map_path) {

    map = tmx::parseMapFile(map_path);

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

    delete map;
  }

private:
  tmx::Map *map;

  QPainter painter;

  unsigned tilewidth;
  unsigned tileheight;
  unsigned width;
  unsigned height;

  QCache<QString, QImage> cache;

  QImage *getTexture(const fs::path& path) {
    QString str(path.string().c_str());
    QImage *img = cache.object(str);

    if (img != nullptr) {
      return img;
    }

    img = new QImage(str);
    assert(!img->isNull());

    cache.insert(str, img);
    return img;
  }

  void drawGID(const QPoint& origin, unsigned gid) {
    auto tileset = map->getTileSetFromGID(gid);
    assert(tileset);
    gid = gid - tileset->getFirstGID();

    if (tileset->hasImage()) {

      auto image = tileset->getImage();
      assert(image);

      QImage *texture = getTexture(image->getSource());
      QSize size = texture->size();

      unsigned margin = tileset->getMargin();
      unsigned spacing = tileset->getSpacing();
      QSize tilesize;
      tilesize.setWidth((size.width() - 2 * margin + spacing) / (tilewidth + spacing));
      tilesize.setHeight((size.height() - 2 * margin + spacing) / (tileheight + spacing));

      unsigned tu = gid % tilesize.width();
      unsigned tv = gid / tilesize.width();
      assert(tv < tilesize.height());

      unsigned du = margin + tu * spacing;
      unsigned dv = margin + tv * spacing;
      assert((tu + 1) * tilewidth + du < size.width());
      assert((tv + 1) * tileheight + dv < size.height());

      QRect rect(tu * tilewidth + du, tv * tileheight + dv, tilewidth, tileheight);
      painter.drawImage(origin, *texture, rect);

    } else {

      auto tile = tileset->getTile(gid);
      assert(tile);
      assert(tile->hasImage());

      auto image = tile->getImage();
      assert(image);

      QImage *texture = getTexture(image->getSource());
      painter.drawImage(origin, *texture);

    }
  }

public:
  virtual void visitTileLayer(tmx::TileLayer& layer) {
    if (!layer.isVisible()) {
      return;
    }

    std::printf("Rendering layer '%s'.\n", layer.getName().c_str());

    int k = 0;
    for (auto cell : layer) {
      int i = k % width;
      int j = k / width;
      assert(j < height);

      QPoint origin(i * tilewidth, j * tileheight);

      unsigned gid = cell.getGID();

      if (gid != 0) {
        drawGID(origin, gid);
      }

      k++;
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
