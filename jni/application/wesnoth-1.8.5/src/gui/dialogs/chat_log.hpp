/* $Id: chat_log.hpp 49342 2011-05-02 18:52:36Z crab $ */
/*
   Copyright (C) 2011 by Yurii Chernyi <terraninfo@terraninfo.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_DIALOGS_CHAT_LOG_HPP_INCLUDED
#define GUI_DIALOGS_CHAT_LOG_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"
#include "../../variable.hpp"

#include <boost/shared_ptr.hpp>

class replay;

namespace gui2 {

class tchat_log: public tdialog {
public:
	class model;
	class view;
	class controller;
	tchat_log(const vconfig &cfg, replay *replay);

	/** Inherited from tdialog. */
	twindow* build_window(CVideo& video);

	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);

	boost::shared_ptr<view> get_view();

private:

	boost::shared_ptr<view> view_;

};

}

#endif /* ! GUI_DIALOGS_CHAT_LOG_HPP_INCLUDED */
