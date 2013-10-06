/*
 * Copyright (c) 2013, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef TMX_MAP_H
#define TMX_MAP_H

#include <cstddef>

#include <string>
#include <vector>

#include <boost/range/iterator_range.hpp>

#include "Layer.h"
#include "LayerVisitor.h"
#include "TileSet.h"

namespace tmx {

  enum class Orientation {
    UNKNOWN,
    ORTHOGONAL,
    ISOMETRIC,
    STAGGERED,
  };

  class Map {
  public:
    Map(const std::string version, Orientation orientation, unsigned width, unsigned height,
        unsigned tilewidth, unsigned tileheight, const std::string& bgcolor)
      : m_version(version), m_orientation(orientation), m_width(width), m_height(height),
        m_tilewidth(tilewidth), m_tileheight(tileheight), m_bgcolor(bgcolor)
    {
    }

    ~Map() {
      for (auto item : m_tilesets) {
        delete item;
      }

      for (auto item : m_layers) {
        delete item;
      }
    }

    const std::string& getVersion() const {
      return m_version;
    }

    Orientation getOrientation() const {
      return m_orientation;
    }

    unsigned getWidth() const {
      return m_width;
    }

    unsigned getHeight() const {
      return m_height;
    }

    unsigned getTileWidth() const {
      return m_tilewidth;
    }

    unsigned getTileHeight() const {
      return m_tileheight;
    }

    const std::string& getBackgroundColor() const {
      return m_bgcolor;
    }

    void addTileSet(TileSet *tileset) {
      if (tileset == nullptr) {
        return;
      }
      m_tilesets.emplace_back(tileset);
    }

    typedef boost::iterator_range<std::vector<TileSet*>::const_iterator> const_tileset_range;

    const_tileset_range getTileSets() const {
      return boost::make_iterator_range(m_tilesets);
    }

    void addLayer(Layer *layer) {
      m_layers.emplace_back(layer);
    }

    typedef boost::iterator_range<std::vector<Layer*>::const_iterator> const_layer_range;

    const_layer_range getLayers() const {
      return boost::make_iterator_range(m_layers);
    }

    void visitLayers(LayerVisitor& visitor) {
      for (auto layer : m_layers) {
        layer->accept(visitor);
      }
    }

    TileSet *getTileSetFromGID(unsigned gid);

  private:
    const std::string m_version;

    const Orientation m_orientation;

    const unsigned m_width;
    const unsigned m_height;

    const unsigned m_tilewidth;
    const unsigned m_tileheight;

    const std::string m_bgcolor;

    std::vector<TileSet*>     m_tilesets;
    std::vector<Layer*>       m_layers;
  };

}

#endif // TMX_MAP_H
