# Sentence Piece Unreal Plugin

> simple plugin to make sentence piece tokenizations available in Unreal Engine

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
   to-do
   ```

### Installation

1. Build sentencepiece lib for you platform manually
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
2. 