//////////////////////////////////////////////////////////////////
//							LOADOUTS							//
//////////////////////////////////////////////////////////////////
//Set to true to Spawn Players with custom loadouts, set false to spawn with Vanilla DayZ gear
m_CustomLoadouts		= true;

//TO USE YOU MUST SET --> 'm_RandomizedLoadouts2' to false!
//TO USE YOU MUST SET --> 'm_CustomLoadouts' to true! (picks random item to spawn on player from each catagory in 'LoaOuts\RandomlyGenerated')
m_StaticLoadouts		= true;

//TO USE YOU MUST SET --> 'm_RandomizedLoadouts2' to false!  
//TO USE YOU MUST SET --> 'm_CustomLoadouts' to true! (picks random item to spawn on player from each catagory in 'LoaOuts\RandomlyGenerated')
m_RandomizedLoadouts	= false;
 
//TO USE YOU MUST SET --> 'm_CustomLoadouts' to true! (picks random item to spawn on player from each catagory in 'LoaOuts\RandomlyGenerated')
//TO USE YOU MUST SET --> 'm_StaticLoadouts' to false!
//TO USE YOU MUST SET --> 'm_RandomizedLoadouts' to false!
m_RandomizedLoadouts2	= true; //loadouts called from an 

//Spawn fresh spawns with a pistol, weapon types can be changed in 'ScriptedMods\LoadOuts\wpnOnSpawn' All the way at the bottom in fucntion 'StartingEquipSetup'
m_SpawnArmed			= false; 

//////////////////////////////////////////////////////////////////
//							DEBUG MONITOR						//
//////////////////////////////////////////////////////////////////
m_debugmonitor			= true; //Enable Debug monitor for normal players

//////////////////////////////////////////////////////////////////
//							SAFEZONES							//
//////////////////////////////////////////////////////////////////
m_SafeZone				= true;  //set true to Enable Safe Zone

//////////////////////////////////////////////////////////////////
//							AIR DROPS							//
//////////////////////////////////////////////////////////////////
m_CustomBuildings		= true;  //Spawns the buildings you add in file BuildingSpawner.c

//////////////////////////////////////////////////////////////////
//							AIR DROPS							//
//////////////////////////////////////////////////////////////////
EnableAirdrops 			= true; //Airdrops randomly across map
<<<<<<< HEAD
SpawnZombie				= true; // Spawn zombie near airdrop when landed
=======
SpawnZombie 			= true; // Spawn zombie near airdrop when landed
>>>>>>> origin/master
ShowSignal 				= true; // Show smoke signal when airdrop landed

//////////////////////////////////////////////////////////////////
//							ZOMBIES								//
//////////////////////////////////////////////////////////////////
m_ZedHordes				= true;  //Random zombie hordes that spawn on the map (The tunables for this mod can be found in 'InfectedHordes.c')
m_walkingZeds			= false; //no running zombies - WIP keep false

<<<<<<< HEAD
//MOTD
m_MOTD					= true; //MOTD
line1					= "Welcome!"; //first line of text in MOTD
line2					= "custom server";//Second line of text in MOTD
line3					= "fallingsheep style";//Third line of text in MOTD

=======
//////////////////////////////////////////////////////////////////
//							PLAYER STATS						//
//////////////////////////////////////////////////////////////////
>>>>>>> origin/master

m_NoThirst				= false; //Set true to disable thirst
m_NoHunger				= false; //Set true to disable hunger
m_NoStamina				= false; //Set true to disable stamina

//////////////////////////////////////////////////////////////////
//							MESSAGES							//
//////////////////////////////////////////////////////////////////
m_SessionFeed			= true;  //Set to true to enable Disconnect/Connect Messages

//////////////////////////////////////////////////////////////////
//							MISC								//
//////////////////////////////////////////////////////////////////
m_LogInTimerLength		= 1;     //in seconds the spawn timer when players login!
m_Debugmode				= false; //Set true to disable db for faster server loading and for testing purposes
m_ProxyExportMode		= false; //Set to true to export a new loot positions xml (export can be found in your Storage folder).

/*Paths:: !!JUST DONT MESS AROUND WITH THESE UNLESS YOU KNOW WHAT YOU ARE DOING!!*/
m_RandomLoadoutsPath = "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\LoadOuts\\RandomlyGenerated\\";
m_AdminListPath      = "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\";