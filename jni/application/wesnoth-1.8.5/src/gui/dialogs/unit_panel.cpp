#include "gui/dialogs/unit_panel.hpp"
#include "gui/dialogs/field.hpp"
#include "gui/dialogs/helper.hpp"

#include "gui/widgets/grid.hpp"
#include "gui/widgets/control.hpp"
#include "gui/widgets/widget.hpp"
#include "gui/widgets/image.hpp"
#include "gui/widgets/label.hpp"
#include "gui/widgets/listbox.hpp"

#include "gui/auxiliary/log.hpp"

#include "unit.hpp"
#include "team.hpp"

#include "marked-up_text.hpp"

#include <sstream>
#include <string>
#include <memory>
#include <map>

namespace gui2 {

namespace {
    void set_use_markup(
            tgrid* grid 
            , const std::string& id
            , const bool use_markup
            )
    {
        tcontrol* widget = find_widget<tcontrol>(grid, id, false, false);
        if(widget) {
            widget->set_use_markup(use_markup);
        }
    }

    void set_label(
            tgrid* grid 
            , const std::string& id
            , const std::string& label
            )
    {
        tcontrol* widget = find_widget<tcontrol>(grid, id, false, false);
        if(widget) {
            widget->set_label(label);
        } else {
            timage* image= find_widget<timage>(grid, id, false, false);
            if (image) {
                image->set_label(label);
            }
        }
    }

    std::string colorize(const std::string& str, const std::string& color)
    {
        return "<span foreground=\"#" + color +"\">" + str + "</span>";
    }

    void set_label(
            std::map<std::string, string_map>& info,
            const std::string& key,
            const std::string& val,
            const std::map<std::string, unsigned>& ellipsize_to) {
        unsigned length = -1;
        std::map<std::string, unsigned>::const_iterator it = ellipsize_to.find(key);
        if (it != ellipsize_to.end()) length = it->second;
        info[key]["label"] = font::pango_escape(ellipsize(val, length));
    }
    void set_color(
            std::map<std::string, string_map>& info,
            const std::string& key,
            SDL_Color color) {
        info[key]["label"] = colorize(info[key]["label"], font::color2hexa(color));
        info[key]["use_markup"] = "true";
    }
}

void markup_unit_moves(std::map<std::string, string_map>& data, const unit& u) {
	SDL_Color move_color = {255, 255, 0, 0};

	if(u.movement_left() == 0) {
		move_color.g = 0;
	} else if(u.movement_left() == u.total_movement() ) {
		move_color.r = 0;
	}

	std::stringstream ss;
	ss << "<span foreground=\"#" << font::color2hexa(move_color) << "\">";
	ss << u.movement_left() << "/" << u.total_movement();
	ss << "</span>";

	data["moves"]["label"] = ss.str();
	data["moves"]["use_markup"] = "true";
}

std::map<std::string, string_map> get_unit_info(const unit_type* type, const std::map<std::string, unsigned>& ellipsize_to) {
	std::map<std::string, string_map> info;

    set_label(info, "type", type->type_name(), ellipsize_to);
    set_label(info, "alignment", unit_type::alignment_id(type->alignment()), ellipsize_to);

	std::stringstream ss;
	ss.str("");
	int pos = 0;
	foreach(const std::string& str, type->abilities())
		ss << (pos++ ? ", " : "") << str;
    set_label(info, "abilities", ss.str(), ellipsize_to);

	ss.str("");
	ss << "Moves: " << type->movement() << "/" << type->movement();
    set_label(info, "moves", ss.str(), ellipsize_to);

    SDL_Color hp_color = {33, 225, 0, 0};
	ss.str("");
	ss << type->hitpoints() << "/" << type->hitpoints();
    set_label(info, "health_hp", ss.str(), ellipsize_to);
    set_color(info, "health_hp", hp_color);

    SDL_Color xp_color = {0, 160, 255, 0};
	ss.str("");
	ss << "0/" << type->experience_needed();
    set_label(info, "health_xp", ss.str(), ellipsize_to);
    set_color(info, "health_xp", xp_color);

	ss.str("");
	ss << type->cost() << " Gold";
    set_label(info, "cost", ss.str(), ellipsize_to);

	int r = 0;
	foreach (const attack_type& a, type->attacks()) {
		std::stringstream id;

		ss.str("");
		ss << a.damage() << "-" << a.num_attacks() << " " << a.name();
		id << "weapon_" << r << "_stat";
        set_label(info, id.str(), ss.str(), ellipsize_to);

		id.str("");
		ss.str("");
		ss << " " << a.range() << " - " << a.type();
		id << "weapon_" << r << "_type";
        set_label(info, id.str(), ss.str(), ellipsize_to);

		id.str("");
		ss.str("");
		id << "weapon_" << r << "_special";
		if (!a.weapon_specials(true).empty()) {
			ss << " " << a.weapon_specials(true);
		}

        set_label(info, id.str(), ss.str(), ellipsize_to);

		++r;
	}
	return info;
}

std::map<std::string, string_map> get_unit_info(const unit& u, const std::map<std::string, unsigned>& ellipsize_to) {
	std::map<std::string, string_map> info = get_unit_info(u.type(), ellipsize_to);
	std::stringstream ss;

	ss << u.absolute_image() << "~RC(" << u.team_color() << ">" << team::get_side_colour_index(u.side()) << ")";
    set_label(info, "portrait", ss.str(), ellipsize_to);

	ss.str("");
	ss << u.level();
    set_label(info, "level", ss.str(), ellipsize_to);

    set_label(info, "name", u.name(), ellipsize_to);
    set_label(info, "traits", u.traits_description(), ellipsize_to);

	ss.str("");
	ss << "Moves: " << u.movement_left() << "/" << u.total_movement();
    set_label(info, "moves", ss.str(), ellipsize_to);

	ss.str("");
	ss << u.hitpoints() << "/" << u.max_hitpoints();
    set_label(info, "health_hp", ss.str(), ellipsize_to);
    set_color(info, "health_hp", u.hp_color());

	ss.str("");
	ss << u.experience() << "/" << u.max_experience();
    set_label(info, "health_xp", ss.str(), ellipsize_to);
    set_color(info, "health_xp", u.xp_color());

	return info;
}

std::map<std::string, string_map> get_unit_info(const unit_type* type, const team* team, const std::map<std::string, unsigned>& ellipsize_to) {
	std::map<std::string, string_map> info = get_unit_info(type, ellipsize_to);

	std::stringstream ss;
	ss << type->image() << "~RC(" << type->flag_rgb() << ">" << team::get_side_colour_index(team->side()) << ")";
    set_label(info, "portrait", ss.str(), ellipsize_to);
	return info;
}

void set_unit_info(tgrid* g, const std::map<std::string, string_map>& data) {
	typedef std::map<std::string, string_map> info_map;
	foreach(const info_map::value_type& val, data) {
		if (val.second.count("label")) {
			set_label(g, val.first, val.second.find("label")->second);
		}
		if (val.second.count("use_markup")) {
			set_use_markup(g, val.first, val.second.find("use_markup")->second == "true");
		}
	}
}

void set_unit_info(tgrid* g, const unit_type* type, const team* team, const ellipsize_map& ellipsize_to) {
	std::map<std::string, string_map> info = get_unit_info(type, team, ellipsize_to);
	set_unit_info(g, info);
}

void set_unit_info(tgrid* g, const unit& u, const ellipsize_map& ellipsize_to) {
	std::map<std::string, string_map> info = get_unit_info(u, ellipsize_to);
	set_unit_info(g, info);
}
}
