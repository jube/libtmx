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
#ifndef TMX_IMAGE_LAYER_H
#define TMX_IMAGE_LAYER_H

#include "Layer.h"
#include "Image.h"

namespace tmx {

  class ImageLayer : public Layer {
  public:
    ImageLayer(const std::string& name, double opacity, bool visible)
      : Layer(name, opacity, visible), m_image(nullptr)
    {
    }

    virtual void accept(LayerVisitor& visitor);

    ~ImageLayer() {
      delete m_image;
    }

    void setImage(Image *image) {
      m_image = image;
    }

    const Image *getImage() const {
      return m_image;
    }

  private:
    const Image *m_image;
  };

}

#endif // TMX_IMAGE_LAYER_H
