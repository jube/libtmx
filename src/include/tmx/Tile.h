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
#ifndef TMX_TILE_H
#define TMX_TILE_H

#include <array>

#include "Component.h"
#include "Image.h"

namespace tmx {

  /**
   * @brief A tile is a rectangular part of a tileset.
   */
  class Tile : public Component {
  public:
    /**
     * @brief Tile constructor.
     */
    Tile(unsigned id, const std::array<unsigned, 4>& terrain, unsigned probability)
      : m_id(id), m_terrain(terrain), m_probability(probability), m_image(nullptr)
    {
    }

    /**
     * @brief Get the local id of the tile.
     *
     * @return the local id of the tile
     */
    unsigned getId() const noexcept {
      return m_id;
    }

    /**
     * @brief Get the terrains on the four corners.
     *
     * @returns the terrains
     */
    const std::array<unsigned, 4>& getTerrain() const noexcept {
      return m_terrain;
    }

    /**
     * @brief Get the terrain in the top left corner.
     *
     * @return the terrain in the top left corner
     */
    unsigned getTopLeftTerrain() const noexcept {
      return m_terrain[0];
    }

    /**
     * @brief Get the terrain in the top right corner.
     *
     * @return the terrain in the top right corner
     */
    unsigned getTopRightTerrain() const noexcept {
      return m_terrain[1];
    }

    /**
     * @brief Get the terrain in the bottom left corner.
     *
     * @return the terrain in the bottom left corner
     */
    unsigned getBottomLeftTerrain() const noexcept {
      return m_terrain[2];
    }

    /**
     * @brief Get the terrain in the bottom right corner.
     *
     * @return the terrain in the bottom right corner
     */
    unsigned getBottomRightTerrain() const noexcept {
      return m_terrain[3];
    }

    /**
     * @brief Get the probability of this tile.
     *
     * @return the probability of this tile
     */
    unsigned getProbability() const noexcept {
      return m_probability;
    }

    /**
     * @brief Get the image of this tile.
     *
     * @param image the image of this tile
     */
    void setImage(std::unique_ptr<Image> image) {
      m_image = std::move(image);
    }

    /**
     * @brief Tell whether the tile has an image.
     *
     * @returns true if the tile has an image (generally false)
     */
    bool hasImage() const noexcept {
      return m_image.get() != nullptr;
    }

    /**
     * @brief Get the image of this tile.
     *
     * @returns the image of this tile
     */
    const Image *getImage() const noexcept {
      return m_image.get();
    }

  private:
    const unsigned m_id;
    const std::array<unsigned, 4> m_terrain;
    const unsigned m_probability;

    std::unique_ptr<Image> m_image;
  };

}

#endif // TMX_TILE_H
