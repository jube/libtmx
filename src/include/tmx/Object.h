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
#ifndef TMX_OBJECT_H
#define TMX_OBJECT_H

#include <string>
#include <vector>

#include "Component.h"
#include "Geometry.h"

namespace tmx {

  /**
   * @brief An object is a geometrical object.
   *
   * There four kinds of geometrical objects: rectangles, ellipses, polylines
   * and polygons.
   */
  class Object : public Component {
  public:
    /**
     * @brief A kind of geometrical object.
     */
    enum Kind {
      RECTANGLE,  /**< Rectangle */
      ELLIPSE,    /**< Ellipse */
      POLYLINE,   /**< Polyline (open line) */
      POLYGON,    /**< Polygon (closed line) */
      TILE,       /**< Tile (image) */
    };

    /**
     * @brief Object constructor.
     */
    Object(const Kind kind, const std::string& name, const std::string& type,
        const Vector2u& origin, double rotation, bool visible)
      : m_kind(kind), m_name(name), m_type(type), m_origin(origin), m_rotation(rotation), m_visible(visible)
    {
    }

    /**
     * @brief Object destructor.
     */
    virtual ~Object();

    /**
     * @brief Get the kind of the object.
     *
     * This may be useful to do some casting.
     *
     * @return the kind of the object
     */
    Kind getKind() const noexcept {
      return m_kind;
    }

    /**
     * @brief Get the name of the object.
     *
     * @return the name of the object
     */
    const std::string& getName() const noexcept {
      return m_name;
    }

    /**
     * @brief Get the type of the object.
     *
     * @return the type of the object.
     */
    const std::string& getType() const noexcept {
      return m_type;
    }

    /**
     * @brief Get the x coordinate of the origin.
     *
     * @return the x coordinate of the origin
     */
    unsigned getX() const noexcept {
      return m_origin.x;
    }

    /**
     * @brief Get the y coordinate of the origin.
     *
     * @return the y coordinate of the origin
     */
    unsigned getY() const noexcept {
      return m_origin.y;
    }

    /**
     * @brief Get the origin.
     *
     * The origin is the base point of the object on the map. All other
     * coordinates are computed from this point.
     *
     * @return the origin
     */
    const Vector2u& getOrigin() const noexcept {
      return m_origin;
    }

    /**
     * @brief Get the rotation of the object.
     *
     * @return the angle of rotation in degrees clockwise
     */
    double getRotation() const noexcept {
      return m_rotation;
    }

    /**
     * @brief Tell whether this object is visible.
     *
     * @returns true if the object is visible
     */
    bool isVisible() const noexcept {
      return m_visible;
    }

    /**
     * @brief Tell whether this object is a rectangle.
     *
     * @returns true if the object is a rectangle
     */
    bool isRectangle() const noexcept {
      return m_kind == RECTANGLE;
    }

    /**
     * @brief Tell whether this object is an ellipse.
     *
     * @returns true if the object is an ellipse
     */
    bool isEllipse() const noexcept {
      return m_kind == ELLIPSE;
    }

    /**
     * @brief Tell whether this object is a polyline.
     *
     * @returns true if the object is a polyline
     */
    bool isPolyline() const noexcept {
      return m_kind == POLYLINE;
    }

    /**
     * @brief Tell whether this object is a polygon.
     *
     * @returns true if the object is a polygon
     */
    bool isPolygon() const noexcept {
      return m_kind == POLYGON;
    }

    /**
     * @brief Tell whether this object is a tile.
     *
     * @returns true if the object is a tile
     */
    bool isTile() const noexcept {
      return m_kind == TILE;
    }

  private:
    const Kind m_kind;
    const std::string m_name;
    const std::string m_type;
    const Vector2u m_origin;
    const double m_rotation;
    const bool m_visible;
  };

  /**
   * @brief A tile object is an image put in the map.
   */
  class TileObject : public Object {
  public:
    /**
     * @brief TileObject constructor.
     */
    TileObject(const std::string& name, const std::string& type,
        const Vector2u& origin, double rotation, bool visible, unsigned gid, bool hflip, bool vflip, bool dflip)
      : Object(TILE, name, type, origin, rotation, visible)
      , m_gid(gid), m_hflip(hflip), m_vflip(vflip), m_dflip(dflip)
    {
    }

    /**
     * @brief Get the global id of the refering tile (if needed)
     *
     * @return the global id
     */
    unsigned getGID() const noexcept {
      return m_gid;
    }

    /**
     * @brief Tell whether the tile object must be flipped horizontally.
     *
     * @returns true if the tile object must be flipped horizontally
     */
    bool isHorizontallyFlipped() const noexcept {
      return m_hflip;
    }

    /**
     * @brief Tell whether the tile object must be flipped vertically.
     *
     * @returns true if the tile object must be flipped vertically
     */
    bool isVerticallyFlipped() const noexcept {
      return m_vflip;
    }

    /**
     * @brief Tell whether the tile object must be flipped diagonally.
     *
     * @returns true if the tile object must be flipped diagonally
     */
    bool isDiagonallyFlipped() const noexcept {
      return m_dflip;
    }

  private:
    const unsigned m_gid;
    bool m_hflip;
    bool m_vflip;
    bool m_dflip;
  };

  /**
   * @brief A boxed object is a geometrical object that is defined by a box.
   *
   * There are two kinds of boxed objects: rectangles and ellipses.
   */
  class Boxed : public Object {
  public:

    /**
     * @brief Boxed constructor.
     */
    Boxed(Kind kind, const std::string& name, const std::string& type,
        const Vector2u& origin, double rotation, bool visible, unsigned width, unsigned height)
      : Object(kind, name, type, origin, rotation, visible), m_width(width), m_height(height)
    {
    }

    /**
     * @brief Get the width of the box.
     *
     * @returns the width of the box
     */
    unsigned getWidth() const noexcept {
      return m_width;
    }

    /**
     * @brief Get the height of the box.
     *
     * @returns the height of the box
     */
    unsigned getHeight() const noexcept {
      return m_height;
    }

  private:
    const unsigned m_width;
    const unsigned m_height;
  };


  /**
   * @brief A rectangle is a geometrical object.
   */
  class Rectangle : public Boxed {
  public:

    /**
     * @brief Rectangle constructor.
     */
    Rectangle(const std::string& name, const std::string& type,
        const Vector2u& origin, double rotation, bool visible, unsigned width, unsigned height)
      : Boxed(RECTANGLE, name, type, origin, rotation, visible, width, height)
    {
    }
  };

  /**
   * @brief An ellipse is a geometrical object.
   */
  class Ellipse : public Boxed {
  public:

    /**
     * @brief Ellipse constructor.
     */
    Ellipse(const std::string& name, const std::string& type,
        const Vector2u& origin, double rotation, bool visible, unsigned width, unsigned height)
      : Boxed(ELLIPSE, name, type, origin, rotation, visible, width, height)
    {
    }
  };

  /**
   * @brief A "polybase" is a base class for lines with multiple points.
   *
   * There are two kinds of polybase: polylines and polygons.
   */
  class PolyBase : public Object {
  public:
    /**
     * @brief PolyBase constructor.
     */
    PolyBase(const Kind kind, const std::string& name, const std::string& type, const Vector2u& origin, double rotation, bool visible)
      : Object(kind, name, type, origin, rotation, visible)
    {
    }

    /**
     * @brief Set the points of the lines.
     *
     * @param points the points
     */
    void setPoints(std::vector<Vector2i> points) {
      m_points = std::move(points);
    }

    /**
     * @brief A point iterator.
     */
    typedef typename std::vector<Vector2i>::const_iterator const_iterator;

    /**
     * @brief Get the begin iterator on the points.
     *
     * @returns the begin iterator
     */
    const_iterator begin() const noexcept {
      return m_points.cbegin();
    }

    /**
     * @brief Get the end iterator on the points.
     *
     * @returns the end iterator
     */
    const_iterator end() const noexcept {
      return m_points.cend();
    }

  private:
    std::vector<Vector2i> m_points;
  };

  /**
   * @brief A polyline is an open set of lines.
   */
  class Polyline : public PolyBase {
  public:
    /**
     * @brief Polyline constructor.
     */
    Polyline(const std::string& name, const std::string& type, const Vector2u& origin, double rotation, bool visible)
      : PolyBase(POLYLINE, name, type, origin, rotation, visible)
    {
    }
  };

  /**
   * @brief A polygon is a closed set of lines.
   */
  class Polygon : public PolyBase {
  public:
    /**
     * @brief Polygon constructor.
     */
    Polygon(const std::string& name, const std::string& type, const Vector2u& origin, double rotation, bool visible)
      : PolyBase(POLYLINE, name, type, origin, rotation, visible)
    {
    }
  };

}

#endif // TMX_OBJECT_H
