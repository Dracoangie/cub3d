<h1 align="center">cub3D</h1>

<p align="center">
    <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge">
    <img src="https://img.shields.io/badge/School-42-black?style=for-the-badge">
    <img src="https://img.shields.io/badge/Graphics-Raycasting-orange?style=for-the-badge">
    <img src="https://img.shields.io/badge/Norminette-Passed-success?style=for-the-badge">
</p>

A **Wolfenstein 3D-inspired raycasting engine** developed in C as part of the 42 School curriculum.

The project implements a complete first-person 3D renderer using the **raycasting** technique, textured walls, player movement, collision detection, and map parsing, all without relying on modern graphics engines.

---

# ✨ Features

- Real-time raycasting engine
- Textured walls
- Floor and ceiling colors
- Player movement
- Rotation with keyboard
- Collision detection
- Configurable maps (.cub)
- Map validation
- Minimap *(if bonus implemented)*
- Mouse rotation *(bonus)*
- Doors *(bonus)*
- Animated sprites *(bonus)*

---

# 🛠 Technologies

- C
- MiniLibX
- X11
- Makefile
- Linux

---

# 📂 Project Structure

```
.
├── includes/
├── src/
│   ├── parse/
│   └── render/
├── textures/
├── maps/
├── LICENSE
├── Makefile
├── README.md
└── cub3D
```

---

# 🚀 Installation

Clone the repository

```bash
git clone https://github.com/Dracoangie/cub3d.git
cd cub3d
```

Compile

```bash
make
```

Run

```bash
./cub3D maps/map.cub
```

---

# 🎮 Controls

| Key | Action |
|------|--------|
| W | Move Forward |
| S | Move Backward |
| A | Strafe Left |
| D | Strafe Right |
| ← | Rotate Left |
| → | Rotate Right |
| ESC | Quit |

---

# 🗺 Map Format

Example:

```text
1111111111111111
1000000000000001
1000011110000001
1000000N00000001
1000000000000001
1111111111111111
```

Player spawn:

- N
- S
- E
- W

---

# 🎨 Configuration (.cub)

```text
NO textures/north.xpm
SO textures/south.xpm
WE textures/west.xpm
EA textures/east.xpm

F 220,100,0
C 100,149,237
```

---

# ⚙️ Raycasting

The rendering engine is based on the classic **DDA (Digital Differential Analyzer)** algorithm.

Each frame:

1. One ray is cast for every screen column.
2. The ray traverses the map grid.
3. Wall intersections are detected.
4. Perpendicular distance is calculated.
5. Texture coordinates are computed.
6. The vertical slice is rendered.

---

# 📖 Learning Objectives

This project focuses on:

- Raycasting mathematics
- DDA algorithm
- Texture mapping
- Event handling
- Parsing
- Memory management
- Graphics programming
- Game loops

---

# 📈 Performance

- Written entirely in C
- No game engine
- Lightweight rendering
- Efficient DDA traversal
- Constant memory usage

---

# 🎥 Inspiration

The project recreates the rendering technique used in **Wolfenstein 3D (1992)** by id Software.

---

# 📜 License

This project is developed for educational purposes as part of the 42 curriculum.
