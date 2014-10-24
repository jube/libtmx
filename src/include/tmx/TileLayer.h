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
#ifndef TMX_LAYER_H
#define TMX_LAYER_H

#include <vector>

#include "Cell.h"
#include "Layer.h"

namespace tmx {

  /**
   * @brief A tile layer is a layer with tiles in cells.
   */
  class TileLayer : public Layer {
  public:
    /**
     * @brief TileLayer constructor.
     */
    TileLayer(const std::string& name, double opacity, bool visible)
      : Layer(name, opacity, visible)
    {
    }

    virtual void accept(const Map& map, LayerVisitor& visitor) const override;

    /**
     * @brief Add a cell to the layer.
     *
     * @param cell the cell
     */
    void addCell(Cell cell) {
      m_cells.emplace_back(cell);
    }

    /**
     * @brief A cell iterator.
     */
    typedef typename std::vector<Cell>::const_iterator const_iterator;

    /**
     * @brief Get the begin iterator on the cells.
     *
     * @return the begin iterator
     */
    const_iterator begin() const noexcept {
      return m_cells.cbegin();
    }

    /**
     * @brief Get the end iterator on the cells.
     *
     * @return the end iterator
     */
    const_iterator end() const noexcept {
      return m_cells.cend();
    }

  private:
    std::vector<Cell> m_cells;
  };

}

#endif // TMX_LAYER_H
