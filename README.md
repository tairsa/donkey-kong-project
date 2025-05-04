# Donkey Kong Console Game – C++ Course Project

This project was developed as part of the "C++ 2025A" course at MTA.  
It implements a Donkey Kong-style console game in C++, with progressively advanced features across multiple exercises.

## 🎮 Features
- Multi-screen support using `.screen` files
- Game state recording and replay via `.steps` and `.result` files
- Ghosts (including special ghosts that can climb ladders)
- Hammer mechanic to destroy ghosts/barrels
- Score tracking and life system
- Silent mode for automated testing (`-load -silent`)
- Console UI with movement and animation using `gotoxy`, `_kbhit`, and `Sleep`

## 🧾 How to Run
- **Normal mode**: `dkong.exe`
- **Save mode**: `dkong.exe -save`
- **Load replay**: `dkong.exe -load`
- **Silent test mode**: `dkong.exe -load -silent`
