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

#include "util/api_fetcher.hpp"

#include <string>
#include <stdexcept>
#include <iostream>


#ifdef EMSCRIPTEN
#include <emscripten/fetch.h>
#include <cstring> // For strcpy
#else
#include "addon/downloader.hpp" // Only needed for non-Emscripten
#endif

#include "util/log.hpp"

#ifdef EMSCRIPTEN
// Structure to hold the original callbacks to pass through emscripten_fetch
struct FetchCallbacks
{
  std::function<void(const std::string&)> success;
  std::function<void(const std::string&)> failure;
};

// Callback for successful fetch
void fetchSucceeded(emscripten_fetch_t *fetch)
{

  FetchCallbacks *callbacks = static_cast<FetchCallbacks *>(fetch->userData);

  if (fetch->status >= 200 && fetch->status < 300)
  {
    std::string response(fetch->data, fetch->numBytes);

    if (callbacks->success)
    {
      callbacks->success(response);
    }
  }
  else
  {
    std::string error_msg = "HTTP error: " + std::to_string(fetch->status);
    
    if (callbacks->failure)
    {
      callbacks->failure(error_msg);
    }
  }

  // Clean up
  emscripten_fetch_close(fetch);
  delete callbacks; // Delete the struct holding the callbacks
}

// Callback for failed fetch
void fetchFailed(emscripten_fetch_t *fetch)
{
  std::string error_msg = "Network error (Status: " + std::to_string(fetch->status);
  if (fetch->statusText && fetch->statusText[0] != '\0')
  {
    error_msg += ", StatusText: " + std::string(fetch->statusText);
  }
  error_msg += ")";
  
  FetchCallbacks *callbacks = static_cast<FetchCallbacks *>(fetch->userData);

  if (callbacks->failure)
  {
    callbacks->failure(error_msg);
  }

  // Clean up
  emscripten_fetch_close(fetch);
  delete callbacks; // Delete the struct holding the callbacks
}
#endif // EMSCRIPTEN

void fetch_api(const std::string& url,
               const std::function<void(const std::string&)>& success_callback,
               const std::function<void(const std::string&)>& failure_callback)
{
  if (url.empty())
  {
    if (failure_callback)
    {
      failure_callback("Empty URL provided");
    }
    return;
  }

#ifdef EMSCRIPTEN
  // --- Emscripten Implementation (Asynchronous Fetch) ---

  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

  // Create a structure on the heap to hold the callbacks
  // This will be deleted in the fetchSucceeded/fetchFailed callbacks
  FetchCallbacks *callback_data = new FetchCallbacks{success_callback, failure_callback};
  attr.userData = callback_data;

  attr.onsuccess = fetchSucceeded;
  attr.onerror = fetchFailed;

  // Set timeout to 15 seconds
  attr.timeoutMSecs = 15000;

  emscripten_fetch(&attr, url.c_str());

  // Note: The function returns here immediately. The result is handled
  // in the fetchSucceeded or fetchFailed callbacks later.

#else
  // --- Native Implementation (Synchronous Download) ---

  try
  {
    // Use the downloader to make a request to the API
    Downloader downloader;
    std::string response = downloader.download(url);

    if (success_callback)
    {
      success_callback(response);
    }
  }
  catch (const std::exception& e)
  {
    std::string error_msg = "Error during API fetch: ";
    error_msg += e.what();
    
    log_warning << error_msg << std::endl;
    
    if (failure_callback)
    {
      failure_callback(error_msg);
    }
  }
#endif // EMSCRIPTEN
}

bool fetch_api_sync(const std::string& url, std::string& response)
{
#ifdef EMSCRIPTEN
  // Emscripten doesn't support synchronous network requests in the main thread
  std::cerr << "[API FETCHER] Synchronous API requests are not supported in Emscripten" << std::endl;
  throw std::runtime_error("Synchronous API requests are not supported in Emscripten");
#else
  // Native Implementation (Synchronous Download)
  try
  {
    // Use the downloader to make a request to the API
    Downloader downloader;
    response = downloader.download(url);

    return true;
  }
  catch (const std::exception& e)
  {
    std::cerr << "[API FETCHER] Error during sync API fetch: " << e.what() << std::endl;
    return false;
  }
#endif
}