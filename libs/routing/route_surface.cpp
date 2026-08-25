#include "routing/route_surface.hpp"

#include "base/assert.hpp"
#include "base/logging.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

namespace routing
{
namespace
{
// Extracts the value of a single "key=value" token, case-insensitive key.
// Case-insensitive comparison of two tag keys.
bool KeysEqual(std::string const & a, std::string const & b)
{
  return a.size() == b.size() &&
         std::equal(a.begin(), a.end(), b.begin(),
                    [](char x, char y) { return std::tolower(static_cast<unsigned char>(x)) ==
                                                std::tolower(static_cast<unsigned char>(y)); });
}

// Extracts the value of a single "key=value" token, case-insensitive key.
// Returns empty string when the key is absent.
std::string GetTagValue(std::string const & wayTags, std::string const & key)
{
  std::istringstream stream(wayTags);
  std::string token;
  while (stream >> token)
  {
    size_t const eq = token.find('=');
    if (eq == std::string::npos)
      continue;
    if (!KeysEqual(token.substr(0, eq), key))
      continue;
    return token.substr(eq + 1);
  }
  return {};
}

// True when the key is present, regardless of its value ("key=" counts as
// present; needed for valueless markers like "direct_segment=").
bool HasTag(std::string const & wayTags, std::string const & key)
{
  std::istringstream stream(wayTags);
  std::string token;
  while (stream >> token)
  {
    size_t const eq = token.find('=');
    if (eq == std::string::npos)
      continue;
    if (KeysEqual(token.substr(0, eq), key))
      return true;
  }
  return false;
}

// OSM tracktype grades: grade1 (solid) .. grade5 (soft). 0 when absent.
int ParseTrackGrade(std::string const & tracktype)
{
  if (tracktype.rfind("grade", 0) != 0)
    return 0;
  if (tracktype.size() != 6)  // "grade" + one digit
    return 0;
  char const c = tracktype.back();
  if (c < '1' || c > '5')
    return 0;
  return c - '0';
}
}  // namespace

RouteSurface SurfaceFromWayTags(std::string const & wayTags)
{
  if (wayTags.empty())
    return RouteSurface::Unknown;

  // Beeline legs BRouter inserts between via points carry no real way data.
  if (HasTag(wayTags, "direct_segment"))
    return RouteSurface::Unknown;

  std::string const surface = GetTagValue(wayTags, "surface");
  std::string const highway = GetTagValue(wayTags, "highway");
  std::string const tracktype = GetTagValue(wayTags, "tracktype");
  int const trackGrade = ParseTrackGrade(tracktype);

  // Singletrack / trail.
  if (HasTag(wayTags, "mtb:scale") || HasTag(wayTags, "sac_scale"))
    return RouteSurface::Singletrack;
  if (highway == "bridleway")
    return RouteSurface::Singletrack;
  if (highway == "path" && (trackGrade == 0 || trackGrade >= 3))
    return RouteSurface::Singletrack;

  // Dirt / unpaved.
  if (surface == "dirt" || surface == "earth" || surface == "ground" || surface == "mud" ||
      surface == "sand" || surface == "unpaved")
    return RouteSurface::Dirt;
  if (trackGrade >= 4)
    return RouteSurface::Dirt;

  // Gravel / compacted.
  if (surface == "gravel" || surface == "fine_gravel" || surface == "compacted" ||
      surface == "pebblestone")
    return RouteSurface::Gravel;
  if (trackGrade >= 2)
    return RouteSurface::Gravel;

  // Paved.
  if (surface == "asphalt" || surface == "paved" || surface == "concrete" ||
      surface == "concrete:lanes" || surface == "concrete:plates" || surface == "paving_stones" ||
      surface == "sett" || surface == "cobblestone")
    return RouteSurface::Paved;
  if (trackGrade == 1)
    return RouteSurface::Paved;

  return RouteSurface::Unknown;
}

std::string DebugPrint(RouteSurface surface)
{
  switch (surface)
  {
  case RouteSurface::Unknown: return "Unknown";
  case RouteSurface::Paved: return "Paved";
  case RouteSurface::Gravel: return "Gravel";
  case RouteSurface::Dirt: return "Dirt";
  case RouteSurface::Singletrack: return "Singletrack";
  case RouteSurface::Count: return "Count";
  }
  UNREACHABLE();
}
}  // namespace routing
