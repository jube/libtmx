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
#ifndef TMX_ADAPTOR_H
#define TMX_ADAPTOR_H

#include <memory>


namespace tmx {

  /**
   * @brief An adaptor to get raw pointer from a unique pointer
   */
  struct Adaptor {

    /**
     * @brief Get the raw pointer from a unique pointer
     *
     * @param p the unique pointer
     * @return the corresponding raw pointer
     */
    template<typename T>
    const T *operator()(const std::unique_ptr<T>& p) const {
      return p.get();
    }
  };

}

#endif // TMX_ADAPTOR_H
