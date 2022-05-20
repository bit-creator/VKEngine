/**
 * @file VkGetter.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module provides the help method for getting some data 
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
export module Vk.Getter;
import <vector>;
import <functional>;

/**
 * @brief this func wrapped Vulkan getter technique
 * 
 * @tparam VkFunc invocable object for getting needed data
 * @tparam VkType type of getting data
 * @tparam Params another needed parametrs
 * @param ret - inout colection of getting data
 * @param params anouther paramentrs
 */
export 
template <
    auto        VkFunc,
    typename    VkType,
    typename... Params
> void VkGet (std::vector<VkType>& ret, Params... params) {
    uint32_t size =0;
    std::invoke(VkFunc, std::forward<Params>(params)..., &size, nullptr);

    if(size != 0) {
        ret.resize(size);
        std::invoke(VkFunc, std::forward<Params>(params)..., &size, ret.data());
    }
}