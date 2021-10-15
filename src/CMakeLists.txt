set(TARGET_VKLOUDSHELLEXT_NAME VkloudShellExt)

add_library(${TARGET_VKLOUDSHELLEXT_NAME}
    SHARED
        DllMain.cpp
        ExportFunctions.def
        ExtensionFactory.h
        Utils/ComClass.h
        ExtensionFactory.cpp)

target_compile_features(${TARGET_VKLOUDSHELLEXT_NAME}
    PRIVATE
        cxx_std_20)
