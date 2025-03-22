//  SuperTux - GLUA Game Client
//  Copyright (C) 2025 Miguel Vila <miguelovila@ua.pt>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "supertux/menu/access_code_menu.hpp"

#include "audio/sound_manager.hpp"
#include "gui/dialog.hpp"
#include "gui/item_textfield.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/fadetoblack.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/gluaconfig.hpp"
#include "supertux/globals.hpp"
#include "supertux/menu/menu_storage.hpp"
#include "supertux/screen_manager.hpp"
#include "util/log.hpp"
#include "video/video_system.hpp"
#include "video/viewport.hpp"
#include "util/token_validator.hpp"

enum AccessCodeMenuIDs
{
  MNID_ACCESSCODE,
  MNID_LOGIN,
  MNID_CANCEL
};

AccessCodeMenu::AccessCodeMenu() :
  m_access_code(""),
  m_login_success(false)
{
  initialize();
}

AccessCodeMenu::~AccessCodeMenu()
{
  // Nothing to do here
}

void AccessCodeMenu::initialize()
{
  add_label(_("Player Login"));
  add_hl();

  add_textfield(_("Access Code: "), &m_access_code)
      .set_help(_("Enter your player access code here"));

  add_hl();
  add_entry(MNID_LOGIN, _("Login"));
  add_entry(MNID_CANCEL, _("Cancel"));

  on_window_resize();
}

void AccessCodeMenu::validate_and_save_token()
{
  if (m_access_code.empty())
  {
    Dialog::show_message(_("Please enter a valid access code"));
    return;
  }

  validate_token(m_access_code, [this]()
                 {
      // Token is valid, no action needed
      Dialog::show_message(_("Welcome, ") + g_glua_config->user_name + "!", false, false,
        []() {
          // Return to main menu
          MenuManager::instance().clear_menu_stack();
          MenuManager::instance().push_menu(MenuStorage::MAIN_MENU);
        }
      ); }, []()
                 {
      // Token is no longer valid, prompt for a new one
      Dialog::show_message(_("Invalid access code. Please try again.")); });
}

void AccessCodeMenu::on_window_resize()
{
  set_center_pos(static_cast<float>(SCREEN_WIDTH) / 2.0f, static_cast<float>(SCREEN_HEIGHT) / 2.0f);
}

void AccessCodeMenu::menu_action(MenuItem &item)
{
  switch (item.get_id())
  {
  case MNID_LOGIN:
    validate_and_save_token();
    break;

  case MNID_CANCEL:
    MenuManager::instance().clear_menu_stack();
    MenuManager::instance().push_menu(MenuStorage::MAIN_MENU);
    break;
  }
}

/* EOF */