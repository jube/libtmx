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
#ifndef TMX_LAYER_BASE_H
#define TMX_LAYER_BASE_H

#include "Base.h"

namespace tmx {
  class LayerVisitor;

  class Layer : public Base {
  public:
    Layer(const std::string& name, double opacity, bool visible)
      : m_name(name), m_opacity(opacity), m_visible(visible)
    {
    }

    virtual ~Layer();

    virtual void accept(LayerVisitor& visitor) = 0;

    const std::string& getName() const {
      return m_name;
    }

    double getOpacity() const {
      return m_opacity;
    }

    bool isVisible() const {
      return m_visible;
    }

  private:
    const std::string m_name;
    double m_opacity;
    bool m_visible;

  };

}


#endif // TMX_LAYER_BASE_H
