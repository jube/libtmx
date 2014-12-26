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
#ifndef TMX_OBJECT_LAYER_H
#define TMX_OBJECT_LAYER_H

#include <vector>

#include <boost/iterator/transform_iterator.hpp>

#include "Adaptor.h"
#include "Layer.h"
#include "Object.h"

namespace tmx {

  /**
   * @brief An object layer is a layers composed of objects.
   *
   * An object layer is called object group in the TMX format.
   */
  class ObjectLayer : public Layer {
  public:
    /**
     * @brief ObjectLayer constructor.
     */
    ObjectLayer(const std::string& name, double opacity, bool visible, const std::string& color)
      : Layer(name, opacity, visible), m_color(color)
    {
    }

    virtual void accept(const Map& map, LayerVisitor& visitor) const override;

    /**
     * @brief Get the color used to display the objects.
     *
     * @return the color to display the objects
     */
    const std::string& getColor() noexcept {
      return m_color;
    }

    /**
     * @brief Add an object.
     *
     * @param obj the object
     */
    void addObject(std::unique_ptr<Object> obj) {
      m_objects.emplace_back(std::move(obj));
    }

    /**
     * @brief An object iterator.
     */
    typedef boost::transform_iterator<Adaptor, std::vector<std::unique_ptr<Object>>::const_iterator> const_iterator;

    /**
     * @brief Get the begin iterator on the objects.
     *
     * @return the begin iterator
     */
    const_iterator begin() const noexcept {
      return boost::make_transform_iterator<Adaptor>(m_objects.cbegin());
    }

    /**
     * @brief Get the end iterator on the objects.
     *
     * @return the end iterator
     */
    const_iterator end() const noexcept {
      return boost::make_transform_iterator<Adaptor>(m_objects.cend());
    }

  private:
    const std::string m_color;
    std::vector<std::unique_ptr<Object>> m_objects;
  };

}

#endif // TMX_OBJECT_LAYER_H
