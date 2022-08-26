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

export import <string>;

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
    std::string err= "failed to create Vulkan object with code: ";
    switch(res) {
        case VK_SUCCESS:                     return;
        case VK_ERROR_OUT_OF_HOST_MEMORY:    { err += "VK_ERROR_OUT_OF_HOST_MEMORY"; break; }
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:  { err += "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break; }
        case VK_ERROR_INITIALIZATION_FAILED: { err += "VK_ERROR_INITIALIZATION_FAILED"; break; }
        case VK_ERROR_EXTENSION_NOT_PRESENT: { err += "VK_ERROR_EXTENSION_NOT_PRESENT"; break; }
        case VK_ERROR_FEATURE_NOT_PRESENT:   { err += "VK_ERROR_FEATURE_NOT_PRESENT"; break; }
        case VK_ERROR_TOO_MANY_OBJECTS:      { err += "VK_ERROR_TOO_MANY_OBJECTS"; break; }
        case VK_ERROR_DEVICE_LOST:           { err += "VK_ERROR_DEVICE_LOST"; break; }
        default:                             { err += "UNKNOWN_ERROR"; break; }
    };
    throw std::runtime_error(err);
}