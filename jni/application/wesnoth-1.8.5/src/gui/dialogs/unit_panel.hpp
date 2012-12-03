#include <map>
#include "config.hpp"
class unit;
class team;
class unit_type;

namespace gui2 {
	class tgrid;
    typedef std::map<std::string, unsigned> ellipsize_map;

	std::map<std::string, string_map> get_unit_info(
            const unit& u, const ellipsize_map& ellipsize_to = ellipsize_map());
	std::map<std::string, string_map> get_unit_info(
            const unit_type* type, const team* team, const ellipsize_map& ellipsize_to = ellipsize_map());

	void markup_unit_moves(std::map<std::string, string_map>& data, const unit& u);

	void set_unit_info(tgrid* g, const string_map& info, const ellipsize_map& ellipsize_to = ellipsize_map());

	void set_unit_info(tgrid* g, const unit& u, const ellipsize_map& ellipsize_to = ellipsize_map());
	void set_unit_info(tgrid* g, const unit_type* type, const team* team, const ellipsize_map& ellipsize_to = ellipsize_map());
}
