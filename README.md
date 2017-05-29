# updevice clion project
Go to File -> Settings and under Build, Execution, Deployment -> CMake set the following:
CMake options: -DCMAKE_TOOLCHAIN_FILE=UpDevice.cmake

Reload your CLion project (Tools -> CMake -> Reset Cache and Reload Project).