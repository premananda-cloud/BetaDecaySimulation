#!/bin/bash

# Build script for Beta Decay Simulation with Geant4
# Make sure you have sourced geant4.sh before running this script

echo "=========================================="
echo "Beta Decay Simulation - Build Script"
echo "=========================================="
echo ""

# Check if Geant4 is sourced
if [ -z "$G4INSTALL" ]; then
    echo "⚠️  Warning: Geant4 environment not detected."
    echo "Please source your geant4.sh file first:"
    echo "  source /home/premananda/geant4/install/bin/geant4.sh"
    echo ""
    read -p "Do you want to continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo ""
echo "Running CMake..."
cmake -DGeant4_DIR=/home/premananda/geant4/install/lib/Geant4-11.2.2 ..

if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed!"
    exit 1
fi

# Build the project
echo ""
echo "Building project..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo ""
echo "✅ Build successful!"
echo ""
echo "Executable location: build/BetaDecaySimulation"
echo ""
echo "To run the simulation:"
echo "  cd build"
echo "  ./BetaDecaySimulation"
echo ""
