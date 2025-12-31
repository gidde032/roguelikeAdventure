# The Adventures of Juggman: A Roguelike Journey

<img width="841" height="563" alt="Screenshot 2025-12-31 at 9 21 13 AM" src="https://github.com/user-attachments/assets/98483090-139d-4a32-96f4-1dc383955e28" />

A terminal-based roguelike dungeon crawler built in C with ncurses. Navigate procedurally generated dungeons, battle monsters, collect items, and ascend through increasingly difficult levels in an attempt to escape the dungeon.

## Features

- **Procedurally Generated Levels**: Each playthrough features unique dungeon layouts with 20-34 randomly generated rooms connected by hallways.
- **Field of View (FOV) System**: Dynamic line-of-sight calculations using Bresenham's algorithm reveal only what is in view of the player.
- **Progressive Difficulty**: Four levels with escalating enemy difficulty and spawn rates.
- **Strategic Combat**: Turn-based combat with multiple enemy types, each with distinct stats and behaviors.
- **Item System**: Healing items (medkits, bandages) and power-ups (attack potions) scattered throughout levels.
- **Persistent Memory Management**: Efficient allocation and deallocation of game entities upon each level-up.

## Technical Implementation

### Core Technologies
- **Language**: C (C99 standard)
- **Graphics Library**: ncurses for terminal rendering and input handling
- **Build System**: GNU Make

### Architecture

```
roguelike/
├── include/
│   └── rogue.h           # Header file with all structs, constants, and function declarations
├── src/
│   ├── main.c            # Entry point and global variable initialization
│   ├── engine.c          # Game loop, screen rendering, and level progression
│   ├── map.c             # Procedural map generation and room creation
│   ├── room.c            # Room geometry and hallway connection algorithms
│   ├── player.c          # Player entity creation and input handling
│   ├── fov.c             # Field of view calculations and line-of-sight algorithms
│   ├── combat.c          # Combat mechanics and collision detection
│   ├── draw.c            # Rendering functions for all game entities
│   ├── goblin.c          # Goblin statistics, creation, and behavior
│   ├── spider.c          # Spider statistics, creation, and behavior
│   ├── troll.c           # Troll statistics, creation, and behavior
│   └── buffs.c           # Item creation and usage mechanics
└── makefile              # Build configuration
```

### Key Algorithms

<img width="753" height="461" alt="Screenshot 2025-12-31 at 9 22 23 AM" src="https://github.com/user-attachments/assets/f68c0684-ec1f-4a13-9558-ff0bf81bad33" />

**Field of View (FOV)**: Implements Bresenham's line algorithm to determine visible tiles within a 15-tile radius, accounting for walls and obstacles blocking player's sightlines.

**Procedural Generation**: 
- Random room placement with collision avoidance to prevent overlap
- Pathfinding algorithm connects room centers with narrow hallways, creating combat chokepoints and cover for the player
- Balanced entity spawning using modular arithmetic for distribution

**Enemy AI**: Monsters use Manhattan distance calculations for pathfinding and pursue the player when within line of sight.

## Prerequisites
- GCC compiler
- ncurses development library
- Make build system

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev
```

**macOS:**
```bash
brew install ncurses
```

**Arch Linux:**
```bash
sudo pacman -S base-devel ncurses
```

## Building and Running

### Quick Start
```bash
make all
```

This compiles the source code and automatically runs the game.

### Manual Build Steps

**Compile only:**
```bash
make rogue
```

**Run the game:**
```bash
./rogue
```

**Clean build artifacts:**
```bash
make clean
```

## How to Play

### Controls
- `W` - Move up
- `A` - Move left
- `S` - Move down
- `D` - Move right
- `R` - Ascend ladder (when standing on `>` or `^` tile)
- `Q` - Quit game

### Game Mechanics

<img width="369" height="263" alt="Screenshot 2025-12-31 at 9 22 59 AM" src="https://github.com/user-attachments/assets/1793f177-75ce-4ee7-9e94-25e0e3ebd77b" />

**Exploration**: Navigate through procedurally generated dungeons represented by ASCII characters:
- `.` - Floor tiles (walkable)
- `#` - Wall tiles (impassable)
- `>` - Ladder to next level
- `^` - Final ladder to escape the dungeon
- `J` - Player character (you)

**Combat**: Engage enemies by moving into them. Combat is automatic and turn-based:
- `G` - Goblins (8-11 HP, 2-3 ATK) - Basic enemies
- `S` - Spiders (5-6 HP, 3-5 ATK) - Fragile, but faster and harder-hitting
- `T` - Trolls (16-21 HP, 5-8 ATK) - Rare and dangerous bruisers, powerful and tanky
- `X` - Dead enemy

**Items**: Walk over items to automatically use them:
- `+` - Medkit (restores 50 HP)
- `*` - Bandage (restores 25 HP)
- `&` - Attack Potion (permanently increases attack by 2)
- `0` - Used healing items
- `u` - Used Attack Potions

**Visibility**: 
- White tiles are currently visible
- Blue tiles have been explored but are not in current view
- Black areas are unexplored

### Win Condition

<img width="565" height="437" alt="Screenshot 2025-12-31 at 11 03 28 AM" src="https://github.com/user-attachments/assets/0cae1262-be61-46ff-8540-6b66fa3ccfd3" />

Successfully ascend all three dungeon levels by locating and using the ladder (`>`) on each floor. Ascend the final ladder (`^`) to win the game and display a congratulatory screen commending your achievement.

### Loss Condition

<img width="540" height="427" alt="Screenshot 2025-12-31 at 10 57 47 AM" src="https://github.com/user-attachments/assets/c5113e49-6a2e-4b50-9305-73496a9dae9d" />

If the player's health points (HP) reach zero, the game is lost, and the death screen is displayed, noting the highest level reached by the player.

### Stat Tracking
The total number of monsters defeated ("juggs hit"), the player's HP and attack, and the current level are tracked during each run.

## Game Stats

<img width="377" height="275" alt="Screenshot 2025-12-31 at 9 28 08 AM" src="https://github.com/user-attachments/assets/f8da9bc5-9c9f-467b-8abf-c28d2d72c7c9" />

The HUD displays:
- **HP**: Current and maximum health points
- **ATK**: Current attack damage
- **LVL**: Current dungeon level (0-3)
- **JUGGS HIT**: Total monsters defeated

## Design Decisions

### Memory Management
Memory is dynamically allocated for all game entities, and carefully deallocated with the `closeGame()` function when needed. Arrays are pre-allocated with sufficient capacity (200 monsters, 30 heals, 30 items) to avoid reallocation during gameplay.

### Modular Architecture
Each enemy type and game system (FOV, combat, rendering) is separated into its own source file for maintainability and scalability. The header file maintains a single source of information for all shared data structures.

### Color System

<img width="754" height="461" alt="Screenshot 2025-12-31 at 9 23 39 AM" src="https://github.com/user-attachments/assets/32cd9fa9-c349-45c2-887c-3eb337d842cd" />

Roguelike Adventure makes use of 8 distinct color pairs to differentiate between entities and provide visual clarity, even in densely populated dungeon areas.

## Known Limitations

- Terminal must support color output
- Minimum terminal size required: 100 columns × 26 rows
- No save/load system (roguelikes feature permadeath by design)

## Future Enhancements

Planned features for expansion:
- Additional monster types with advanced behaviors
- More complex item system (weapons, armor, inventory), along with procedurally generated chests containing random items
- Boss encounters at level transitions
- Unique dungeon themes with visual variation
- Score tracking and leaderboard system

---

## Credits

- **Author**: Finn Gidden
- **Contact**: gidde032@umn.edu
