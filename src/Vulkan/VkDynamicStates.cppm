/**
 * @file VkColorBlender.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the abstraction on dynamic states
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module Vk.DynamicState;

import Vulkan;

import <vector>;
import <concepts>;

export enum class Dynamic {
    Viewport            =VK_DYNAMIC_STATE_VIEWPORT,
    LineWidth           =VK_DYNAMIC_STATE_LINE_WIDTH
    // etc...
};

template<typename T, typename ... U>
concept IsAllOf = (std::same_as<T, U> && ...);

export class DynamicStates {
private:
    std::vector<Dynamic>             _states;

public:
    template < typename... Dyn >
    DynamicStates(Dyn... states) 
        requires IsAllOf<Dynamic, Dyn...> 
            || IsAllOf<VkDynamicState, Dyn...>
    : _states{std::forward<Dyn>(states)...,} {
    }

    DynamicStates(std::initializer_list<Dynamic> states) 
        : _states(states) {
    }

    ~DynamicStates();

    VkPipelineDynamicStateCreateInfo
    getState();
};


// DynamicStates::DynamicStates() {
// }

DynamicStates::~DynamicStates() {
}

VkPipelineDynamicStateCreateInfo
DynamicStates::getState() {
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = _states.size();
    dynamicState.pDynamicStates = (VkDynamicState *)_states.data();

    return dynamicState;
}