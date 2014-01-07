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
#ifndef TMX_TILE_SET_H
#define TMX_TILE_SET_H

#include <string>
#include <vector>

#include <boost/range/iterator_range.hpp>

#include "Base.h"
#include "Image.h"
#include "Terrain.h"
#include "Tile.h"

namespace tmx {

  /**
   * @brief A rectangle representing the portion of the image corresponding
   * to an id.
   */
  struct Rect {
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;
  };

  /**
   * @brief A tileset is a set of tiles in a single file (image or TSX file).
   */
  class TileSet : public Base {
  public:
    /**
     * @brief TileSet constructor.
     */
    TileSet(unsigned firstgid, const std::string& name, unsigned tilewidth, unsigned tileheight, unsigned spacing, unsigned margin)
      : m_firstgid(firstgid), m_name(name), m_tilewidth(tilewidth), m_tileheight(tileheight), m_spacing(spacing), m_margin(margin),
      m_x(0), m_y(0), m_image(nullptr)
    {
    }

    /**
     * @brief TileSet destructor.
     */
    ~TileSet() {
      delete m_image;

      for (auto item : m_terrains) {
        delete item;
      }

      for (auto item : m_tiles) {
        delete item;
      }
    }

    /**
     * @brief Get the first global id of this tileset.
     *
     * @return the first global id of the tileset
     */
    unsigned getFirstGID() const {
      return m_firstgid;
    }

    /**
     * @brief Get the name of the tileset.
     *
     * @return the name of the tileset
     */
    const std::string& getName() const {
      return m_name;
    }

    /**
     * @brief Get the width of the tiles.
     *
     * @return the width of the tiles
     */
    unsigned getTileWidth() const {
      return m_tilewidth;
    }

    /**
     * @brief Get the height of the tiles.
     *
     * @return the height of the tiles
     */
    unsigned getTileHeight() const {
      return m_tileheight;
    }

    /**
     * @brief Get the spacing between tiles.
     *
     * @return the spacing between tiles (in pixels)
     */
    unsigned getSpacing() const {
      return m_spacing;
    }

    /**
     * @brief Get the margin around tiles.
     *
     * @returns the margin around tiles (in pixels)
     */
    unsigned getMargin() const {
      return m_margin;
    }

    /**
     * @brief Set the offset of the tileset.
     *
     * @param x the x coordinate of the offset
     * @param y the y coordinate of the offset
     */
    void setOffset(int x, int y) {
      m_x = x;
      m_y = y;
    }

    /**
     * @brief Get the x offset of the tileset.
     *
     * @returns the x offset of the tileset (in pixels)
     */
    int getOffsetX() const {
      return m_x;
    }

    /**
     * @brief Get the y offset of the tileset.
     *
     * @returns the y offset of the tileset (in pixels)
     */
    int getOffsetY() const {
      return m_y;
    }

    /**
     * @brief Set an image associated to the tileset.
     *
     * @param image the image associated to the tileset
     */
    void setImage(Image *image) {
      m_image = image;
    }

    /**
     * @brief Tell whether the tileset has an image.
     *
     * @returns true if the tileset has an image
     */
    bool hasImage() const {
      return m_image != nullptr;
    }

    /**
     * @brief Get the image associated to the tileset.
     *
     * @returns the image associated to the tileset
     */
    const Image *getImage() const {
      return m_image;
    }

    /**
     * @brief Add terrain information to the tileset.
     *
     * @param terrain the terrain information
     */
    void addTerrain(Terrain *terrain) {
      m_terrains.emplace_back(terrain);
    }

    /**
     * @brief A terrain range.
     */
    typedef boost::iterator_range<std::vector<Terrain*>::const_iterator> const_terrain_range;

    /**
     * @brief Get the terrains.
     *
     * @returns a terrain range
     */
    const_terrain_range getTerrains() const {
      return boost::make_iterator_range(m_terrains);
    }

    /**
     * @brief Add a tile to the tileset.
     *
     * @param tile the tile
     */
    void addTile(Tile *tile) {
      m_tiles.emplace_back(tile);
    }

    /**
     * @brief A tile iterator.
     */
    typedef std::vector<Tile*>::const_iterator const_iterator;

    /**
     * @brief Get the begin iterator on the tiles.
     *
     * @return the begin iterator on the tiles
     */
    const_iterator begin() const {
      return m_tiles.cbegin();
    }

    /**
     * @brief Get the end iterator on the tiles.
     *
     * @return the end iterator on the tiles
     */
    const_iterator end() const {
      return m_tiles.cend();
    }

    /**
     * @brief Get the tile corresponding to an id.
     *
     * @param id the id of the tile
     * @returns the tile
     */
    const Tile *getTile(unsigned id) const;

    /**
     * @brief Get the coordinates of a tile corresponding to an id.
     *
     * @param id the id of the tile
     * @param size the size of the image corresponding to the tile
     * @returns the coordinates in the form of a rectangle
     */
    Rect getCoords(unsigned id, Size size) const;

  private:
    const unsigned m_firstgid;
    const std::string m_name;
    const unsigned m_tilewidth;
    const unsigned m_tileheight;
    const unsigned m_spacing;
    const unsigned m_margin;

    int m_x;
    int m_y;

    const Image *m_image;
    std::vector<Terrain*> m_terrains;
    std::vector<Tile*> m_tiles;
  };

}

#endif // TMX_TILE_SET_H
