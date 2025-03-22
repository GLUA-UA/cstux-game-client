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

#ifndef HEADER_SUPERTUX_MENU_ACCESS_CODE_MENU_HPP
#define HEADER_SUPERTUX_MENU_ACCESS_CODE_MENU_HPP

#include "gui/menu.hpp"
#include <functional>
#include <string>

/** Menu for entering user access code/token */
class AccessCodeMenu final : public Menu
{
private:
  std::string m_access_code;
  bool m_login_success;

public:
  AccessCodeMenu();
  ~AccessCodeMenu() override;

  void menu_action(MenuItem &item) override;
  void on_window_resize() override;

private:
  void initialize();
  void validate_and_save_token();

private:
  AccessCodeMenu(const AccessCodeMenu &) = delete;
  AccessCodeMenu &operator=(const AccessCodeMenu &) = delete;
};

#endif

/* EOF */