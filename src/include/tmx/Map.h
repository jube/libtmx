/*
 * Copyright (c) 2013-2014, Julien Bernard
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

#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "Layer.h"
#include "LayerVisitor.h"
#include "TileSet.h"

/**
 * @brief The namespace for all `libtmx` classes.
 */
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
   * @brief the render order of the tiles.
   */
  enum class RenderOrder {
    RIGHT_DOWN, /**< Right down order */
    RIGHT_UP,   /**< Right up order */
    LEFT_DOWN,  /**< Left down order */
    LEFT_UP,    /**< Left up order */
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
        unsigned tilewidth, unsigned tileheight, const std::string& bgcolor, RenderOrder render_order)
      : m_version(version), m_orientation(orientation), m_width(width), m_height(height),
        m_tilewidth(tilewidth), m_tileheight(tileheight), m_bgcolor(bgcolor), m_render_order(render_order)
    {
    }

    /**
     * @name Properties
     * @{
     */
    /**
     * @brief Get the version of the TMX format.
     *
     * @returns the version of the TMX format (generally "1.0")
     */
    const std::string& getVersion() const noexcept {
      return m_version;
    }

    /**
     * @brief Get the orientation of the map.
     *
     * @returns the orientation of the map
     */
    Orientation getOrientation() const noexcept {
      return m_orientation;
    }

    /**
     * @brief Get the width of the map.
     *
     * @returns the width of the map (in number of tiles)
     */
    unsigned getWidth() const noexcept {
      return m_width;
    }

    /**
     * @brief Get the height of the map.
     *
     * @returns the height of the map (in number of tiles)
     */
    unsigned getHeight() const noexcept {
      return m_height;
    }

    /**
     * @brief Get the width of tiles.
     *
     * @returns the width of tiles (in pixels)
     */
    unsigned getTileWidth() const noexcept {
      return m_tilewidth;
    }

    /**
     * @brief Get the height of tiles.
     *
     * @returns the height of tiles (in pixels)
     */
    unsigned getTileHeight() const noexcept {
      return m_tileheight;
    }

    /**
     * @brief Get the background color.
     *
     * @returns the background color
     */
    const std::string& getBackgroundColor() const noexcept {
      return m_bgcolor;
    }

    /**
     * @brief Get the render order.
     *
     * @returns the render order
     */
    RenderOrder getRenderOrder() const noexcept {
      return m_render_order;
    }
    /** @} */

    /**
     * @brief A tileset range.
     */
    typedef boost::transformed_range<Adaptor, const boost::iterator_range<std::vector<std::unique_ptr<TileSet>>::const_iterator>> const_tileset_range;

    /**
     * @name Tileset handling
     * @{
     */
    /**
     * @brief Add a tileset.
     *
     * @param tileset the tileset
     */
    void addTileSet(std::unique_ptr<TileSet> tileset) {
      if (!tileset) {
        return;
      }

      m_tilesets.emplace_back(std::move(tileset));
    }

    /**
     * @brief Get the tilesets.
     *
     * @return a tileset range
     */
    const_tileset_range getTileSets() const noexcept {
      return boost::make_iterator_range(m_tilesets) | boost::adaptors::transformed(Adaptor());
    }

    /**
     * @brief Get the tileset corresponding to a global id.
     *
     * @param gid a global id
     * @returns the corresponding tileset
     */
    const TileSet *getTileSetFromGID(unsigned gid) const noexcept;
    /** @} */

    /**
     * @brief A layer range.
     */
    typedef boost::transformed_range<Adaptor, const boost::iterator_range<std::vector<std::unique_ptr<Layer>>::const_iterator>> const_layer_range;

    /**
     * @name Layer handling
     * @{
     */
    /**
     * @brief Add a layer.
     *
     * @param layer the layer
     */
    void addLayer(std::unique_ptr<Layer> layer) {
      m_layers.emplace_back(std::move(layer));
    }

    /**
     * @brief Get the layers.
     *
     * @returns a layer range
     */
    const_layer_range getLayers() const noexcept {
      return boost::make_iterator_range(m_layers) | boost::adaptors::transformed(Adaptor());
    }

    /**
     * @brief Visit the layers with a visitor.
     *
     * @param visitor the visitor
     */
    void visitLayers(LayerVisitor& visitor) const {
      for (auto layer : getLayers()) {
        layer->accept(*this, visitor);
      }
    }
    /** @} */

    /**
     * @name Map parsing
     * @{
     */
    /**
     * @brief Parse a TMX file.
     *
     * @param filename the name of the TMX file
     * @returns a map
     */
    static std::unique_ptr<Map> parseFile(const boost::filesystem::path& filename);
    /** @} */

  private:
    const std::string m_version;

    const Orientation m_orientation;

    const unsigned m_width;
    const unsigned m_height;

    const unsigned m_tilewidth;
    const unsigned m_tileheight;

    const std::string m_bgcolor;

    const RenderOrder m_render_order;

    std::vector<std::unique_ptr<TileSet>> m_tilesets;
    std::vector<std::unique_ptr<Layer>> m_layers;
  };

}

#endif // TMX_MAP_H
