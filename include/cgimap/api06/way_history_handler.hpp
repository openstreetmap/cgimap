#ifndef API06_WAY_HISTORY_HANDLER_HPP
#define API06_WAY_HISTORY_HANDLER_HPP

#include "cgimap/handler.hpp"
#include "cgimap/osm_current_responder.hpp"
#include "cgimap/request.hpp"

namespace api06 {

class way_history_responder : public osm_current_responder {
public:
  way_history_responder(mime::type, osm_nwr_id_t, bool include_nodes, data_selection_ptr &);
  ~way_history_responder();

private:
  osm_nwr_id_t id;
};

class way_history_handler : public handler {
public:
  way_history_handler(request &, osm_nwr_id_t);
  ~way_history_handler();

  std::string log_name() const;
  responder_ptr_t responder(data_selection_ptr &) const;
  static bool validate_request(request &);

private:
  osm_nwr_id_t id;
  bool include_nodes;
};

} // namespace api06

#endif /* API06_WAY_HISTORY_HANDLER_HPP */
