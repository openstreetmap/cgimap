#include "cgimap/api06/way_rels_handler.hpp"
#include "cgimap/http.hpp"

#include <sstream>

using std::stringstream;
using std::vector;

namespace api06 {

way_rels_responder::way_rels_responder(mime::type mt, osm_id_t id_,
                                       factory_ptr &w_)
    : osm_current_responder(mt, w_), id(id_) {
  vector<osm_id_t> ids;
  ids.push_back(id);

  if (sel->select_ways(ids) > 0) {
    sel->select_relations_from_ways();
  }
  sel->drop_ways();
}

way_rels_responder::~way_rels_responder() {}

way_rels_handler::way_rels_handler(request &, osm_id_t id_) : id(id_) {}

way_rels_handler::~way_rels_handler() {}

std::string way_rels_handler::log_name() const { return "way/relations"; }

responder_ptr_t way_rels_handler::responder(factory_ptr &x) const {
  return responder_ptr_t(new way_rels_responder(mime_type, id, x));
}

} // namespace api06
