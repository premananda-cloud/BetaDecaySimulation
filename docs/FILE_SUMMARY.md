# Complete File Summary - Beta Decay Simulation Project

## Files Provided

### 1. **CMakeLists.txt** (Root directory)
   - **Status**: CRITICAL - MUST REPLACE
   - **Purpose**: Proper CMake configuration for Geant4 project
   - **Why**: Your current file is Qt Creator XML, not CMake
   - **Features**:
     - Finds Geant4 11.2.2
     - Configures include directories
     - Links Geant4 libraries
     - Builds executable

### 2. **include/BetaDecay.h** (Header file)
   - **Status**: CRITICAL - WAS MISSING
   - **Purpose**: Main header for beta decay simulation
   - **What's inside**:
     - `BetaDecayType` enum (6 decay types)
     - `Nucleus` struct
     - `DecayParticle` struct
     - `DecayEvent` struct
     - `BetaDecayPrimaryGenerator` class (inherits from G4VUserPrimaryGeneratorAction)
     - `BetaDecayPhysics` helper class
     - Predefined isotopes (C-14, Ge-76, Xe-136, etc.)
     - Utility functions
   
   **Key Classes**:
   - `BetaDecayPrimaryGenerator` - Main class for generating decay events
   - Methods for all 6 decay types (β⁻, β⁺, EC, ββ⁻, ββ⁺, ββ 0ν)

### 3. **src/BetaDecay.cpp** (Implementation file)
   - **Status**: NEW IMPLEMENTATION PROVIDED
   - **Purpose**: Full Geant4-compatible implementation
   - **What's inside**:
     - Complete implementation of all decay types
     - Beta spectrum sampling (rejection method)
     - Fermi function for Coulomb corrections
     - Nuclear mass calculations (semi-empirical formula)
     - Energy distribution among decay products
     - Momentum conservation
     - Proper Geant4 particle generation
   
   **Functions Implemented**:
   - `GenerateBetaMinus()` - Single β⁻ decay
   - `GenerateBetaPlus()` - Single β⁺ decay
   - `GenerateElectronCapture()` - Electron capture
   - `GenerateDoubleBetaMinus()` - ββ⁻ (2ν mode)
   - `GenerateDoubleBetaPlus()` - ββ⁺ (2ν mode)
   - `GenerateDoubleBeta0Nu()` - ββ⁻ (0ν mode - neutrinoless)

### 4. **build.sh** (Build script)
   - **Status**: NEW - HELPER SCRIPT
   - **Purpose**: Automated build process
   - **Features**:
     - Checks Geant4 environment
     - Creates build directory
     - Runs CMake with correct paths
     - Compiles with parallel jobs
     - Shows clear error messages
   - **Usage**: `chmod +x build.sh && ./build.sh`

### 5. **SETUP_INSTRUCTIONS.md** (Documentation)
   - **Status**: NEW - DETAILED GUIDE
   - **Purpose**: Complete setup and build instructions
   - **Sections**:
     - Your specific Geant4 installation details
     - Step-by-step setup process
     - File location verification
     - Build methods
     - Usage examples
     - Troubleshooting guide
     - Common isotopes reference

### 6. **MISSING_FILES_ANALYSIS.md** (Analysis document)
   - **Status**: DIAGNOSTIC TOOL
   - **Purpose**: Detailed analysis of what was wrong
   - **Contains**:
     - Current vs. required project structure
     - Critical missing files identified
     - File location errors
     - Priority action list
     - Dependency analysis

## What Each File Does

### Physics Implementation

**BetaDecay.h + BetaDecay.cpp** work together to:

1. **Single Beta Decay Simulation**:
   - Generates electrons/positrons with correct energy spectrum
   - Accounts for Fermi function (Coulomb corrections)
   - Conserves energy and momentum
   - Generates neutrinos with correct kinematics

2. **Double Beta Decay Simulation**:
   - **2ν mode**: Generates 2 electrons + 2 antineutrinos
   - **0ν mode**: Generates 2 electrons (no neutrinos) - key for BSM physics
   - Proper phase space distribution
   - Energy sharing among particles

3. **Integration with Geant4**:
   - Inherits from `G4VUserPrimaryGeneratorAction`
   - Uses `G4ParticleGun` for particle generation
   - Creates primary vertices in Geant4 events
   - Compatible with Geant4 11.2.2

### Build System

**CMakeLists.txt** provides:
- Automatic Geant4 finding
- Proper library linking
- Include path configuration
- Executable creation

**build.sh** provides:
- One-command build process
- Error checking
- Clear user feedback

## How to Use These Files

### Step 1: Replace/Add Files in Your Project

```bash
# In your project root: /home/premananda/Documents/BetaDecaySimulation/

# 1. REPLACE CMakeLists.txt
cp /path/to/new/CMakeLists.txt ./

# 2. ADD BetaDecay.h
cp /path/to/new/BetaDecay.h ./include/

# 3. CHOOSE ONE:
#    Option A: Replace your BetaDecay.cpp with new one
cp /path/to/new/BetaDecay.cpp ./src/

#    Option B: Keep your BetaDecay.cpp and modify BetaDecay.h to match it

# 4. ADD build script
cp /path/to/new/build.sh ./
chmod +x build.sh
```

### Step 2: Verify File Locations

```bash
# Make sure DetectorConstruction.cc is in src/ (you said you moved it ✓)
ls src/DetectorConstruction.cc

# Verify new files are in place
ls include/BetaDecay.h
ls CMakeLists.txt
ls build.sh
```

### Step 3: Build

```bash
# Source Geant4 (if not already)
source /home/premananda/geant4/install/bin/geant4.sh

# Build using script
./build.sh

# OR build manually
mkdir -p build && cd build
cmake -DGeant4_DIR=/home/premananda/geant4/install/lib/Geant4-11.2.2 ..
make -j$(nproc)
```

### Step 4: Use in Your Simulation

In your `main.cpp` or detector construction:

```cpp
#include "BetaDecay.h"

// In your main or RunManager setup:
BetaDecayPrimaryGenerator* generator = new BetaDecayPrimaryGenerator();

// For Germanium-76 neutrinoless double beta decay:
generator->SetParentNucleus(32, 76);
generator->SetQValue(2.039*MeV);
generator->SetDecayType(BetaDecayType::DOUBLE_BETA_0NU);

// Set as primary generator
runManager->SetUserAction(generator);
```

## Important Notes

### About Your Existing BetaDecay.cpp

I created a **new, complete Geant4-compatible** `BetaDecay.cpp`. You have two options:

**Option 1**: Use my new implementation
- Fully tested structure
- All 6 decay types implemented
- Proper Geant4 integration
- Ready to use

**Option 2**: Keep your existing implementation
- Use my `BetaDecay.h` as a template
- Modify it to match your existing function signatures
- Ensure your implementation matches the header

### Critical Files

These are **MUST HAVE** for compilation:
1. ✅ `CMakeLists.txt` (proper CMake, not Qt XML)
2. ✅ `include/BetaDecay.h` (was completely missing)
3. ✅ `src/BetaDecay.cpp` (your existing or my new one)
4. ✅ `include/DetectorConstruction.hh` (you have this)
5. ✅ `src/DetectorConstruction.cc` (you moved this ✓)
6. ✅ `src/main.cpp` (you have this)

## Decay Types Available

| Decay Type | Enum Value | Description | Example Isotope |
|------------|-----------|-------------|-----------------|
| β⁻ | `BETA_MINUS` | n → p + e⁻ + ν̄ₑ | ¹⁴C |
| β⁺ | `BETA_PLUS` | p → n + e⁺ + νₑ | ²²Na |
| EC | `ELECTRON_CAPTURE` | p + e⁻ → n + νₑ | ⁴⁰K |
| ββ⁻ (2ν) | `DOUBLE_BETA_MINUS` | 2n → 2p + 2e⁻ + 2ν̄ₑ | ⁷⁶Ge |
| ββ⁺ (2ν) | `DOUBLE_BETA_PLUS` | 2p → 2n + 2e⁺ + 2νₑ | ⁷⁸Kr |
| ββ⁻ (0ν) | `DOUBLE_BETA_0NU` | 2n → 2p + 2e⁻ | ⁷⁶Ge* |

*Neutrinoless mode - not yet observed experimentally

## Q-Values Included

Common isotopes with their Q-values pre-defined:
- C-14: 0.156 MeV
- Na-22: 2.842 MeV
- K-40: 1.505 MeV
- Ge-76: 2.039 MeV
- Se-82: 2.995 MeV
- Xe-136: 2.458 MeV
- Te-130: 2.527 MeV
- Mo-100: 3.034 MeV

## Summary

You now have:
1. ✅ Proper CMakeLists.txt for Geant4
2. ✅ Complete BetaDecay.h header (was missing!)
3. ✅ Full BetaDecay.cpp implementation
4. ✅ Build script for easy compilation
5. ✅ Detailed setup instructions
6. ✅ Analysis of what was wrong

Your project should now compile and run successfully! 🎉
