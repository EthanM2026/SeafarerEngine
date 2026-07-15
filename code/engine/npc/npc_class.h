#ifndef NPC_CLASS_H_INCLUDED
#define NPC_CLASS_H_INCLUDED


#define MAXIMUM_RADAR_TARGETS 32

#define JET_MODE_INTERCEPT 1
#define JET_MODE_ASM 2
#define JET_MODE_DOGFIGHT 3
#define JET_MODE_STRIKE 4
#define JET_MODE_SUPPRESS 5
#define JET_MODE_RECON 6
#define JET_MODE_ESCORT 7
#define JET_MODE_EM_WAR 8
#define JET_MODE_STEALTH 9
#define JET_MODE_SURVIVAL 10
#define JET_MODE_TRAVEL 11
#define JET_MODE_REST_AND_REFIT 12

#define CARRIER_MODE_FLIGHT_OPS 1
#define CARRIER_MODE_STRIKE 2
#define CARRIER_MODE_AIR_DEFENSE 3
#define CARRIER_MODE_COMMAND 4
#define CARRIER_MODE_POWER 5
#define CARRIER_MODE_ASSISTANCE 6
#define CARRIER_MODE_CONTROL 7
#define CARRIER_MODE_DEFENSE 8
#define CARRIER_MODE_REPLENISHMENT 9
#define CARRIER_MODE_DAMAGE_CONTROL 10
#define CARRIER_MODE_TRAVEL 11
#define CARRIER_MODE_REST_AND_REFIT 12

//1. Flight Operations Mode Think: "Mobile airport."
//2. Strike Mode Think: "Floating air force base."
//3. Air Defense Mode Think: "Guardian of the fleet."
//4. Fleet Command Mode "Floating Pentagon branch office."
//5. Power Projection Mode "The navy's diplomatic giant."
//6. Humanitarian Assistance Mode "Floating emergency response center."
//7. Sea Control Mode "King of the surrounding ocean."
//8. Self-Defense Mode "Survive first, fight second."
//9. Replenishment Mode "Pit stop for a 100,000-ton race car."
//10. Damage Control Mode "The ship heals itself."

#define BATTLESHIP_MODE_BOMBARD 1
#define BATTLESHIP_MODE_COMBAT 2
#define BATTLESHIP_MODE_AAD 3
#define BATTLESHIP_MODE_FORTRESS 4
#define BATTLESHIP_MODE_ESCORT 5
#define BATTLESHIP_MODE_COMMAND 6
#define BATTLESHIP_MODE_HK 7
#define BATTLESHIP_MODE_COASTAL_ASSAULT 8
#define BATTLESHIP_MODE_DAMAGE_CONTROL
#define BATTLESHIP_MODE_INTIMIDATION 10
#define BATTLESHIP_MODE_TRAVEL 11
#define BATTLESHIP_MODE_REST_AND_REFIT 12

//1. Bombardment Mode "Mobile artillery fortress."
//2. Surface Combat Mode "Duelist of the seas."
//3. Anti-Air Defense Mode "Steel porcupine."
//4. Fortress Mode "Floating castle."
//5. Fleet Escort Mode "Bodyguard of the fleet."
//6. Command Mode "Naval headquarters."
//7. Hunter-Killer Mode "Predator at sea."
//8. Coastal Assault Mode "The army's giant offshore artillery battery."
//9. Damage Control Mode "Keep the ship alive."
//10. Intimidation Mode "Gunboat diplomacy at 45,000 tons."

#define CRUISER_MODE_AIR_DEFENSE 1
#define CRUISER_MODE_SURFACE_WARFARE 2
#define CRUISER_MODE_ASM 3
#define CRUISER_MODE_COMMAND 4
#define CRUISER_MODE_STRIKE 5
#define CRUISER_MODE_ESCORT 6
#define CRUISER_MODE_RECON 7
#define CRUISER_MODE_EM_WAR 8
#define CRUISER_MODE_INDOP 9
#define CRUISER_MODE_DAMAGE_CONTROL 10
#define CRUISER_MODE_TRAVEL 11
#define CRUISER_MODE_REST_AND_REFIT 12

//1. Air Defense Mode Think: "Guardian umbrella."
//2. Surface Warfare Mode Think: "Missile duelist."
//3. Anti-Submarine Mode Think: "Ocean hunter."
//4. Command Mode Think: "Floating command center."
//5. Strike Mode Think: "Missile artillery platform."
//6. Escort Mode Think: "Bodyguard."
//7. Reconnaissance Mode Think: "Fleet scout."
//8. Electronic Warfare Mode Think: "Electronic ghost."
//9. Independent Operations Mode Think: "Lone ranger."
//10. Damage Control Mode Think: "Stay in the fight."

#define ESCORT_MODE_SCREEN 1
#define ESCORT_MODE_ANTIAIR 2
#define ESCORT_MODE_ASM 3
#define ESCORT_MODE_ASURF 4
#define ESCORT_MODE_INTERCEPT 5
#define ESCORT_MODE_PICKET 6
#define ESCORT_MODE_ESCORT 7
#define ESCORT_MODE_EM_WAR 8
#define ESCORT_MODE_RESCUE 9
#define ESCORT_MODE_SACRIFICE 10
#define ESCORT_MODE_TRAVEL 11
#define ESCORT_MODE_REST_AND_REFIT 12

//1. Screen Mode "Protective shield."
//2. Anti-Air Mode "Missile catcher."
//3. Anti-Submarine Mode "Submarine hunter."
//4. Anti-Surface Mode "Pack hunter."
//5. Intercept Mode "Fleet bouncer."
//6. Picket Mode "Lookout on the horizon."
//7. Convoy Escort Mode "Shepherd of the convoy."
//8. Electronic Warfare Mode "Master of deception."
//9. Rescue Mode "Guardian angel."
//10. Sacrifice Mode "Not on my watch."

#define TRANSPORT_MODE_CARGO 1
#define TRANSPORT_MODE_TRANSPORT 2
#define TRANSPORT_MODE_DEPLOY 3
#define TRANSPORT_MODE_AAM 4
#define TRANSPORT_MODE_AIRLIFT 5
#define TRANSPORT_MODE_REPLENISH 6
#define TRANSPORT_MODE_RELIEF 7
#define TRANSPORT_MODE_EVAC 8
#define TRANSPORT_MODE_MOBILE_BASE 9
#define TRANSPORT_MODE_SURVIVAL 10
#define TRANSPORT_MODE_TRAVEL 11
#define TRANSPORT_MODE_REST_AND_REFIT 12

//1. Cargo Mode"Floating warehouse."
//2. Troop Transport Mode "Sea-going barracks."
//3. Deployment Mode "Open the gates."
//4. Amphibious Assault Mode "Invasion support."
//5. Airlift Support Mode "Vertical logistics."
//6. Replenishment Mode "Fleet lifeline."
//7. Humanitarian Relief Mode "Disaster response hub."
//8. Evacuation Mode "Sea bridge to safety."
//9. Mobile Base Mode "Floating supply depot."
//10. Survival Mode "Protect the mission."

#define PATROL_MODE_PATROL 1
#define PATROL_MODE_INTERCEPT 2
#define PATROL_MODE_INSPECT 3
#define PATROL_MODE_SEARCH 4
#define PATROL_MODE_RESCUE 5
#define PATROL_MODE_SECURITY 6
#define PATROL_MODE_COMBAT 7
#define PATROL_MODE_STEALTH 8
#define PATROL_MODE_SMUGGLER_HUNTER 9
#define PATROL_MODE_EMERGENCY 10
#define PATROL_MODE_TRAVEL 11
#define PATROL_MODE_REST_AND_REFIT 12

//1. Patrol Mode "Eyes on the water."
//2. Intercept Mode "Catch them."
//3. Inspection Mode "Papers, please."
//4. Search Mode "Find the contact."
//5. Rescue Mode "Save lives first."
//6. Security Mode "Harbor guardian."
//7. Combat Mode "Fast attack craft."
//8. Stealth Observation Mode "Shadow on the horizon."
//9. Smuggler-Hunter Mode "Customs with an engine."
//10. Emergency Response Mode "911 on the water."

#define FAST_ATTACK_MODE_SPRINT 1
#define FAST_ATTACK_MODE_AMBUSH 2
#define FAST_ATTACK_MODE_STRIKE 3
#define FAST_ATTACK_MODE_SWARM 4
#define FAST_ATTACK_MODE_GUNRUN 5
#define FAST_ATTACK_MODE_HUNTER 6
#define FAST_ATTACK_MODE_EVASION 7
#define FAST_ATTACK_MODE_INTERDICT 8
#define FAST_ATTACK_MODE_COASTAL_DEFENSE 9
#define FAST_ATTACK_MODE_LAST_STRIKE 10
#define FAST_ATTACK_MODE_TRAVEL 11
#define FAST_ATTACK_MODE_REST_AND_REFIT 12

//1. Sprint Mode "Gone before you can react."
//2. Ambush Mode "Knife in the shadows."
//3. Strike Mode "Launch everything."
//4. Swarm Mode "Death by a thousand cuts."
//5. Gun Run Mode "Drive-by at sea."
//6. Hunter Mode "Predator."
//7. Evasion Mode "Don't get caught."
//8. Interdiction Mode "Fast maritime enforcement."
//9. Coastal Defense Mode "The coastline's hidden fangs."
//10. Last Strike Mode "One good hit is enough."

#define MINE_LAYER_MODE_HUNT 1
#define MINE_LAYER_MODE_CLEAR 2
#define MINE_LAYER_MODE_SWEEP 3
#define MINE_LAYER_MODE_SURVEY 4
#define MINE_LAYER_MODE_BARRIER 5
#define MINE_LAYER_MODE_CHANNEL 6
#define MINE_LAYER_MODE_ESCORT 7
#define MINE_LAYER_MODE_AUTOP 8
#define MINE_LAYER_MODE_HAZARD 9
#define MINE_LAYER_MODE_STEALTH 10
#define MINE_LAYER_MODE_TRAVEL 11
#define MINE_LAYER_MODE_REST_AND_REFIT 12

//1. Mine Hunting Mode "Underwater detective."
//2. Mine Clearance Mode "Bomb squad at sea."
//3. Mine Sweeping Mode "Erase the minefield."
//4. Survey Mode "Know the seabed."
//5. Barrier Deployment Mode "Build the trap."
//6. Channel Opening Mode "Open the gate."
//7. Escort Clearance Mode "Lead the way."
//8. Autonomous Operations Mode "Let the robots go first."
//9. Hazard Response Mode "Underwater EOD team."
//10. Stealth Mode "Don't wake the mine."

#define COMMAND_MODE_COMMAND 1
#define COMMAND_MODE_WIDER_STRATCOORD 2
#define COMMAND_MODE_IMM_FLEET_CONTROL 3
#define COMMAND_MODE_INTEL 4
#define COMMAND_MODE_COMMS 5
#define COMMAND_MODE_JOINTOP 6
#define COMMAND_MODE_CRISISRES 7
#define COMMAND_MODE_EM_WAR 8
#define COMMAND_MODE_FLAGSHIP 9
#define COMMAND_MODE_CONTINUITY 10
#define COMMAND_MODE_TRAVEL 11
#define COMMAND_MODE_REST_AND_REFIT 12

//1. Command Mode
//Full battle staff active.
//Coordinates fleet-wide operations.
//Maintains the common operational picture.

//Think: "The admiral's bridge."

//2. Strategic Coordination Mode
//Focuses on theater-wide planning.
//Integrates naval, air, land, cyber, and space assets.
//Long-term operational decisions prioritized.

//Think: "Mission control."

//3. Fleet Control Mode
//Direct management of naval formations.
//Assigns targets, patrol areas, and tasking.
//Tracks friendly and enemy movements in real time.

//Think: "Conductor of the orchestra."

//1. Command Mode "The admiral's bridge."
//2. Strategic Coordination Mode "Mission control."
//3. Fleet Control Mode "Conductor of the orchestra."
//4. Intelligence Mode "The fleet's brain."
//5. Communications Relay Mode "The network."
//6. Joint Operations Mode "Everyone on the same page."
//7. Crisis Response Mode "War room at sea."
//8. Electronic Warfare Coordination Mode "Master of the spectrum."
//9. Flagship Presence Mode "Floating embassy."
//10. Continuity Mode "The command center that survives."

#define SHIP_CARRIER 1 //Carry subfighters.
#define SHIP_BATTLESHIP 2 //Main, slow ships
#define SHIP_CRUISER 3 //Scouting, raiding and protection of supply lines
#define SHIP_ESCORT 4 //protect high-value target
#define SHIP_TRANSPORT 5 //Deployment and home.
#define SHIP_PATROL 6 //Does patrols. Night watch.
#define SHIP_FAST_ATTACK 7 //Attack hard and run fast
#define SHIP_MINE_WARFARE 8 //Lay mines.
#define SHIP_COMMAND_AND_SUPPORT 9



//Radar / Tracks
//Flight state (speed, altitude, etc.)
//Commands (goal heading, etc.)
//Fatigue level
//LSTM
//Steering / Throttle
#endif // NPC_CLASS_H_INCLUDED
