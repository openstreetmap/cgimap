#include "cgimap/api06/relation_rels_handler.hpp"
#include "cgimap/http.hpp"

#include <sstream>

using std::stringstream;
using std::vector;

namespace api06 {

relation_rels_responder::relation_rels_responder(mime::type mt, osm_id_t id_,
                                                 factory_ptr &w_)
    : osm_current_responder(mt, w_), id(id_) {
  vector<osm_id_t> ids;
  ids.push_back(id);

  if (sel->select_relations(ids) == 0) {
    throw http::not_found("");
  } else {
    sel->select_relations_from_relations();
    check_visibility();
  }
}

relation_rels_responder::~relation_rels_responder() {}

relation_rels_handler::relation_rels_handler(request &, osm_id_t id_) : id(id_) {}

relation_rels_handler::~relation_rels_handler() {}

std::string relation_rels_handler::log_name() const { return "relation/relations"; }

responder_ptr_t relation_rels_handler::responder(factory_ptr &x) const {
  return responder_ptr_t(new relation_rels_responder(mime_type, id, x));
}

void relation_rels_responder::check_visibility() {
  if (sel->check_relation_visibility(id) == data_selection::deleted) {
    // TODO: fix error message / throw structure to emit better error message
    throw http::gone();
  }
}

} // namespace api06
