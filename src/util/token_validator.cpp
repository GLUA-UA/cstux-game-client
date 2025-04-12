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

#include "util/token_validator.hpp"

#include <functional>
#include <string>

#include "util/api_fetcher.hpp"
#include "supertux/gameconfig.hpp"
#include "supertux/gluaconfig.hpp"
#include "supertux/globals.hpp"
#include "util/log.hpp"
#include <iostream>

void validate_token(const std::string &token,
                    const std::function<void()> &success_callback,
                    const std::function<void()> &failure_callback)
{
  if (token.empty())
  {
    std::cerr << "[TOKEN VALIDATOR] No token provided for validation" << std::endl;
    if (failure_callback)
      failure_callback();
    return;
  }

  // API endpoint URL
  std::string api_url = g_glua_config->game_server_url + "/api/v1/auth/login?accesscode=" + token;

  std::cout << "[TOKEN VALIDATOR] Validating token: " << api_url << std::endl;

  fetch_api(
    api_url,
    // Success callback - process the response
    [token, success_callback, failure_callback](const std::string& response) {
      // Check the response content
      if (response.find("status=valid") != std::string::npos)
      {
        std::string validated_token;
        std::string player_name;

        // Skip the "Status:valid " prefix (13 characters)
        std::string remaining = response.substr(13);

        // Find the position of the first space after the token
        size_t space_pos = remaining.find(' ');
        if (space_pos != std::string::npos)
        {
          // Extract the token and player name
          validated_token = remaining.substr(6, space_pos - 6);
          player_name = remaining.substr(space_pos + 6);

          // Store the validated token in the global config
          g_glua_config->user_token = validated_token;
          g_glua_config->user_name = player_name;
          g_glua_config->save();

          std::cout << "[TOKEN VALIDATOR] User \"" << player_name << "\" logged in with token: " << validated_token << std::endl;

          if (success_callback)
            success_callback();
        }
        else
        {
          std::cerr << "[TOKEN VALIDATOR] validation failed: Could not parse response" << std::endl;
          if (failure_callback)
            failure_callback();
        }
      }
      else
      {
        std::cerr << "[TOKEN VALIDATOR] validation failed: Invalid status in response" << std::endl;
        if (failure_callback) failure_callback();
      }
    },
    // Failure callback - just pass through to the original failure callback
    [failure_callback](const std::string& error) {
      std::cerr << "[TOKEN VALIDATOR] validation failed: " << error << std::endl;
      if (failure_callback)
        failure_callback();
    }
  );
}