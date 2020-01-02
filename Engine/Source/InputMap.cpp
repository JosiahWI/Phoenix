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

#include <Quartz2/InputMap.hpp>

using namespace q2;

InputMap::InputMap(){}

void InputMap::registerInput(std::string id, SDL_Scancode defaultKey, std::function<void()> action){
    m_inputs[id].action = action;
    setInput(id, defaultKey);
}

void InputMap::setInput(std::string id, SDL_Scancode key){
    for(auto & [id, input]: m_inputs){
        std::vector<SDL_Scancode>::iterator it = std::find(input.keys.begin(), input.keys.end(), key);
        if(it != input.keys.end()){
            input.keys.erase(it);
        }
    }
    m_inputs[id].keys.push_back(key);
}

void InputMap::tick(){
	const Uint8* keys = SDL_GetKeyboardState(NULL);
    for(auto & [id, input]: m_inputs){
        for(SDL_Scancode key: input.keys){
            if(keys[key]){
                input.action();
            }
        }
    }
}