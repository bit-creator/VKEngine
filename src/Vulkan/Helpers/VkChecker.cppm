/**
 * @file VkChecker.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the help method for checking Vulkan object create status
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
export module Vk.Checker;

import Vulkan;

import <memory>;

/**
 * @brief this function wraps various ways to create an object
 *  
 * @tparam VkFunc invocable object for getting needed data
 * @tparam Params another needed parametrs
 * 
 * @param params anouther paramentrs
 */
export template <
    auto        VkFunc,
    typename... Params
> void VkCreate (Params... params) {
    auto res = VkFunc(std::forward<Params>(params)...);
    switch(res) {
        case VK_SUCCESS: return;
        // case ...
        // default: throw ...;
    };
}