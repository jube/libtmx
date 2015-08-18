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
#include <tmx/Map.h>

#include <cassert>
#include <cstring>
#include <cstdlib>

#include <string>
#include <iostream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/split.hpp>

#include <tinyxml2.h>
#include <zlib.h>

#include <tmx/Image.h>
#include <tmx/ImageLayer.h>
#include <tmx/Layer.h>
#include <tmx/Object.h>
#include <tmx/ObjectLayer.h>
#include <tmx/Terrain.h>
#include <tmx/Tile.h>
#include <tmx/TileLayer.h>
#include <tmx/TileSet.h>

#define INVALID static_cast<unsigned>(-1)

namespace fs = boost::filesystem;

namespace tmx {

  template<typename T, typename... Args>
  static inline std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  static const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
  static const unsigned FLIPPED_VERTICALLY_FLAG   = 0x40000000;
  static const unsigned FLIPPED_DIAGONALLY_FLAG   = 0x20000000;

  std::tuple<bool, bool, bool, unsigned> decodeGID(unsigned gid) {
    // Read out the flags
    bool hflip = (gid & FLIPPED_HORIZONTALLY_FLAG);
    bool vflip = (gid & FLIPPED_VERTICALLY_FLAG);
    bool dflip = (gid & FLIPPED_DIAGONALLY_FLAG);

    // Clear the flags
    gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

    return std::make_tuple(hflip, vflip, dflip, gid);
  }

  namespace {

    enum class Requirement {
      OPTIONAL,
      MANDATORY,
    };

    class XMLElementWrapper {
    public:
      XMLElementWrapper(const tinyxml2::XMLElement *elt)
        : m_elt(elt)
      {
      }

      bool is(const char *name) const {
        return !std::strcmp(m_elt->Name(), name);
      }

      bool hasChild(const char *name) const {
        return m_elt->FirstChildElement(name) != nullptr;
      }

      template<typename Func>
      void parseEachElement(Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement();

        while (child != nullptr) {
          func(child);
          child = child->NextSiblingElement();
        }
      }

      template<typename Func>
      void parseManyElements(const char *name, Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement(name);

        while (child != nullptr) {
          func(child);
          child = child->NextSiblingElement(name);
        }
      }

      template<typename Func>
      void parseOneElement(const char *name, Func func) const {
        const tinyxml2::XMLElement *child = m_elt->FirstChildElement(name);

        if (child == nullptr) {
          return;
        }

        func(child);
        child = child->NextSiblingElement(name);

        if (child != nullptr) {
          std::clog << "Multiple chidren where a single child was expected for element: " << name << '\n';
        }
      }

      bool hasAttribute(const char *name) const {
        return m_elt->Attribute(name) != nullptr;
      }

      unsigned getUIntAttribute(const char *name, Requirement req = Requirement::MANDATORY, unsigned val = 0) const {
        int err = m_elt->QueryUnsignedAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      int getIntAttribute(const char *name, Requirement req = Requirement::MANDATORY, int val = 0) const {
        int err = m_elt->QueryIntAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      double getDoubleAttribute(const char *name, Requirement req = Requirement::MANDATORY, double val = 0.) const {
        int err = m_elt->QueryDoubleAttribute(name, &val);
        return handleErrorAndReturn(name, val, err, req);
      }

      bool getBoolAttribute(const char *name, Requirement req = Requirement::MANDATORY, bool val = false) const {
        unsigned zeroOrOne = val ? 1 : 0;
        int err = m_elt->QueryUnsignedAttribute(name, &zeroOrOne);
        zeroOrOne = handleErrorAndReturn(name, zeroOrOne, err, req);
        assert(zeroOrOne == 0 || zeroOrOne == 1);
        return zeroOrOne == 1;
      }

      std::string getStringAttribute(const char *name, Requirement req = Requirement::MANDATORY, const char *val = "") const {
        const char *attr = m_elt->Attribute(name);
        int err = tinyxml2::XML_NO_ERROR;

        if (!attr) {
          attr = val;
          err = tinyxml2::XML_NO_ATTRIBUTE;
        }

        assert(attr);
        return handleErrorAndReturn(name, std::string(attr), err, req);
      }

      bool isEnumAttribute(const char *name, const char *value) const {
        return m_elt->Attribute(name, value) != nullptr;
      }

      std::string getText() const {
        const char *text = m_elt->GetText();
        return text ? text : "";
      }

    private:
      const tinyxml2::XMLElement * const m_elt;

      template<typename T>
      T handleErrorAndReturn(const char *name, T val, int err, Requirement req) const {
        if (err == tinyxml2::XML_NO_ERROR) {
          return val;
        }

        if (err == tinyxml2::XML_NO_ATTRIBUTE) {
          if (req == Requirement::MANDATORY) {
            std::clog << "Error! Mandatory attribute is missing: " << name << '\n';
          }
          return val;
        }

        std::clog << "Error! Unknown error with attribute: " << name << '\n';
        return val;
      }
    };

    enum class Format {
      XML,
      BASE64,
      BASE64_ZLIB,
      BASE64_GZIP,
      CSV,
    };

    class Parser {
    public:

      // http://en.wikibooks.org/wiki/Algorithm_implementation/Miscellaneous/Base64
      std::vector<uint8_t> parseDataBase64(const std::string& input) {
        std::string cleanInput = boost::algorithm::erase_all_copy(input, "\n");
        cleanInput = boost::algorithm::erase_all_copy(cleanInput, " ");

        std::size_t len = cleanInput.size();
        assert(len % 4 == 0);

        size_t padding = 0;
        if (len >= 2) {
          if (input[len - 1] == '=') {
            padding++;
          }

          if (input[len - 2] == '=') {
            padding++;
          }
        }

        // Setup a vector to hold the result
        std::vector<uint8_t> decoded;
        decoded.reserve(((len / 4) * 3) - padding);

        for (std::size_t i = 0; i < len; ) {

          uint32_t tmp = 0; // Holds decoded quanta
          for (int k = 0; k < 4; ++k) {
            tmp <<= 6;

            char c = cleanInput.at(i);

            if (c >= 'A' && c <= 'Z') {
              tmp |= c - 'A';
            } else if (c >= 'a' && c <= 'z') {
              tmp |= c - 'a' + 26;
            } else if (c >= '0' && c <= '9') {
              tmp |= c - '0' + 52;
            } else if (c == '+') {
              tmp |= 0x3E;
            } else if (c == '/') {
              tmp |= 0x3F;
            } else if (c == '=') { //pad
              switch(len - i) {
              case 1: // One pad character
                decoded.push_back((tmp >> 16) & 0x000000FF);
                decoded.push_back((tmp >> 8 ) & 0x000000FF);
                return decoded;
              case 2: // Two pad characters
                decoded.push_back((tmp >> 10) & 0x000000FF);
                return decoded;
              default:
                assert(false);
              }
            } else {
              std::clog << "Unknown character: '" << c << "' (" << static_cast<int>(c) << ")\n";
              assert(false);
            }

            ++i;
          }

          decoded.push_back((tmp >> 16) & 0x000000FF);
          decoded.push_back((tmp >> 8 ) & 0x000000FF);
          decoded.push_back((tmp      ) & 0x000000FF);
        }

        return decoded;
      }

      #define TMPLEN 1024

      std::vector<uint8_t> parseDataCompressed(const std::vector<uint8_t>& input) {
        std::vector<uint8_t> uncompressed;

        uint8_t tmp[TMPLEN];

        z_stream stream;
        std::memset(&stream, 0, sizeof(stream));

        stream.next_in = input.data();
        stream.avail_in = input.size();
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;

        inflateInit2(&stream, 15 + 32); // allow to decode gzip and zlib format

        int n = 0;
        for (;;) {
          n++;
          stream.next_out = tmp;
          stream.avail_out = TMPLEN;

          int err = inflate(&stream, Z_SYNC_FLUSH);

          if (err == Z_OK) {
            uncompressed.insert(uncompressed.end(), tmp, tmp + TMPLEN - stream.avail_out);
          } else if (err == Z_STREAM_END) {
            uncompressed.insert(uncompressed.end(), tmp, tmp + TMPLEN - stream.avail_out);
            break;
          } else {
            assert(false);
          }
        }

        assert(stream.avail_in == 0);

        inflateEnd(&stream);

        return uncompressed;
      }


      /*
       * Fragment parsers
       */


      Format parseDataFormat(const XMLElementWrapper elt) {
        assert(elt.is("data"));

        if (elt.isEnumAttribute("encoding", "csv")) {
          return Format::CSV;
        }

        if (elt.isEnumAttribute("encoding", "base64")) {

          if (elt.isEnumAttribute("compression", "zlib")) {
            return Format::BASE64_ZLIB;
          }

          if (elt.isEnumAttribute("compression", "gzip")) {
            return Format::BASE64_GZIP;
          }

          return Format::BASE64;
        }

        return Format::XML;
      }

      std::vector<uint8_t> parseDataBuffer(const XMLElementWrapper elt, Format format) {
        assert(elt.is("data"));

        std::vector<uint8_t> data;

        switch (format) {
          case Format::XML:
          case Format::CSV:
            assert(false);
            break;

          case Format::BASE64:
            data = parseDataBase64(elt.getText());
            break;

          case Format::BASE64_ZLIB:
          case Format::BASE64_GZIP:
            data = parseDataCompressed(parseDataBase64(elt.getText()));
            break;
        }

        data.shrink_to_fit();

        return data;
      }

      void parseComponent(const XMLElementWrapper elt, Component *component) {
        elt.parseOneElement("properties", [component](const XMLElementWrapper elt) {
          elt.parseManyElements("property", [component](const XMLElementWrapper elt) {
            std::string name = elt.getStringAttribute("name");
            assert(!name.empty());
            std::string value = elt.getStringAttribute("value");

            component->addProperty(name, value);
          });
        });
      }

      std::unique_ptr<Image> parseImage(const XMLElementWrapper elt) {
        assert(elt.is("image"));

        std::string format = elt.getStringAttribute("format", Requirement::OPTIONAL);
        std::string source = elt.getStringAttribute("source");
        std::string trans = elt.getStringAttribute("trans", Requirement::OPTIONAL);
        unsigned width = elt.getUIntAttribute("width", Requirement::OPTIONAL);
        unsigned height = elt.getUIntAttribute("height", Requirement::OPTIONAL);

        elt.parseOneElement("data", [](const XMLElementWrapper elt) {
          assert(false && "Not implemented"); // TODO
        });

        return makeUnique<Image>(format, currentPath / source, trans, width, height);
      }

      std::unique_ptr<ImageLayer> parseImageLayer(const XMLElementWrapper elt) {
        assert(elt.is("imagelayer"));

        std::string name = elt.getStringAttribute("name");
        double opacity = elt.getDoubleAttribute("opacity", Requirement::OPTIONAL, 1.0);
        bool visible = elt.getBoolAttribute("visible", Requirement::OPTIONAL, true);

        auto imageLayerPtr = makeUnique<ImageLayer>(name, opacity, visible);
        auto imageLayer = imageLayerPtr.get();

        parseComponent(elt, imageLayer);

        elt.parseOneElement("image", [imageLayer,this](const XMLElementWrapper elt) {
          imageLayer->setImage(parseImage(elt));
        });

        return imageLayerPtr;
      }

      std::vector<Vector2i> parsePoints(const std::string& points) {

        std::vector<Vector2i> ret;

        std::vector<std::string> items;
        boost::algorithm::split(items, points, boost::algorithm::is_any_of(" "));

        for (auto item : items) {
          std::vector<std::string> coords;
          boost::algorithm::split(coords, item, boost::algorithm::is_any_of(","));

          assert(coords.size() == 2);
#if __cplusplus >= 201103L
          int x = std::stoi(coords[0]);
          int y = std::stoi(coords[1]);
#else
          int x = std::strtol(coords[0].c_str(), nullptr, 0);
          int y = std::strtol(coords[1].c_str(), nullptr, 0);
#endif

          ret.push_back({ x, y });
        }

        return ret;
      }

      std::unique_ptr<Object> parseObject(const XMLElementWrapper elt) {
        assert(elt.is("object"));

        unsigned id = elt.getUIntAttribute("id", Requirement::OPTIONAL);
        std::string name = elt.getStringAttribute("name", Requirement::OPTIONAL);
        std::string type = elt.getStringAttribute("type", Requirement::OPTIONAL);
        unsigned x = elt.getUIntAttribute("x");
        unsigned y = elt.getUIntAttribute("y");
        double rotation = elt.getDoubleAttribute("rotation", Requirement::OPTIONAL);
        bool visible = elt.getBoolAttribute("visible", Requirement::OPTIONAL, true);

        Vector2u origin{x, y};

        if (elt.hasChild("polygon")) {
          auto objectPtr = makeUnique<Polygon>(id, name, type, origin, rotation, visible);
          auto object = objectPtr.get();

          parseComponent(elt, object);

          elt.parseOneElement("polygon", [object,this](const XMLElementWrapper elt) {
            std::string points = elt.getStringAttribute("points");
            object->setPoints(std::move(parsePoints(points)));
          });

          return std::move(objectPtr);
        }

        if (elt.hasChild("polyline")) {
          auto objectPtr = makeUnique<Polyline>(id, name, type, origin, rotation, visible);
          auto object = objectPtr.get();

          parseComponent(elt, object);

          elt.parseOneElement("polyline", [object,this](const XMLElementWrapper elt) {
            std::string points = elt.getStringAttribute("points");
            object->setPoints(std::move(parsePoints(points)));
          });

          return std::move(objectPtr);
        }

        if (elt.hasAttribute("gid")) {
          unsigned gid = elt.getUIntAttribute("gid");

          bool hflip, vflip, dflip;
          std::tie(hflip, vflip, dflip, gid) = decodeGID(gid);

          auto objectPtr = makeUnique<TileObject>(id, name, type, origin, rotation, visible, gid, hflip, vflip, dflip);
          auto object = objectPtr.get();

          parseComponent(elt, object);

          return std::move(objectPtr);
        }

        unsigned width = elt.getUIntAttribute("width", Requirement::OPTIONAL);
        unsigned height = elt.getUIntAttribute("height", Requirement::OPTIONAL);

        if (elt.hasChild("ellipse")) {
          auto objectPtr = makeUnique<Ellipse>(id, name, type, origin, rotation, visible, width, height);
          auto object = objectPtr.get();

          parseComponent(elt, object);

          return std::move(objectPtr);
        }

        auto objectPtr = makeUnique<Rectangle>(id, name, type, origin, rotation, visible, width, height);
        auto object = objectPtr.get();

        parseComponent(elt, object);

        return std::move(objectPtr);
      }

      std::unique_ptr<ObjectLayer> parseObjectGroup(const XMLElementWrapper elt) {
        assert(elt.is("objectgroup"));

        std::string name = elt.getStringAttribute("name");
        double opacity = elt.getDoubleAttribute("opacity", Requirement::OPTIONAL, 1.0);
        bool visible = elt.getBoolAttribute("visible", Requirement::OPTIONAL, true);

        std::string color = elt.getStringAttribute("color", Requirement::OPTIONAL);

        DrawOrder order = DrawOrder::TOP_DOWN;

        if (elt.hasAttribute("draworder")) {
          if (elt.isEnumAttribute("draworder", "topdown")) {
            order = DrawOrder::TOP_DOWN;
          } else if (elt.isEnumAttribute("draworder", "index")) {
            order = DrawOrder::INDEX;
          } else {
            std::clog << "Error! Wrong draw order string: '" << elt.getStringAttribute("draworder") << "'\n";
          }
        }

        auto objectLayerPtr = makeUnique<ObjectLayer>(name, opacity, visible, color, order);
        auto objectLayer = objectLayerPtr.get();

        parseComponent(elt, objectLayer);

        elt.parseManyElements("object", [objectLayer,this](const XMLElementWrapper elt) {
          objectLayer->addObject(parseObject(elt));
        });

        return objectLayerPtr;
      }

      std::unique_ptr<TileLayer> parseLayer(const XMLElementWrapper elt) {
        assert(elt.is("layer"));

        std::string name = elt.getStringAttribute("name");
        double opacity = elt.getDoubleAttribute("opacity", Requirement::OPTIONAL, 1.0);
        bool visible = elt.getBoolAttribute("visible", Requirement::OPTIONAL, true);

        auto tileLayerPtr = makeUnique<TileLayer>(name, opacity, visible);
        auto tileLayer = tileLayerPtr.get();

        parseComponent(elt, tileLayer);

        elt.parseOneElement("data", [tileLayer,this](const XMLElementWrapper elt) {
          Format format = parseDataFormat(elt);

          switch (format) {
            case Format::BASE64:
            case Format::BASE64_ZLIB:
            case Format::BASE64_GZIP:
              {
                std::vector<uint8_t> data = parseDataBuffer(elt, format);

                const std::size_t sz = data.size();
                assert(sz % 4 == 0);
                for (std::size_t i = 0; i < sz; i += 4) {
                  unsigned gid = data[i] | (data[i + 1] << 8) | (data[i + 2] << 16) | (data[i + 3] << 24);

                  bool hflip, vflip, dflip;
                  std::tie(hflip, vflip, dflip, gid) = decodeGID(gid);

                  tileLayer->addCell({ gid, hflip, vflip, dflip });
                }
              }
              break;
            case Format::CSV:
              {
                const std::string csv = elt.getText();
                std::vector<std::string> items;
                boost::algorithm::split(items, csv, boost::algorithm::is_any_of(","));

                for (auto item : items) {
#if __cplusplus >= 201103L
                  unsigned gid = std::stoul(item);
#else
                  unsigned gid = std::strtoul(item.c_str(), nullptr, 0);
#endif
                  tileLayer->addCell({ gid });
                }
              }
              break;
            case Format::XML:
              elt.parseManyElements("tile", [tileLayer](const XMLElementWrapper elt) {
                unsigned gid = elt.getUIntAttribute("gid");
                tileLayer->addCell({ gid });
              });
              break;
          }
        });

        return tileLayerPtr;
      }

      std::unique_ptr<Tile> parseTile(const XMLElementWrapper elt) {
        assert(elt.is("tile"));

        unsigned id = elt.getUIntAttribute("id");

        std::array<unsigned, 4> terrain = { { INVALID, INVALID, INVALID, INVALID } };
        std::string attr = elt.getStringAttribute("terrain", Requirement::OPTIONAL);

        if (!attr.empty()) {
          std::vector<std::string> items;
          boost::algorithm::split(items, attr, boost::algorithm::is_any_of(","));

          unsigned t = 0;
          for (auto item : items) {
            if (!item.empty()) {
#if __cplusplus >= 201103L
              terrain[t++] = std::stoul(item);
#else
              terrain[t++] = std::strtoul(item.c_str(), nullptr, 0);
#endif
            }
          }
        }

        unsigned probability = elt.getUIntAttribute("probability", Requirement::OPTIONAL, 100);

        auto tilePtr = makeUnique<Tile>(id, terrain, probability);
        auto tile = tilePtr.get();

        parseComponent(elt, tile);

        elt.parseOneElement("image", [tile,this](const XMLElementWrapper elt) {
          tile->setImage(parseImage(elt));
        });

        return tilePtr;
      }

      std::unique_ptr<Terrain> parseTerrain(const XMLElementWrapper elt) {
        assert(elt.is("terrain"));

        std::string name = elt.getStringAttribute("name");
        unsigned tile = elt.getUIntAttribute("tile");

        auto terrainPtr = makeUnique<Terrain>(name, tile);
        auto terrain = terrainPtr.get();

        parseComponent(elt, terrain);

        return terrainPtr;
      }

      std::unique_ptr<TileSet> parseTileSetFromElement(unsigned firstgid, const XMLElementWrapper elt) {
        assert(elt.is("tileset"));

        std::string name = elt.getStringAttribute("name", Requirement::OPTIONAL);
        unsigned tilewidth = elt.getUIntAttribute("tilewidth", Requirement::OPTIONAL);
        unsigned tileheight = elt.getUIntAttribute("tileheight", Requirement::OPTIONAL);
        unsigned spacing = elt.getUIntAttribute("spacing", Requirement::OPTIONAL);
        unsigned margin = elt.getUIntAttribute("margin", Requirement::OPTIONAL);
        unsigned tilecount = elt.getUIntAttribute("tilecount", Requirement::OPTIONAL);

        auto tilesetPtr = makeUnique<TileSet>(firstgid, name, tilewidth, tileheight, spacing, margin, tilecount);
        auto tileset = tilesetPtr.get();

        parseComponent(elt, tileset);

        elt.parseOneElement("tileoffset", [tileset](const XMLElementWrapper elt) {
          int x = elt.getIntAttribute("x");
          int y = elt.getIntAttribute("y");
          tileset->setOffset(x, y);
        });

        elt.parseOneElement("image", [tileset,this](const XMLElementWrapper elt) {
          tileset->setImage(parseImage(elt));
        });

        elt.parseOneElement("terraintypes", [tileset,this](const XMLElementWrapper elt) {
          elt.parseManyElements("terrain", [tileset,this](const XMLElementWrapper elt) {
            tileset->addTerrain(parseTerrain(elt));
          });
        });

        elt.parseManyElements("tile", [tileset,this](const XMLElementWrapper elt) {
          tileset->addTile(parseTile(elt));
        });

        return tilesetPtr;
      }

      std::unique_ptr<TileSet> parseTileSetFromFile(unsigned firstgid, const std::string& filename) {
        fs::path tilesetPath = currentPath / filename;

        tinyxml2::XMLDocument doc;
        int err = doc.LoadFile(tilesetPath.string().c_str());

        if (doc.Error()) {
          std::clog << "Error! Unable to load a TSX file: " << tilesetPath << '\n';
          assert(err != tinyxml2::XML_NO_ERROR);
          return nullptr;
        }

        assert(err == tinyxml2::XML_NO_ERROR);

        currentPath = tilesetPath.parent_path();

        const tinyxml2::XMLElement *elt = doc.RootElement();

        if (elt->Attribute("firstgid")) {
          std::clog << "Warning! Attribute 'firstgid' present in a TSX file: " << tilesetPath << '\n';
        }

        if (elt->Attribute("source")) {
          std::clog << "Warning! Attribute 'source' present in a TSX file: " << tilesetPath << '\n';
        }

        auto tileset = parseTileSetFromElement(firstgid, elt);

        currentPath = mapPath.parent_path();

        return tileset;
      }

      std::unique_ptr<TileSet> parseTileSet(const XMLElementWrapper elt) {
        assert(elt.is("tileset"));

        unsigned firstgid = elt.getUIntAttribute("firstgid");
        std::string source = elt.getStringAttribute("source", Requirement::OPTIONAL);

        if (!source.empty()) {
          return parseTileSetFromFile(firstgid, source);
        }

        return parseTileSetFromElement(firstgid, elt);
      }

      std::unique_ptr<Map> parseMap(const XMLElementWrapper elt) {
        assert(elt.is("map"));

        std::string version =  elt.getStringAttribute("version", Requirement::OPTIONAL, "1.0");

        Orientation orientation = Orientation::UNKNOWN;

        if (elt.isEnumAttribute("orientation", "orthogonal")) {
          orientation = Orientation::ORTHOGONAL;
        } else if (elt.isEnumAttribute("orientation", "isometric")) {
          orientation = Orientation::ISOMETRIC;
        } else if (elt.isEnumAttribute("orientation", "staggered")) {
          orientation = Orientation::STAGGERED;
        } else if (elt.isEnumAttribute("orientation", "hexagonal")) {
          orientation = Orientation::HEXAGONAL;
        } else {
          std::clog << "Error! Wrong orientation string: '" << elt.getStringAttribute("orientation") << "'\n";
        }

        unsigned width = elt.getUIntAttribute("width");
        unsigned height = elt.getUIntAttribute("height");
        unsigned tilewidth = elt.getUIntAttribute("tilewidth");
        unsigned tileheight = elt.getUIntAttribute("tileheight");
        std::string bgcolor = elt.getStringAttribute("backgroundcolor", Requirement::OPTIONAL, "#FFFFFF");

        RenderOrder renderOrder = RenderOrder::RIGHT_DOWN; // default value

        if (elt.hasAttribute("renderorder")) {
          if (elt.isEnumAttribute("renderorder", "right-down")) {
            renderOrder = RenderOrder::RIGHT_DOWN;
          } else if (elt.isEnumAttribute("renderorder", "right-up")) {
            renderOrder = RenderOrder::RIGHT_UP;
          } else if (elt.isEnumAttribute("renderorder", "left-down")) {
            renderOrder = RenderOrder::LEFT_DOWN;
          } else if (elt.isEnumAttribute("renderorder", "left-up")) {
            renderOrder = RenderOrder::LEFT_UP;
          } else {
            std::clog << "Error! Wrong render order string: '" << elt.getStringAttribute("renderorder") << "'\n";
          }
        }

        unsigned hexSideLength = elt.getUIntAttribute("hexsidelength", Requirement::OPTIONAL);

        StaggerAxis axis = StaggerAxis::Y;

        if (elt.hasAttribute("staggeraxis")) {
          if (elt.isEnumAttribute("staggeraxis", "x")) {
            axis = StaggerAxis::X;
          } else if (elt.isEnumAttribute("staggeraxis", "y")) {
            axis = StaggerAxis::Y;
          } else {
            std::clog << "Error! Wrong stagger axis string: '" << elt.getStringAttribute("staggeraxis") << "'\n";
          }
        }


        StaggerIndex index = StaggerIndex::ODD;

        if (elt.hasAttribute("staggerindex")) {
          if (elt.isEnumAttribute("staggerindex", "odd")) {
            index = StaggerIndex::ODD;
          } else if (elt.isEnumAttribute("staggerindex", "even")) {
            index = StaggerIndex::EVEN;
          } else {
            std::clog << "Error! Wrong stagger index string: '" << elt.getStringAttribute("staggerindex") << "'\n";
          }
        }

        unsigned nextObjectId = elt.getUIntAttribute("nextobjectid", Requirement::OPTIONAL);

        auto mapPtr = makeUnique<Map>(version, orientation, width, height, tilewidth, tileheight, bgcolor, renderOrder,
            hexSideLength, axis, index, nextObjectId);
        auto map = mapPtr.get();
        parseComponent(elt, map);

        elt.parseManyElements("tileset", [map,this](const XMLElementWrapper elt) {
          map->addTileSet(parseTileSet(elt));
        });

        elt.parseEachElement([map,this](const XMLElementWrapper elt) {
          if (elt.is("layer")) {
            map->addLayer(parseLayer(elt));
          } else if (elt.is("objectgroup")) {
            map->addLayer(parseObjectGroup(elt));
          } else if (elt.is("imagelayer")) {
            map->addLayer(parseImageLayer(elt));
          }
        });

        return mapPtr;
      }

      Parser(const boost::filesystem::path& filename) : mapPath(filename) { }

      std::unique_ptr<Map> parse() {
        if (!fs::is_regular_file(mapPath)) {
          std::clog << "Error! Unknown TMX file: " << mapPath << '\n';
          return nullptr;
        }

        tinyxml2::XMLDocument doc;
        int err = doc.LoadFile(mapPath.string().c_str());

        if (doc.Error()) {
          assert(err != tinyxml2::XML_NO_ERROR);
          std::clog << "Error! Unable to load the TMX file: " << mapPath << '\n';
          return nullptr;
        }

        currentPath = mapPath.parent_path();

        assert(err == tinyxml2::XML_NO_ERROR);
        return parseMap(doc.RootElement());
      }

      fs::path mapPath;
      fs::path currentPath;
    };

  }

  std::unique_ptr<Map> Map::parseFile(const boost::filesystem::path& filename) {
    Parser parser(filename);
    return parser.parse();
  }

}
