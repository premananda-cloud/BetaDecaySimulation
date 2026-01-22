# Beta Decay Simulation using Geant4

A comprehensive Geant4 simulation for studying single and double beta decay processes.

## Overview

This simulation implements:
- **Single Beta- Decay**: Sr-90 → Y-90 + e- + ν̄e (T1/2 = 28.8 years)
- **Single Beta- Decay**: Y-90 → Zr-90 + e- + ν̄e (T1/2 = 64 hours)


## Features

- Realistic detector geometry with source and scintillator detector
- Full radioactive decay physics including beta decay
- Energy deposition tracking in detector
- Beta particle counting (single and double)
- Visualization support with OpenGL
- Batch and interactive modes

## Requirements

- Geant4 (version 11.x recommended)
- CMake (version 3.16 or higher)
- C++ compiler with C++17 support
- OpenGL (for visualization)

## Building the Simulation

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make -j4
```

## Running the Simulation

### Interactive Mode (with visualization)
```bash
./betadecay
```

This will:
1. Open an OpenGL window showing the detector geometry
2. Run 100 events by default
3. Display particle trajectories
4. Show energy deposition statistics

### Batch Mode
```bash
./betadecay run.mac
```

This will run 1000 events without visualization and print statistics.

## Detector Geometry

- **World Volume**: 2m x 2m x 2m air-filled box
- **Source**: Cylindrical strontium volume (radius: 1 cm, height: 2 cm)
- **Detector**: Cylindrical plastic scintillator (radius: 5 cm, height: 10 cm)
- **Detector Position**: 10 cm from source along z-axis

## Physics

The simulation uses:
- `G4RadioactiveDecayPhysics`: Essential for beta decay
- `G4EmStandardPhysics`: Electromagnetic interactions
- `G4DecayPhysics`: General particle decay
- `G4IonPhysics`: Ion interactions

Production cuts are set to 1 micrometer for accurate beta particle tracking.

## Output

The simulation provides:
- Total energy deposited in the detector
- Mean energy per event with statistical uncertainty
- Count of beta- decays detected
- Count of beta+ decays detected
- Count of double beta decay events
- Per-event information (every 10th event in interactive mode)

## Customization

### Change Isotopes
Edit `src/PrimaryGeneratorAction.cc` to modify:
- Isotope selection (Z, A values)
- Branching ratios (probability percentages)
- Source position distribution

### Modify Detector
Edit `src/DetectorConstruction.cc` to change:
- Detector material (default: plastic scintillator)
- Detector size and position
- Source material and geometry
- Add additional detectors

### Adjust Physics
Edit `src/PhysicsList.cc` to:
- Change production cuts (affects precision vs. speed)
- Add/remove physics processes
- Modify physics parameters

## Visualization Colors

- **Red (semi-transparent)**: Source volume
- **Blue (semi-transparent)**: Detector volume
- **Blue trajectories**: Electrons (e-)
- **Red trajectories**: Positrons (e+)
- **Green trajectories**: Gamma rays
- **Yellow trajectories**: Neutrons

## Output Example

```
--------------------End of Global Run-----------------------
 The run consists of 1000 events
 Total energy deposited in detector: 15.234 MeV
 Mean energy per event: 15.234 keV +- 2.134 keV
------------------------------------------------------------
 Beta Decay Statistics:
   Beta- decays detected: 532
   Beta+ decays detected: 0
   Double beta decays detected: 8
------------------------------------------------------------
```

## Technical Notes

### Beta Decay Process
- Beta decay is simulated using Geant4's `G4RadioactiveDecay` process
- The decay chain is automatically followed (e.g., Sr-90 → Y-90 → Zr-90)
- Neutrinos are tracked but don't interact with the detector
- Decay times follow exponential distribution based on half-life

### Double Beta Decay Detection
- Identified by counting electrons per event
- Event is flagged when 2+ primary electrons are created
- Extremely rare due to very long half-life of Ca-48

### Energy Spectrum
- Beta particles have continuous energy spectrum (not monoenergetic)
- Maximum energy depends on Q-value of the decay
- Energy deposition includes all secondary particles

## Troubleshooting

### Compilation Issues
- Ensure Geant4 environment variables are set: `source /path/to/geant4/install/bin/geant4.sh`
- Check CMake finds Geant4: Look for "Found Geant4" message

### Visualization Not Working
- Check OpenGL support: `glxinfo | grep OpenGL`
- Try different visualization driver in `vis.mac`
- Run in batch mode if visualization unavailable

### Low Beta Detection Rate
- Increase number of events in macro files
- Check detector position and size
- Verify physics list includes `G4RadioactiveDecayPhysics`

## References

1. Geant4 Documentation: https://geant4.web.cern.ch
2. Geant4 Physics Reference Manual
3. Nuclear Data Tables for decay chains and half-lives

Premananda singh

Created for studying beta decay processes in nuclear physics experiments.

## License

This simulation is provided as-is for educational and research purposes.
