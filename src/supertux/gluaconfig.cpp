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

#include "supertux/gluaconfig.hpp"

#include "util/reader_collection.hpp"
#include "util/reader_document.hpp"
#include "util/reader_mapping.hpp"
#include "util/writer.hpp"
#include "util/log.hpp"

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

GluaConfig::GluaConfig() : game_server_url("http://localhost:3000"),
                           user_token(""),
                           user_name("")
{
}

void GluaConfig::load()
{
#ifdef __EMSCRIPTEN__
  EM_ASM({ supertux_loadFiles(); }, 0); // EM_ASM is a variadic macro and Clang requires at least 1 value for the variadic argument.
#endif

  auto doc = ReaderDocument::from_file("glua-config");
  auto root = doc.get_root();
  if (root.get_name() != "supertux-glua-config")
  {
    throw std::runtime_error("File is not a supertux-glua-config file");
  }

  auto config_mapping = root.get_mapping();
  config_mapping.get("game_server_url", game_server_url);
  config_mapping.get("user_token", user_token);
  config_mapping.get("user_name", user_name);
}

void GluaConfig::save()
{
  Writer writer("glua-config");

  writer.start_list("supertux-glua-config");

  writer.write("game_server_url", game_server_url, false);
  writer.write("user_token", user_token, false);
  writer.write("user_name", user_name, false);

  writer.end_list("supertux-glua-config");
}

/* EOF */
