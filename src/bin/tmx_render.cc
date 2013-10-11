#include <cstdio>
#include <cstdlib>

#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

#include <tmx/TMX.h>
#include <tmx/TileLayer.h>

namespace fs = boost::filesystem;

template<typename T>
class ResourceManager {
public:

  T& getOrLoad(const fs::path& path) {
    auto it = m_cache.find(path);

    if (it != m_cache.end()) {
      return *it->second;
    }

    std::unique_ptr<T> obj(new T);
    bool loaded = obj->loadFromFile(path.string());
    assert(loaded);

    auto inserted = m_cache.emplace(path, std::move(obj));
    assert(inserted.second);

    return *inserted.first->second;
  }

private:
  std::map<fs::path, std::unique_ptr<T>> m_cache;
};

class LayerRenderer : public tmx::LayerVisitor {
public:

  void renderMap(const fs::path& map_path) {

    map = tmx::parseMapFile(map_path);

    if (!map) {
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
    bool ret = result.create(width * tilewidth, height * tileheight);
    assert(ret);

    result.clear(sf::Color(0, 0, 0));
    result.setSmooth(true);

    map->visitLayers(*this);

    result.display();

    sf::Image image = result.getTexture().copyToImage();
    image.saveToFile("map.png");

    delete map;


  }

private:
  tmx::Map *map;

  ResourceManager<sf::Texture> textures;

  unsigned tilewidth;
  unsigned tileheight;
  unsigned width;
  unsigned height;

  sf::RenderTexture result;

public:
  virtual void visitTileLayer(tmx::TileLayer& layer) {
    std::printf("Rendering layer '%s'.\n", layer.getName().c_str());

    int k = 0;
    for (auto cell : layer) {
      int i = k % width;
      int j = k / width;
      assert(j < height);

      unsigned gid = cell.getGID();

      if (gid == 0) {
        k++;
        continue;
      }

      auto tileset = map->getTileSetFromGID(gid);
      assert(tileset);

      gid = gid - tileset->getFirstGID();

      sf::Sprite sprite;

      if (tileset->hasImage()) {

        auto image = tileset->getImage();
        assert(image);

        sf::Texture& tex = textures.getOrLoad(image->getSource());
        sprite.setTexture(tex);

        sf::Vector2u size = tex.getSize();
        unsigned tu = gid % (size.x / tilewidth);
        unsigned tv = gid / (size.x / tilewidth);
        assert(tv < (size.y / tileheight));
        sprite.setTextureRect(sf::IntRect(tu * tilewidth, tv * tileheight, tilewidth, tileheight));

      } else {

        auto tile = tileset->getTile(gid);
        assert(tile);
        assert(tile->hasImage());

        auto image = tile->getImage();
        assert(image);

        sf::Texture& tex = textures.getOrLoad(image->getSource());
        sprite.setTexture(tex);

      }

      sprite.setPosition(i * tilewidth, j * tileheight);
      result.draw(sprite);

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
