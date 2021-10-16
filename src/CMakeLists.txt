set(TARGET_VKLOUDSHELLEXT_NAME VkloudShellExt)

find_package(RapidJSON REQUIRED)

add_library(${TARGET_VKLOUDSHELLEXT_NAME}
    SHARED
        DllMain.cpp
        ExportFunctions.def
        ExtensionFactory.h
        ExtensionFactory.cpp
        ContextMenuExtension.h
        ContextMenuExtension.cpp
        Utils/RegisterHelper.h
        Utils/RegisterHelper.cpp
        Utils/Handle.h
        Utils/ComClass.h
        Control/NamedPipeClient.h
        Control/NamedPipeClient.cpp)

target_link_libraries(${TARGET_VKLOUDSHELLEXT_NAME}
    RapidJSON::RapidJSON
    Userenv)

target_compile_features(${TARGET_VKLOUDSHELLEXT_NAME}
    PRIVATE
        cxx_std_20)

add_compile_definitions(UNICODE)
