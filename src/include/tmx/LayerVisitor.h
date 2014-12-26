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
#ifndef TMX_LAYER_VISITOR_H
#define TMX_LAYER_VISITOR_H

namespace tmx {
  class Map;
  class TileLayer;
  class ObjectLayer;
  class ImageLayer;

  /**
   * @brief A layer visitor is a visitor for layers in the visitor pattern.
   */
  class LayerVisitor {
  public:
    /**
     * @brief LayerVisitor destructor.
     */
    virtual ~LayerVisitor();

    /**
     * @brief Visit a tile layer.
     *
     * @param layer the tile layer
     */
    virtual void visitTileLayer(const Map& map, const TileLayer& layer);

    /**
     * @brief Visit an object layer.
     *
     * @param layer the object layer
     */
    virtual void visitObjectLayer(const Map& map, const ObjectLayer& layer);

    /**
     * @brief Visit an image layer.
     *
     * @param layer the image layer
     */
    virtual void visitImageLayer(const Map& map, const ImageLayer& layer);
  };

}


#endif // TMX_LAYER_VISITOR_H
