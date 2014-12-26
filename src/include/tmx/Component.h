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
#ifndef TMX_COMPONENT_H
#define TMX_COMPONENT_H

#include <map>
#include <string>

namespace tmx {

  /**
   * @brief A base class for classes that have properties.
   */
  class Component {
  public:

    /**
     * @brief Tell if the object has a given property.
     *
     * @param key the property key
     * @returns true if the object has the given property
     */
    bool hasProperty(const std::string& key) const noexcept;

    /**
     * @brief Get a property value.
     *
     * @param key the property key
     * @param def a default value if the property does not exist
     * @returns the value of the given property
     */
    const std::string& getProperty(const std::string& key, const std::string& def) const noexcept;

    /**
     * @brief Add a property.
     *
     * @param key the property key
     * @param value the property value
     */
    bool addProperty(const std::string& key, const std::string& value);

  private:
    std::map<std::string, std::string> m_prop;
  };


}


#endif // TMX_COMPONENT_H
