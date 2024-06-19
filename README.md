# fablegen

## Building


### Local
Builds are managed via CMake. The steps to generate the necessary files and
properly build the project are as follows:
1. Install nanobind with pip:
   ```bash
   pip install nanobind
   ```
2. Note where nanobind was installed. We'll need to reference the location
   of the associated python executable and set its location as an environment
   variable:
   ```bash
   export PYTHON_EXECUTABLE=path/to/executable
   ```
   This allows us to set a different value locally vs in CI and use the same
   CMakeLists.txt file in both locations
3. Create a `build/` directory in `cd` into it:
   ```bash
   mkdir build && cd build
   ```
4. Run cmake to generate the necessary build files:
   ```bash
   cmake ..
   ```
5. Return to the root of the directory and run make:
   ```bash
   cd .. && make
   ```
   This should run the build and result in a `.so` file in the `build/` directory
