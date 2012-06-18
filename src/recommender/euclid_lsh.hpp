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

#include <stdint.h>
#include <map>
#include <pficommon/data/serialization.h>
#include "recommender_base.hpp"
#include "../storage/lsh_index_storage.hpp"

namespace jubatus {
namespace recommender {

class euclid_lsh : public recommender_base {
 public:
  using recommender_base::similar_row;
  using recommender_base::neighbor_row;

  euclid_lsh();
  explicit euclid_lsh(const std::map<std::string, std::string>& config);
  ~euclid_lsh();

  virtual void neighbor_row(const sfv_t& query, std::vector<std::pair<std::string, float> >& ids, size_t ret_num) const;
  virtual void similar_row(const sfv_t& query, std::vector<std::pair<std::string, float> >& ids, size_t ret_num) const;
  virtual void clear();
  virtual void clear_row(const std::string& id);
  virtual void update_row(const std::string& id, const sfv_diff_t& diff);
  virtual void get_all_row_ids(std::vector<std::string>& ids) const;

  virtual std::string type() const;
  virtual storage::lsh_index_storage* get_storage();
  virtual const storage::lsh_index_storage* get_const_storage() const;

 private:
  friend class pfi::data::serialization::access;
  template<typename Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(lsh_index_) & MEMBER(bin_width_) & MEMBER(num_probe_);
  }

  virtual bool save_impl(std::ostream& os);
  virtual bool load_impl(std::istream& is);

  storage::lsh_index_storage lsh_index_;
  float bin_width_;
  uint32_t num_probe_;
};

}
}