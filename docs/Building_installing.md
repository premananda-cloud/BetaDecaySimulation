# Build and Installation Instructions

## Prerequisites

Before building, ensure you have Geant4 installed. Based on your filepath, you should source the Geant4 environment:

```bash
source /home/premananda/geant4/install/bin/geant4.sh
```

Or set the environment variable:
```bash
export CMAKE_PREFIX_PATH=/home/premananda/geant4/install:$CMAKE_PREFIX_PATH
```

## Quick Start

### 1. Extract/Navigate to Project Directory
```bash
cd BetaDecaySimulation
```

### 2. Create Build Directory
```bash
mkdir build
cd build
```

### 3. Configure with CMake
```bash
cmake ..
```

You should see output like:
```
-- Found Geant4: /home/premananda/geant4/install
-- Configuring done
-- Generating done
```

### 4. Compile
```bash
make -j$(nproc)
```

Or for specific number of cores:
```bash
make -j4
```

### 5. Run the Simulation

#### Interactive Mode (with visualization):
```bash
./betadecay
```

#### Batch Mode (no visualization):
```bash
./betadecay run.mac
```

## Detailed Build Process

### Step-by-Step CMake Configuration

If CMake cannot find Geant4, try:

```bash
cmake -DGeant4_DIR=/home/premananda/geant4/install/lib/Geant4-11.2.2 ..
```

Or:
```bash
cmake -DCMAKE_PREFIX_PATH=/home/premananda/geant4/install ..
```

### Checking CMake Output

Successful configuration shows:
```
-- The C compiler identification is GNU
-- The CXX compiler identification is GNU
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Found Geant4: /path/to/geant4/install
-- Geant4 version: 11.2.2
-- Configuring done
-- Generating done
-- Build files have been written to: /path/to/BetaDecaySimulation/build
```

### Build Output

Successful compilation shows:
```
Scanning dependencies of target betadecay
[  7%] Building CXX object CMakeFiles/betadecay.dir/betadecay.cc.o
[ 14%] Building CXX object CMakeFiles/betadecay.dir/src/DetectorConstruction.cc.o
[ 21%] Building CXX object CMakeFiles/betadecay.dir/src/EventAction.cc.o
[ 28%] Building CXX object CMakeFiles/betadecay.dir/src/PhysicsList.cc.o
[ 35%] Building CXX object CMakeFiles/betadecay.dir/src/PrimaryGeneratorAction.cc.o
[ 42%] Building CXX object CMakeFiles/betadecay.dir/src/RunAction.cc.o
[ 50%] Building CXX object CMakeFiles/betadecay.dir/src/SteppingAction.cc.o
[100%] Linking CXX executable betadecay
[100%] Built target betadecay
```

## Running the Simulation

### Interactive Mode Features
- Opens OpenGL visualization window
- Shows detector geometry in 3D
- Displays particle trajectories with color coding:
  * Blue: electrons (e-)
  * Red: positrons (e+)
  * Green: gamma rays
  * Yellow: neutrons
- Runs 100 events by default
- Real-time event visualization

### Batch Mode Features
- No visualization (faster)
- Runs 1000 events by default
- Outputs statistics to terminal
- Suitable for large simulation runs

## Customizing Run Parameters

### Modify Number of Events

Edit `run.mac`:
```bash
# Change this line:
/run/beamOn 1000
# To desired number, e.g.:
/run/beamOn 10000
```

Edit `vis.mac`:
```bash
# Change this line:
/run/beamOn 100
# To desired number
```

### Change Verbosity

In macro files, adjust:
```bash
/control/verbose 2    # Control messages
/run/verbose 2        # Run information
/event/verbose 0      # Event details (0=off, 1=on)
/tracking/verbose 0   # Track details (0=off, 1=on, 2=detailed)
```

### Run with Different Isotopes

Modify `src/PrimaryGeneratorAction.cc`:
- Change Z (atomic number) and A (mass number)
- Adjust branching ratios in `GeneratePrimaries()` function

## Troubleshooting

### Problem: CMake can't find Geant4

**Solution 1**: Set environment
```bash
source /home/premananda/geant4/install/bin/geant4.sh
```

**Solution 2**: Specify Geant4 location explicitly
```bash
cmake -DGeant4_DIR=/home/premananda/geant4/install/lib/Geant4-11.2.2 ..
```

### Problem: Compilation errors about missing headers

**Solution**: Ensure Geant4 development files are installed
```bash
# Check Geant4 installation
ls /home/premananda/geant4/install/include/Geant4/
```

### Problem: OpenGL visualization doesn't work

**Solution 1**: Check OpenGL support
```bash
glxinfo | grep "OpenGL version"
```

**Solution 2**: Use batch mode instead
```bash
./betadecay run.mac
```

**Solution 3**: Try different visualization driver
Edit `vis.mac` and change:
```bash
/vis/open OGL 600x600-0+0
```
To:
```bash
/vis/open HepRepFile
# or
/vis/open DAWNFILE
```

### Problem: Segmentation fault at runtime

**Solution 1**: Check all libraries loaded
```bash
ldd ./betadecay
```

**Solution 2**: Verify Geant4 environment
```bash
echo $G4LEVELGAMMADATA
echo $G4RADIOACTIVEDATA
```

**Solution 3**: Initialize explicitly
```bash
source /home/premananda/geant4/install/bin/geant4.sh
```

### Problem: No beta decays detected

**Solution**: Verify physics list includes radioactive decay
- Check that `G4RadioactiveDecayPhysics` is registered
- Increase number of events
- Check source isotope has beta decay mode

## Performance Optimization

### Fast Compilation
```bash
# Use all CPU cores
make -j$(nproc)

# Or specify number
make -j8
```

### Fast Simulation
```bash
# Run batch mode
./betadecay run.mac

# Reduce verbose output
# Edit macro files to set verbose levels to 0
```

### Memory Usage
```bash
# Monitor memory during run
top -p $(pgrep betadecay)

# Or use
/usr/bin/time -v ./betadecay run.mac
```

## Advanced Configuration

### Install System-Wide
```bash
sudo make install
```

### Clean Build
```bash
# In build directory
make clean

# Or remove and recreate
cd ..
rm -rf build
mkdir build
cd build
cmake ..
make
```

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Release Build (optimized)
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Output Files

The simulation creates:
- Terminal output with statistics
- Optional ROOT files (if configured)
- Visualization files (if using file-based drivers)

## Next Steps

After successful build:
1. Run interactive mode to visualize geometry
2. Run batch mode for statistics collection
3. Modify source code for your specific needs
4. Analyze output data
5. Adjust detector geometry or physics as needed

## Getting Help

If you encounter issues:
1. Check Geant4 version compatibility
2. Verify all environment variables are set
3. Review Geant4 installation documentation
4. Check compile/runtime errors carefully
5. Enable verbose output for debugging

## Additional Resources

- Geant4 User Guide: https://geant4-userdoc.web.cern.ch/
- Geant4 Physics Reference: https://geant4-userdoc.web.cern.ch/UsersGuides/PhysicsReferenceManual/html/index.html
- Geant4 Examples: $G4INSTALL/share/Geant4/examples/