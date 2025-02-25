# my-cpp-project/README.md

# My C++ Project

## Overview
This project is a C++ application that utilizes CMake for build management. It consists of an executable, a library, tests, and vendor dependencies.

## Folder Structure
- **executable/**: Contains the main application code.
- **library/**: Contains the library code that the executable uses.
- **tests/**: Contains unit tests for the library.
- **vendor/**: Contains third-party dependencies.

## Setup Instructions
1. Ensure you have CMake installed on your system.
2. Clone the repository:
   ```sh
   git clone <repository-url>
   cd <repository-name>
   ```
3. Create a build directory:
   ```sh
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```sh
   cmake ..
   ```
5. Build the project:
   ```sh
   cmake --build .
   ```

## Usage
After building the project, you can run the executable located in the `executable` directory.

## Running Tests
To run the tests, navigate to the `tests` directory and execute the test binary.

## Contributing
Feel free to submit issues or pull requests for improvements or bug fixes.
