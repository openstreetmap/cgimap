#include "cgimap/api06/node_rels_handler.hpp"
#include "cgimap/http.hpp"

#include <sstream>

using std::stringstream;
using std::vector;

namespace api06 {

node_rels_responder::node_rels_responder(mime::type mt, osm_id_t id_,
                                         factory_ptr &w_)
    : osm_current_responder(mt, w_), id(id_) {
  vector<osm_id_t> ids;
  ids.push_back(id);

  // NOTE: doesn't matter if there are no matching nodes, the API will just return
  // an empty <osm/> element. also doesn't matter if it has been deleted, only the
  // list of relation IDs matters.
  if (sel->select_nodes(ids) > 0) {
    sel->select_relations_from_nodes();
  }

  // filter out anything that's not a relation
  sel->drop_nodes();
}

node_rels_responder::~node_rels_responder() {}

node_rels_handler::node_rels_handler(request &, osm_id_t id_) : id(id_) {}

node_rels_handler::~node_rels_handler() {}

std::string node_rels_handler::log_name() const { return "node/relations"; }

responder_ptr_t node_rels_handler::responder(factory_ptr &x) const {
  return responder_ptr_t(new node_rels_responder(mime_type, id, x));
}

} // namespace api06
