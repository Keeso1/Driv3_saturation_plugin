![Banner Image](banner.png)

<!-- Recommended banner dimensions: 1280x640 pixels. Please replace 'path/to/your/banner.png' with the actual path to your banner image. -->

# Driv3 saturation plugin

## Overview

`Driv3` is a modern C++17 audio saturation plugin developed using the JUCE framework. It is designed to function as both a VST3 plugin for seamless integration into Digital Audio Workstations (DAWs) and a standalone application for independent use.

## Features

- **Three saturation types:** Soft clip, Hard clip and custom Fuzz clip.
- **Frequency visualisation:** Using the Juce fast fourier transformation class.

## Getting Started

To build and run `Driv3 saturation plugin`, you will need the following:

### Prerequisites

- **C++ Compiler:** A C++17 compatible compiler (e.g., MSVC for Windows, Clang for macOS/Linux).
- **IDE:**
  - **Windows:** Visual Studio 2022 (or newer).
  - **macOS:** Xcode.
  - **Linux:** A suitable C++ IDE (e.g., CLion, VS Code with C++ extensions).

### Building the Project

1. From the project root run "cmake -S . -B build"
2. Then to build the project run "cmake --build build"

(If you want to build the release version run "cmake --build build --config Release")

## Usage

### As a VST3 Plugin

1.  The generated `.vst3` file should automatically be copied to your DAW's VST3 plugin folder (e.g., `C:\Program Files\Common Files\VST3` on Windows, `/Library/Audio/Plug-Ins/VST3` on macOS).
2.  Rescan your plugin folders within your DAW.
3.  Insert `Driv3 saturation plugin` as an effect or instrument on an audio track.

### As a Standalone Application

Simply run the executable generated during the build process.

## TODOs

- Fix slider bounds so that the sliderthumb is correctly positioned and stays within bounds.
- Add effects to the background to make it look more realistic.
- Add light beam that moves across the screen. (Could be animation or a gif).
- Add presetbrowser and ability to save presets.

## Contributing

Contributions are welcome! Please feel free to fork the repository, make your changes, and submit a pull request.
