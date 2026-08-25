#pragma once

#include "geometry/point2d.hpp"
#include "geometry/point_with_altitude.hpp"

#include "routing/turns.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace routing
{
// Holds one parsed maneuver. In BRouter mode 9 ("BRouter style") maneuvers
// arrive per-trackpoint as <brouter:voicehint>cmd;distanceToNext,geometry
// inside <trkpt><extensions>.
struct TurnHint
{
  int turnCode = 0;       // BRouter turn code (0 = continue; see parser constants)
  double distanceToNextM = 0.0;  // distance from this point to the next maneuver
  int offset = -1;        // trkpt offset within the track
  std::string streetName;
  std::string ref;
  std::string destination;
};

// One stretch of constant <brouter:way> content: |firstPointIdx| is the first
// trackpoint carrying |tags|; the tags stay valid up to (excluding) the next
// run's start (or the end of the track). Trackpoints before the first run
// carry no tags. Kept run-length encoded because long routes easily span
// thousands of points but far fewer distinct ways.
struct WayTagsRun
{
  size_t firstPointIdx = 0;
  std::string tags;
};

// A single parsed BRouter response: one track polyline plus its turn
// instructions, per-point elevations, way tags and speeds. BRouter returns
// exactly one <trk> per response (the requested alternative index selects
// which).
struct BrouterTrack
{
  std::vector<m2::PointD> points;
  std::vector<TurnHint> hints;
  std::vector<geometry::Altitude> altitudes;
  // Run-length encoded <brouter:way> content, ordered by firstPointIdx
  // (see WayTagsRun). Resolve individual points with WayTagsAt().
  std::vector<WayTagsRun> wayTagRuns;
  // Per-point <brouter:speed> in km/h, 0 when absent.
  std::vector<double> speedKphPerPoint;
  // Per-point <time> as UTC epoch seconds, 0 when absent.
  std::vector<double> timeEpochPerPoint;
  // Total route time in seconds from the <brouter:info> metadata, 0 when
  // absent. This is only a fallback: BRouter 1.7.x ignores a plain showspeed
  // request parameter (it is a profile expression variable), so the app
  // injects "profile:showspeed=1" to make mode 9 emit exact per-point
  // <brouter:speed> elements instead. Also note BRouter writes the same
  // <brouter:info> on every alternative, so the total is only exact for the
  // primary route.
  double totalTimeSec = 0.0;
};

// \returns the tags of the run covering |pointIdx| (see WayTagsRun), or
// nullptr when the point carries no way tags.
std::string const * WayTagsAt(std::vector<WayTagsRun> const & runs, size_t pointIdx);

// Parse the BRouter GPX payload once (single pass over the document), filling
// all BrouterTrack members. Returns an empty points vector on failure.
BrouterTrack ParseGpxResponse(std::string const & gpx);

// Map a BRouter OsmAnd turn code to a OM CarDirection. Returns None for
// "continue" and unknown codes (no arrow rendered).
turns::CarDirection BrouterTurnToCarDirection(int code, double angleDeg);

// Roundabout exit number encoded in a turn code; 0 for non-roundabout codes.
uint32_t BrouterTurnExitNumber(int code);

// Build a per-segment cumulative-time vector for the track. Exactly one
// strategy is used per track, picked by data quality: exact per-point speeds
// (<brouter:speed>, enabled via the injected profile:showspeed profile
// variable and thus present per alternative), else <time> stamp deltas, else
// the <brouter:info> total distributed proportionally to distance. Strategies
// are never mixed within a track: the shared metadata total overlaps with
// time already covered by exact per-point data, and blending the two can
// yield negative segment times. Returns an all-zero vector when the track
// carries none of them.
std::vector<double> BuildCumulativeTimes(BrouterTrack const & track);
}  // namespace routing
