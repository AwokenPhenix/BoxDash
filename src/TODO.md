// TODO (distribution):
// - Move runtime assets (levels, sounds, etc.) out of src/
// - Load assets relative to the executable (e.g. "levels/level1.txt")
// - Use CMake to automatically copy the levels/ folder next to the .exe
// - Ensure release builds ship with exe + levels folder together
// - Prepare asset layout for Steam upload (no absolute paths)

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
