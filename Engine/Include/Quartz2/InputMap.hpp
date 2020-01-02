// Copyright 2019 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <Quartz2/Singleton.hpp>

#include <SDL.h>

#include <map>
#include <string>
#include <functional>
#include <vector>

namespace q2
{
    enum EKeyState
	{
		KEY_PRESSED,
		KEY_HELD,
		KEY_RELEASED
	};

    struct Input{
        std::vector<SDL_Scancode> keys;
        std::function<void()> action;
    };
    
    class InputMap : public Singleton<InputMap>{
        std::map<std::string, Input> m_inputs;

        public:

        InputMap();

        /**
         * @brief Registers a new input
         * 
         * @param id The unique ID for the input in the format core:moveForward
         * @param defaultKey The default key this is set to, will overwrite other keys already loaded
         * @param action The function that is executed when this input is triggered
         */
        void registerInput(std::string id, SDL_Scancode defaultKey, std::function<void()> action);
        /**
         * @brief Adds a key to an input, if key is associated to another input, will delete it.
         * 
         * @param id The unique ID for the input in the format core:moveForward
         * @param key The key this is set to, will overwrite if key is set elsewhere
         */
        void setInput(std::string id, SDL_Scancode key);

        /**
         * @brief Check for input, should be ran every frame
         * 
         */
        void tick();
    };
}