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
#include <tmx/Layer.h>
#include <tmx/LayerVisitor.h>
#include <tmx/ImageLayer.h>
#include <tmx/ObjectLayer.h>
#include <tmx/TileLayer.h>

namespace tmx {

  Layer::~Layer() {
  }

  void ImageLayer::accept(const Map& map, LayerVisitor& visitor) const {
    visitor.visitImageLayer(map, *this);
  }

  void ObjectLayer::accept(const Map& map, LayerVisitor& visitor) const {
    visitor.visitObjectLayer(map, *this);
  }

  void TileLayer::accept(const Map& map, LayerVisitor& visitor) const {
    visitor.visitTileLayer(map, *this);
  }

}
