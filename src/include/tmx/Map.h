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

  /**
   * @brief the orientation of the map.
   */
  enum class Orientation {
    UNKNOWN,    /**< Unknown orientation */
    ORTHOGONAL, /**< Orthogonal orientation */
    ISOMETRIC,  /**< Isometric orientation */
    STAGGERED,  /**< Staggered orientation */
  };

  /**
   * @brief A map is a set of tilesets and a set of different layers.
   *
   * Tilesets describe what to draw while layers describe how to draw things.
   */
  class Map {
  public:
    /**
     * @brief Map constructor.
     */
    Map(const std::string version, Orientation orientation, unsigned width, unsigned height,
        unsigned tilewidth, unsigned tileheight, const std::string& bgcolor)
      : m_version(version), m_orientation(orientation), m_width(width), m_height(height),
        m_tilewidth(tilewidth), m_tileheight(tileheight), m_bgcolor(bgcolor)
    {
    }

    /**
     * @brief Map destructor.
     */
    ~Map() {
      for (auto item : m_tilesets) {
        delete item;
      }

      for (auto item : m_layers) {
        delete item;
      }
    }

    /**
     * @brief Get the version of the TMX format.
     *
     * @returns the version of the TMX format (generally "1.0")
     */
    const std::string& getVersion() const {
      return m_version;
    }

    /**
     * @brief Get the orientation of the map.
     *
     * @returns the orientation of the map
     */
    Orientation getOrientation() const {
      return m_orientation;
    }

    /**
     * @brief Get the width of the map.
     *
     * @returns the width of the map (in number of tiles)
     */
    unsigned getWidth() const {
      return m_width;
    }

    /**
     * @brief Get the height of the map.
     *
     * @returns the height of the map (in number of tiles)
     */
    unsigned getHeight() const {
      return m_height;
    }

    /**
     * @brief Get the width of tiles.
     *
     * @returns the width of tiles (in pixels)
     */
    unsigned getTileWidth() const {
      return m_tilewidth;
    }

    /**
     * @brief Get the height of tiles.
     *
     * @returns the height of tiles (in pixels)
     */
    unsigned getTileHeight() const {
      return m_tileheight;
    }

    /**
     * @brief Get the background color.
     *
     * @returns the background color
     */
    const std::string& getBackgroundColor() const {
      return m_bgcolor;
    }

    /**
     * @brief Add a tileset.
     *
     * @param tileset the tileset
     */
    void addTileSet(TileSet *tileset) {
      if (tileset == nullptr) {
        return;
      }
      m_tilesets.emplace_back(tileset);
    }

    /**
     * @brief A tileset range.
     */
    typedef boost::iterator_range<std::vector<TileSet*>::const_iterator> const_tileset_range;

    /**
     * @brief Get the tilesets.
     *
     * @return a tileset range
     */
    const_tileset_range getTileSets() const {
      return boost::make_iterator_range(m_tilesets);
    }

    /**
     * @brief Add a layer.
     *
     * @param layer the layer
     */
    void addLayer(Layer *layer) {
      m_layers.emplace_back(layer);
    }

    /**
     * @brief A layer range.
     */
    typedef boost::iterator_range<std::vector<Layer*>::const_iterator> const_layer_range;

    /**
     * @brief Get the layers.
     *
     * @returns a layer range
     */
    const_layer_range getLayers() const {
      return boost::make_iterator_range(m_layers);
    }

    /**
     * @brief Visit the layers with a visitor.
     *
     * @param visitor the visitor
     */
    void visitLayers(LayerVisitor& visitor) {
      for (auto layer : m_layers) {
        layer->accept(visitor);
      }
    }

    /**
     * @brief Get the tileset corresponding to a global id.
     *
     * @param gid a global id
     * @returns the corresponding tileset
     */
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
