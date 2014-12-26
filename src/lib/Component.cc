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
#include <tmx/Component.h>

namespace tmx {

  bool Component::hasProperty(const std::string& key) const noexcept {
    return m_prop.count(key) > 0;
  }

  const std::string& Component::getProperty(const std::string& key, const std::string& def) const noexcept {
    auto it = m_prop.find(key);

    if (it != m_prop.end()) {
      return it->second;
    }

    return def;
  }

  bool Component::addProperty(const std::string& key, const std::string& value) {
#if __cplusplus >= 201103L
    auto ret = m_prop.emplace(key, value);
#else
    auto ret = m_prop.insert(std::make_pair(key, value));
#endif
    return ret.second;
  }

}
