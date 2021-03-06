#ifndef GIS__GC_UTILS_H_INCLUDED
#define GIS__GC_UTILS_H_INCLUDED

// Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, 51 Franklin
// Street, Suite 500, Boston, MA 02110-1335 USA.

/// @file
///
/// This file declares the interface of various utility functions for
/// geometrycollections. The functions may throw exceptions.

#include <memory>  // std::unique_ptr

#include "dd/types/spatial_reference_system.h"  // dd::Spatial_reference_system
#include "difference_functor.h"
#include "geometries.h"
#include "union_functor.h"

namespace gis {

/// Invalid geometry exception.
///
/// Thrown when it is discovered that at an input parameter contains an invalid
/// geometry.
class invalid_geometry_exception : public std::exception {};

/// Splits a geometrycollection into points, linestrings and polygons.
///
/// All output collections may contain overlapping geometries and
/// duplicates. This is not a problem for the multipoint and multilinestring
/// outputs, but the multipolygon may be geometrically invalid.
///
/// @param[in] gc Geometry collection.
/// @param[out] mpt All points in the geometrycollection.
/// @param[out] mls All linestrings in the geometrycollection.
/// @param[out] mpy All polygons in the geometrycollection.
void split_gc(const Geometrycollection *gc, std::unique_ptr<Multipoint> *mpt,
              std::unique_ptr<Multilinestring> *mls,
              std::unique_ptr<Multipolygon> *mpy);

/// Cleans up overlapping geometries so that the geometrycollection is broken
/// down into non-overlapping collections.
///
/// All input collections may contain overlapping geometries and
/// duplicates, including the multipolygon.
///
/// May throw exceptions from BG operations.
///
/// @param[in] semi_major Semi-major axis of ellipsoid.
/// @param[in] semi_minor Semi-minor axis of ellipsoid.
/// @param[inout] mpt All points in the geometrycollection.
/// @param[inout] mls All linestrings in the geometrycollection.
/// @param[inout] mpy All polygons in the geometrycollection.
void gc_union(double semi_major, double semi_minor,
              std::unique_ptr<Multipoint> *mpt,
              std::unique_ptr<Multilinestring> *mls,
              std::unique_ptr<Multipolygon> *mpy);

}  // namespace gis

#endif  // GIS__GC_UTILS_H_INCLUDED
