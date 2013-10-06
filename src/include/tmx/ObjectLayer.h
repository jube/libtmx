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
#ifndef TMX_OBJECT_LAYER_H
#define TMX_OBJECT_LAYER_H

#include <vector>

#include "Layer.h"
#include "Object.h"

namespace tmx {

  class ObjectLayer : public Layer {
  public:
    ObjectLayer(const std::string& name, double opacity, bool visible, const std::string& color)
      : Layer(name, opacity, visible), m_color(color)
    {
    }

    ~ObjectLayer() {
      for (auto item : m_objects) {
        delete item;
      }
    }

    virtual void accept(LayerVisitor& visitor);

    const std::string& getColor() {
      return m_color;
    }

    void addObject(Object *obj) {
      m_objects.emplace_back(obj);
    }

    typedef std::vector<Object*>::const_iterator iterator;

    iterator begin() const {
      return m_objects.cbegin();
    }

    iterator end() const {
      return m_objects.cend();
    }

  private:
    const std::string m_color;
    std::vector<Object*> m_objects;
  };

}

#endif // TMX_OBJECT_LAYER_H
