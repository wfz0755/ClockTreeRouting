#pragma once

// STL libraries
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip> 
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <cmath>
#include <functional>
#include "stdafx.h"
#include "dataanalysis.h"

// Boost libraries
#include <boost/program_options.hpp>
#include <boost/icl/split_interval_map.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
