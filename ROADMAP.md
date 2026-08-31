# CoBike Roadmap

CoBike is offline-first bikepacking navigation built on top of CoMaps and Organic Maps. It uses [BRouter](https://github.com/abrensch/brouter) for routing.

This page tracks what we plan to do. A box with `[x]` is done. An empty box `[ ]` is still to do. Each item can be turned straight into a GitHub issue. Just copy the title and the first paragraph as the issue body.

If you want to put this in a GitHub Project, these labels work well:

- `priority:high`, `priority:medium`, `priority:low`
- `size:s`, `size:m`, `size:l`
- `phase:1`, `phase:2`, `phase:3`

## Phase 1: Quick wins

Most of the data for these features already comes back from BRouter. The goal is small changes that make CoBike feel like the gravel Komoot of the open source world.

- [ ] **Surface analysis** (`size:m` `priority:high`)
  Read the `surface` and `smoothness` tags from BRouter's GPX output. Show a short breakdown for each route, like "62% paved, 30% gravel, 8% singletrack". Also color the route line by surface type. This is the one feature that turns a normal cycling app into a real bikepacking app.

- [ ] **Alternatives with stats** (`size:s` `priority:high`)
  We already ask BRouter for alternative routes. Today there's no way to compare them. Show a short summary for each one: distance, climbing, and surface mix. Picking a route shouldn't feel like a guess.

- [ ] **GPX export with turn instructions** (`size:s` `priority:high`)
  Let users export the planned route as a GPX file. Include BRouter's turn instructions so it works on Garmin and Wahoo bike computers. Organic Maps only exports KML or KMZ today, which most bike computers don't read well.

- [ ] **Cue sheet** (`size:s` `priority:medium`)
  A printable turn-by-turn list. Every long ride should have a paper backup in case the phone dies.

## Phase 2: Bikepacking places

The OSM tags for these are already in the map data. The real work is the "search along the route" query and a few quick filters in the navigation view.

- [ ] **Water sources** (`size:m` `priority:high`)
  Show `amenity=drinking_water`, `man_made=water_tap`, and `natural=spring` as a toggle layer. Also let users search along the current route. Water is the first thing bikepackers run out of in the field.

- [ ] **Food resupply** (`size:m` `priority:high`)
  Bakeries, grocery stores, and supermarkets with opening hours. Sorted by distance from the route.

- [ ] **Shelter and bivouac** (`size:m` `priority:high`)
  `tourism=wilderness_hut`, `amenity=shelter`, and `tourism=camp_site`, including bothies. When the weather turns bad, this info matters a lot.

- [ ] **Bail-out options** (`size:s` `priority:medium`)
  Train stations close to the route. Useful for emergencies and for mixing the bike with trains.

- [ ] **Bike support** (`size:s` `priority:low`)
  Bike shops, repair stations, and public pumps along the route.

## Phase 3: Bigger bets

These take more time and carry more risk. We'll only do them once the earlier phases feel solid.

- [ ] **BRouter profile picker** (`size:l` `priority:medium`)
  Today the app is hardcoded to `v=bicycle, fast=1`. Let users pick a profile like trekking or fastbike. Also expose the main settings, like wet roads or avoiding steep climbs.

- [ ] **Follow a GPX track** (`size:l` `priority:medium`)
  Let users import a planned track and follow it without re-routing. Useful for riders who trust their plan and don't want the app to "help". BRouter can snap to the track.

- [ ] **Sunrise and sunset per stage** (`size:s` `priority:low`)
  Pure offline math. Helps users plan realistic daily distances.

- [ ] **Battery saver mode** (`size:m` `priority:medium`)
  Auto-dim or turn the screen off between turns while navigating. On multi-day trips, battery life is a safety thing, not just a nice-to-have.

## Not on the plan right now

- Weather or wind forecasts. They need the network, and that breaks the offline-first rule.
- Community routes or heatmaps. They need a backend.
- Multi-device sync and shared collections. They need user accounts.
