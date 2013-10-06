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
#ifndef TMX_OBJECT_H
#define TMX_OBJECT_H

#include <string>
#include <vector>

#include "Base.h"

namespace tmx {

  struct Vector2u {
    unsigned x;
    unsigned y;
  };

  struct Vector2i {
    int x;
    int y;
  };

  class Object : public Base {
  public:
    enum Kind {
      RECTANGLE,
      ELLIPSE,
      POLYLINE,
      POLYGON,
    };

    Object(const Kind kind, const std::string& name, const std::string& type,
        const Vector2u& origin, unsigned gid, bool visible)
      : m_kind(kind), m_name(name), m_type(type), m_origin(origin), m_gid(gid), m_visible(visible)
    {
    }

    virtual ~Object();

    const std::string& getName() const {
      return m_name;
    }

    const std::string& getType() const {
      return m_type;
    }

    unsigned getX() const {
      return m_origin.x;
    }

    unsigned getY() const {
      return m_origin.y;
    }

    const Vector2u& getOrigin() const {
      return m_origin;
    }

    unsigned getGID() const {
      return m_gid;
    }

    bool isVisible() const {
      return m_visible;
    }

    bool isRectangle() const {
      return m_kind == RECTANGLE;
    }

    bool isEllipse() const {
      return m_kind == ELLIPSE;
    }

    bool isPolyline() const {
      return m_kind == POLYLINE;
    }

    bool isPolygon() const {
      return m_kind == POLYGON;
    }

  private:
    const Kind m_kind;
    const std::string m_name;
    const std::string m_type;
    const Vector2u m_origin;
    const unsigned m_gid;
    const bool m_visible;
  };

  class Boxed : public Object {
  public:

    Boxed(Kind kind, const std::string& name, const std::string& type,
        const Vector2u& origin, unsigned gid, bool visible, unsigned width, unsigned height)
      : Object(kind, name, type, origin, gid, visible), m_width(width), m_height(height)
    {
    }

    unsigned getWidth() const {
      return m_width;
    }

    unsigned getHeight() const {
      return m_height;
    }

  private:
    const unsigned m_width;
    const unsigned m_height;
  };


  class Rectangle : public Boxed {
  public:
    Rectangle(const std::string& name, const std::string& type,
        const Vector2u& origin, unsigned gid, bool visible, unsigned width, unsigned height)
      : Boxed(RECTANGLE, name, type, origin, gid, visible, width, height)
    {
    }
  };

  class Ellipse : public Boxed {
  public:
    Ellipse(const std::string& name, const std::string& type,
        const Vector2u& origin, unsigned gid, bool visible, unsigned width, unsigned height)
      : Boxed(ELLIPSE, name, type, origin, gid, visible, width, height)
    {
    }
  };

  class PolyBase : public Object {
  public:
    PolyBase(const Kind kind, const std::string& name, const std::string& type, const Vector2u& origin, unsigned gid, bool visible)
      : Object(kind, name, type, origin, gid, visible)
    {
    }

    void setPoints(const std::vector<Vector2i>& points) {
      m_points = points;
    }

    typedef typename std::vector<Vector2i>::const_iterator const_iterator;

    const_iterator begin() const {
      return m_points.cbegin();
    }

    const_iterator end() const {
      return m_points.cend();
    }

  private:
    std::vector<Vector2i> m_points;
  };

  class Polyline : public PolyBase {
  public:
    Polyline(const std::string& name, const std::string& type, const Vector2u& origin, unsigned gid, bool visible)
      : PolyBase(POLYLINE, name, type, origin, gid, visible)
    {
    }
  };

  class Polygon : public PolyBase {
  public:
    Polygon(const std::string& name, const std::string& type, const Vector2u& origin, unsigned gid, bool visible)
      : PolyBase(POLYLINE, name, type, origin, gid, visible)
    {
    }
  };

}

#endif // TMX_OBJECT_H
