/* $Id: editor_main.cpp 48450 2011-02-08 20:55:18Z mordante $ */
/*
   Copyright (C) 2008 - 2011 by Tomasz Sniatowski <kailoran@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#define GETTEXT_DOMAIN "wesnoth-editor"

#include "editor_controller.hpp"

#include "../construct_dialog.hpp"
#include "../gettext.hpp"
#include "../filesystem.hpp"

#include <boost/algorithm/string/replace.hpp>

lg::log_domain log_editor("editor");

namespace editor {

EXIT_STATUS start(const config& game_conf, CVideo& video, const std::string& filename /* = "" */,
	bool take_screenshot /* = false */, const std::string& screenshot_filename /* = "map_screenshot.bmp" */)
{
	EXIT_STATUS e = EXIT_ERROR;
	try {
		hotkey::scope_changer h_(game_conf, "hotkey_editor");
		hotkey::deactivate_all_scopes();
		hotkey::set_scope_active(hotkey::SCOPE_GENERAL);
		hotkey::set_scope_active(hotkey::SCOPE_EDITOR);
		editor_controller editor(game_conf, video, NULL);
		if (!filename.empty()) {
			if (is_directory(filename)) {
				editor.set_default_dir(filename);
				editor.load_map_dialog(true);
			} else {
				editor.load_map(filename, false);
			}
		}
		if(take_screenshot) {
			editor.do_screenshot(screenshot_filename);
			e = EXIT_NORMAL;
		} else {
			e = editor.main_loop();
		}
	} catch (editor_exception& e) {
		ERR_ED << "Editor exception in editor::start: " << e.what() << "\n";
		throw;
	}
	if (editor_action::get_instance_count() != 0) {
		ERR_ED << "Possibly leaked " << editor_action::get_instance_count() << " action objects\n";
	}
	return e;
}

} //end namespace editor
