// author: LostPeter
// time:   2022-12-03

#ifndef _PRE_INCLUDE_THIRD_H_
#define _PRE_INCLUDE_THIRD_H_


#include "PreMacro.h"


//GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/ext/scalar_common.hpp"

//Vulkan
#include <vulkan/vulkan.h>

//stb_image
#include <stb_image.h>

//imgui
#include <imgui/imgui.h>
#include <impl_glfw/imgui_impl_glfw.h>
#include <impl_vulkan/imgui_impl_vulkan.h>

// spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


#endif