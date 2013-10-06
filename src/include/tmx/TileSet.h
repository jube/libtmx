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

  class TileSet : public Base {
  public:
    TileSet(unsigned firstgid, const std::string& name, unsigned tilewidth, unsigned tileheight, unsigned spacing, unsigned margin)
      : m_firstgid(firstgid), m_name(name), m_tilewidth(tilewidth), m_tileheight(tileheight), m_spacing(spacing), m_margin(margin),
      m_x(0), m_y(0), m_image(nullptr)
    {
    }

    ~TileSet() {
      delete m_image;

      for (auto item : m_terrains) {
        delete item;
      }

      for (auto item : m_tiles) {
        delete item;
      }
    }

    unsigned getFirstGID() const {
      return m_firstgid;
    }

    const std::string& getName() const {
      return m_name;
    }

    unsigned getTileWidth() const {
      return m_tilewidth;
    }

    unsigned getTileHeight() const {
      return m_tileheight;
    }

    unsigned getSpacing() const {
      return m_spacing;
    }

    unsigned getMargin() const {
      return m_margin;
    }

    void setOffset(int x, int y) {
      m_x = x;
      m_y = y;
    }

    int getOffsetX() const {
      return m_x;
    }

    int getOffsetY() const {
      return m_y;
    }

    void setImage(Image *image) {
      m_image = image;
    }

    bool hasImage() const {
      return m_image != nullptr;
    }

    const Image *getImage() const {
      return m_image;
    }

    void addTerrain(Terrain *terrain) {
      m_terrains.emplace_back(terrain);
    }

    typedef boost::iterator_range<std::vector<Terrain*>::const_iterator> const_terrain_range;

    const_terrain_range getTerrains() const {
      return boost::make_iterator_range(m_terrains);
    }


    void addTile(Tile *tile) {
      m_tiles.emplace_back(tile);
    }

    typedef std::vector<Tile*>::const_iterator const_iterator;

    const_iterator begin() const {
      return m_tiles.cbegin();
    }

    const_iterator end() const {
      return m_tiles.cend();
    }

    const Tile *getTile(unsigned id) const;

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
