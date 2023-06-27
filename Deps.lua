VULKAN_SDK = os.getenv("VULKAN_SDK")
PYTHON_PATH = os.getenv("PYTHON_LIBS")
print(PYTHON_PATH)

IncludeDir = {}
IncludeDir["GLFW"] = "TurboGE/vendor/GLFW/include"
IncludeDir["Glad"] = "TurboGE/vendor/Glad/include"
IncludeDir["Imgui"] = "TurboGE/vendor/imgui"
IncludeDir["glm"] = "TurboGE/vendor/glm"
IncludeDir["stb"] = "TurboGE/vendor/stbImage"
IncludeDir["entt"] = "TurboGE/vendor/entt/include"
IncludeDir["yamlcpp"] = "TurboGE/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "TurboGE/vendor/ImGuizmo"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["Box2D"] = "TurboGE/vendor/box2d/include"
IncludeDir["Python"] = "%{PYTHON_PATH}/include"

LibDir = {}

LibDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibDir["Python"] = "%{PYTHON_PATH}/libs"

Lib = {}
Lib["Vulkan"] = "%{LibDir.VulkanSDK}/vulkan-1.lib"
Lib["VulkanUtils"] = "%{LibDir.VulkanSDK}/VkLayer_utils.lib"

Lib["Python_Debug"] = "%{LibDir.Python}/python311_d.lib"
Lib["Python_Release"] = "%{LibDir.Python}/python311.lib"

Lib["ShaderC_Debug"] = "%{LibDir.VulkanSDK}/shaderc_sharedd.lib"
Lib["SPIRV_Cross_Debug"] = "%{LibDir.VulkanSDK}/spirv-cross-cored.lib"
Lib["SPIRV_Cross_GLSL_Debug"] = "%{LibDir.VulkanSDK}/spirv-cross-glsld.lib"
Lib["SPIRV_Tools_Debug"] = "%{LibDir.VulkanSDK}/SPIRV-Toolsd.lib"

Lib["ShaderC_Release"] = "%{LibDir.VulkanSDK}/shaderc_shared.lib"
Lib["SPIRV_Cross_Release"] = "%{LibDir.VulkanSDK}/spirv-cross-core.lib"
Lib["SPIRV_Cross_GLSL_Release"] = "%{LibDir.VulkanSDK}/spirv-cross-glsl.lib"

Dll = {}
Dll["Python_Debug"] = "\"%{PYTHON_PATH}/python311_d.dll\""
Dll["Python_Release"] = "\"%{PYTHON_PATH}/python311.dll\""
