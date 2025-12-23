// TODO (distribution):
// - Move runtime assets (levels, sounds, etc.) out of src/
// - Load assets relative to the executable (e.g. "levels/level1.txt")
// - Use CMake to automatically copy the levels/ folder next to the .exe
// - Ensure release builds ship with exe + levels folder together
// - Prepare asset layout for Steam upload (no absolute paths)

// =========================
// TODO (Audio System)
// =========================
// - Add centralized Audio system (SFML Audio)
// - Load sound effects from assets/audio/
// - Support one-shot SFX (jump, dash, attack, slam, hit, death)
// - Prevent sound spam (retrigger control / cooldowns)
// - Add master volume control
// - Add background music streaming (.ogg)
// - Prepare for settings menu (mute / volume sliders)
// - Ensure assets are loaded relative to the executable

// =========================
// TODO (Menus & End Screens)
// =========================
//
// MAIN MENU
// - Add main menu state (Title Screen)
// - Options:
//     * Start Game
//     * Level Select (later)
//     * Settings (audio / controls)
//     * Quit
// - Keyboard + mouse navigation
// - Neon / Geometry Dash–style visuals
//
// PAUSE MENU
// - Toggle pause menu (Esc)
// - Resume
// - Restart Level
// - Return to Main Menu
// - Pause gameplay updates while menu is active
//
// END / WIN SCREEN
// - End-goal triggers win state
// - Display "WIN" or victory message
// - Show time / deaths (later)
// - Input:
//     * Continue to next level
//     * Restart
//     * Return to menu
// - Neon glow + subtle animation
//
// GAME STATES
// - Implement GameState enum:
//     * Menu
//     * Playing
//     * Paused
//     * Win
//     * GameOver (optional)
// - Centralize state transitions
//
// POLISH (LATER)
// - Menu sound effects
// - Screen fade in/out between states
// - Controller support

// =========================
// TODO (Future Gameplay Systems)
// =========================
//
// ENEMIES / HAZARDS
// - Beam enemy:
//   * Large screen-spanning beam hazard
//   * Activates on a timer or trigger
//   * Damages player on contact
//   * Likely implemented as a hazard volume + visual effect
//   * Does NOT interact with platforms
//
// - Projectile enemy:
//   * Shoots small straight-line projectiles
//   * Projectiles only collide with the player
//   * No collision with platforms or other enemies
//   * Player can destroy projectiles via:
//       - Normal attack
//       - Slam
//   * If projectile hits player -> death / damage
//
// MOVEMENT FEEL
// - Split jump behavior:
//   * First jump: shorter / weaker
//   * Double jump: stronger upward dash-like impulse
//   * Double jump should feel explosive and directional
//   * Likely separate constants:
//       - FIRST_JUMP_VEL
//       - DOUBLE_JUMP_VEL
//
// GOAL / LEVEL PROGRESSION
// - End-goal entity ("Love Interest"):
//   * Pink square visual
//   * Static, non-hostile
//   * Touching it immediately advances to next level
//   * Placeholder for later boss-drop spawn
//   * Future:
//       - Boss death spawns this object
//       - Triggers level transition / cutscene
//
// LEVEL SYSTEM
// - Support multiple levels:
//   * level1.txt, level2.txt, etc.
//   * On goal touch -> load next level
//   * Reset player + enemies
//
// POLISH (LATER)
// - Beam VFX (glow, pulse, warning flash)
// - Projectile trails
// - Goal idle animation / glow
// - Screen flash on level completion
