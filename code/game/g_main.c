/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//

#include "g_local.h"

level_locals_t	level;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	    // track this variable, and announce if changed
  qboolean teamShader;        // track and if changed, update shader state
} cvarTable_t;

gentity_t		g_entities[MAX_GENTITIES];
gclient_t		g_clients[MAX_CLIENTS];

vmCvar_t	g_gametype;
vmCvar_t	g_dmflags;
vmCvar_t        g_videoflags;
vmCvar_t	g_elimflags;
vmCvar_t	g_voteflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_capturelimit;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_spectatorSpeed;
vmCvar_t	g_gravity;
vmCvar_t	g_gravityModifier;
vmCvar_t        g_damageModifier;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_quadfactor;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_respawntime;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_overrideWeaponRespawn;
vmCvar_t	g_weaponTeamRespawn;
vmCvar_t	g_motd;
vmCvar_t        g_motdfile;
vmCvar_t        g_votemaps;
vmCvar_t        g_recommendedMapsFile;
vmCvar_t        g_votecustom;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_logfile;
vmCvar_t	g_logfileSync;
vmCvar_t	g_blood;
vmCvar_t	g_podiumDist;
vmCvar_t	g_podiumDrop;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_smoothClients;
vmCvar_t	pmove_fixed;
vmCvar_t	pmove_msec;
vmCvar_t        pmove_float;
vmCvar_t	g_rankings;
vmCvar_t	g_listEntity;
vmCvar_t	g_obeliskHealth;
vmCvar_t	g_obeliskRegenPeriod;
vmCvar_t	g_obeliskRegenAmount;
vmCvar_t	g_obeliskRespawnDelay;
vmCvar_t	g_cubeTimeout;
#ifdef MISSIONPACK
vmCvar_t	g_redteam;
vmCvar_t	g_blueteam;
vmCvar_t	g_singlePlayer;
#endif
vmCvar_t	g_redclan;
vmCvar_t	g_blueclan;
vmCvar_t	g_enableDust;
vmCvar_t	g_enableBreath;
vmCvar_t	g_proxMineTimeout;
vmCvar_t	g_music;
vmCvar_t        g_spawnprotect;
//Following for elimination:
vmCvar_t	g_elimination_selfdamage;
vmCvar_t	g_elimination_startHealth;
vmCvar_t	g_elimination_startArmor;
vmCvar_t	g_elimination_bfg;
vmCvar_t	g_elimination_grapple;
vmCvar_t	g_elimination_roundtime;
vmCvar_t	g_elimination_warmup;
vmCvar_t	g_elimination_activewarmup;
vmCvar_t        g_elimination_allgametypes;
vmCvar_t	g_elimination_machinegun;
vmCvar_t	g_elimination_shotgun;
vmCvar_t	g_elimination_grenade;
vmCvar_t	g_elimination_rocket;
vmCvar_t	g_elimination_railgun;
vmCvar_t	g_elimination_lightning;
vmCvar_t	g_elimination_plasmagun;
vmCvar_t	g_elimination_chain;
vmCvar_t	g_elimination_mine;
vmCvar_t	g_elimination_nail;

vmCvar_t        g_elimination_lockspectator;

vmCvar_t	g_rockets;

//dmn_clowns suggestions (with my idea of implementing):
vmCvar_t	g_instantgib;
vmCvar_t	g_vampire;
vmCvar_t	g_vampireMaxHealth;

vmCvar_t	g_midAir;

//Regen
vmCvar_t	g_regen;
int	g_ffa_gt; //Are this a FFA gametype even if gametype is high?
vmCvar_t	g_lms_lives;
vmCvar_t	g_lms_mode;
vmCvar_t	g_elimination_ctf_oneway;
vmCvar_t        g_awardpushing; //The server can decide if players are awarded for pushing people in lave etc.
vmCvar_t        g_persistantpowerups; //Allow missionpack style persistant powerups?

vmCvar_t        g_catchup; //Favors the week players

vmCvar_t         g_autonextmap; //Autochange map
vmCvar_t         g_mappools; //mappools to be used for autochange

vmCvar_t        g_voteNames;
vmCvar_t        g_voteBan;
vmCvar_t        g_voteGametypes;
vmCvar_t        g_voteMinTimelimit;
vmCvar_t        g_voteMaxTimelimit;
vmCvar_t        g_voteMinFraglimit;
vmCvar_t        g_voteMaxFraglimit;
vmCvar_t        g_voteMinBots;
vmCvar_t        g_voteMaxBots;
vmCvar_t        g_maxvotes;

vmCvar_t        g_humanplayers;

//used for voIP
vmCvar_t         g_redTeamClientNumbers;
vmCvar_t         g_blueTeamClientNumbers;

//unlagged - server options
vmCvar_t	g_delagHitscan;
vmCvar_t	g_truePing;
vmCvar_t	sv_fps;
vmCvar_t        g_lagLightning; //Adds a little lag to the lightninggun to make it less powerfull
//unlagged - server options
vmCvar_t        g_teleMissiles;
vmCvar_t        g_pushGrenades;
vmCvar_t        g_newShotgun;
vmCvar_t        g_ratPhysics;
vmCvar_t        g_rampJump;
vmCvar_t        g_additiveJump;
vmCvar_t        g_lavaDamage;
vmCvar_t        g_slimeDamage;
vmCvar_t        g_allowTimenudge;
vmCvar_t        g_fastSwitch;
vmCvar_t        g_fastWeapons;
vmCvar_t        g_regularFootsteps;
vmCvar_t        g_passThroughInvisWalls;
vmCvar_t        g_ambientSound; 
vmCvar_t        g_rocketSpeed; 
vmCvar_t        g_maxExtrapolatedFrames; 
vmCvar_t        g_unlagMissileMaxLatency; 
vmCvar_t        g_unlagDebug; 
vmCvar_t        g_unlagMode; 
vmCvar_t        g_unlagLimitVariance;
vmCvar_t        g_unlagLimitVarianceMs;
vmCvar_t        g_unlagMissileDefaultNudge; 
vmCvar_t        g_unlagLaunchLagMode; 
vmCvar_t        g_unlagLatencyMode; 
vmCvar_t        g_unlagCorrectFrameOffset; 
vmCvar_t        g_unlagPrestep; 
vmCvar_t        g_unlagImmediateRun; 
vmCvar_t        g_unlagFlight; 

//vmCvar_t	g_tournamentMinSpawnDistance;
vmCvar_t	g_tournamentSpawnsystem;
vmCvar_t	g_ffaSpawnsystem;

vmCvar_t	g_enableGreenArmor;

vmCvar_t	g_readSpawnVarFiles;

vmCvar_t	g_damageThroughWalls;

vmCvar_t	g_pingEqualizer;

vmCvar_t        g_autoClans;

vmCvar_t        g_quadWhore;

vmCvar_t        g_killDropsFlag;

vmCvar_t        g_killSafety;

vmCvar_t        g_startWhenReady;
vmCvar_t        g_autoStartTime;
vmCvar_t        g_countDownHealthArmor;
vmCvar_t        g_powerupGlows;
vmCvar_t        g_screenShake;
vmCvar_t        g_allowBrightModels;
vmCvar_t        g_brightPlayerShells;
vmCvar_t        g_friendsWallHack;
vmCvar_t        g_friendsFlagIndicator;
vmCvar_t        g_specShowZoom;
vmCvar_t        g_itemPickup;
vmCvar_t        g_itemDrop;
vmCvar_t        g_usesRatVM;
vmCvar_t        g_usesRatEngine;
vmCvar_t        g_mixedMode;
vmCvar_t        g_broadcastClients;
vmCvar_t        g_useExtendedScores;
vmCvar_t        g_ratVmPredictMissiles;
vmCvar_t        g_ratVmMissileNudge;
vmCvar_t        g_ratFlags;
vmCvar_t        sv_allowDuplicateGuid;

vmCvar_t        g_botshandicapped;

// weapon config
vmCvar_t        g_mgDamage;
vmCvar_t        g_mgTeamDamage;
vmCvar_t        g_railgunDamage;
vmCvar_t        g_lgDamage;

vmCvar_t        g_teamslocked;
vmCvar_t        g_autoTeamsUnlock;
vmCvar_t        g_tourneylocked;
vmCvar_t        g_specMuted;
vmCvar_t        g_tournamentMuteSpec;

vmCvar_t        g_timeoutAllowed;
vmCvar_t        g_timeinAllowed;
vmCvar_t        g_timeoutTime;
vmCvar_t        g_timeoutOvertimeStep;

vmCvar_t        g_autoFollowKiller;
vmCvar_t        g_autoFollowSwitchTime;

vmCvar_t        g_shaderremap;
vmCvar_t        g_shaderremap_flag;
vmCvar_t        g_shaderremap_flagreset;
vmCvar_t        g_shaderremap_banner;
vmCvar_t        g_shaderremap_bannerreset;

//KK-OAX
vmCvar_t        g_sprees;
vmCvar_t        g_altExcellent;
vmCvar_t        g_spreeDiv;

//Command/Chat spamming/flooding
vmCvar_t        g_floodMaxDemerits;
vmCvar_t        g_floodMinTime;

//Admin
vmCvar_t        g_admin;
vmCvar_t        g_adminLog;
vmCvar_t        g_adminParseSay;
vmCvar_t        g_adminNameProtect;
vmCvar_t        g_adminTempBan;
vmCvar_t        g_adminMaxBan;
vmCvar_t        g_specChat;
vmCvar_t        g_publicAdminMessages;

vmCvar_t        g_maxWarnings;
vmCvar_t        g_warningExpire;

vmCvar_t        g_minNameChangePeriod;
vmCvar_t        g_maxNameChanges;

vmCvar_t        g_timestamp_startgame;

// bk001129 - made static to avoid aliasing
static cvarTable_t		gameCvarTable[] = {
	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ NULL, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ NULL, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
	{ &g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
        { &g_videoflags, "videoflags", "7", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
        { &g_elimflags, "elimflags", "0", CVAR_SERVERINFO, 0, qfalse  },
        { &g_voteflags, "voteflags", "0", CVAR_SERVERINFO, 0, qfalse  },
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE  },
	{ &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },

	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_doWarmup, "g_doWarmup", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_logfile, "g_log", "games.log", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logfileSync, "g_logsync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "320", 0, 0, qtrue  },
	{ &g_spectatorSpeed, "g_spectatorSpeed", "400", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_gravity, "g_gravity", "800", 0, 0, qtrue  },
	{ &g_gravityModifier, "g_gravityModifier", "1", 0, 0, qtrue  },
        { &g_damageModifier, "g_damageModifier", "0", 0, 0, qtrue },
	{ &g_knockback, "g_knockback", "1000", 0, 0, qtrue  },
	{ &g_quadfactor, "g_quadfactor", "3", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },
	{ &g_overrideWeaponRespawn, "g_overrideWeaponRespawn", "0", 0, 0, qtrue  },
	{ &g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", 0, 0, qtrue },
	{ &g_forcerespawn, "g_forcerespawn", "20", 0, 0, qtrue },
        { &g_respawntime, "g_respawntime", "0", CVAR_ARCHIVE, 0, qtrue },
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },
        { &g_motdfile, "g_motdfile", "motd.cfg", 0, 0, qfalse },
	{ &g_blood, "com_blood", "1", 0, 0, qfalse },

	{ &g_podiumDist, "g_podiumDist", "80", 0, 0, qfalse },
	{ &g_podiumDrop, "g_podiumDrop", "70", 0, 0, qfalse },

        //Votes start:
	{ &g_allowVote, "g_allowVote", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_maxvotes, "g_maxVotes", MAX_VOTE_COUNT, CVAR_ARCHIVE, 0, qfalse },
        { &g_voteNames, "g_voteNames", "/map_restart/nextmap/map/g_gametype/kick/clientkick/g_doWarmup/timelimit/fraglimit/shuffle/bots/botskill/", CVAR_ARCHIVE, 0, qfalse }, //clientkick g_doWarmup timelimit fraglimit
        { &g_voteBan, "g_voteBan", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_voteGametypes, "g_voteGametypes", "/0/1/3/4/5/6/7/8/9/10/11/12/", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMaxTimelimit, "g_voteMaxTimelimit", "1000", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMinTimelimit, "g_voteMinTimelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMaxFraglimit, "g_voteMaxFraglimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMinFraglimit, "g_voteMinFraglimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMaxBots, "g_voteMaxBots", "20", CVAR_ARCHIVE, 0, qfalse },
        { &g_voteMinBots, "g_voteMinBots", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_votemaps, "g_votemapsfile", "votemaps.cfg", 0, 0, qfalse },
        { &g_votecustom, "g_votecustomfile", "votecustom.cfg", 0, 0, qfalse },
        
        { &g_recommendedMapsFile, "g_recommendedMapsFile", "recommendedmaps.cfg", 0, 0, qfalse },

	{ &g_listEntity, "g_listEntity", "0", 0, 0, qfalse },

	{ &g_obeliskHealth, "g_obeliskHealth", "2500", 0, 0, qfalse },
	{ &g_obeliskRegenPeriod, "g_obeliskRegenPeriod", "1", 0, 0, qfalse },
	{ &g_obeliskRegenAmount, "g_obeliskRegenAmount", "15", 0, 0, qfalse },
	{ &g_obeliskRespawnDelay, "g_obeliskRespawnDelay", "10", 0, 0, qfalse },

	{ &g_cubeTimeout, "g_cubeTimeout", "30", 0, 0, qfalse },
        #ifdef MISSIONPACK
	{ &g_redteam, "g_redteam", "Stroggs", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue },
	{ &g_blueteam, "g_blueteam", "Pagans", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue  },
	{ &g_singlePlayer, "ui_singlePlayerActive", "", 0, 0, qfalse, qfalse  },
        #endif
	{ &g_redclan, "g_redclan", "rat", 0 , 0, qfalse, qtrue },
	{ &g_blueclan, "g_blueclan", "rat", 0, 0, qfalse, qtrue  },

	{ &g_enableDust, "g_enableDust", "0", 0, 0, qtrue, qfalse },
	{ &g_enableBreath, "g_enableBreath", "0", 0, 0, qtrue, qfalse },
	{ &g_proxMineTimeout, "g_proxMineTimeout", "20000", 0, 0, qfalse },

	{ &g_smoothClients, "g_smoothClients", "1", 0, 0, qfalse},
	{ &pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO | CVAR_ARCHIVE, 0, qfalse},
	{ &pmove_msec, "pmove_msec", "11", CVAR_SYSTEMINFO | CVAR_ARCHIVE, 0, qfalse},

        { &pmove_float, "pmove_float", "1", CVAR_SYSTEMINFO | CVAR_ARCHIVE, 0, qtrue},

//unlagged - server options
	{ &g_delagHitscan, "g_delagHitscan", "0", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qtrue },
	{ &g_truePing, "g_truePing", "0", CVAR_ARCHIVE, 0, qtrue },
	// it's CVAR_SYSTEMINFO so the client's sv_fps will be automagically set to its value
	{ &sv_fps, "sv_fps", "20", CVAR_SYSTEMINFO | CVAR_ARCHIVE, 0, qfalse },
        { &g_lagLightning, "g_lagLightning", "0", CVAR_ARCHIVE, 0, qtrue },
//unlagged - server options
        { &g_ambientSound, "g_ambientSound", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_rocketSpeed, "g_rocketSpeed", "1000", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qtrue },
	// TODO: CVAR_ARCHIVE
        { &g_maxExtrapolatedFrames, "g_maxExtrapolatedFrames", "2", 0 , 0, qfalse },
        { &g_unlagMissileMaxLatency, "g_unlagMissileMaxLatency", "200", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse },
        { &g_unlagDebug, "g_unlagDebug", "0", 0, 0, qfalse },
        { &g_unlagMode, "g_unlagMode", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagLimitVariance, "g_unlagLimitVariance", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagLimitVarianceMs, "g_unlagLimitVarianceMs", "25", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagMissileDefaultNudge, "g_unlagMissileDefaultNudge", "50", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagLaunchLagMode, "g_unlagLaunchLagMode", "2", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagLatencyMode, "g_unlagLatencyMode", "3", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagCorrectFrameOffset, "g_unlagCorrectFrameOffset", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagPrestep, "g_unlagPrestep", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagImmediateRun, "g_unlagImmediateRun", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_unlagFlight, "g_unlagFlight", "0", CVAR_ARCHIVE, 0, qfalse },

        //{ &g_tournamentMinSpawnDistance, "g_tournamentMinSpawnDistance", "900", CVAR_ARCHIVE, 0, qfalse },
        { &g_tournamentSpawnsystem, "g_tournamentSpawnsystem", "1", CVAR_ARCHIVE, 0, qfalse },

        { &g_ffaSpawnsystem, "g_ffaSpawnsystem", "1", CVAR_ARCHIVE, 0, qfalse },

        { &g_enableGreenArmor, "g_enableGreenArmor", "0", CVAR_ARCHIVE, 0, qfalse },

        { &g_readSpawnVarFiles, "g_readSpawnVarFiles", "0", CVAR_ARCHIVE, 0, qfalse },

        { &g_damageThroughWalls, "g_damageThroughWalls", "0", CVAR_ARCHIVE, 0, qtrue },

        { &g_pingEqualizer, "g_pingEqualizer", "0", CVAR_ARCHIVE, 0, qfalse },

        { &g_teleMissiles, "g_teleMissiles", "0", CVAR_ARCHIVE, 0, qtrue },
        { &g_pushGrenades, "g_pushGrenades", "0", CVAR_ARCHIVE, 0, qtrue },

        { &g_newShotgun, "g_newShotgun", "1", CVAR_ARCHIVE, 0, qtrue },

	{ &g_ratPhysics,   "g_ratPhysics", "0", CVAR_ARCHIVE, 0, qtrue },
	{ &g_rampJump,     "g_rampJump", "0", CVAR_ARCHIVE, 0, qtrue },
	{ &g_additiveJump,     "g_additiveJump", "0", CVAR_ARCHIVE, 0, qtrue },
	{ &g_lavaDamage,     "g_lavaDamage", "10", CVAR_ARCHIVE, 0, qfalse },
	{ &g_slimeDamage,     "g_slimeDamage", "4", CVAR_ARCHIVE, 0, qfalse },
	{ &g_fastSwitch,   "g_fastSwitch", "1", CVAR_ARCHIVE, 0, qtrue },
	{ &g_fastWeapons,  "g_fastWeapons", "1", CVAR_ARCHIVE, 0, qtrue },
	{ &g_regularFootsteps,  "g_regularFootsteps", "1", CVAR_ARCHIVE, 0, qfalse },
	// TODO: CVAR_ARCHIVE
	{ &g_passThroughInvisWalls,  "g_passThroughInvisWalls", "0", 0, 0, qtrue },
	{ &g_allowTimenudge,     "g_allowTimenudge", "1", CVAR_ARCHIVE, 0, qfalse },

        { &g_autoClans, "g_autoClans", "0", CVAR_ARCHIVE , 0, qfalse },

        { &g_quadWhore, "g_quadWhore", "0", CVAR_ARCHIVE , 0, qfalse },

        { &g_killDropsFlag, "g_killDropsFlag", "1", CVAR_ARCHIVE , 0, qtrue },

        { &g_killSafety, "g_killSafety", "500", CVAR_ARCHIVE , 0, qfalse },

        { &g_startWhenReady, "g_startWhenReady", "0", CVAR_ARCHIVE | CVAR_SERVERINFO, 0, qfalse },
        { &g_autoStartTime, "g_autoStartTime", "0", CVAR_ARCHIVE, 0, qfalse },

        { &g_countDownHealthArmor, "g_countDownHealthArmor", "1", CVAR_ARCHIVE , 0, qfalse },
	
        { &g_powerupGlows, "g_powerupGlows", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_screenShake, "g_screenShake", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_allowBrightModels, "g_allowBrightModels", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_brightPlayerShells, "g_brightPlayerShells", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_friendsWallHack, "g_friendsWallHack", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_friendsFlagIndicator, "g_friendsFlagIndicator", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_specShowZoom, "g_specShowZoom", "1", CVAR_ARCHIVE, 0, qfalse },

        { &g_itemPickup, "g_itemPickup", "0", CVAR_ARCHIVE , 0, qtrue },
        { &g_itemDrop, "g_itemDrop", "0", CVAR_ARCHIVE , 0, qtrue },
        { &g_usesRatVM, "g_usesRatVM", "0", 0, 0, qfalse },
        { &g_usesRatEngine, "g_usesRatEngine", "0", CVAR_ROM | CVAR_INIT, 0, qfalse },
        { &g_mixedMode, "g_mixedMode", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_broadcastClients, "g_broadcastClients", "0", 0, 0, qfalse },
        { &g_useExtendedScores, "g_useExtendedScores", "0", CVAR_ARCHIVE, 0, qfalse },
        { &g_ratVmPredictMissiles, "g_ratVmPredictMissiles", "1", CVAR_ARCHIVE, 0, qfalse },
        { &g_ratVmMissileNudge, "g_ratVmMissileNudge", "58", CVAR_SERVERINFO, 0, qfalse },
        { &g_ratFlags, "g_ratFlags", "0", CVAR_SERVERINFO, 0, qfalse },
        { &sv_allowDuplicateGuid, "sv_allowDuplicateGuid", "0", 0, 0, qfalse },

        { &g_botshandicapped, "g_botshandicapped", "1", CVAR_ARCHIVE, 0, qfalse },
// weapon config
	{ &g_mgDamage,			"g_mgDamage", "6", 0, 0, qtrue },
	{ &g_mgTeamDamage,		"g_mgTeamDamage", "5", 0, 0, qtrue },
	{ &g_railgunDamage,		"g_railgunDamage", "80", 0, 0, qtrue },
	{ &g_lgDamage, 			"g_lgDamage", "7", 0, 0, qtrue },

	{ &g_teamslocked, 		"g_teamslocked", "0", 0, 0, qfalse },
	{ &g_autoTeamsUnlock, 		"g_autoTeamsUnlock", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &g_tourneylocked, 		"g_tourneylocked", "0", 0, 0, qfalse },
	{ &g_specMuted, 		"g_specMuted", "0", 0, 0, qfalse },
	{ &g_tournamentMuteSpec,        "g_tournamentMuteSpec", "0", CVAR_ARCHIVE, 0, qtrue },

	{ &g_timeoutAllowed, 		"g_timeoutAllowed", "0", 0, 0, qfalse },
	{ &g_timeinAllowed, 		"g_timeinAllowed", "1", 0, 0, qfalse },
	{ &g_timeoutTime, 		"g_timeoutTime", "30", 0, 0, qfalse },
	{ &g_timeoutOvertimeStep,	"g_timeoutOvertimeStep", "30", 0, 0, qfalse },

	{ &g_autoFollowKiller,	"g_autoFollowKiller", "0", 0, 0, qfalse },
	{ &g_autoFollowSwitchTime,	"g_autoFollowSwitchTime", "60", 0, 0, qfalse },


	{ &g_shaderremap,		"g_shaderremap", "0", 0, 0, qfalse },
	{ &g_shaderremap_flag,          "g_shaderremap_flag", "1", 0, 0, qfalse },
	{ &g_shaderremap_flagreset,     "g_shaderremap_flagreset", "1", 0, 0, qfalse },
	{ &g_shaderremap_banner,        "g_shaderremap_banner", "1", 0, 0, qfalse },
	{ &g_shaderremap_bannerreset,   "g_shaderremap_bannerreset", "1", 0, 0, qfalse },

	{ &g_rankings, "g_rankings", "0", 0, 0, qfalse},
        { &g_music, "g_music", "", 0, 0, qfalse},
        { &g_spawnprotect, "g_spawnprotect", "500", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue},
	//Now for elimination stuff:
	{ &g_elimination_selfdamage, "elimination_selfdamage", "0", 0, 0, qtrue },
	{ &g_elimination_startHealth, "elimination_startHealth", "200", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_startArmor, "elimination_startArmor", "150", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_bfg, "elimination_bfg", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
        { &g_elimination_grapple, "elimination_grapple", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_roundtime, "elimination_roundtime", "120", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_warmup, "elimination_warmup", "7", CVAR_ARCHIVE | CVAR_NORESTART , 0, qtrue },
	{ &g_elimination_activewarmup, "elimination_activewarmup", "5", CVAR_ARCHIVE | CVAR_NORESTART , 0, qtrue },
        { &g_elimination_allgametypes, "g_elimination", "0", CVAR_LATCH | CVAR_NORESTART, 0, qfalse },

	{ &g_elimination_machinegun, "elimination_machinegun", "500", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_shotgun, "elimination_shotgun", "500", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_grenade, "elimination_grenade", "100", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_rocket, "elimination_rocket", "50", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_railgun, "elimination_railgun", "20", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_lightning, "elimination_lightning", "300", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_plasmagun, "elimination_plasmagun", "200", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_chain, "elimination_chain", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_mine, "elimination_mine", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_nail, "elimination_nail", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },

	{ &g_elimination_ctf_oneway, "elimination_ctf_oneway", "0", CVAR_ARCHIVE| CVAR_NORESTART, 0, qtrue },

        { &g_elimination_lockspectator, "elimination_lockspectator", "0", CVAR_NORESTART, 0, qtrue },
        
        { &g_awardpushing, "g_awardpushing", "1", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

        //g_persistantpowerups
        #ifdef MISSIONPACK
        { &g_persistantpowerups, "g_runes", "1", CVAR_LATCH, 0, qfalse },
        #else
        { &g_persistantpowerups, "g_runes", "0", CVAR_LATCH|CVAR_ARCHIVE, 0, qfalse },
        #endif


	//nexuiz style rocket arena
	{ &g_rockets, "g_rockets", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_NORESTART, 0, qfalse },

	//Instantgib and Vampire thingies
	{ &g_instantgib, "g_instantgib", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse },
	{ &g_vampire, "g_vampire", "0.0", CVAR_NORESTART, 0, qtrue },
	{ &g_midAir, "g_midAir", "0", CVAR_NORESTART | CVAR_ARCHIVE, 0, qtrue },
	{ &g_regen, "g_regen", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_vampireMaxHealth, "g_vampire_max_health", "500", CVAR_NORESTART, 0, qtrue },
	{ &g_lms_lives, "g_lms_lives", "1", CVAR_NORESTART, 0, qtrue },
	{ &g_lms_mode, "g_lms_mode", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

        { &g_catchup, "g_catchup", "0", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue},

        { &g_autonextmap, "g_autonextmap", "0", CVAR_ARCHIVE | CVAR_NORESTART, 0, qfalse},
        { &g_mappools, "g_mappools", "0\\maps_dm.cfg\\1\\maps_tourney.cfg\\3\\maps_tdm.cfg\\4\\maps_ctf.cfg\\5\\maps_oneflag.cfg\\6\\maps_obelisk.cfg\
\\7\\maps_harvester.cfg\\8\\maps_elimination.cfg\\9\\maps_ctf.cfg\\10\\maps_lms.cfg\\11\\maps_dd.cfg\\12\\maps_dom.cfg\\", CVAR_ARCHIVE | CVAR_NORESTART, 0, qfalse},
        { &g_humanplayers, "g_humanplayers", "0", CVAR_ROM | CVAR_NORESTART, 0, qfalse },
//used for voIP
        { &g_redTeamClientNumbers, "g_redTeamClientNumbers", "0",CVAR_ROM, 0, qfalse },
        { &g_blueTeamClientNumbers, "g_blueTeamClientNumbers", "0",CVAR_ROM, 0, qfalse },
        
        //KK-OAX
        { &g_sprees, "g_sprees", "sprees.dat", 0, 0, qfalse },
        { &g_altExcellent, "g_altExcellent", "0", CVAR_SERVERINFO, 0, qtrue}, 
        { &g_spreeDiv, "g_spreeDiv", "5", 0, 0, qfalse},
        
        //Used for command/chat flooding
        { &g_floodMaxDemerits, "g_floodMaxDemerits", "5000", CVAR_ARCHIVE, 0, qfalse  },
        { &g_floodMinTime, "g_floodMinTime", "2000", CVAR_ARCHIVE, 0, qfalse  },
        
        //Admin
        { &g_admin, "g_admin", "admin.dat", CVAR_ARCHIVE, 0, qfalse  },
        { &g_adminLog, "g_adminLog", "admin.log", CVAR_ARCHIVE, 0, qfalse  },
        { &g_adminParseSay, "g_adminParseSay", "1", CVAR_ARCHIVE, 0, qfalse  },
        { &g_adminNameProtect, "g_adminNameProtect", "1", CVAR_ARCHIVE, 0, qfalse  },
        { &g_adminTempBan, "g_adminTempBan", "2m", CVAR_ARCHIVE, 0, qfalse  },
        { &g_adminMaxBan, "g_adminMaxBan", "2w", CVAR_ARCHIVE, 0, qfalse  },
        
        { &g_specChat, "g_specChat", "1", CVAR_ARCHIVE, 0, qfalse  },
        { &g_publicAdminMessages, "g_publicAdminMessages", "1", CVAR_ARCHIVE, 0, qfalse  },
        
        { &g_maxWarnings, "g_maxWarnings", "3", CVAR_ARCHIVE, 0, qfalse },
	    { &g_warningExpire, "g_warningExpire", "3600", CVAR_ARCHIVE, 0, qfalse },
	    
	    { &g_minNameChangePeriod, "g_minNameChangePeriod", "10", 0, 0, qfalse},
        { &g_maxNameChanges, "g_maxNameChanges", "50", 0, 0, qfalse},

        { &g_timestamp_startgame, "g_timestamp", "0001-01-01 00:00:00", 0, 0, qfalse}
        
};

// bk001129 - made static to avoid aliasing
static int gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );


void G_InitGame( int levelTime, int randomSeed, int restart );
void G_RunFrame( int levelTime );
void G_ShutdownGame( int restart );
void CheckExitRules( void );


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (intptr_t)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 );
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0 );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		return ConsoleCommand();
	case BOTAI_START_FRAME:
		return BotAIStartFrame( arg0 );
	}

	return -1;
}


void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Printf( text );
}

void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Error( text );
}

/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=1, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}
        G_Printf ("%i teams with %i entities\n", c, c2);
}

void G_RemapTeamShaders( void ) {
	char string[1024];
	char mapname[MAX_QPATH];

	float f = level.time * 0.001;
	ClearRemaps();
#ifdef MISSIONPACK
	Com_sprintf( string, sizeof(string), "team_icon/%s_red", g_redteam.string );
	AddRemap("textures/ctf2/redteam01", string, f); 
	AddRemap("textures/ctf2/redteam02", string, f); 
	Com_sprintf( string, sizeof(string), "team_icon/%s_blue", g_blueteam.string );
	AddRemap("textures/ctf2/blueteam01", string, f); 
	AddRemap("textures/ctf2/blueteam02", string, f); 
#endif

	if (g_shaderremap.integer) {
		qboolean has_banner = qfalse;
		qboolean has_bannerq3 = qfalse;
		trap_Cvar_VariableStringBuffer( "mapname", mapname, sizeof( mapname ) );

		if (Q_stricmp(mapname, "oasago2") == 0
				|| Q_stricmp(mapname, "oasago2f3") == 0
				|| Q_stricmp(mapname, "oasago2_081") == 0
				|| Q_stricmp(mapname, "ps37ctf") == 0
				|| Q_stricmp(mapname, "ps37ctf2") == 0
				|| Q_stricmp(mapname, "ps37ctf-mmp") == 0
				|| Q_stricmp(mapname, "oa_ctf2") == 0
				|| Q_stricmp(mapname, "mlctf1beta") == 0
				|| Q_stricmp(mapname, "oa_ctf2old") == 0
				|| Q_stricmp(mapname, "czest3ctf") == 0
				|| Q_stricmp(mapname, "oa_minia") == 0
				|| Q_stricmp(mapname, "oasago1") == 0
				|| Q_stricmp(mapname, "oa_thor") == 0
				|| Q_stricmp(mapname, "wrackdm17") == 0
				) {
			has_banner = qtrue;
		}
		if (Q_stricmp(mapname, "13dream") == 0
				|| Q_stricmp(mapname, "ct3ctf2") == 0
				|| Q_stricmp(mapname, "geit3ctf1") == 0
				|| Q_stricmp(mapname, "hctf3") == 0
				|| Q_stricmp(mapname, "mkbase") == 0
				|| Q_stricmp(mapname, "q3ctfp13") == 0
				|| Q_stricmp(mapname, "q3ctfp22_final") == 0
				|| Q_stricmp(mapname, "q3wcp20") == 0
				|| Q_stricmp(mapname, "rooftopsctf") == 0
				|| Q_stricmp(mapname, "stchctf9a") == 0
				|| Q_stricmp(mapname, "woohaa") == 0) {
			has_bannerq3 = qtrue;
		}
		// RATOA
		if( g_redclan.string[0] ) {
			if (g_shaderremap_flag.integer && (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTF_ELIMINATION)) {
				Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_redflag", g_redclan.string );
				AddRemap("models/flags/r_flag", string, f); 
			}
			if (g_shaderremap_banner.integer) {
				if (has_banner) {
					Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_red_banner", g_redclan.string );
					if (Q_stricmp(mapname, "mlctf1beta") == 0) {
						AddRemap("textures/ctf2/red_banner02", string, f); 
					} else {
						AddRemap("textures/clown/red_banner", string, f); 
					}
				}
				if (has_bannerq3) {
					Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_red_bannerq3", g_redclan.string );
					AddRemap("textures/ctf/ctf_redflag", string, f); 
				}
			}
		}  else {
			if (g_shaderremap_flagreset.integer && (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTF_ELIMINATION)) {
				AddRemap("models/flags/r_flag", "models/flags/r_flag", f); 
			}
			if (g_shaderremap_bannerreset.integer) {
				if (has_banner) {
					if (Q_stricmp(mapname, "mlctf1beta") == 0) {
						AddRemap("textures/ctf2/red_banner02", "textures/ctf2/red_banner02", f); 
					} else {
						AddRemap("textures/clown/red_banner", "textures/clown/red_banner", f); 
					}
				}
				if (has_bannerq3) {
					AddRemap("textures/ctf/ctf_redflag", "textures/ctf_ctf_redflag", f); 
				}
			}
		}
		if( g_blueclan.string[0] ) {
			if (g_shaderremap_flag.integer && (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTF_ELIMINATION)) {
				Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_blueflag", g_blueclan.string );
				AddRemap("models/flags/b_flag", string, f); 
			}
			if (g_shaderremap_banner.integer) {
				if (has_banner) {
					Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_blue_banner", g_blueclan.string );
					if (Q_stricmp(mapname, "mlctf1beta") == 0) {
						AddRemap("textures/ctf2/blue_banner02", string, f); 
					} else {
						AddRemap("textures/clown/blue_banner", string, f); 
					}
				}
				if (has_bannerq3) {
					Com_sprintf( string, sizeof(string), "team_icon/ratoa/%s_blue_bannerq3", g_blueclan.string );
					AddRemap("textures/ctf/ctf_blueflag", string, f); 
				}
			}
		}  else {
			if (g_shaderremap_flagreset.integer && (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTF_ELIMINATION)) {
				AddRemap("models/flags/b_flag", "models/flags/b_flag", f); 
			}
			if (g_shaderremap_bannerreset.integer) {
				if (has_banner) {
					if (Q_stricmp(mapname, "mlctf1beta") == 0) {
						AddRemap("textures/ctf2/blue_banner02", "textures/ctf2/blue_banner02", f); 
					} else {
						AddRemap("textures/clown/blue_banner", "textures/clown/blue_banner", f); 
					}
				}
				if (has_bannerq3) {
					AddRemap("textures/ctf/ctf_blueflag", "textures/ctf/ctf_blueflag", f); 
				}
			}
		}
	}
	trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
}

void G_PingEqualizerReset(void) {
	fileHandle_t f;
	int len;
	if (!g_pingEqualizer.integer) {
		return;
	}
	if (level.warmupTime == 0 ){
		return;
	}
	Com_Printf("Resetting ping equalizer...\n");
	len = trap_FS_FOpenFile( "pingequalizer.log", &f, FS_WRITE );
	if (len < 0 ) {
		return;
	}
	trap_FS_Write( "\n", 1, f );
	trap_FS_FCloseFile( f );
	level.pingEqualized = qfalse;
	trap_SendServerCommand( -1, va("print \"^5Server: resetting ping equalizer...\n"));
}

void G_PingEqualizerWrite(void) {
	fileHandle_t f;
	int len;
	char *s;
	qboolean equalize = qfalse;
	gclient_t *c1 = NULL;
	gclient_t *c2 = NULL;
	if (!g_pingEqualizer.integer || g_gametype.integer != GT_TOURNAMENT) {
		return;
	}
	if (level.warmupTime == 0 // game already running
			|| level.numPlayingClients != 2 // not enough players
			|| level.pingEqualized // already equalized
			) {
		return;
	}
	c1 = &level.clients[level.sortedClients[0]];
	c2 = &level.clients[level.sortedClients[1]];
	if (!c1 || !c2) {
		return;
	}
	if (level.warmupTime > 0 
			&& level.time < level.warmupTime
			&& level.warmupTime - level.time <= 10000 ) {
		// warmup already running, equalize now!
		equalize = qtrue;
	} else if (level.warmupTime == -1
			&& c1->pers.enterTime + 5000 <= level.time
			&& c2->pers.enterTime + 5000 <= level.time
		  ) {
		// in \ready phase, both joined at least 5s ago
		equalize = qtrue;
	}
	if (equalize) {
		gclient_t *lower = NULL;
		int pingdiff = 0;

		level.pingEqualized = qtrue;

		if (g_entities[level.sortedClients[0]].r.svFlags & SVF_BOT
				|| g_entities[level.sortedClients[1]].r.svFlags & SVF_BOT) {
			return;
		}
		if (c1->pers.realPing > c2->pers.realPing) {
			pingdiff = c1->pers.realPing - c2->pers.realPing;
			lower = c2;
		} else if (c1->pers.realPing < c2->pers.realPing) {
			pingdiff = c2->pers.realPing - c1->pers.realPing;
			lower = c1;
		}
		if (!lower) {
			return;
		}
		Com_Printf("Updating ping equalizer...\n");
		len = trap_FS_FOpenFile( "pingequalizer.log", &f, FS_WRITE );
		if (len < 0 ) {
			return;
		}
		s = va("%s %i\n", lower->pers.ip, pingdiff);
		trap_FS_Write( s, strlen(s), f );
		trap_FS_FCloseFile( f );

		trap_SendServerCommand( -1, va("print \"^5Server: equalizing pings...\n"));
	}
}


void G_UpdateActionCamera(void) {
	int i;
	gclient_t *cl;
	gentity_t *ent;
	int clientNum = level.followauto;

	cl = &level.clients[ clientNum ];
	if ( cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR ) {
		if (cl->ps.powerups[PW_REDFLAG] || cl->ps.powerups[PW_BLUEFLAG]) {
			return;
		}
	}
	// if the a flag is taken, follow flag carrier
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t *cl2 = &level.clients[i];
		if ( cl2->pers.connected != CON_CONNECTED 
				|| cl2->sess.sessionTeam == TEAM_SPECTATOR )
			continue;
		if (cl2->ps.powerups[PW_REDFLAG] || cl2->ps.powerups[PW_BLUEFLAG]) {
			level.followauto = i;
			level.followautoTime = level.time;
			return;
		}
	}

	cl = &level.clients[ clientNum ];
	if ( cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR ) {
		ent = &g_entities[ clientNum ];
		if (g_autoFollowKiller.integer) {
			if (ent->health <= 0) {
				if (cl->lasthurt_client < level.maxclients) {
					level.followauto = cl->lasthurt_client;
					level.followautoTime = level.time;
					return;
				}
			}
		}
		if (level.followautoTime + g_autoFollowSwitchTime.integer * 1000 > level.time) {
			return;
		}
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t *cl2 = &level.clients[i];
		if ( cl2->pers.connected != CON_CONNECTED || cl2->sess.sessionTeam == TEAM_SPECTATOR )
			continue;
		if (i == clientNum) {
			continue;
		}
		ent = &g_entities[ i ];
		if (ent->health >= 0) {
			level.followauto = i;
			level.followautoTime = level.time;
			return;
		}
	}
}


/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;

		if (cv->teamShader) {
			remapped = qtrue;
		}
	}

	if (remapped) {
		G_RemapTeamShaders();
	}

	// check some things
	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
                G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	//set FFA status for high gametypes:
	if ( g_gametype.integer == GT_LMS ) {
		g_ffa_gt = 1;	//Last Man standig is a FFA gametype
	} else {
		g_ffa_gt = 0;	//If >GT_CTF use bases
	}

	level.warmupModificationCount = g_warmup.modificationCount;
}

void G_UpdateRatFlags( void ) {
	int rflags = 0;

	if (g_itemPickup.integer) {
		rflags |= RAT_EASYPICKUP;
	}

	if (g_powerupGlows.integer) {
		rflags |= RAT_POWERUPGLOWS;
	}

	if (g_screenShake.integer) {
		rflags |= RAT_SCREENSHAKE;
	}

	if (g_ratVmPredictMissiles.integer) {
		rflags |= RAT_PREDICTMISSILES;
	}

	if (g_fastSwitch.integer) {
		rflags |= RAT_FASTSWITCH;
	}

	if (g_fastWeapons.integer) {
		rflags |= RAT_FASTWEAPONS;
	}

	if (g_ratPhysics.integer == 1) {
		rflags |= RAT_RATPHYSICS;
	}

	if (g_rampJump.integer) {
		rflags |= RAT_RAMPJUMP;
	}


	if (g_allowBrightModels.integer) {
		rflags |= RAT_ALLOWBRIGHTSKINS;
	}

	if (g_friendsWallHack.integer) {
		rflags |= RAT_FRIENDSWALLHACK;
	}

	if (g_specShowZoom.integer) {
		rflags |= RAT_SPECSHOWZOOM;
	}

	if (g_brightPlayerShells.integer) {
		rflags |= RAT_BRIGHTSHELL;
	}

	if (g_newShotgun.integer) {
		rflags |= RAT_NEWSHOTGUN;
	}

	if (g_additiveJump.integer) {
		rflags |= RAT_ADDITIVEJUMP;
	}

	if (!g_allowTimenudge.integer) {
		rflags |= RAT_NOTIMENUDGE;
	}

	if (g_friendsFlagIndicator.integer) {
		rflags |= RAT_FLAGINDICATOR;
	}

	if (g_regularFootsteps.integer) {
		rflags |= RAT_REGULARFOOTSTEPS;
	}

	if (g_passThroughInvisWalls.integer) {
		rflags |= RAT_NOINVISWALLS;
	}

	// XXX --> also update code where this is called!

	trap_Cvar_Set("g_ratFlags",va("%i",rflags));
}

/*
=================
G_UpdateCvars
=================
*/
void G_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;
	qboolean updateRatFlags = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount ) {
				cv->modificationCount = cv->vmCvar->modificationCount;

				if ( cv->trackChange ) {
					trap_SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", 
						cv->cvarName, cv->vmCvar->string ) );
				}

                                if ( cv->vmCvar == &g_votecustom )
                                    VoteParseCustomVotes();

                                //Here comes the cvars that must trigger a map_restart
                                if (cv->vmCvar == &g_instantgib || cv->vmCvar == &g_rockets  ||  cv->vmCvar == &g_elimination_allgametypes) {
                                    trap_Cvar_Set("sv_dorestart","1");
                                }
                                
                                if ( cv->vmCvar == &g_voteNames ) {
                                    //Set vote flags
                                    int voteflags=0;
                                    if( allowedVote("map_restart") )
                                        voteflags|=VF_map_restart;

                                    if( allowedVote("map") )
                                        voteflags|=VF_map;

                                    if( allowedVote("clientkick") )
                                        voteflags|=VF_clientkick;

                                    if( allowedVote("shuffle") )
                                        voteflags|=VF_shuffle;

                                    if( allowedVote("nextmap") )
                                        voteflags|=VF_nextmap;

                                    if( allowedVote("g_gametype") )
                                        voteflags|=VF_g_gametype;
                                    
                                    if( allowedVote("g_doWarmup") )
                                        voteflags|=VF_g_doWarmup;

                                    if( allowedVote("timelimit") )
                                        voteflags|=VF_timelimit;

                                    if( allowedVote("fraglimit") )
                                        voteflags|=VF_fraglimit;

                                    if( allowedVote("custom") )
                                        voteflags|=VF_custom;

                                    trap_Cvar_Set("voteflags",va("%i",voteflags));
                                }
      
				if (cv->teamShader) {
					remapped = qtrue;
				}

				if (cv->vmCvar == &g_itemPickup 
						|| cv->vmCvar == &g_powerupGlows
						|| cv->vmCvar == &g_screenShake
						|| cv->vmCvar == &g_ratVmPredictMissiles
						|| cv->vmCvar == &g_fastSwitch
						|| cv->vmCvar == &g_fastWeapons
						|| cv->vmCvar == &g_ratPhysics
						|| cv->vmCvar == &g_rampJump
						|| cv->vmCvar == &g_allowBrightModels
						|| cv->vmCvar == &g_friendsWallHack
						|| cv->vmCvar == &g_specShowZoom
						|| cv->vmCvar == &g_brightPlayerShells
						|| cv->vmCvar == &g_newShotgun
						|| cv->vmCvar == &g_additiveJump
						|| cv->vmCvar == &g_friendsFlagIndicator
						|| cv->vmCvar == &g_regularFootsteps
						|| cv->vmCvar == &g_passThroughInvisWalls
						) {
					updateRatFlags = qtrue;
				}
			}
		}
	}

	if (remapped) {
		G_RemapTeamShaders();
	}

	if (updateRatFlags) {
		G_UpdateRatFlags();
	}
}

qboolean G_AutoStartReady( void ) {
	return	(g_autoStartTime.integer > 0 && g_autoStartTime.integer*1000 < (level.time - level.startTime));
}

/*
 Sets the cvar g_timestamp. Return 0 if success or !0 for errors.
 */
int G_UpdateTimestamp( void ) {
    int ret = 0;
    qtime_t timestamp;
    ret = trap_RealTime(&timestamp);
    trap_Cvar_Set("g_timestamp",va("%04i-%02i-%02i %02i:%02i:%02i",
    1900+timestamp.tm_year,1+timestamp.tm_mon, timestamp.tm_mday,
    timestamp.tm_hour,timestamp.tm_min,timestamp.tm_sec));

    return ret;
}

/*
============
G_InitGame

============
*/
void G_InitGame( int levelTime, int randomSeed, int restart ) {
	int					i;

        
        G_Printf ("------- Game Initialization -------\n");
        G_Printf ("gamename: %s\n", GAMEVERSION);
        G_Printf ("gamedate: %s\n", __DATE__);

	srand( randomSeed );

	G_RegisterCvars();

        G_UpdateTimestamp();
        
        //disable unwanted cvars
        if( g_gametype.integer == GT_SINGLE_PLAYER )
        {
            g_instantgib.integer = 0;
            g_rockets.integer = 0;
            g_vampire.value = 0.0f;
        }

	G_ProcessIPBans();
    
    //KK-OAX Changed to Tremulous's BG_InitMemory
	BG_InitMemory();

	// set some level globals
	memset( &level, 0, sizeof( level ) );
	
	level.time = levelTime;
	level.startTime = levelTime;

	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	if ( g_gametype.integer != GT_SINGLE_PLAYER && g_logfile.string[0] ) {
		if ( g_logfileSync.integer ) {
			trap_FS_FOpenFile( g_logfile.string, &level.logFile, FS_APPEND_SYNC );
		} else {
			trap_FS_FOpenFile( g_logfile.string, &level.logFile, FS_APPEND );
		}
		if ( !level.logFile ) {
			G_Printf( "WARNING: Couldn't open logfile: %s\n", g_logfile.string );
		} else {
			char	serverinfo[MAX_INFO_STRING];

			trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

			G_LogPrintf("------------------------------------------------------------\n" );
			G_LogPrintf("InitGame: %s\n", serverinfo );
                        G_LogPrintf("Info: ServerInfo length: %d of %d\n", strlen(serverinfo), MAX_INFO_STRING );
		}
	} else {
		G_Printf( "Not logging to disk.\n" );
	}

        //Parse the custom vote names:
        VoteParseCustomVotes();

	G_InitWorldSession();
    
    //KK-OAX Get Admin Configuration
    G_admin_readconfig( NULL, 0 );
	//Let's Load up any killing sprees/multikills
	G_ReadAltKillSettings( NULL, 0 );

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;

	// initialize all clients for this game
	level.maxclients = g_maxclients.integer;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}

	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

        for ( i=0 ; i<MAX_CLIENTS ; i++ ) {
                g_entities[i].classname = "clientslot";
        }
        
	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
		&level.clients[0].ps, sizeof( level.clients[0] ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	G_SetTeleporterDestinations();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	if( g_gametype.integer >= GT_TEAM && (g_ffa_gt!=1)) {
		G_CheckTeamItems();
	}

	SaveRegisteredItems();

	trap_Cvar_Set("g_usesRatEngine", va("%i", trap_Cvar_VariableIntegerValue( "sv_ratEngine" )));
        
        G_Printf ("-----------------------------------\n");

	if( g_gametype.integer == GT_SINGLE_PLAYER || trap_Cvar_VariableIntegerValue( "com_buildScript" ) ) {
		G_ModelIndex( SP_PODIUM_MODEL );
	}

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_InitBots( restart );
	}

	G_RemapTeamShaders();

	//elimination:
	level.roundNumber = 1;
	level.roundNumberStarted = 0;
	level.roundStartTime = level.time+g_elimination_warmup.integer*1000;
	level.roundRespawned = qfalse;
	level.eliminationSides = rand()%2; //0 or 1

	//Challenges:
	level.teamSize = 0;
	level.hadBots = qfalse;

	if(g_gametype.integer == GT_DOUBLE_D)
		Team_SpawnDoubleDominationPoints();

	if(g_gametype.integer == GT_DOMINATION ){
		level.dom_scoreGiven = 0;
		for(i=0;i<MAX_DOMINATION_POINTS;i++)
			level.pointStatusDom[i] = TEAM_NONE;
		level.domination_points_count = 0; //make sure its not too big
	}

        PlayerStoreInit();

        //Set vote flags
        {
            int voteflags=0;
            if( allowedVote("map_restart") )
                voteflags|=VF_map_restart;

            if( allowedVote("map") )
                voteflags|=VF_map;

            if( allowedVote("clientkick") )
                voteflags|=VF_clientkick;

            if( allowedVote("shuffle") )
                voteflags|=VF_shuffle;

            if( allowedVote("nextmap") )
                voteflags|=VF_nextmap;

            if( allowedVote("g_gametype") )
                voteflags|=VF_g_gametype;

            if( allowedVote("g_doWarmup") )
                voteflags|=VF_g_doWarmup;

            if( allowedVote("timelimit") )
                voteflags|=VF_timelimit;

            if( allowedVote("fraglimit") )
                voteflags|=VF_fraglimit;

            if( allowedVote("custom") )
                voteflags|=VF_custom;

            trap_Cvar_Set("voteflags",va("%i",voteflags));
        }


	G_PingEqualizerReset();

	if (g_teamslocked.integer > 0 ) {
		level.RedTeamLocked = qtrue;
		level.BlueTeamLocked = qtrue;
		level.FFALocked = qtrue;
		trap_Cvar_Set("g_teamslocked",va("%i", g_teamslocked.integer - 1));
	}
}



/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
        G_Printf ("==== ShutdownGame ====\n");

	if ( level.logFile ) {
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
                level.logFile = 0;
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();
	
	//KK-OAX Admin Cleanup
    G_admin_cleanup( );
    G_admin_namelog_cleanup( );

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}
}



//===================================================================

void QDECL Com_Error ( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	Q_vsnprintf (text, sizeof(text), error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

        G_Printf ("%s", text);
}

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/


/*
=============
AddTournamentPlayer

If there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	if (level.numNonSpectatorClients >= 2) {
		return;
	}

	// never change during intermission
	if ( level.intermissiontime ) {
		return;
	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD || 
			client->sess.spectatorClient < 0  ) {
			continue;
		}

		if ( client->sess.spectatorGroup == SPECTATORGROUP_AFK ||
				client->sess.spectatorGroup == SPECTATORGROUP_NOTREADY) {
			continue;
		}

		if(!nextInLine || client->sess.spectatorNum > nextInLine->sess.spectatorNum) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}

/*
=======================
AddTournamentQueue
	  	 
Add client to end of tournament queue
=======================
*/
void AddTournamentQueue(gclient_t *client)
{
    int index;
    gclient_t *curclient;
    for(index = 0; index < level.maxclients; index++)
    {
        curclient = &level.clients[index];
        if(curclient->pers.connected != CON_DISCONNECTED)
        {
            if(curclient == client)
		    curclient->sess.spectatorNum = 0;
            else if(curclient->sess.sessionTeam == TEAM_SPECTATOR)
		    curclient->sess.spectatorNum++;
        }
    }
}

/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
RemoveTournamentWinner
=======================
*/
void RemoveTournamentWinner( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[0];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
AdjustTournamentScores
=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	clientNum = level.sortedClients[0];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.wins++;
		ClientUserinfoChanged( clientNum );
	}

	clientNum = level.sortedClients[1];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.losses++;
		ClientUserinfoChanged( clientNum );
	}

}

/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}

	// afk spectators
	if ( ca->sess.spectatorGroup == SPECTATORGROUP_AFK ) {
		return 1;
	}
	if ( cb->sess.spectatorGroup == SPECTATORGROUP_AFK ) {
		return -1;
	}

	//// notready spectators
	//if ( ca->sess.spectatorGroup == SPECTATORGROUP_NOTREADY ) {
	//	return 1;
	//}
	//if ( cb->sess.spectatorGroup == SPECTATORGROUP_NOTREADY ) {
	//	return -1;
	//}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorNum > cb->sess.spectatorNum ) {
			return -1;
		}
		if ( ca->sess.spectatorNum < cb->sess.spectatorNum ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

        //In elimination and CTF elimination, sort dead players last
        //if((g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION)
        //        && level.roundNumber==level.roundNumberStarted && (ca->isEliminated != cb->isEliminated)) {
        //    if( ca->isEliminated )
        //        return 1;
        //    if( cb->isEliminated )
        //        return -1;
        //} // confusing

	// then sort by score
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	return 0;
}

/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void CalculateRanks( void ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
        int             humanplayers;
	gclient_t	*cl;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
        humanplayers = 0; // don't count bots
	for ( i = 0; i < TEAM_NUM_TEAMS; i++ ) {
		level.numteamVotingClients[i] = 0;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

                        //We just set humanplayers to 0 during intermission
                        if ( !level.intermissiontime && level.clients[i].pers.connected == CON_CONNECTED && !(g_entities[i].r.svFlags & SVF_BOT) ) {
                            humanplayers++;
                        }

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
                                level.numNonSpectatorClients++;
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						if ( level.clients[i].sess.sessionTeam == TEAM_RED )
							level.numteamVotingClients[0]++;
						else if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							level.numteamVotingClients[1]++;
					}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = 0;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}
	}

	// see if it is time to end the level
	CheckExitRules();

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) {
		SendScoreboardMessageToAllClients();
	}
        
        if(g_humanplayers.integer != humanplayers) //Presume all spectators are humans!
            trap_Cvar_Set( "g_humanplayers", va("%i", humanplayers) );
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			//DeathmatchScoreboardMessage( g_entities + i, (g_usesRatVM.integer > 0 || G_MixedClientHasRatVM( &level.clients[i])));
			DeathmatchScoreboardMessageAuto( g_entities + i);
			if (g_gametype.integer == GT_ELIMINATION ||
					g_gametype.integer == GT_CTF_ELIMINATION ||
					g_gametype.integer == GT_LMS) {
				EliminationMessage( g_entities + i );
			}
		}
	}
}

/*
========================
SendElimiantionMessageToAllClients

Used to send information important to Elimination
========================
*/
void SendEliminationMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			EliminationMessage( g_entities + i );
		}
	}
}

/*
========================
SendDDtimetakenMessageToAllClients

Do this if a team just started dominating.
========================
*/
void SendDDtimetakenMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DoubleDominationScoreTimeMessage( g_entities + i );
		}
	}
}

/*
========================
SendAttackingTeamMessageToAllClients

Used for CTF Elimination oneway
========================
*/
void SendAttackingTeamMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			AttackingTeamMessage( g_entities + i );
		}
	}
}

/*
========================
SendDominationPointsStatusMessageToAllClients

Used for Standard domination
========================
*/
void SendDominationPointsStatusMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DominationPointStatusMessage( g_entities + i );
		}
	}
}
/*
========================
SendYourTeamMessageToTeam

Tell all players on a given team who there allies are. Used for VoIP
========================
*/
void SendYourTeamMessageToTeam( team_t team ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED && level.clients[ i ].sess.sessionTeam == team ) {
			YourTeamMessage( g_entities + i );
		}
	}
}


/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}

	FindIntermissionPoint();
	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, ent->client->ps.origin );
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pm_type = PM_INTERMISSION;

	// clean up powerup info
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	ent->client->ps.eFlags = 0;
	ent->s.eFlags = 0;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;

	if ( level.intermissiontime ) {
		return;		// already active
	}

	// if in tournement mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		AdjustTournamentScores();
	}

	level.intermissiontime = level.time;
	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		if (client->health <= 0) {
			ClientRespawn(client);
		}
		MoveClientToIntermission( client );
	}
#ifdef MISSIONPACK
	if (g_singlePlayer.integer) {
		trap_Cvar_Set("ui_singlePlayerActive", "0");
		UpdateTournamentInfo();
	}
#else
	// if single player game
	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		UpdateTournamentInfo();
		SpawnModelsOnVictoryPads();
	}
#endif
	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();

}


/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar 

=============
*/
void ExitLevel (void) {
	int		i;
	gclient_t *cl;
	char nextmap[MAX_STRING_CHARS];
	char d1[MAX_STRING_CHARS];
        char	serverinfo[MAX_INFO_STRING];

	//bot interbreeding
	BotInterbreedEndMatch();

	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT  ) {
		if ( !level.restarted ) {
			RemoveTournamentLoser();
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			level.changemap = NULL;
			level.intermissiontime = 0;
		}
		return;	
	}

	trap_Cvar_VariableStringBuffer( "nextmap", nextmap, sizeof(nextmap) );
	trap_Cvar_VariableStringBuffer( "d1", d1, sizeof(d1) );
        
        trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

        //Here the game finds the nextmap if g_autonextmap is set
        if(g_autonextmap.integer ) {
            char filename[MAX_FILEPATH];
            fileHandle_t file,mapfile;
            //Look in g_mappools.string for the file to look for maps in
            Q_strncpyz(filename,Info_ValueForKey(g_mappools.string, va("%i",g_gametype.integer)),MAX_FILEPATH);
            //If we found a filename:
            if(filename[0]) {
                //Read the file:
                /*int len =*/ trap_FS_FOpenFile(filename, &file, FS_READ);
                if(!file)
                    trap_FS_FOpenFile(va("%s.org",filename), &file, FS_READ);
                if(file) {
                    char  buffer[4*1024]; // buffer to read file into
                    char mapnames[1024][20]; // Array of mapnames in the map pool
                    char *pointer;
                    int choice, count=0; //The random choice from mapnames and count of mapnames
                    int i;
                    memset(&buffer,0,sizeof(buffer));
                    trap_FS_Read(&buffer,sizeof(buffer),file);
                    pointer = buffer;
                    while ( qtrue ) {
                        Q_strncpyz(mapnames[count],COM_Parse( &pointer ),20);
                        if ( !mapnames[count][0] ) {
                            break;
                        }
                        G_Printf("Mapname in mappool: %s\n",mapnames[count]);
                        count++;
                    }
                    trap_FS_FCloseFile(file);
                    //It is possible that the maps in the file read are flawed, so we try up to ten times:
                    for(i=0;i<10;i++) {
                        choice = (count > 0)? rand()%count : 0;
                        if(Q_strequal(mapnames[choice],Info_ValueForKey(serverinfo,"mapname")))
                            continue;
                        //Now check that the map exists:
                        trap_FS_FOpenFile(va("maps/%s.bsp",mapnames[choice]),&mapfile,FS_READ);
                        if(mapfile) {
                            G_Printf("Picked map number %i - %s\n",choice,mapnames[choice]);
                            Q_strncpyz(nextmap,va("map %s",mapnames[choice]),sizeof(nextmap));
                            trap_Cvar_Set("nextmap",nextmap);
                            trap_FS_FCloseFile(mapfile);
                            break;
                        }
                    }
                }
            }
        }

	if( !Q_stricmp( nextmap, "map_restart 0" ) && Q_stricmp( d1, "" ) ) {
		trap_Cvar_Set( "nextmap", "vstr d2" );
		trap_SendConsoleCommand( EXEC_APPEND, "vstr d1\n" );
	} else {
		trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );
	}

	level.changemap = NULL;
	level.intermissiontime = 0;

	// reset all the scores so we don't enter the intermission again
	level.teamScores[TEAM_RED] = 0;
	level.teamScores[TEAM_BLUE] = 0;
	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.persistant[PERS_SCORE] = 0;
	}

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< g_maxclients.integer ; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}

/*
=================
G_LogPrintf

Print to the logfile with a time stamp if it is open
=================
*/
void QDECL G_LogPrintf( const char *fmt, ... ) {
	va_list		argptr;
	char		string[1024];
	int			min, tens, sec;

	sec = level.time / 1000;

	min = sec / 60;
	sec -= min * 60;
	tens = sec / 10;
	sec -= tens * 10;

	Com_sprintf( string, sizeof(string), "%3i:%i%i ", min, tens, sec );

	va_start( argptr, fmt );
	Q_vsnprintf(string + 7, sizeof(string) - 7, fmt, argptr);
	va_end( argptr );

	if ( g_dedicated.integer ) {
		G_Printf( "%s", string + 7 );
	}

	if ( !level.logFile ) {
		return;
	}

	trap_FS_Write( string, strlen( string ), level.logFile );
}

/*
================
LogExit

Append information about this game to the log file
================
*/
void LogExit( const char *string ) {
	int				i, numSorted;
	gclient_t		*cl;
#ifdef MISSIONPACK
	qboolean won = qtrue;
 #endif
	G_LogPrintf( "Exit: %s\n", string );

	level.intermissionQueued = level.time;

	// this will keep the clients from playing any voice sounds
	// that will get cut off when the queued intermission starts
	trap_SetConfigstring( CS_INTERMISSION, "1" );

	// don't send more than 32 scores (FIXME?)
	numSorted = level.numConnectedClients;
	if ( numSorted > 32 ) {
		numSorted = 32;
	}

	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		G_LogPrintf( "red:%i  blue:%i\n",
			level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE] );
	}

	for (i=0 ; i < numSorted ; i++) {
		int		ping;

		cl = &level.clients[level.sortedClients[i]];

		if ( cl->sess.sessionTeam == TEAM_SPECTATOR ) {
			continue;
		}
		if ( cl->pers.connected == CON_CONNECTING ) {
			continue;
		}

		ping = cl->ps.ping < 999 ? cl->ps.ping : 999;

		G_LogPrintf( "score: %i  ping: %i  client: %i %s\n", cl->ps.persistant[PERS_SCORE], ping, level.sortedClients[i],	cl->pers.netname );
#ifdef MISSIONPACK
		if (g_singlePlayer.integer && g_gametype.integer == GT_TOURNAMENT) {
			if (g_entities[cl - level.clients].r.svFlags & SVF_BOT && cl->ps.persistant[PERS_RANK] == 0) {
				won = qfalse;
			}
		}
#endif

	}

#ifdef MISSIONPACK
	if (g_singlePlayer.integer) {
		if (g_gametype.integer >= GT_CTF && g_ffa_gt==0) {
			won = level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE];
		}
		trap_SendConsoleCommand( EXEC_APPEND, (won) ? "spWin\n" : "spLose\n" );
	}
#endif


}


/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
wait 10 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady, playerCount;
	int			i;
	gclient_t	*cl;
	int			readyMask;

	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		return;
	}

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
        playerCount = 0;
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

                playerCount++;
		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// only test ready status when there are real players present
	if ( playerCount > 0 ) {
		// if nobody wants to go, clear timer
		if ( !ready ) {
			level.readyToExit = qfalse;
			return;
		}

		// if everyone wants to go, go now
		if ( !notReady ) {
			ExitLevel();
			return;
		}
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited ten seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + 10000 ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}
        
        //Sago: In Elimination and Oneway Flag Capture teams must win by two points.
        if ( g_gametype.integer == GT_ELIMINATION || 
                (g_gametype.integer == GT_CTF_ELIMINATION && g_elimination_ctf_oneway.integer)) {
            return (level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] /*|| 
                    level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE]+1 ||
                    level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE]-1*/);
        }
	
	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void CheckExitRules( void ) {
 	int			i;
	gclient_t	*cl;
	// if at the intermission, wait for all non-bots to
	// signal ready, then go to next level
	if ( level.intermissiontime ) {
		CheckIntermissionExit ();
		return;
	} else {
            //sago: Find the reason for this to be neccesary.
            for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
                }
                cl->ps.stats[STAT_CLIENTS_READY] = 0;
            }
        }

	if ( level.intermissionQueued ) {
#ifdef MISSIONPACK
		int time = (g_singlePlayer.integer) ? SP_INTERMISSION_DELAY_TIME : INTERMISSION_DELAY_TIME;
		if ( level.time - level.intermissionQueued >= time ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
#else
		if ( level.time - level.intermissionQueued >= INTERMISSION_DELAY_TIME ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
#endif
		return;
	}

	// check for sudden death
	if ( ScoreIsTied() ) {
		// always wait for sudden death
		return;
	}

	if ( g_timelimit.integer > 0 && !level.warmupTime ) {
		//if ( (level.time - level.startTime)/60000 >= g_timelimit.integer ) {
		if ( (level.time - level.startTime) >= g_timelimit.integer * 60000 + level.timeoutOvertime) {
			trap_SendServerCommand( -1, "print \"Timelimit hit.\n\"");
			LogExit( "Timelimit hit." );
			return;
		}
	}

	if (g_gametype.integer == GT_TOURNAMENT && level.tournamentForfeited) {
		LogExit("Match ended due to forfeit!\n");
		return;
	}

	if ( level.numPlayingClients < 2 ) {
		return;
	}

	if ( (g_gametype.integer < GT_CTF || g_ffa_gt>0 ) && g_fraglimit.integer ) {
		if ( level.teamScores[TEAM_RED] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the fraglimit.\n\"" );
			LogExit( "Fraglimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the fraglimit.\n\"" );
			LogExit( "Fraglimit hit." );
			return;
		}

		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( cl->sess.sessionTeam != TEAM_FREE ) {
				continue;
			}

			if ( cl->ps.persistant[PERS_SCORE] >= g_fraglimit.integer ) {
				LogExit( "Fraglimit hit." );
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " hit the fraglimit.\n\"",
					cl->pers.netname ) );
				return;
			}
		}
	}

	if ( (g_gametype.integer >= GT_CTF && g_ffa_gt<1) && g_capturelimit.integer ) {

		if ( level.teamScores[TEAM_RED] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}
	}
}

//LMS - Last man Stading functions:
void StartLMSRound(void) {
	int		countsLiving;
	countsLiving = TeamLivingCount( -1, TEAM_FREE );
	if(countsLiving<2) {
		trap_SendServerCommand( -1, "print \"Not enough players to start the round\n\"");
		level.roundNumberStarted = level.roundNumber-1;
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}

	//If we are enough to start a round:
	level.roundNumberStarted = level.roundNumber; //Set numbers

        G_LogPrintf( "LMS: %i %i %i: Round %i has started!\n", level.roundNumber, -1, 0, level.roundNumber );
        
	SendEliminationMessageToAllClients();
	EnableWeapons();
}

void PrintElimRoundStats(void) {
	gclient_t *client;
	int maxKills = -1;
	int maxKillsClient = -1;
	int maxDG = -1;
	int maxDGClient = -1;
	int minDT = INT_MAX;
	int minDTClient = -1;
	int i;

	if (!g_usesRatVM.integer) {
		return;
	}

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];

		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
			continue;
		}

		if (client->elimRoundKills > maxKills 
				|| (client->elimRoundKills == maxKills && maxKillsClient != -1 && client->elimRoundDmgDone > level.clients[maxKillsClient].elimRoundDmgDone)) {
			maxKills = client->elimRoundKills;
			maxKillsClient = i;
		}

		if (client->elimRoundDmgDone > maxDG) {
			maxDG = client->elimRoundDmgDone;
			maxDGClient = i;
		}

		if (client->elimRoundDmgTaken < minDT) {
			minDT = client->elimRoundDmgTaken;
			minDTClient = i;
		}

	}

	if (maxKillsClient != -1) {
		trap_SendServerCommand(-1, va("print \"Most kills: %s " S_COLOR_RED "%i\n\"", 
					level.clients[maxKillsClient].pers.netname,
					maxKills));
	}
	if (minDTClient != -1 && g_gametype.integer != GT_LMS) {
		trap_SendServerCommand(-1, va("print\"Least Damage Taken: %s " S_COLOR_RED "%i\n\"", 
					level.clients[minDTClient].pers.netname,
					minDT));
	}
	if (maxDGClient != -1) {
		trap_SendServerCommand(-1, va("print\"Most Damage Given: %s " S_COLOR_RED "%i\n\"", 
					level.clients[maxDGClient].pers.netname,
					maxDG));
	}
}

//the elimination start function
void StartEliminationRound(void) {
	int i;
	int		countsLiving[TEAM_NUM_TEAMS];
	countsLiving[TEAM_BLUE] = TeamLivingCount( -1, TEAM_BLUE );
	countsLiving[TEAM_RED] = TeamLivingCount( -1, TEAM_RED );
	if((countsLiving[TEAM_BLUE]==0) || (countsLiving[TEAM_RED]==0))
	{
		trap_SendServerCommand( -1, "print \"Not enough players to start the round\n\"");
		level.roundNumberStarted = level.roundNumber-1;
		level.roundRespawned = qfalse;
		//Remember that one of the teams is empty!
		level.roundRedPlayers = countsLiving[TEAM_RED];
		level.roundBluePlayers = countsLiving[TEAM_BLUE];
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}
	
	//If we are enough to start a round:
	level.roundNumberStarted = level.roundNumber; //Set numbers
	level.roundRedPlayers = countsLiving[TEAM_RED];
	level.roundBluePlayers = countsLiving[TEAM_BLUE];
	if(g_gametype.integer == GT_CTF_ELIMINATION) {
		Team_ReturnFlag( TEAM_RED );
		Team_ReturnFlag( TEAM_BLUE );
	}
        if(g_gametype.integer == GT_ELIMINATION) {
            G_LogPrintf( "ELIMINATION: %i %i %i: Round %i has started!\n", level.roundNumber, -1, 0, level.roundNumber );
        } else if(g_gametype.integer == GT_CTF_ELIMINATION) {
            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: Round %i has started!\n", level.roundNumber, -1, -1, 4, level.roundNumber );
        }
	SendEliminationMessageToAllClients();
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients(); //Ensure that evaryone know who should attack.
	EnableWeapons();
	G_SendTeamPlayerCounts();

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t *client = &level.clients[i];
		client->elimRoundDmgDone = 0;
		client->elimRoundDmgTaken = 0;
		client->elimRoundKills = 0;
	}
}

//things to do at end of round:
void EndEliminationRound(void)
{
	PrintElimRoundStats();
	DisableWeapons();
	level.roundNumber++;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
	SendEliminationMessageToAllClients();
        CalculateRanks();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
	G_SendTeamPlayerCounts();
}

//Things to do if we don't want to move the roundNumber
void RestartEliminationRound(void) {
	DisableWeapons();
	level.roundNumberStarted = level.roundNumber-1;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
        if(!level.intermissiontime)
            SendEliminationMessageToAllClients();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
}

//Things to do during match warmup
void WarmupEliminationRound(void) {
	EnableWeapons();
	level.roundNumberStarted = level.roundNumber-1;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
	SendEliminationMessageToAllClients();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
}

/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/

/*
CheckDoubleDomination
*/

void CheckDoubleDomination( void ) {
	if ( level.numPlayingClients < 1 ) {
		return;
	}

	if ( level.warmupTime != 0) {
            if( ((level.pointStatusA == TEAM_BLUE && level.pointStatusB == TEAM_BLUE) ||
                 (level.pointStatusA == TEAM_RED && level.pointStatusB == TEAM_RED)) &&
                    level.timeTaken + 10*1000 <= level.time ) {
                        Team_RemoveDoubleDominationPoints();
                        level.roundStartTime = level.time + 10*1000;
                        SendScoreboardMessageToAllClients();
            }
            return;
        }

	if(g_gametype.integer != GT_DOUBLE_D)
		return;

	//Don't score if we are in intermission. Both points might have been taken when we went into intermission
	if(level.intermissiontime)
		return;

	if(level.pointStatusA == TEAM_RED && level.pointStatusB == TEAM_RED && level.timeTaken + 10*1000 <= level.time) {
		//Red scores
		trap_SendServerCommand( -1, "print \"Red team scores!\n\"");
		AddTeamScore(level.intermission_origin,TEAM_RED,1);
                G_LogPrintf( "DD: %i %i %i: %s scores!\n", -1, TEAM_RED, 2, TeamName(TEAM_RED) );
		Team_ForceGesture(TEAM_RED);
		Team_DD_bonusAtPoints(TEAM_RED);
		Team_RemoveDoubleDominationPoints();
		//We start again in 10 seconds:
		level.roundStartTime = level.time + 10*1000;
		SendScoreboardMessageToAllClients();
		CalculateRanks();
	}

	if(level.pointStatusA == TEAM_BLUE && level.pointStatusB == TEAM_BLUE && level.timeTaken + 10*1000 <= level.time) {
		//Blue scores
		trap_SendServerCommand( -1, "print \"Blue team scores!\n\"");
		AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
                G_LogPrintf( "DD: %i %i %i: %s scores!\n", -1, TEAM_BLUE, 2, TeamName(TEAM_BLUE) );
		Team_ForceGesture(TEAM_BLUE);
		Team_DD_bonusAtPoints(TEAM_BLUE);
		Team_RemoveDoubleDominationPoints();
		//We start again in 10 seconds:
		level.roundStartTime = level.time + 10*1000;
		SendScoreboardMessageToAllClients();
		CalculateRanks();
	}

	if((level.pointStatusA == TEAM_NONE || level.pointStatusB == TEAM_NONE) && level.time>level.roundStartTime) {
		trap_SendServerCommand( -1, "print \"A new round has started\n\"");
		Team_SpawnDoubleDominationPoints();
		SendScoreboardMessageToAllClients();
	}
}

/*
CheckLMS
*/

void CheckLMS(void) {
	int mode;
	mode = g_lms_mode.integer;
	if ( level.numPlayingClients < 1 ) {
		return;
	}

	

	//We don't want to do anything in intermission
	if(level.intermissiontime) {
		if(level.roundRespawned) {
			RestartEliminationRound();
		}
		level.roundStartTime = level.time; //so that a player might join at any time to fix the bots+no humans+autojoin bug
		return;
	}

	if(g_gametype.integer == GT_LMS)
	{
		int		countsLiving[TEAM_NUM_TEAMS];
		//trap_SendServerCommand( -1, "print \"This is LMS!\n\"");
		countsLiving[TEAM_FREE] = TeamLivingCount( -1, TEAM_FREE );
		if(countsLiving[TEAM_FREE]==1 && level.roundNumber==level.roundNumberStarted)
		{
			if(mode <=1 )
			LMSpoint();
			trap_SendServerCommand( -1, "print \"We have a winner!\n\"");
			EndEliminationRound();
			Team_ForceGesture(TEAM_FREE);
		}

		if(countsLiving[TEAM_FREE]==0 && level.roundNumber==level.roundNumberStarted)
		{
			trap_SendServerCommand( -1, "print \"All death... how sad\n\"");
			EndEliminationRound();
		}

		if((g_elimination_roundtime.integer) && (level.roundNumber==level.roundNumberStarted)&&(g_lms_mode.integer == 1 || g_lms_mode.integer==3)&&(level.time>=level.roundStartTime+1000*g_elimination_roundtime.integer))
		{
			trap_SendServerCommand( -1, "print \"Time up - Overtime disabled\n\"");
			if(mode <=1 )
			LMSpoint();
			EndEliminationRound();
		}

		//This might be better placed another place:
		if(g_elimination_activewarmup.integer<2)
			g_elimination_activewarmup.integer=2; //We need at least 2 seconds to spawn all players
		if(g_elimination_activewarmup.integer >= g_elimination_warmup.integer) //This must not be true
			g_elimination_warmup.integer = g_elimination_activewarmup.integer+1; //Increase warmup

		//Force respawn
		if(level.roundNumber != level.roundNumberStarted && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer && !level.roundRespawned)
		{
			level.roundRespawned = qtrue;
			RespawnAll();
			DisableWeapons();
			SendEliminationMessageToAllClients();
		}

		if(level.time<=level.roundStartTime && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer)
		{
			RespawnDead();
			//DisableWeapons();
		}

		if(level.roundNumber == level.roundNumberStarted)
		{
			EnableWeapons();
		}

		if((level.roundNumber>level.roundNumberStarted)&&(level.time>=level.roundStartTime))
			StartLMSRound();
	
		if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime && level.numPlayingClients < 2)
		{
			RespawnDead(); //Allow player to run around anyway
			WarmupEliminationRound(); //Start over
			return;
		}

		if(level.warmupTime != 0) {
			if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
			{
				RespawnDead();
				WarmupEliminationRound();
			}
		}

	}
}

/*
=============
CheckElimination
=============
*/
void CheckElimination(void) {
	if ( level.numPlayingClients < 1 ) {
		if( (g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION) &&
			( level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime ))
			RestartEliminationRound(); //For spectators
		return;
	}	

	//We don't want to do anything in itnermission
	if(level.intermissiontime) {
		if(level.roundRespawned)
			RestartEliminationRound();
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}	

	if(g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION)
	{
		int		counts[TEAM_NUM_TEAMS];
		int		countsLiving[TEAM_NUM_TEAMS];
		int		countsHealth[TEAM_NUM_TEAMS];
		counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE, qtrue );
		counts[TEAM_RED] = TeamCount( -1, TEAM_RED, qtrue );

		countsLiving[TEAM_BLUE] = TeamLivingCount( -1, TEAM_BLUE );
		countsLiving[TEAM_RED] = TeamLivingCount( -1, TEAM_RED );

		countsHealth[TEAM_BLUE] = TeamHealthCount( -1, TEAM_BLUE );
		countsHealth[TEAM_RED] = TeamHealthCount( -1, TEAM_RED );

		if(level.roundBluePlayers != 0 && level.roundRedPlayers != 0) { //Cannot score if one of the team never got any living players
			if((countsLiving[TEAM_BLUE]==0)&&(level.roundNumber==level.roundNumberStarted))
			{
				//Blue team has been eliminated!
				trap_SendServerCommand( -1, "print \"Blue Team eliminated!\n\"");
				AddTeamScore(level.intermission_origin,TEAM_RED,1);
                                if(g_gametype.integer == GT_ELIMINATION) {
                                    G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i by eleminating the enemy team!\n", level.roundNumber, TEAM_RED, 1, TeamName(TEAM_RED), level.roundNumber );
                                } else {
                                    G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i by eleminating the enemy team!\n", level.roundNumber, -1, TEAM_RED, 6, TeamName(TEAM_RED), level.roundNumber );
                                }
				EndEliminationRound();
				Team_ForceGesture(TEAM_RED);
			}
			else if((countsLiving[TEAM_RED]==0)&&(level.roundNumber==level.roundNumberStarted))
			{
				//Red team eliminated!
				trap_SendServerCommand( -1, "print \"Red Team eliminated!\n\"");
				AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
                                if(g_gametype.integer == GT_ELIMINATION) {
                                    G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i by eleminating the enemy team!\n", level.roundNumber, TEAM_BLUE, 1, TeamName(TEAM_BLUE), level.roundNumber );
                                } else {
                                    G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i by eleminating the enemy team!\n", level.roundNumber, -1, TEAM_BLUE, 6, TeamName(TEAM_BLUE), level.roundNumber );
                                }
				EndEliminationRound();
				Team_ForceGesture(TEAM_BLUE);
			}
		}

		//Time up
		if((level.roundNumber==level.roundNumberStarted)&&(g_elimination_roundtime.integer)&&(level.time>=level.roundStartTime+1000*g_elimination_roundtime.integer))
		{
			trap_SendServerCommand( -1, "print \"No teams eliminated.\n\"");

			if(level.roundBluePlayers != 0 && level.roundRedPlayers != 0) {//We don't want to divide by zero. (should not be possible)
				if(g_gametype.integer == GT_CTF_ELIMINATION && g_elimination_ctf_oneway.integer) {
					//One way CTF, make defensice team the winner.
					if ( (level.eliminationSides+level.roundNumber)%2 == 0 ) { //Red was attacking
						trap_SendServerCommand( -1, "print \"Blue team defended the base\n\"");
						AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
                                                G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i by defending the flag!\n", level.roundNumber, -1, TEAM_BLUE, 5, TeamName(TEAM_BLUE), level.roundNumber );
					}
					else {
						trap_SendServerCommand( -1, "print \"Red team defended the base\n\"");
						AddTeamScore(level.intermission_origin,TEAM_RED,1);
                                                G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i by defending the flag!\n", level.roundNumber, -1, TEAM_RED, 5, TeamName(TEAM_RED), level.roundNumber );
					}
				}
				else if(((double)countsLiving[TEAM_RED])/((double)level.roundRedPlayers)>((double)countsLiving[TEAM_BLUE])/((double)level.roundBluePlayers))
				{
					//Red team has higher procentage survivors
					trap_SendServerCommand( -1, "print \"Red team has most survivers!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_RED,1);
                                        if(g_gametype.integer == GT_ELIMINATION) {
                                            G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i due to more survivors!\n", level.roundNumber, TEAM_RED, 2, TeamName(TEAM_RED), level.roundNumber );
                                        } else {
                                            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i due to more survivors!\n", level.roundNumber, -1, TEAM_RED, 7, TeamName(TEAM_RED), level.roundNumber );
                                        }
				}
				else if(((double)countsLiving[TEAM_RED])/((double)level.roundRedPlayers)<((double)countsLiving[TEAM_BLUE])/((double)level.roundBluePlayers))
				{
					//Blue team has higher procentage survivors
					trap_SendServerCommand( -1, "print \"Blue team has most survivers!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_BLUE,1);	
                                        if(g_gametype.integer == GT_ELIMINATION) {
                                            G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i due to more survivors!\n", level.roundNumber, TEAM_BLUE, 2, TeamName(TEAM_BLUE), level.roundNumber );
                                        } else {
                                            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i due to more survivors!\n", level.roundNumber, -1, TEAM_BLUE, 7, TeamName(TEAM_BLUE), level.roundNumber );
                                        }
				}
				else if(countsHealth[TEAM_RED]>countsHealth[TEAM_BLUE])
				{
					//Red team has more health
					trap_SendServerCommand( -1, "print \"Red team has more health left!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_RED,1);
                                        if(g_gametype.integer == GT_ELIMINATION) {
                                            G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i due to more health left!\n", level.roundNumber, TEAM_RED, 3, TeamName(TEAM_RED), level.roundNumber );
                                        } else {
                                            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i due to more health left!\n", level.roundNumber, -1, TEAM_RED, 8, TeamName(TEAM_RED), level.roundNumber );
                                        }
				}
				else if(countsHealth[TEAM_RED]<countsHealth[TEAM_BLUE])
				{
					//Blue team has more health
					trap_SendServerCommand( -1, "print \"Blue team has more health left!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
                                        if(g_gametype.integer == GT_ELIMINATION) {
                                            G_LogPrintf( "ELIMINATION: %i %i %i: %s wins round %i due to more health left!\n", level.roundNumber, TEAM_BLUE, 3, TeamName(TEAM_BLUE), level.roundNumber );
                                        } else {
                                            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: %s wins round %i due to more health left!\n", level.roundNumber, -1, TEAM_BLUE, 8, TeamName(TEAM_BLUE), level.roundNumber );
                                        }
				}
			}
                        //Draw
                        if(g_gametype.integer == GT_ELIMINATION) {
                            G_LogPrintf( "ELIMINATION: %i %i %i: Round %i ended in a draw!\n", level.roundNumber, -1, 4, level.roundNumber );
                        } else {
                            G_LogPrintf( "CTF_ELIMINATION: %i %i %i %i: Round %i ended in a draw!\n", level.roundNumber, -1, -1, 9, level.roundNumber );
                        }
			EndEliminationRound();
		}

		//This might be better placed another place:
		if(g_elimination_activewarmup.integer<1)
			g_elimination_activewarmup.integer=1; //We need at least 1 second to spawn all players
		if(g_elimination_activewarmup.integer >= g_elimination_warmup.integer) //This must not be true
			g_elimination_warmup.integer = g_elimination_activewarmup.integer+1; //Increase warmup

		//Force respawn
		if(level.roundNumber!=level.roundNumberStarted && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer && !level.roundRespawned)
		{
			level.roundRespawned = qtrue;
			RespawnAll();
			SendEliminationMessageToAllClients();
		}

		if(level.time<=level.roundStartTime && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer)
		{
			RespawnDead();
		}
			

		if((level.roundNumber>level.roundNumberStarted)&&(level.time>=level.roundStartTime))
			StartEliminationRound();
	
		if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
		if(counts[TEAM_BLUE]<1 || counts[TEAM_RED]<1)
		{
			RespawnDead(); //Allow players to run around anyway
			WarmupEliminationRound(); //Start over
			return;
		}

		if(level.warmupTime != 0) {
			if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
			{
				RespawnDead();
				WarmupEliminationRound();
			}
		}
	}
}

/*
=============
CheckDomination
=============
*/
void CheckDomination(void) {
	int i;
        int scoreFactor = 1;

	if ( (level.numPlayingClients < 1) || (g_gametype.integer != GT_DOMINATION) ) {
		return;
	}

	//Do nothing if warmup
	if(level.warmupTime != 0)
		return; 

	//Don't score if we are in intermission. Just plain stupid
	if(level.intermissiontime)
		return;

	//Sago: I use if instead of while, since if the server stops for more than 2 seconds people should not be allowed to score anyway
	if(level.domination_points_count>3)
            scoreFactor = 2; //score more slowly if there are many points
        if(level.time>=level.dom_scoreGiven*DOM_SECSPERPOINT*scoreFactor) {
		for(i=0;i<level.domination_points_count;i++) {
			if ( level.pointStatusDom[i] == TEAM_RED )
				AddTeamScore(level.intermission_origin,TEAM_RED,1);
			if ( level.pointStatusDom[i] == TEAM_BLUE )
				AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
                        G_LogPrintf( "DOM: %i %i %i %i: %s holds point %s for 1 point!\n",
                                    -1,i,1,level.pointStatusDom[i],
                                    TeamName(level.pointStatusDom[i]),level.domination_points_names[i]);
		}
		level.dom_scoreGiven++;
		while(level.time>level.dom_scoreGiven*DOM_SECSPERPOINT*scoreFactor)
			level.dom_scoreGiven++;
		CalculateRanks();
	}
}

/*
=============
CheckTournament

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournament( void ) {
	// check because we run 3 game frames before calling Connect and/or ClientBegin
	// for clients on a map_restart
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		if (!level.warmupTime && level.numPlayingClients < 2) {
			level.tournamentForfeited = qtrue;
			return;
		}

		// pull in a spectator if needed
		if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();
			if (level.pingEqualized) {
				G_PingEqualizerReset();
			}
		}

		// if we don't have two players, go back to "waiting for players"
		if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return;
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			if ( level.numPlayingClients == 2 ) {
				if ( ( g_startWhenReady.integer && 
				       ( g_entities[level.sortedClients[0]].client->ready || ( g_entities[level.sortedClients[0]].r.svFlags & SVF_BOT ) ) && 
				       ( g_entities[level.sortedClients[1]].client->ready || ( g_entities[level.sortedClients[1]].r.svFlags & SVF_BOT ) ) 
				      ) || !g_startWhenReady.integer || !g_doWarmup.integer || G_AutoStartReady()) {
					// fudge by -1 to account for extra delays
					if ( g_warmup.integer > 1 ) {
						level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
					} else {
						level.warmupTime = 0;
					}

					trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				}
			}
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	} else if ( g_gametype.integer != GT_SINGLE_PLAYER && level.warmupTime != 0 ) {
		int		counts[TEAM_NUM_TEAMS];
		qboolean	notEnough = qfalse;
		int i;
		int clientsReady = 0;
		int clientsReadyRed = 0;
		int clientsReadyBlue = 0;

		memset(counts, 0, sizeof(counts));
		if ( g_gametype.integer > GT_TEAM && !g_ffa_gt ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE, qtrue);
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED, qtrue);

			if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}

		if( g_startWhenReady.integer ){
			for( i = 0; i < level.numPlayingClients; i++ ){
				if( ( g_entities[level.sortedClients[i]].client->ready || g_entities[level.sortedClients[i]].r.svFlags & SVF_BOT ) && g_entities[level.sortedClients[i]].inuse ) {
					clientsReady++;
					switch (g_entities[level.sortedClients[i]].client->sess.sessionTeam) {
						case TEAM_RED:
							clientsReadyRed++;
							break;
						case TEAM_BLUE:
							clientsReadyBlue++;
							break;
						default:
							break;
					}
				}
			}
		}

		if ( g_doWarmup.integer && g_startWhenReady.integer == 1 
				&& ( clientsReady < level.numPlayingClients/2 + 1 )
				&& !G_AutoStartReady()) {
			notEnough = qtrue;
		} else if ( g_doWarmup.integer && g_startWhenReady.integer == 2 
				&& ( clientsReady < level.numPlayingClients )
				&& !G_AutoStartReady()) {
			notEnough = qtrue;
		} else if ( g_doWarmup.integer && g_startWhenReady.integer == 3 
				&& !G_AutoStartReady()) {
			if (g_gametype.integer >= GT_TEAM && !g_ffa_gt) {
				if ( clientsReadyRed < counts[TEAM_RED]/2 + 1  || 
						clientsReadyBlue < counts[TEAM_BLUE]/2 + 1) {
					notEnough = qtrue;
				}
			} else if (( clientsReady < level.numPlayingClients/2 + 1 )) {
				notEnough = qtrue;

			}
		}

		if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return; // still waiting for team members
		} 

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	}
}




/*
==================
PrintTeam
==================
*/
void PrintTeam(int team, char *message) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		trap_SendServerCommand( i, message );
	}
}

/*
==================
SetLeader
==================
*/
void SetLeader(int team, int client) {
	int i;

	if ( level.clients[client].pers.connected == CON_DISCONNECTED ) {
		PrintTeam(team, va("print \"%s is not connected\n\"", level.clients[client].pers.netname) );
		return;
	}
	if (level.clients[client].sess.sessionTeam != team) {
		PrintTeam(team, va("print \"%s is not on the team anymore\n\"", level.clients[client].pers.netname) );
		return;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader) {
			level.clients[i].sess.teamLeader = qfalse;
			ClientUserinfoChanged(i);
		}
	}
	level.clients[client].sess.teamLeader = qtrue;
	ClientUserinfoChanged( client );
	PrintTeam(team, va("print \"%s is the new team leader\n\"", level.clients[client].pers.netname) );
}

/*
==================
CheckTeamLeader
==================
*/
void CheckTeamLeader( int team ) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader)
			break;
	}
	if (i >= level.maxclients) {
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			if (!(g_entities[i].r.svFlags & SVF_BOT)) {
				level.clients[i].sess.teamLeader = qtrue;
				break;
			}
		}
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			level.clients[i].sess.teamLeader = qtrue;
			break;
		}
	}
}

/*
==================
CheckTeamVote
==================
*/
void CheckTeamVote( int team ) {
	int cs_offset;

	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !level.teamVoteTime[cs_offset] ) {
		return;
	}
	if ( level.time - level.teamVoteTime[cs_offset] >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
	} else {
		if ( level.teamVoteYes[cs_offset] > level.numteamVotingClients[cs_offset]/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Team vote passed.\n\"" );
			//
			if ( !Q_strncmp( "leader", level.teamVoteString[cs_offset], 6) ) {
				//set the team leader
				SetLeader(team, atoi(level.teamVoteString[cs_offset] + 7));
			}
			else {
				trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.teamVoteString[cs_offset] ) );
			}
		} else if ( level.teamVoteNo[cs_offset] >= level.numteamVotingClients[cs_offset]/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.teamVoteTime[cs_offset] = 0;
	trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset, "" );

}


/*
==================
CheckCvars
==================
*/
void CheckCvars( void ) {
	static int lastMod = -1;

	if ( g_password.modificationCount != lastMod ) {
		lastMod = g_password.modificationCount;
		if ( *g_password.string && Q_stricmp( g_password.string, "none" ) ) {
			trap_Cvar_Set( "g_needpass", "1" );
		} else {
			trap_Cvar_Set( "g_needpass", "0" );
		}
	}
}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}
	
	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}
	ent->think (ent);
}

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/
void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	int			msec;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.timeoutRealLevelTime = levelTime;
	

	if (level.timeout && levelTime >= level.timeoutEnd) {
		G_Timein();
	}

	if (!level.timeout)
		level.time = levelTime;

	msec = level.time - level.previousTime;

	// get any cvar changes
	G_UpdateCvars();

	G_UpdateRatFlags();

	G_UpdateActionCamera();

	if (level.timeout) {
		G_TimeinWarning(levelTime);
		return;
	}

        if( (g_gametype.integer==GT_ELIMINATION || g_gametype.integer==GT_CTF_ELIMINATION) && !(g_elimflags.integer & EF_NO_FREESPEC) && g_elimination_lockspectator.integer>1)
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer|EF_NO_FREESPEC));
        else
        if( (g_elimflags.integer & EF_NO_FREESPEC) && g_elimination_lockspectator.integer<2)
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer&(~EF_NO_FREESPEC) ) );

        if( g_elimination_ctf_oneway.integer && !(g_elimflags.integer & EF_ONEWAY) ) {
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer|EF_ONEWAY ) );
            //If the server admin has enabled it midgame imidiantly braodcast attacking team
            SendAttackingTeamMessageToAllClients();
        }
        else
        if( !g_elimination_ctf_oneway.integer && (g_elimflags.integer & EF_ONEWAY) ) {
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer&(~EF_ONEWAY) ) );
        }

	//
	// go through all allocated objects
	//
	//start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( ent->s.event ) {
				ent->s.event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ent->client->ps.externalEvent = 0;
					// predicted events should never be set to zero
					//ent->client->ps.events[0] = 0;
					//ent->client->ps.events[1] = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

//unlagged - backward reconciliation #2
		// we'll run missiles separately to save CPU in backward reconciliation
/*
		if ( ent->s.eType == ET_MISSILE ) {
			G_RunMissile( ent );
			continue;
		}
*/
//unlagged - backward reconciliation #2

		if ( ent->s.eType == ET_ITEM || ent->physicsObject ) {
			G_RunItem( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER ) {
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) {
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}

	for (i=0 ; i < level.num_entities ; ++i ) {
		ent = &g_entities[i];
		G_MissileRunPrestep(ent, msec);
	}

	if (g_unlagFlight.integer) {
		int projectileDelagTime = level.previousTime;
		if (level.previousTime > UNLAG_MAX_BACKTRACK
				&& msec > 0) {
			projectileDelagTime -= (UNLAG_MAX_BACKTRACK/msec) * msec;
		}
		while (projectileDelagTime <= level.previousTime) {
			int lag = level.previousTime - projectileDelagTime;

			G_TimeShiftAllClients( projectileDelagTime, NULL );
			ent = &g_entities[0];
			for (i=0 ; i<level.num_entities ; i++, ent++) {
				if ( !ent->inuse ) {
					continue;
				}

				// temporary entities don't think
				if ( ent->freeAfterEvent ) {
					continue;
				}

				if ( ent->s.eType == ET_MISSILE ) {
					if (ent->launchLag < lag || ent->launchLag >= lag + msec) {
						continue;
					}
					//G_TimeShiftAllClients( projectileDelagTime, ent->parent );
					if ( ent->parent && ent->parent->client && ent->parent->inuse && ent->parent->client->sess.sessionTeam < TEAM_SPECTATOR ) {
						G_UnTimeShiftClient( ent->parent );
					}

					//Com_Printf("delag running missile, level.time = %d, level.previousTime = %d, lag = %d\n", level.time, level.previousTime, lag);
					G_RunMissile( ent );

					if ( ent->parent && ent->parent->client && ent->parent->inuse && ent->parent->client->sess.sessionTeam < TEAM_SPECTATOR ) {
						G_TimeShiftClient( ent->parent, projectileDelagTime, qfalse, NULL );
					}
					//G_UnTimeShiftAllClients( ent->parent );
				}
			}

			G_UnTimeShiftAllClients( NULL );
			projectileDelagTime += msec;
			if (msec == 0) {
				break;
			}
		}
	} else {
		//unlagged - backward reconciliation #2
		// NOW run the missiles, with all players backward-reconciled
		// to the positions they were in exactly 50ms ago, at the end
		// of the last server frame
		G_TimeShiftAllClients( level.previousTime, NULL );

		ent = &g_entities[0];
		for (i=0 ; i<level.num_entities ; i++, ent++) {
			if ( !ent->inuse ) {
				continue;
			}

			// temporary entities don't think
			if ( ent->freeAfterEvent ) {
				continue;
			}

			if ( ent->s.eType == ET_MISSILE ) {
				G_RunMissile( ent );
			}
		}

		G_UnTimeShiftAllClients( NULL );
	}
//unlagged - backward reconciliation #2

//end = trap_Milliseconds();

//start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
//end = trap_Milliseconds();

	// see if it is time to do a tournement restart
	CheckTournament();

	//Check Elimination state
	CheckElimination();
	CheckLMS();

	//Check Double Domination
	CheckDoubleDomination();

	CheckDomination();

	//Sago: I just need to think why I placed this here... they should only spawn once
	if(g_gametype.integer == GT_DOMINATION)
		Team_Dom_SpawnPoints();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// cancel vote if timed out
	CheckVote();

	// check team votes
	CheckTeamVote( TEAM_RED );
	CheckTeamVote( TEAM_BLUE );

	// for tracking changes
	CheckCvars();

	if (g_listEntity.integer) {
		for (i = 0; i < MAX_GENTITIES; i++) {
			G_Printf("%4i: %s\n", i, g_entities[i].classname);
		}
		trap_Cvar_Set("g_listEntity", "0");
	}

	G_PingEqualizerWrite();

	G_CheckUnlockTeams();

//unlagged - backward reconciliation #4
	// record the time at the end of this frame - it should be about
	// the time the next frame begins - when the server starts
	// accepting commands from connected clients
	level.frameStartTime = trap_Milliseconds();
//unlagged - backward reconciliation #4
}


void G_SetRuleset(int ruleset) {
	if (ruleset == RULES_FAST) {
		trap_Cvar_Set("g_fastSwitch", "1");
		trap_Cvar_Set("g_fastWeapons", "1");
		trap_Cvar_Set("g_additiveJump", "1");
		trap_Cvar_Set("g_ratPhysics", "0");
		trap_Cvar_Set("g_rampJump", "0");
		trap_Cvar_Set("g_itemPickup", "1");
		trap_Cvar_Set("g_screenShake", "0");
		trap_Cvar_Set("g_teleMissiles", "1");
		trap_Cvar_Set("g_pushGrenades", "1");
	} else if (ruleset == RULES_SLOW) {
		trap_Cvar_Set("g_fastSwitch", "0");
		trap_Cvar_Set("g_fastWeapons", "1");
		trap_Cvar_Set("g_additiveJump", "0");
		trap_Cvar_Set("g_ratPhysics", "0");
		trap_Cvar_Set("g_rampJump", "0");
		trap_Cvar_Set("g_itemPickup", "1");
		trap_Cvar_Set("g_screenShake", "0");
		trap_Cvar_Set("g_teleMissiles", "1");
		trap_Cvar_Set("g_pushGrenades", "1");
	}
}

void G_LockTeams(void) {
	level.RedTeamLocked = qtrue;
	level.BlueTeamLocked = qtrue;
	level.FFALocked = qtrue;
	if (level.warmupTime != 0) {
		// during warmup, make sure teams stay locked when the game starts
		trap_Cvar_Set("g_teamslocked", "1");
	} else {
		// game already started, don't lock next game
		trap_Cvar_Set("g_teamslocked", "0");
	}
	trap_SendServerCommand( -1, va("print \"^5Server: teams locked!\n"));
}

void G_UnlockTeams(void) {
	level.RedTeamLocked = qfalse;
	level.BlueTeamLocked = qfalse;
	level.FFALocked = qfalse;
	trap_Cvar_Set("g_teamslocked", "0");
	trap_SendServerCommand( -1, va("print \"^5Server: teams unlocked!\n"));
}

void G_CheckUnlockTeams(void) {
	qboolean unlock = qfalse;

	if (!g_autoTeamsUnlock.integer) {
		return;
	}

	if (level.time - level.startTime < 15000) {
		return;
	}

	if (!(level.RedTeamLocked || level.BlueTeamLocked || level.FFALocked)) {
		return;
	}
	if (g_gametype.integer >= GT_TEAM && g_ffa_gt != 1) {
		if (G_CountHumanPlayers(TEAM_RED) == 0 
				|| G_CountHumanPlayers(TEAM_BLUE) == 0) {
			unlock = qtrue;
		}
	} else {
		if (G_CountHumanPlayers(TEAM_FREE) == 0) {
			unlock = qtrue;
		}
	}
	if (unlock) {
		trap_SendServerCommand( -1, va("print \"^5Server: unlocking teams due to lack of human players!\n"));
		G_UnlockTeams();
	}
}
