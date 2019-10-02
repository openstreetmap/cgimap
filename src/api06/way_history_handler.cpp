#include "cgimap/api06/way_history_handler.hpp"
#include "cgimap/http.hpp"
#include "cgimap/request_helpers.hpp"

#include <sstream>

using std::stringstream;
using std::vector;

namespace {
bool has_nodes_param(const std::pair<std::string, std::string> &p) {
  return p.first == "nodes";
}
}

namespace api06 {

way_history_responder::way_history_responder(mime::type mt, osm_nwr_id_t id_, bool include_nodes, data_selection_ptr &w_)
  : osm_current_responder(mt, w_), id(id_) {

  if (sel->select_ways_with_history({id}) == 0) {
    throw http::not_found("");
  }
  if (include_nodes)
    sel->select_historical_nodes_for_ways({id});
}

way_history_responder::~way_history_responder() = default;

way_history_handler::way_history_handler(request &req, osm_nwr_id_t id_) : id(id_), include_nodes(validate_request(req)) {}

way_history_handler::~way_history_handler() = default;

std::string way_history_handler::log_name() const { return "way/history"; }

responder_ptr_t way_history_handler::responder(data_selection_ptr &w) const {
  return responder_ptr_t(new way_history_responder(mime_type, id, include_nodes, w));
}

bool way_history_handler::validate_request(request &req) {
  std::string decoded = http::urldecode(get_query_string(req));
  const auto params = http::parse_params(decoded);
  auto itr =
    std::find_if(params.begin(), params.end(), has_nodes_param);

  return (itr != params.end());
}


} // namespace api06
