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
#ifndef TMX_GEOMETRY_H
#define TMX_GEOMETRY_H

namespace tmx {

  /**
   * @brief A size of an image.
   */
  struct Size {
    unsigned width;   /**< the width of the image */
    unsigned height;  /**< the height of the image */
  };

  /**
   * @brief A rectangle representing the portion of an image.
   */
  struct Rect {
    unsigned x;       /**< the x coordinate of the top left corner */
    unsigned y;       /**< the y coordinate of the top left corner */
    unsigned width;   /**< the width of the rectangle */
    unsigned height;  /**< the height of the rectangle */
  };

  /**
   * @brief A vector of unsigned integers.
   */
  struct Vector2u {
    unsigned x; /**< the x coordinate */
    unsigned y; /**< the y coordinate */
  };

  /**
   * @brief A vector of signed integers.
   */
  struct Vector2i {
    int x; /**< the x coordinate */
    int y; /**< the y coordinate */
  };

}


#endif // TMX_GEOMETRY_H
