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

#ifdef EMSCRIPTEN
#include <emscripten/fetch.h>
#include <cstring> // For strcpy
#else
#include "addon/downloader.hpp" // Only needed for non-Emscripten
#endif

#include "supertux/gameconfig.hpp"
#include "supertux/gluaconfig.hpp"
#include "supertux/globals.hpp"
#include "util/log.hpp"

#ifdef EMSCRIPTEN
// Structure to hold the original callbacks to pass through emscripten_fetch
struct FetchCallbacks
{
  std::function<void()> success;
  std::function<void()> failure;
};

// Callback for successful fetch
void downloadSucceeded(emscripten_fetch_t *fetch)
{
  log_info << "Access code validation response received (Status: "
           << fetch->status << ")" << std::endl;

  FetchCallbacks *callbacks = static_cast<FetchCallbacks *>(fetch->userData);

  if (fetch->status == 200)
  {
    std::string response(fetch->data, fetch->numBytes);
    log_info << "Access code validation response body: " << response
             << std::endl;

    // Check the response content (same logic as before)
    if (response.find("Status:valid") != std::string::npos)
    {
      log_info << "Access code validation successful" << std::endl;

      std::string validated_token;
      std::string player_name;

      // Skip the "Status:valid " prefix (13 characters)
      std::string remaining = response.substr(13);

      // Find the position of the first space after the token
      size_t space_pos = remaining.find(' ');

      if (space_pos != std::string::npos)
      {
        // Extract the token and player name
        validated_token = remaining.substr(0, space_pos);
        player_name = remaining.substr(space_pos + 1);

        // Store the validated token in the global config
        g_glua_config->user_token = validated_token;
        g_glua_config->user_name = player_name;
        g_glua_config->save();

        log_info << "Parsed token: " << validated_token << std::endl;
        log_info << "Player name: " << player_name << std::endl;

        if (callbacks->success)
          callbacks->success();
      }
      else
      {
        log_warning << "Token validation failed: Could not parse response"
                    << std::endl;
        if (callbacks->failure)
          callbacks->failure();
      }
    }
    else
    {
      log_warning << "Token validation failed: Invalid status in response"
                  << std::endl;
      if (callbacks->failure)
        callbacks->failure();
    }
  }
  else
  {
    log_warning << "Token validation failed: HTTP status " << fetch->status
                << std::endl;
    if (callbacks->failure)
      callbacks->failure();
  }

  // Clean up
  emscripten_fetch_close(fetch);
  delete callbacks; // Delete the struct holding the callbacks
}

// Callback for failed fetch
void downloadFailed(emscripten_fetch_t *fetch)
{
  log_warning << "Token validation failed: Network error (Status: "
              << fetch->status << ", StatusText: " << fetch->statusText << ")"
              << std::endl;

  FetchCallbacks *callbacks = static_cast<FetchCallbacks *>(fetch->userData);

  if (callbacks->failure)
    callbacks->failure();

  // Clean up
  emscripten_fetch_close(fetch);
  delete callbacks; // Delete the struct holding the callbacks
}
#endif // EMSCRIPTEN

void validate_token(const std::string &token,
                    const std::function<void()> &success_callback,
                    const std::function<void()> &failure_callback)
{
  if (token.empty())
  {
    log_warning << "Empty token provided for validation" << std::endl;
    if (failure_callback)
      failure_callback();
    return;
  }

  // API endpoint URL
  std::string api_url =
      g_glua_config->game_server_url + "/verify_access_code?access_code=" + token;

  log_info << "Validating access code via URL: " << api_url << std::endl;

#ifdef EMSCRIPTEN
  // --- Emscripten Implementation (Asynchronous Fetch) ---

  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

  // Create a structure on the heap to hold the callbacks
  // This will be deleted in the downloadSucceeded/downloadFailed callbacks
  FetchCallbacks *callback_data = new FetchCallbacks{success_callback, failure_callback};
  attr.userData = callback_data;

  attr.onsuccess = downloadSucceeded;
  attr.onerror = downloadFailed;
  // You might want to handle onabort and ontimeout specifically too
  // attr.onabort = downloadFailed;
  // attr.ontimeout = downloadFailed;

  log_info << "Initiating fetch request..." << std::endl;
  emscripten_fetch(&attr, api_url.c_str());

  // Note: The function returns here immediately. The result is handled
  // in the downloadSucceeded or downloadFailed callbacks later.

#else
  // --- Native Implementation (Synchronous Download) ---

  log_info << "Using native downloader..." << std::endl;
  try
  {
    // Use the downloader to make a request to the API
    Downloader downloader;
    std::string response = downloader.download(api_url);

    log_info << "Access code validation response: " << response << std::endl;

    // Check the response
    if (response.find("Status:valid") != std::string::npos)
    {
      log_info << "Access code validation successful" << std::endl;

      std::string validated_token;
      std::string player_name;

      // Skip the "Status:valid " prefix (13 characters)
      std::string remaining = response.substr(13);

      // Find the position of the first space after the token
      size_t space_pos = remaining.find(' ');

      if (space_pos != std::string::npos)
      {
        // Extract the token and player name
        validated_token = remaining.substr(0, space_pos);
        player_name = remaining.substr(space_pos + 1);

        // Store the validated token in the global config
        g_glua_config->user_token = validated_token;
        g_glua_config->user_name = player_name;
        g_glua_config->save();

        log_info << "Parsed token: " << validated_token << std::endl;
        log_info << "Player name: " << player_name << std::endl;

        if (success_callback)
          success_callback();
      }
      else
      {
        log_warning << "Token validation failed: Could not parse response"
                    << std::endl;
        if (failure_callback)
          failure_callback();
      }
    }
    else
    {
      log_warning << "Token validation failed: Invalid status in response"
                  << std::endl;
      if (failure_callback)
        failure_callback();
    }
  }
  catch (const std::exception &e)
  {
    log_warning << "Error during token validation: " << e.what() << std::endl;
    if (failure_callback)
      failure_callback();
  }
#endif // EMSCRIPTEN
}
