# Simple 2D Pong Game

A classic Pong game implemented in C++ with SFML, featuring real-time graphics, smooth paddle movement, collision detection, and score tracking.


# Developer

<p align="center">
  <!-- 👤 PROFILE PICTURE PLACEHOLDER -->
  <img src="cyiza.png" width="120" style="border-radius:50%" />
</p>

<p align="center">
  <b>Mpayimana Cyiza Landry</b><br/>
  Senior Software Engineer
</p>

---

## Features

- **Real-time Graphics**: Smooth 60 FPS rendering with SFML
- **Dual Player Support**: Two-player local gameplay
- **Physics System**: Realistic ball physics with collision detection
- **Score Tracking**: First to 10 points wins
- **Dynamic Difficulty**: Ball speed increases with each paddle hit
- **Game States**: Pause and Game Over screens
- **Responsive Controls**: Smooth keyboard-controlled paddle movement

## Controls

| Action | Player 1 | Player 2 |
|--------|----------|----------|
| Move Up | W Key | Up Arrow |
| Move Down | S Key | Down Arrow |
| Pause Game | P Key | P Key |
| Restart (Game Over) | R Key | R Key |
| Exit Game | ESC Key | ESC Key |

## Requirements

- C++17 or higher
- SFML 2.5 or higher
- CMake 3.10 or higher (recommended)

## Installation

### Method 1: Using CMake (Recommended)

1. **Install SFML:**
   - **Ubuntu/Debian:** `sudo apt-get install libsfml-dev`
   - **macOS:** `brew install sfml`
   - **Windows:** Download from [SFML website](https://www.sfml-dev.org/download.php)

2. **Clone and Build:**
   ```bash
   git clone https://github.com/CyizaLandry5/Simple-2D-Pong-Game.git
   cd pong-game
   mkdir build
   cd build
   cmake ..
   make