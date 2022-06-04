/**
 * @file AppSettings.cppm
 * @author Illia.Abernikhin (https://github.com/bit-creator)
 * @brief  the module stored info for configuring the app
 * @version 1.0.0
 * @date 2021-09-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

export module App.Settings;

export enum class BuildType {
    Release,
    Debug
};

export struct Version {
    unsigned major;
    unsigned minor;
    unsigned path;
};

export inline constexpr char                    versionStr[]      ="1.0.0\0";
export inline constexpr Version                 version           ={1, 0, 0};
  
export inline constexpr char                    name[]            ="VKEngine\0";
export inline constexpr unsigned                windowHeight      =1080;
export inline constexpr unsigned                windowWidth       =1920;
export inline constexpr BuildType               buildType         =BuildType::Debug;
export inline constexpr char                    shaderDirectory[] ="assets/shaders/";
export inline constexpr char                    binaryDirectory[] ="assets/bin/";
export inline constexpr char                    preRegistredShader[] ="simple.glsl";

// export inline constexpr char       validation[][]     ={
//                                         "VK_LAYER_MESA_device_select\0",
//                                         "VK_LAYER_MESA_overlay\0",
//                                         "VK_LAYER_LUNARG_standard_validation\0",
//                                         "VK_LAYER_KHRONOS_validation\0"
//                                     };
