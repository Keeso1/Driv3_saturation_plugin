![Banner Image](banner.png)

<!-- Recommended banner dimensions: 1280x640 pixels. Please replace 'path/to/your/banner.png' with the actual path to your banner image. -->

# Driv3 saturation plugin

## Overview

`Driv3 saturation plugin` is a modern C++17 audio plugin developed using the powerful JUCE framework. It is designed to function as both a VST3 plugin for seamless integration into Digital Audio Workstations (DAWs) and a standalone application for independent use. This project serves as a robust foundation for developing high-performance audio processing tools with a flexible and intuitive user interface.

## Features

- **High-Quality Audio Processing:** Leverages the JUCE framework for efficient and reliable audio signal manipulation.
- **VST3 Plugin Support:** Easily integrates into any VST3-compatible DAW.
- **Standalone Application:** Run the plugin independently without a DAW for quick testing or specific use cases.
- **Cross-Platform Compatibility:** Built with JUCE, ensuring potential compatibility across Windows, macOS, and Linux.
- **Customizable UI:** Features a responsive and customizable graphical user interface.

## Getting Started

To build and run `Driv3 saturation plugin`, you will need the following:

### Prerequisites

- **JUCE Framework:** Download and install the latest version of the JUCE framework.
- **C++ Compiler:** A C++17 compatible compiler (e.g., MSVC for Windows, Clang for macOS/Linux).
- **IDE:**
  - **Windows:** Visual Studio 2022 (or newer).
  - **macOS:** Xcode.
  - **Linux:** A suitable C++ IDE (e.g., CLion, VS Code with C++ extensions).

### Building the Project

1.  **Open with Projucer:** Navigate to the project root and open the `Driv3_saturation_plugin.jucer` file with the JUCE Projucer application.
2.  **Configure & Save:** In Projucer, ensure your desired exporters (e.g., Visual Studio, Xcode) are enabled and configured correctly. Save the project, which will generate the necessary project files for your chosen IDE.
3.  **Build in IDE:** Open the generated project file (e.g., `.sln` for Visual Studio, `.xcodeproj` for Xcode) in your IDE. Build the project.

## Usage

### As a VST3 Plugin

1.  The generated `.vst3` file should automatically be copied to your DAW's VST3 plugin folder (e.g., `C:\Program Files\Common Files\VST3` on Windows, `/Library/Audio/Plug-Ins/VST3` on macOS).
2.  Rescan your plugin folders within your DAW.
3.  Insert `Driv3 saturation plugin` as an effect or instrument on an audio track.

### As a Standalone Application

Simply run the executable generated during the build process.

## Development

The core logic of the plugin resides in `Source/PluginProcessor.cpp`, handling all audio processing. The user interface is defined in `Source/PluginEditor.cpp`.

## TODOs

- Fix slider bounds so that the sliderthumb is correctly positioned and stays within bounds.
- Add effects to the background to make it look more realistic.
- Add light beam that moves across the screen. (Could be animation or a gif).
- Add Oscilloscope or some kind of signal visualisation.
- Add a testbutton that produces a simple sinewave at the start of the signal chain.
- Add presetbrowser and ability to save presets.

## Contributing

Contributions are welcome! Please feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details. (Note: A `LICENSE` file does not currently exist in the repository. Please create one if you intend to use the MIT License.)
