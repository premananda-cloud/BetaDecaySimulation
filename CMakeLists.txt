cmake_minimum_required(VERSION 3.16)
project(BetaDecaySimulation VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Geant4 directory - point to the directory containing Geant4Config.cmake
# Since you found it at /home/premananda/geant4/build/Geant4Config.cmake
set(Geant4_DIR "/home/premananda/geant4/build" CACHE PATH "Path to Geant4Config.cmake")

# Find Geant4 package
find_package(Geant4 REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Include Geant4 directories
include(${Geant4_USE_FILE})

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/BetaDecaySimulation.cc
    src/DetectorConstruction.cc
    src/PhysicsList.cc
    src/PrimaryGeneratorAction.cc
    src/RunAction.cc
    src/EventAction.cc
    src/SteppingAction.cc
)

# Headers
set(HEADERS
    include/BetaDecaySimulation.hh
    include/DetectorConstruction.hh
    include/PhysicsList.hh
    include/PrimaryGeneratorAction.hh
    include/RunAction.hh
    include/EventAction.hh
    include/SteppingAction.hh
)

# Create executable
add_executable(BetaDecaySimulation ${SOURCES} ${HEADERS})

# Link libraries - Modern Geant4 uses imported targets
target_link_libraries(BetaDecaySimulation
    Geant4::G4run
    Geant4::G4event
    Geant4::G4tracking
    Geant4::G4processes
    Geant4::G4digits_hits
    Geant4::G4track
    Geant4::G4particles
    Geant4::G4geometry
    Geant4::G4materials
    Geant4::G4graphics_reps
    Geant4::G4intercoms
    Geant4::G4global
    Geant4::G4clhep
    Geant4::G4Tree
    Geant4::G4FR
    Geant4::G4GMocren
    Geant4::G4visHepRep
    Geant4::G4RayTracer
    Geant4::G4VRML
    Geant4::G4OpenGL
    Geant4::G4Qt3D
    Qt5::Core
    Qt5::Widgets
)

# OR simpler if available (depends on Geant4 version):
# target_link_libraries(BetaDecaySimulation
#     Geant4::G4basic
#     Qt5::Core
#     Qt5::Widgets
# )

# Modern way to include directories
target_include_directories(BetaDecaySimulation PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Copy macro files to build directory
file(COPY macros DESTINATION ${CMAKE_BINARY_DIR})

# Optional: Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optional: Enable better compiler warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(BetaDecaySimulation PRIVATE -Wall -Wextra -Wpedantic)
endif()
