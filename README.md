# GunSurvivors - UE5.6 2D Top-Down Shooter

A simple top-down 2D shooter developed in **Unreal Engine 5.6** as a study project.  
The player controls a pawn and must shoot waves of incoming enemies, surviving as long as possible and earning points for every enemy defeated.

## 🎮 Gameplay Overview

- **Genre:** 2D Top-Down Shooter / Survival Arcade
- **Objective:** Survive endless waves of enemies and achieve the highest score.
- **Controls:** Keyboard and Mouse (customizable via Enhanced Input System).

## 🚀 Features

- **Flipbook Animation:**  
  - Creation and assignment of flipbooks to Actors.
  - Dynamic flipbook switching based on the current animation state (idle, walking, shooting, etc.).
- **Enemy Waves & Difficulty:**  
  - Enemies spawn in waves with increasing difficulty.
  - The longer the player survives, the faster enemies appear.
- **Enhanced Input System:**  
  - User input managed via Enhanced Input System.
  - Input Actions are bound programmatically in C++ for flexibility and scalability.
- **Minimal HUD:**  
  - Custom mouse cursor acts as a crosshair.
  - Real-time score counter displays points earned.
- **Expandable Architecture:**  
  - The project is structured to allow future additions or as a learning base for 2D shooter mechanics in Unreal.

## 🛠️ Skills Acquired

Throughout this project, I learned and implemented several core Unreal Engine concepts:

- **Flipbook creation and assignment** to actors using **Paper2D**.
- **Switching flipbooks** based on animation state changes (e.g., idle, run, die).
- **Spawning enemy waves** with incrementally increasing spawn rates for added challenge.
- **User input setup** with the **Enhanced Input System** and binding **InputActions** in C++ code.
- **Implementing a minimal HUD**, including a custom crosshair cursor and live score display.
