// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastracture and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#pragma once

#include <cmath>
#include <pficommon/data/unordered_map.h>
#include "similarity_base.hpp"

namespace jubatus {
namespace recommender {

class similarity_euclid : public similarity_base {
public:
  virtual ~similarity_euclid(){};
  virtual float operator() (const sfvi_t& fv1, const sfvi_t& fv2) const {
    size_t i1 = 0;
    size_t i2 = 0;
    float ret = 0.f;
    while (i1 < fv1.size() && i2 < fv2.size()){
      uint64_t id1 = fv1[i1].first;
      uint64_t id2 = fv2[i2].first;
      if (id1 < id2){
        ret += fv1[i1].second * fv1[i1].second;
        ++i1;
      } else if (id1 > id2){
        ret += fv2[i2].second * fv2[i2].second;
        ++i2;
      } else {
        float dif = fv1[i1].second - fv2[i2].second;
        ret += dif * dif;
        ++i1;
        ++i2;
      }
    }

    while (i1 < fv1.size()){
      ret += fv1[i1].second * fv1[i1].second;
      ++i1;
    }

    while (i2 < fv2.size()){
      ret += fv2[i2].second * fv2[i2].second;
      ++i2;
    }

    return std::exp(-1/t * ret);
  }

  virtual std::string name() const {
    return std::string("euclid");
  }

protected:
  static const float t = 1.f;
};

} // namespace recommender
} // namespace jubatus