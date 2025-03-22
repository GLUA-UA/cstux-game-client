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

#include "supertux/menu/tournament_status_menu.hpp"

#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "supertux/game_manager.hpp"
#include "supertux/menu/menu_storage.hpp"
#include "supertux/world.hpp"
#include "util/gettext.hpp"

TournamentStatusMenu::TournamentStatusMenu()
{
  add_label(_("Tournament Status"));
  add_hl();
  add_entry(WORLDSET_STORY, _("Story Mode"));
  add_hl();
  add_back(_("Back"));
}

void TournamentStatusMenu::menu_action(MenuItem &item)
{
  switch (item.get_id())
  {
  case WORLDSET_STORY:
  {
    std::unique_ptr<World> world = World::from_directory("levels/world1");
    GameManager::current()->start_worldmap(*world);
    break;
  }
  }
}
/* EOF */
