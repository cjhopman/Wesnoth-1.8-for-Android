/* $Id: label.cpp 48450 2011-02-08 20:55:18Z mordante $ */
/*
   Copyright (C) 2008 - 2011 by Mark de Wever <koraq@xs4all.nl>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/label.hpp"


namespace gui2 {

void tlabel::set_state(const tstate state)
{
	if(state != state_) {
		state_ = state;
		set_dirty(true);
	}
}

const std::string& tlabel::get_control_type() const
{
	static const std::string type = "label";
	return type;
}

} // namespace gui2

