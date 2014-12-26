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
#ifndef TMX_IMAGE_H
#define TMX_IMAGE_H

#include <string>

#include <boost/filesystem.hpp>

#include "Geometry.h"

namespace tmx {

  /**
   * @brief An image is an image file on the system.
   */
  class Image {
  public:
    /**
     * @brief Image constructor.
     */
    Image(const std::string& format, const boost::filesystem::path& source, const std::string& trans,
        unsigned width, unsigned height)
      : m_format(format), m_source(source), m_trans(trans), m_width(width), m_height(height)
    {
    }

    /**
     * @brief Get the format of the file (if provided).
     *
     * @returns the format of the file or empty string.
     */
    const std::string& getFormat() const noexcept {
      return m_format;
    }

    /**
     * @brief Get the path to the image file.
     *
     * @returns the path to the image file
     */
    const boost::filesystem::path& getSource() const noexcept {
      return m_source;
    }

    /**
     * @brief Get the transparent color (if provided)
     *
     * @returns the transparent color
     */
    const std::string getTransparent() const noexcept {
      return m_trans;
    }

    /**
     * @brief Get the width of the image (if provided)
     *
     * @returns the width of the image
     */
    unsigned getWidth() const noexcept {
      return m_width;
    }

    /**
     * @brief Get the height of the image (if provided)
     *
     * @returns the height of the image
     */
    unsigned getHeight() const noexcept {
      return m_height;
    }

    /**
     * @brief Tell whether the width and height are set.
     *
     * @returns true if the image has a declared size
     */
    bool hasSize() const noexcept {
      return m_width > 0 && m_height > 0;
    }

    /**
     * @brief Get the declared size of the image.
     *
     * @returns the size of the image.
     */
    Size getSize() const noexcept {
      return { m_width, m_height };
    }

  private:
    const std::string m_format;
    const boost::filesystem::path m_source;
    const std::string m_trans;

    const unsigned m_width;
    const unsigned m_height;
  };

}

#endif // TMX_IMAGE_H
