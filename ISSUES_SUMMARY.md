# GunSurvivors - Available Issues Summary

## Repository Overview
**GunSurvivors** is a 2D top-down shooter game developed in Unreal Engine 5.6. This document provides a comprehensive overview of all available issues in the repository.

## Open Issues Summary

### Total Issues: 6
- **Open Issues**: 5 (excluding the current WIP PR)
- **Milestones**: 2 active milestones
- **All issues are labeled as**: Enhancement

---

## Issue Details

### 🎯 Issue #1: Add Pause Screen
- **Status**: Open
- **Priority**: High (part of itch.io Release milestone)
- **Assignee**: Davhel98
- **Milestone**: itch.io Release
- **Description**: Add a Pause screen invokable by pressing Esc key with Resume and Quit Game buttons
- **Implementation Requirements**:
  - ESC key binding
  - UserWidget for pause screen
  - Resume functionality
  - Quit Game functionality

### 🎯 Issue #2: Add Second Enemy Type
- **Status**: Open
- **Priority**: High (part of itch.io Release milestone)
- **Assignee**: Davhel98
- **Milestone**: itch.io Release
- **Description**: Add a second enemy blueprint with different characteristics
- **Implementation Requirements**:
  - Different sprites/flipbooks
  - Lower movement speed
  - More shots required to kill
  - Spawn on higher difficulties

### 🎯 Issue #3: Add Command Scheme Overlay
- **Status**: Open
- **Priority**: High (part of itch.io Release milestone)
- **Assignee**: Davhel98
- **Milestone**: itch.io Release
- **Description**: Add the command scheme on a UserWidget
- **Implementation Requirements**:
  - UserWidget creation
  - Display key bindings
  - User-friendly interface

### 🎯 Issue #4: Add GunComponent
- **Status**: Open
- **Priority**: High (part of itch.io Release milestone)
- **Assignee**: Davhel98
- **Milestone**: itch.io Release
- **Description**: Add a GunComponent as a socket for equipped gun
- **Implementation Requirements**:
  - Shoot damage property
  - Shoot speed property
  - Ammo type property
  - Has infinite ammo property

### 📱 Issue #5: Android Build
- **Status**: Open
- **Priority**: Medium (part of Android release milestone)
- **Assignee**: Davhel98
- **Milestone**: Android release
- **Description**: Add touch controls support and test on Android devices
- **Implementation Requirements**:
  - Touch controls implementation
  - Android device testing
  - Mobile-friendly UI adjustments

---

## Milestones Overview

### 🚀 Milestone 1: itch.io Release
- **Status**: Open
- **Issues**: 4 open issues (#1, #2, #3, #4)
- **Description**: The minimum features the game must have to be published on itch.io
- **Priority**: High - Required for initial release

### 📱 Milestone 2: Android Release
- **Status**: Open
- **Issues**: 1 open issue (#5)
- **Description**: Features needed for Android platform release
- **Priority**: Medium - Future enhancement

---

## Technical Context

### Current Codebase Structure
- **Language**: C++ with Unreal Engine 5.6
- **Architecture**: Component-based with Actor classes
- **Key Classes**:
  - `TopDownCharacter` - Player character
  - `Enemy` - Base enemy class
  - `Bullet` - Projectile system
  - `EnemySpawner` - Enemy spawning system
  - `GunSurvivorGameMode` - Game mode management

### Input System
- Uses Enhanced Input System
- Keyboard and Mouse controls
- Programmatic input binding in C++

### Current Features
- Flipbook animations with Paper2D
- Enemy wave spawning with increasing difficulty
- Basic HUD with crosshair and score display
- Collision detection and bullet mechanics

---

## Recommendations for Issue Resolution

### Priority Order (Suggested)
1. **Issue #1 (Pause Screen)** - Essential for game usability
2. **Issue #4 (GunComponent)** - Core gameplay enhancement
3. **Issue #2 (Second Enemy Type)** - Gameplay variety
4. **Issue #3 (Command Scheme Overlay)** - User experience improvement
5. **Issue #5 (Android Build)** - Platform expansion

### Implementation Notes
- All issues are well-defined with clear requirements
- Issues #1-#4 are critical for the itch.io release
- Issue #5 can be tackled independently after core features
- Each issue has specific implementation requirements listed

---

## Next Steps
- Review and prioritize issues based on release timeline
- Consider breaking down complex issues into smaller tasks
- Ensure proper testing for each implemented feature
- Update milestone progress as issues are resolved