# Sentence Piece Unreal Plugin

> simple plugin to make the sentencepiece tokenizer available in Unreal Engine

supported platforms:
- MacOS 26 (Intel & ARM)
- Windows 11 (x64 only)

### Prerequisites
- MacOS
   ```sh
   brew install cmake
   ```
- Windows
   ```sh
   scoop install main/cmake
   ```

### Installation

1. Build the sentencepiece lib for your platform manually
   1. MacOS
      1. Make build script executable
         ```sh
         cd Source/ThirdParty/BuildScripts && chmod +x Build_Mac.sh
         ```
      2. Execute the install script
         ```sh
         ./Build_Mac.sh
         ```
   2. Windows
      1. Run the `Build_Windows.bat` file
      ```sh
      ./Source/ThirdParty/BuildScripts/Build_Windows.bat
      ```
2. Build the plugin using UBT (should happen automatically if placed in the `/Plugins` directory)