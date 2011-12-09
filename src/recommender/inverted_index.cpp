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

#include <algorithm>
#include "inverted_index.hpp"
#include "../storage/norm.hpp"
#include "../common/exception.hpp"
#include "../common/vector_util.hpp"

using namespace std;
using namespace pfi::data;
using namespace pfi::lang;

namespace jubatus {
namespace recommender {

inverted_index::inverted_index() { 
}

inverted_index::~inverted_index() {
}

void inverted_index::similar_row(const sfv_t& query, std::vector<std::pair<std::string, float> > & ids, size_t ret_num) const{
  ids.clear();
  pfi::data::unordered_map<string, float> scores;
  float query_sq_norm = 0.f;
  for (size_t i = 0; i < query.size(); ++i){
    float val = query[i].second;
    query_sq_norm += val * val;
  }
  float query_norm = sqrt(query_sq_norm);
  if (query_norm == 0.f) return;

  for (size_t i = 0; i < query.size(); ++i){
    const string& fid = query[i].first;
    float val = query[i].second;
    sfv_t column;
    inv_.get_row(fid, column);
    for (size_t j = 0; j < column.size(); ++j){
      scores[column[j].first] += column[j].second * val;
    }
  }
  
  vector<pair<float, string> > sorted_scores;
  for (pfi::data::unordered_map<string, float>::const_iterator it = scores.begin(); it != scores.end(); ++it){
    float norm = orig_.calc_norm(it->first);
    float normed_score = (norm != 0.f) ? it->second / norm / query_norm : 0.f;
    sorted_scores.push_back(make_pair(normed_score, it->first));
  }
  sort(sorted_scores.rbegin(), sorted_scores.rend());
  for (size_t i = 0; i < sorted_scores.size() && i < ret_num; ++i){
    ids.push_back(make_pair(sorted_scores[i].second, sorted_scores[i].first));
  }
}

void inverted_index::clear(){
  orig_.clear();
  inv_.clear();
}

void inverted_index::clear_row(const std::string& id){
  vector<pair<string, float> > columns;
  orig_.get_row(id, columns);
  for (size_t i = 0; i < columns.size(); ++i){
    inv_.remove(columns[i].first, id);
  }
  orig_.remove_row(id);
}

void inverted_index::update_row(const std::string& id, const sfv_diff_t& diff){
  orig_.set_row(id, diff);
  for (size_t i = 0; i < diff.size(); ++i){
    inv_.set(diff[i].first, id, diff[i].second);
  }
}

string inverted_index::name() const {
  return string("inverted_index");
}

} // namespace recommender
} // namespace jubatus
