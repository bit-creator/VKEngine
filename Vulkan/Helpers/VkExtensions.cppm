/**
 * @file VkExtensions.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the help class for manage Vulkan extensions
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
export module Vk.Extensions;

import Vk;

import <cstring>;
import <vector>;
import <type_traits>;

import Vk.Getter;

/**
 * @class Extensions
 * @brief provides API to checking suppurted Vulkan extensions
 * 
 * @tparam VkObj object whose extensions are checkied
 */
export template < typename VkObj >
class Extensions {
private:
    const VkObj& _obj;

public:
    /**
     * @brief capture object ref
     * @warning needed only for PhysicalDevice obj
     */
    Extensions(const VkObj& obj): _obj(obj) {  }

    /**
     * @brief store all supported extensions to extensions
     * @param extensions inout collection for extensions 
     */
    void getAllSupported(std::vector<VkExtensionProperties>& extensions) {
        if constexpr (std::is_same_v<VkObj, VkInstance>) {
            VkGet<vkEnumerateInstanceExtensionProperties>(extensions, nullptr);
        } else if constexpr (std::is_same_v<VkObj, VkPhysicalDevice>) {
            VkGet<vkEnumerateDeviceExtensionProperties>(extensions, _obj, nullptr);
        }
    }

    /**
     * @brief check supporting for many extensions
     * @param required colection with required extensions
     * @return true if all required extensions supported, false if no
     */
    bool isSupported(const std::vector<const char*>& required) {
        uint32_t supported =0;

        for(const auto req: required) {
            supported += isSupported(req);
        }

        return supported == required.size();
    }

    /**
     * @brief check supporting for extension
     * @param req required extension
     * @return true if extension supp orted, false if no
     */
    bool isSupported(const char* req) {
        bool result = true;

        std::vector<VkExtensionProperties> extensions;
        getAllSupported(extensions);

        bool hasExtension = false;
        for(const auto& vkExt: extensions) {
            hasExtension += !strcmp(vkExt.extensionName, req);
        }
        if(!hasExtension) {
            //Log something like "req unsupported"
            result = false;
        }
        return result;
    }
}; // Extensions