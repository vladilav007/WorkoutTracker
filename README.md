# Workout Tracker

A Qt-based application for tracking and planning workouts.

## Features

- Calendar-based workout planning
- Workout template management
- Progress tracking
- Data persistence

## Requirements

- C++17 or later
- Qt 6
- CMake 3.16 or later

## Building

1. Clone the repository:
```bash
git clone [repository-url]
cd WorkoutTracker
```

2. Create build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

## Project Structure

```
WorkoutTracker/
├── src/            # Source files
│   ├── models/     # Data models
│   ├── views/      # UI components
│   ├── controllers/# Business logic
│   └── main.cpp    # Entry point
├── resources/      # Application resources
├── tests/          # Unit tests
├── docs/          # Documentation
└── CMakeLists.txt # Build configuration
```

## License

[License information]