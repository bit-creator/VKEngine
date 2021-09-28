/**
 * Now compiler have some trouble with exporting this code;
 * use it like a header
 */

// module;
// #include <vector>
// export module Getter;

// export 
/**
 * @brief this func wrapped Vulkan getter technique
 * 
 * @tparam VkFunc invocable object for getting needed data
 * @tparam VkType type of getting data
 * @tparam Params another needed parametrs
 * @param ret - inout colection of getting data
 * @param params anouther paramentrs
 */
template <
    auto        VkFunc,
    typename    VkType,
    typename... Params
> void VkGet (std::vector<VkType>& ret, Params... params) {
    unsigned size;
    // std::invoke(VkFunc, std::forward<Params>(params)..., &size, nullptr);
    VkFunc(std::forward<Params>(params)..., &size, nullptr);
    ret.resize(size);
    // std::invoke(VkFunc, std::forward<Params>(params)..., &size, ret.data());
    VkFunc(std::forward<Params>(params)..., &size, ret.data());
}