# Simple Bare-metal Task Scheduler

## Prerequisites
To replicate this project, you need the following:

- Linux (Ubuntu) OS, this project is tested with Ubuntu 22.04
    - Install GNU Arm Toolchain - `sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch`
    - Install OpenOCD - `sudo apt install openocd`
    - Install CMake - `sudo apt install cmake`. This project is tested with CMake version 3.22  
- For each case above, add the respective paths to your `Path` environment variable.

## Debugging with VS Code
To debug with VS code, you need the following extensions and you need to create a launch configuration as described below.

- C/C++ extension
- Cortex-Debug (by marus25)

### Create the `launch.json` file
- Open the **Run and Debug** panel in VS Code
- Click on **creae a launch.json file** and choose **Cortex-Debug** from the list
- Replace the generated `.vscode/launch.json` with the template below:

```
json{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug STM32 (OpenOCD)",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "openocd",
            "cwd": "${workspaceRoot}",
            "executable": "${workspaceRoot}/build/your_project.elf",
            "device": "STM32F401RE", 
            "runToEntryPoint": "main",
            "configFiles": [
                "interface/stlink.cfg",
                "board/st_nucleo_f4.cfg"
            ]
        }
    ]
}
```
#### Key Fields to customize:
- `executable`: Modify this to point to your `.elf` binary
- `device`: Enter your MCU part number e.g. STM32F401RE 
- `configFiles`: Modify this based on your hardware. The first string defines your debugger probe and the second string defines the chip family, e.g. `interface/stlink.cfg` and `board/st_nucleo_f4.cfg`for an **stlink** debugger and a **cortex-m4** processor.

## Issues
On Linux, you may have to add your linux user to the `plugdev` group to allow `openocd` access the usb hardware.

`$ sudo usermode -aG plugdev [user]`

where [user] refers to the linux username.

## Flash and Debug
- Connect the hardware by plugging your ST-LINK or STM32 Nucleo board into your USB port
- Compile the binary and start debugging