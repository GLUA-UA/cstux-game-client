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

#ifndef HEADER_SUPERTUX_UTIL_TOKEN_VALIDATOR_HPP
#define HEADER_SUPERTUX_UTIL_TOKEN_VALIDATOR_HPP

#include <string>
#include <functional>

/**
 * Validates a user token by making an API request to the token verification server.
 *
 * @param token The token to validate
 * @param success_callback Function to call when validation succeeds
 * @param failure_callback Function to call when validation fails
 */
void validate_token(const std::string &token,
                    const std::function<void()> &success_callback,
                    const std::function<void()> &failure_callback);

#endif