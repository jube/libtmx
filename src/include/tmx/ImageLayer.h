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
#ifndef TMX_IMAGE_LAYER_H
#define TMX_IMAGE_LAYER_H

#include "Layer.h"
#include "Image.h"

namespace tmx {

  /**
   * @brief An image layer is a layer with a single image.
   */
  class ImageLayer : public Layer {
  public:
    /**
     * @brief ImageLayer constructor
     */
    ImageLayer(const std::string& name, double opacity, bool visible)
      : Layer(name, opacity, visible), m_image(nullptr)
    {
    }

    virtual void accept(const Map& map, LayerVisitor& visitor) const override;

    /**
     * @brief Assign the image for the layer.
     *
     * @param image the image
     */
    void setImage(std::unique_ptr<Image> image) {
      m_image = std::move(image);
    }

    /**
     * @brief Get the image assigned to the layer
     *
     * @returns the image
     */
    const Image *getImage() const noexcept {
      return m_image.get();
    }

  private:
    std::unique_ptr<Image> m_image;
  };

}

#endif // TMX_IMAGE_LAYER_H
