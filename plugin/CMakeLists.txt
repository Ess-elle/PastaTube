cmake_minimum_required(VERSION 3.22)

project(AudioPlugin VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/failingplugin")

# Add the plugin target
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME esselleAudio
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE 
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE ESSA
    PLUGIN_CODE AUPL
    FORMATS VST3 Standalone
    PRODUCT_NAME "failingplugin"
)

target_sources(${PROJECT_NAME}
    PRIVATE
    source/PluginEditor.cpp
    source/PluginProcessor.cpp
    ${INCLUDE_DIR}/PluginEditor.h
    ${INCLUDE_DIR}/PluginProcessor.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Add the standalone application target
if (NOT TARGET ${PROJECT_NAME}_Standalone)
    juce_add_gui_app(${PROJECT_NAME}_Standalone
        PRODUCT_NAME "pluginname Standalone"
)

# Link standalone app to the plugin
target_link_libraries(${PROJECT_NAME}_Standalone
    PRIVATE
    ${PROJECT_NAME}
)

# Use the same sources and include directories for standalone app
target_sources(${PROJECT_NAME}_Standalone
    PRIVATE
    source/PluginEditor.cpp
    source/PluginProcessor.cpp
    ${INCLUDE_DIR}/PluginEditor.h
    ${INCLUDE_DIR}/PluginProcessor.h
)

target_include_directories(${PROJECT_NAME}_Standalone
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME}_Standalone
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_APPLICATION_NAME_STRING="failingplugin Standalone"
        JucePlugin_Name="failingplugin"
        JucePlugin_Desc="failingplugin Standalone"
        JucePlugin_Manufacturer="esselleAudio"
        JucePlugin_ManufacturerWebsite="https://www.siobhan-leese.com"
        JucePlugin_ManufacturerEmail="siobhanleese@gmail.com"
        JucePlugin_ManufacturerCode=ESSA
        JucePlugin_PluginCode=AUPL
        JucePlugin_IsSynth=0
        JucePlugin_WantsMidiInput=0
        JucePlugin_ProducesMidiOutput=0
        JucePlugin_IsMidiEffect=0
        JucePlugin_EditorRequiresKeyboardFocus=0
        JucePlugin_Version=1.0.0
        JucePlugin_VersionCode=0x10000
        JucePlugin_VersionString="1.0.0"
        JucePlugin_VSTUniqueID=JucePlugin_PluginCode
        JucePlugin_VSTCategory=kPlugCategEffect
)
endif()