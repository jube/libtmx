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
#ifndef TMX_TILE_H
#define TMX_TILE_H

#include <array>

#include "Base.h"
#include "Image.h"

namespace tmx {

  class Tile : public Base {
  public:
    Tile(unsigned id, const std::array<unsigned, 4>& terrain, unsigned probability)
      : m_id(id), m_terrain(terrain), m_probability(probability), m_image(nullptr)
    {
    }

    ~Tile() {
      delete m_image;
    }

    unsigned getId() const {
      return m_id;
    }

    const std::array<unsigned, 4>& getTerrain() const {
      return m_terrain;
    }

    unsigned getTopLeftTerrain() const {
      return m_terrain[0];
    }

    unsigned getTopRightTerrain() const {
      return m_terrain[1];
    }

    unsigned getBottomLeftTerrain() const {
      return m_terrain[2];
    }

    unsigned getBottomRightTerrain() const {
      return m_terrain[3];
    }

    unsigned getProbability() const {
      return m_probability;
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

  private:
    const unsigned m_id;
    const std::array<unsigned, 4> m_terrain;
    const unsigned m_probability;

    const Image *m_image;
  };

}

#endif // TMX_TILE_H
