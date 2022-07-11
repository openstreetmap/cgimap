#include "cgimap/api06/changeset_handler.hpp"
#include "cgimap/request_helpers.hpp"
#include "cgimap/http.hpp"
#include "cgimap/config.hpp"

#include <sstream>
#include <fmt/core.h>

using std::vector;

namespace api06 {

changeset_responder::changeset_responder(mime::type mt, osm_changeset_id_t id_,
                                         bool include_discussion_,
                                         data_selection &w_)
  : osm_current_responder(mt, w_), id(id_),
    include_discussion(include_discussion_) {

  if (sel.select_changesets({id}) == 0) {
    throw http::not_found(fmt::format("Changeset {:d} was not found.", id));
  }

  if (include_discussion) {
    sel.select_changeset_discussions();
  }
}

namespace {
// functor to use in find_if to locate the "include_discussion" header
struct discussion_header_finder {
  bool operator()(const std::pair<std::string, std::string> &header) const {
    static const std::string target("include_discussion");
    return header.first == target;
  }
};
}

changeset_handler::changeset_handler(request &req, osm_changeset_id_t id_)
  : id(id_), include_discussion(false) {
  using std::vector;
  using std::pair;
  using std::string;

  string decoded = http::urldecode(get_query_string(req));
  const vector<pair<string, string> > params = http::parse_params(decoded);
  auto itr =
    std::find_if(params.begin(), params.end(), discussion_header_finder());

  include_discussion = (itr != params.end());
}


std::string changeset_handler::log_name() const { return "changeset"; }

responder_ptr_t changeset_handler::responder(data_selection &w) const {
  return responder_ptr_t(new changeset_responder(mime_type, id, include_discussion, w));
}

} // namespace api06
