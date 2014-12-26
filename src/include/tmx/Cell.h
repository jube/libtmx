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
#ifndef TMX_CELL_H
#define TMX_CELL_H

namespace tmx {

  /**
   * @brief A cell is a square on a map layer that is filled with a tile.
   */
  class Cell {
  public:

    /**
     * @brief Cell constructor.
     */
    Cell(unsigned gid, bool hflip = false, bool vflip = false, bool dflip = false)
      : m_gid(gid), m_hflip(hflip), m_vflip(vflip), m_dflip(dflip)
    {
    }

    /**
     * @brief Get the global id of the tile.
     *
     * @returns the global id of the tile.
     */
    unsigned getGID() const noexcept {
      return m_gid;
    }

    /**
     * @brief Tell whether the tile must be flipped horizontally.
     *
     * @returns true if the tile must be flipped horizontally
     */
    bool isHorizontallyFlipped() const noexcept {
      return m_hflip;
    }

    /**
     * @brief Tell whether the tile must be flipped vertically.
     *
     * @returns true if the tile must be flipped vertically
     */
    bool isVerticallyFlipped() const noexcept {
      return m_vflip;
    }

    /**
     * @brief Tell whether the tile must be flipped diagonally.
     *
     * @returns true if the tile must be flipped diagonally
     */
    bool isDiagonallyFlipped() const noexcept {
      return m_dflip;
    }

  private:
    unsigned m_gid;
    bool m_hflip;
    bool m_vflip;
    bool m_dflip;
  };

}


#endif // TMX_CELL_H
