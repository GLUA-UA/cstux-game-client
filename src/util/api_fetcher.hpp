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

#ifndef HEADER_SUPERTUX_UTIL_API_FETCHER_HPP
#define HEADER_SUPERTUX_UTIL_API_FETCHER_HPP

#include <functional>
#include <string>

/**
 * Sends a GET request to the specified URL and processes the response.
 * 
 * Works consistently in both native and Emscripten environments.
 * 
 * @param url The full URL including any query parameters
 * @param success_callback Function called on successful response with response text as parameter
 * @param failure_callback Function called on failed request with error message as parameter
 */
void fetch_api(const std::string& url,
               const std::function<void(const std::string&)>& success_callback,
               const std::function<void(const std::string&)>& failure_callback);

/**
 * Synchronous version of fetch_api that only works in native environments.
 * In Emscripten, this will throw an exception as synchronous requests are not supported.
 * 
 * @param url The full URL including any query parameters
 * @param response A reference to a string where the response text will be stored
 * @return true if the request was successful, false otherwise
 */
bool fetch_api_sync(const std::string& url, std::string& response);

#endif