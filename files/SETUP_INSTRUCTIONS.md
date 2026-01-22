# Beta Decay Simulation - Setup & Build Instructions

## Your Geant4 Installation
- **Geant4 Directory**: `/home/premananda/geant4/install/lib/Geant4-11.2.2`
- **Geant4 Version**: 11.2.2

## Project Structure After Fixes

```
BetaDecaySimulation/
├── CMakeLists.txt              ⬅️ REPLACE with new version
├── build.sh                    ⬅️ NEW - Build script
├── README.md
├── Building_installing.md
├── Project_structure.md
├── quizk_start.md
│
├── include/
│   ├── BetaDecay.h            ⬅️ NEW - CRITICAL missing file
│   ├── DetectorConstruction.hh
│   └── main_header.h          (review if still needed)
│
├── src/
│   ├── BetaDecay.cpp          ✅ Keep your existing version OR use new one
│   ├── DetectorConstruction.cc ⬅️ MOVED from include/
│   └── main.cpp
│
└── build/                      (auto-generated)
```

## Step-by-Step Setup

### 1. Source Geant4 Environment
Every time you open a new terminal, run:
```bash
source /home/premananda/geant4/install/bin/geant4.sh
```

**Tip**: Add this to your `~/.bashrc` to make it automatic:
```bash
echo "source /home/premananda/geant4/install/bin/geant4.sh" >> ~/.bashrc
```

### 2. Replace/Add Missing Files

#### Option A: Use the provided files
Copy the new files I created:
- `CMakeLists.txt` → Replace your existing one
- `include/BetaDecay.h` → Add this file (CRITICAL)
- `src/BetaDecay.cpp` → Replace OR merge with your existing one
- `build.sh` → Add this new build script

#### Option B: Keep your existing BetaDecay.cpp
If you want to keep your current `src/BetaDecay.cpp`, just add:
- `CMakeLists.txt` (replace)
- `include/BetaDecay.h` (add)

Then modify `BetaDecay.h` to match your actual implementation.

### 3. Verify File Locations
Make sure:
```bash
# DetectorConstruction.cc should be in src/, NOT include/
ls src/DetectorConstruction.cc    # Should exist
ls include/DetectorConstruction.cc # Should NOT exist

# BetaDecay.h must exist in include/
ls include/BetaDecay.h            # Should exist
```

If `DetectorConstruction.cc` is still in `include/`, move it:
```bash
mv include/DetectorConstruction.cc src/
```

### 4. Build the Project

#### Method 1: Using the build script (easiest)
```bash
chmod +x build.sh
./build.sh
```

#### Method 2: Manual build
```bash
# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake -DGeant4_DIR=/home/premananda/geant4/install/lib/Geant4-11.2.2 ..

# Build
make -j$(nproc)

# The executable will be: build/BetaDecaySimulation
```

### 5. Run the Simulation
```bash
cd build
./BetaDecaySimulation
```

## Features Implemented

### Single Beta Decay
- **β⁻ decay**: Carbon-14, etc.
- **β⁺ decay**: Sodium-22, etc.
- **Electron Capture**: Potassium-40, etc.

### Double Beta Decay
- **ββ⁻ (2ν mode)**: Germanium-76, Selenium-82, Xenon-136, etc.
- **ββ⁺ (2ν mode)**: Two-neutrino double beta plus
- **ββ⁻ (0ν mode)**: Neutrinoless double beta decay (hypothetical)

## Using the Simulation

### In your main.cpp or macro files:

```cpp
#include "BetaDecay.h"

// Create the primary generator
BetaDecayPrimaryGenerator* primaryGenerator = new BetaDecayPrimaryGenerator();

// Configure for single beta decay
primaryGenerator->SetParentNucleus(6, 14);  // Carbon-14
primaryGenerator->SetQValue(0.156*MeV);
primaryGenerator->SetDecayType(BetaDecayType::BETA_MINUS);

// OR configure for double beta decay
primaryGenerator->SetParentNucleus(32, 76);  // Germanium-76
primaryGenerator->SetQValue(2.039*MeV);
primaryGenerator->SetDecayType(BetaDecayType::DOUBLE_BETA_MINUS);

// OR neutrinoless mode
primaryGenerator->SetDecayType(BetaDecayType::DOUBLE_BETA_0NU);
```

### Common Isotopes Available

```cpp
// In your code, you can use predefined isotopes:
using namespace BetaDecayIsotopes;

// Single beta decay
primaryGenerator->SetParentNucleus(C14.Z, C14.A);
primaryGenerator->SetQValue(QValues::C14_BETA_MINUS*MeV);

// Double beta decay
primaryGenerator->SetParentNucleus(Ge76.Z, Ge76.A);
primaryGenerator->SetQValue(QValues::Ge76_DBD*MeV);
primaryGenerator->SetDecayType(BetaDecayType::DOUBLE_BETA_0NU);
```

## Troubleshooting

### Problem: "CMake Error: Could not find Geant4"
**Solution**: Make sure you sourced geant4.sh:
```bash
source /home/premananda/geant4/install/bin/geant4.sh
```

### Problem: "BetaDecay.h: No such file or directory"
**Solution**: Create the `include/BetaDecay.h` file (I provided this)

### Problem: "undefined reference to BetaDecayPrimaryGenerator"
**Solution**: Make sure `src/BetaDecay.cpp` is in your src/ directory and matches the header

### Problem: Build fails with detector errors
**Solution**: Make sure `DetectorConstruction.cc` is in `src/`, not `include/`

## Verifying Your Setup

Run this checklist:

```bash
# 1. Geant4 environment
echo $G4INSTALL
# Should show: /home/premananda/geant4/install

# 2. Required files exist
ls include/BetaDecay.h
ls include/DetectorConstruction.hh
ls src/BetaDecay.cpp
ls src/DetectorConstruction.cc
ls src/main.cpp
ls CMakeLists.txt

# 3. All files should exist with no errors
```

## Next Steps

1. ✅ Verify all files are in correct locations
2. ✅ Source Geant4 environment
3. ✅ Run build script or manual build
4. ✅ Test with simple decay
5. 🎯 Run your simulations!

## Need Help?

If you encounter issues:
1. Check that Geant4 is properly sourced
2. Verify file locations match the structure above
3. Make sure `BetaDecay.h` matches your `BetaDecay.cpp` implementation
4. Check CMake output for specific errors

Good luck with your beta decay simulations! 🎯⚛️
