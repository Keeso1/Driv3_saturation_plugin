cmake_minimum_required(VERSION 3.22)

# Version is needed by JUCE.
project(AudioPlugin VERSION 0.1.0)

# Adding a directory with the library/application name as a subfolder of the
# include folder is a good practice. It helps avoid name clashes later on.
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/driv3")

# Adds a plugin target (that's basically what the Projucer does).
if(WIN32)
    set(VST3_COPY_ARGS VST3_COPY_DIR "$ENV{COMMONPROGRAMFILES}/VST3")
else()
    set(VST3_COPY_ARGS "")
endif()

juce_add_plugin(
  ${PROJECT_NAME}
  COMPANY_NAME
  keesoCorp # change this
  IS_SYNTH
  FALSE # may change this
  NEEDS_MIDI_INPUT
  FALSE # may change this
  NEEDS_MIDI_OUTPUT
  FALSE # may change this
  PLUGIN_MANUFACTURER_CODE
  Kscp # change this (for max compatibility, first letter must be uppercase, remaining lowercase)
  PLUGIN_CODE
  Drst # change this (for max compatibility, first letter must be uppercase, remaining lowercase)
  FORMATS
  Standalone
  VST3 # may change this
  PRODUCT_NAME
  driv3 # change this
  COPY_PLUGIN_AFTER_BUILD TRUE
  ${VST3_COPY_ARGS}
)

# Adds assets as BinaryData

juce_add_binary_data(BinaryData 
  SOURCES
    resources/background_blur_filter.svg
    resources/foreground_ui.svg
    resources/gain_slider_thumb.svg
    resources/gain_slider.svg
    resources/mix_slider_thumb.svg
    resources/mix_slider_filllayer.svg
    resources/mix_slider_backgroundlayer.svg
    resources/drive_slider_layer1.svg
    resources/drive_slider_layer2.svg
    resources/bypass_off_hover.svg
    resources/bypass_off.svg
    resources/bypass_on_hover.svg
    resources/bypass_on.svg
    resources/softclipOff.svg
    resources/hardclipOff.svg
    resources/fuzzclipOff.svg
    resources/softclipOn.svg
    resources/hardclipOn.svg
    resources/fuzzclipOn.svg
)

# Sets the source files of the plugin project.
set(SOURCE_FILES source/PluginEditor.cpp source/PluginProcessor.cpp source/GainSliderLookAndFeel.cpp source/MixSliderLookAndFeel.cpp source/CustomLookAndFeel.cpp source/AnalyserComponent.cpp)
# Optional; includes header files in the project file tree in Visual Studio
set(HEADER_FILES ${INCLUDE_DIR}/PluginEditor.h ${INCLUDE_DIR}/PluginProcessor.h ${INCLUDE_DIR}/GainSliderLookAndFeel.h ${INCLUDE_DIR}/HorizontalMeter.h ${INCLUDE_DIR}/MixSliderLookAndFeel.h ${INCLUDE_DIR}/CustomLookAndFeel.h ${INCLUDE_DIR}/AnalyserComponent.h)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Links to all necessary dependencies. The present ones are recommended by JUCE.
# If you use one of the additional modules, like the DSP module, you need to specify it here.
target_link_libraries_system(${PROJECT_NAME} PUBLIC juce::juce_audio_utils juce::juce_dsp)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE BinaryData PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags
                         juce::juce_recommended_warning_flags
)

# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

# Enables strict C++ warnings and treats warnings as errors.
# This needs to be set up only for your projects, not 3rd party
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_OPTIONS "${PROJECT_WARNINGS_CXX}")


# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)
