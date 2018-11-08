class DayZSurvival : MissionServer
{
	ref AirDrop AirDropClass; // Class definition
	bool m_StaticLoadouts;
	bool m_RandomizedLoadouts;
	bool m_RandomizedLoadouts2;
	bool m_CustomLoadouts;
	string m_RandomLoadoutsPath;
	string m_AdminListPath;
	bool m_SpawnArmed;
	bool m_StaminaStatus;
	bool m_SafeZone;
	bool m_CustomBuildings;
	bool m_SessionFeed;
	bool m_ZedHordes;
	bool m_ProxyExportMode;
	float m_LogInTimerLength;
	float TimerSlice; // Timeslice
	bool m_debugmonitor;
	bool m_Debugmode;
	bool m_NoThirst;	
	bool m_NoHunger;
	bool m_NoStamina;
	bool m_walkingZeds;
	bool EnableAirdrops;
	bool SpawnZombie;
    bool ShowSignal;
	
	// Called within class as extentions NOT class mainscope DO NOT DEFINE CLASS IN FILE! 
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ModSettings.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\BuildingSpawner.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\MOTDMessages.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\SafeZoneFunctions.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\AdminTool\\AdminTool.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\ExportFunctions.c"
	#include "$CurrentDir:\\mpmissions\\DayZSurvival.chernarusplus\\ScriptedMods\\LoadStaticLDFunctions.c"

	ref InfectedHordes ZmbEvents;
	
	//This is for the randomly generated loadouts type1
	ref TStringArray LoadoutCatagories = {"Bags","Gloves","Vests","Tops","Pants","Boots","HeadGear"}; //Add any new catagories here, make sure the name matches everywhere used including file
	ref TStringArray Bags = {};
	ref TStringArray Gloves = {};
	ref TStringArray Vests = {};
	ref TStringArray Tops = {};
	ref TStringArray Pants = {};
	ref TStringArray Boots = {};
	ref TStringArray HeadGear = {};
	ref map<string,string>	PoweredOptics = new map<string,string>; //Type of optics, type of battery
	
	//This is for the randomly generated loadouts type2
	//array of items to random add
	TStringArray tops = {"Shirt_BlueCheck","Shirt_RedCheck","Shirt_GreenCheck","Shirt_WhiteCheck","Shirt_PlaneBlack","HikingJacket_Blue","HikingJacket_Green","HikingJacket_Red","Sweater_Blue","Sweater_Gray","Sweater_Green","Sweater_Red","TShirt_Beige","TShirt_Black","TShirt_Blue","TShirt_Green","TShirt_Grey","TShirt_OrangeWhiteStripes","TShirt_Red","TShirt_RedBlackStripes","TShirt_White"};
	TStringArray pants = {"Jeans_Black","Jeans_BlueDark","Jeans_Blue","Jeans_Brown","Jeans_Green","Jeans_Grey"};
	TStringArray shoes = {"AthleticShoes_Black","AthleticShoes_Blue","AthleticShoes_Brown","AthleticShoes_Green","AthleticShoes_Grey","HikingBootsLow_Beige","HikingBootsLow_Black","HikingBootsLow_Blue","HikingBootsLow_Grey","HikingBoots_Black","HikingBoots_Brown","HikingJacket_Black"};
	TStringArray tool = {"OrienteeringCompass","StoneKnife","PurificationTablets","RoadFlare"};
	TStringArray medic = {"Rags","BandageDressing"};
	TStringArray drink = {"SodaCan_Cola","SodaCan_Kvass","SodaCan_Pipsi","SodaCan_Spite"};
	TStringArray food = {"Worm","SmallGuts","PowderedMilk","PeachesCan","Pear"};
	TStringArray radio = {"PersonalRadio"};
	TStringArray battery = {"Battery9V"};
	
	void DayZSurvival()
	{
		AirDropClass = new AirDrop;
	}
	
	void ~DayZSurvival()
	{
		
	}
	
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		// FPS Fix
		TimerSlice += timeslice;
		if (TimerSlice >= AirDropClass.TimesliceMultiplyier)
		{
			AirDropClass.CreateAirDrop();
			TimerSlice = 0;	
		}
	}
	
	override void OnPreloadEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int queueTime)
	{
		if (GetHive())
		{
			// Preload data on client by character from database
			useDB = true;
			queueTime = m_LogInTimerLength;
		}
		else
		{
			// Preload data on client without database
			useDB = false;
			pos = "7500 0 7500";
			yaw = 0;
			queueTime = m_LogInTimerLength;
		}
	}

	override void TickScheduler(float timeslice)
	{
		GetGame().GetWorld().GetPlayerList(m_Players);
		if( m_Players.Count() == 0 ) return;
		for(int i = 0; i < SCHEDULER_PLAYERS_PER_TICK; i++)
		{
			if(m_currentPlayer >= m_Players.Count() )
			{
				m_currentPlayer = 0;
			}

			PlayerBase currentPlayer = PlayerBase.Cast(m_Players.Get(m_currentPlayer));
			if (m_StaminaStatus) { currentPlayer.GetStatStamina().Set(1000); }
			if (m_SafeZone) { SafeZoneHandle(currentPlayer); } //Check if player is near safezone
			currentPlayer.OnTick();
			m_currentPlayer++;
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;

		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
		Class.CastTo(m_player, playerEnt);
		
		GetGame().SelectPlayer(identity, m_player);
		return m_player;
	}

	void ConstructLoadouts(bool update)
	{
		FileHandle currentFile;
		string line_content = "";

		if (update) {
			if (m_RandomizedLoadouts)
			{
				Bags.Clear();
				Gloves.Clear();
				Vests.Clear();
				Tops.Clear();
				Pants.Clear();
				Boots.Clear();
				HeadGear.Clear();
			}
		}

		if (m_RandomizedLoadouts)
		{
			//Load randomized ld items
			for ( int i = 0; i < LoadoutCatagories.Count(); ++i )
			{
				string currentCatagory = LoadoutCatagories.Get(i);
				currentFile = OpenFile(m_RandomLoadoutsPath + currentCatagory + ".txt", FileMode.READ);
				if (currentFile != 0)
				{
					line_content = "";
					while ( FGets(currentFile,line_content) > 0 )
					{
						switch(currentCatagory)
						{
							case "Bags":
							Bags.Insert(line_content);
							break;

							case "Gloves":
							Gloves.Insert(line_content);
							break;

							case "Vests":
							Vests.Insert(line_content);
							break;

							case "Tops":
							Tops.Insert(line_content);
							break;

							case "Pants":
							Pants.Insert(line_content);
							break;

							case "Boots":
							Boots.Insert(line_content);
							break;

							case "HeadGear":
							HeadGear.Insert(line_content);
							break;
						}
					}
					CloseFile(currentFile);
			    }
			}
		}
	}

	override void OnInit()
	{
		super.OnInit();

		if (!m_Debugmode)
		{
			Hive ce = CreateHive();
			if (ce)
			ce.InitOffline();
		}

		if (m_ProxyExportMode)
		{
			CETesting TestHive = GetTesting();
			TestHive.ExportProxyProto();
			TestHive.ExportProxyData( "7500 0 7500", 15000 );
		}

		//-----Add Admins from txt-----
		FileHandle AdminUIDSFile = OpenFile(m_AdminListPath + "Admins.txt", FileMode.READ);
		if (AdminUIDSFile != 0)
		{
			string line_content = "";
			while ( FGets(AdminUIDSFile,line_content) > 0 )
			{
				m_AdminList.Insert(line_content,"null"); //UID , NAME
				Print("Adding Admin: "+ line_content + " To the Admin List!");
			}
			CloseFile(AdminUIDSFile);
		}

		//-----------
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.PlayerCounter, 110000, true);  //Default 120000 2 mins Looped
		//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.CustomMOTD, TIME_INTERVAL, true);  //Default 120000 2 mins Looped
		//----------------------------------
		//Call for admin tool scripts
		AdminTool(); 

		// Debug monitor
		if(m_debugmonitor)
		{
			loadDebug();
		}
		if (m_SessionFeed) { g_Game.SetProfileString("SessionFeed", "true"); } else { g_Game.SetProfileString("SessionFeed", "false"); }
		if (m_CustomBuildings) { BuildingSpawner(); } //Spawn for custom buildings
		if (m_ZedHordes) { ZmbEvents = new InfectedHordes(); }
		ConstructLoadouts(false); //Read and construct loadouts Array from files.
		//----------------------------------

		PoweredOptics.Insert("m4_carryhandleoptic","");
		PoweredOptics.Insert("buisoptic","");
		PoweredOptics.Insert("M68Optic","Battery9V");
		PoweredOptics.Insert("m4_t3nrdsoptic","Battery9V");
		PoweredOptics.Insert("fnp45_mrdsoptic","Battery9V");
		PoweredOptics.Insert("crossbow_redpointoptic","Battery9V");
		PoweredOptics.Insert("reflexoptic","Battery9V");
		PoweredOptics.Insert("acogoptic","");
		PoweredOptics.Insert("puscopeoptic","");
		PoweredOptics.Insert("kashtanoptic","");
		PoweredOptics.Insert("huntingoptic","");
		PoweredOptics.Insert("pistoloptic","");
		PoweredOptics.Insert("pso1optic","");
		PoweredOptics.Insert("pso11optic","Battery9V");
		PoweredOptics.Insert("grozaoptic","");
		PoweredOptics.Insert("kobraoptic","Battery9V");
		//-----------------------------
	}

	void GlobalMessage(int Channel, string Message)
	{
		if (Message != "")
		{
			GetGame().ChatPlayer(Channel,Message);
		}
	}

	void PlayerCounter()
	{
		array<Man> players = new array<Man>;
	    GetGame().GetPlayers( players );
	    int numbOfplayers = players.Count();
	    GlobalMessage(1,"Online Players: "+ numbOfplayers.ToString());
	}
	
	void SpawnGunIn(PlayerBase player, string item, bool isMainGun, array<string> attachments, array<string> Extras)
	{
		EntityAI itemAI;

		EntityAI myAttachmentAI;
		EntityAI myAttachmentIB;

		EntityAI ExtraEntity;

		int MinQuantity;
		Magazine mag;

		if (isMainGun)
		{
			itemAI = player.GetHumanInventory().CreateInHands( item );

			player.SetQuickBarEntityShortcut(itemAI, 2, true); //Puts gun on hotkey 3

			if ( attachments != NULL )
			{
					
				for (int i = 0; i < attachments.Count(); ++i)
				{
					myAttachmentAI = itemAI.GetInventory().CreateInInventory( attachments.Get(i) );
					if (PoweredOptics.Contains(attachments.Get(i)))
					{
						myAttachmentAI.GetInventory().CreateInInventory( "Battery9V" );
					}
				}
			}
				
			if ( Extras != NULL )
			{
				for (int ii = 0; ii < Extras.Count(); ++ii)
				{
					if (GetGame().IsKindOf( Extras.Get(ii), "Magazine_Base") && ! (GetGame().IsKindOf( Extras.Get(ii), "Ammunition_Base")) )
					{
						mag = player.GetHumanInventory().CreateInInventory(Extras.Get(ii));
						MinQuantity = 2;
						if (mag)
						{
							mag.ServerSetAmmoCount(Math.RandomIntInclusive(MinQuantity,mag.GetAmmoMax()));
							player.SetQuickBarEntityShortcut(mag, 0, true);  //Puts main weapons mag on hotkey 1
						}
					}
					else
					{
						ExtraEntity = player.GetInventory().CreateInInventory( Extras.Get(ii) );
					}
				}
			}
		}
		else
		{
			//For Pistols/Secondary that spawn in inevntory
			if ( item != "" ) 
			{
				itemAI = player.GetHumanInventory().CreateInInventory( item );

				player.SetQuickBarEntityShortcut(itemAI, 3, true);  //Puts the Secondary weapon on hotkey 4
			
				if ( attachments != NULL && attachments.Count() > 0 )
				{
					for (int iz = 0; iz < attachments.Count(); ++iz)
					{
						myAttachmentIB = itemAI.GetInventory().CreateAttachment( attachments.Get(iz) );
						if (PoweredOptics.Contains(attachments.Get(iz)))
						{
							myAttachmentIB.GetInventory().CreateInInventory( "Battery9V" );
						}
					}
				}
				
				if ( Extras != NULL && Extras.Count() > 0 )
				{
					for (int ip = 0; ip < Extras.Count(); ++ip)
					{
						if (GetGame().IsKindOf( Extras.Get(ip), "Magazine_Base") && ! (GetGame().IsKindOf( Extras.Get(ip), "Ammunition_Base")) )
						{
							mag = player.GetInventory().CreateInInventory( Extras.Get(ip) );
							player.SetQuickBarEntityShortcut(mag, 1, true);   //Puts the mag for the secondary on hotkey 2

							MinQuantity = 2;
							mag.ServerSetAmmoCount(Math.RandomIntInclusive(MinQuantity,mag.GetAmmoMax()));
						}
						else
						{
							ExtraEntity = player.GetInventory().CreateInInventory( Extras.Get(ip) );
						}
					}
				}
			
			}
		}
		
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		ItemBase itemBs;
		EntityAI itemEnt;
		
		if (m_CustomLoadouts)
		{
			if (m_StaticLoadouts)
			{
				bool reqld = LoadRandomStaticLD(player);
			}
			else if (m_RandomizedLoadouts)
			{
				//safe guard
				m_RandomizedLoadouts2 = false;//turn off other load outs just in case
				
				player.RemoveAllItems();

				if (Bags.Count() > 0) { player.GetInventory().CreateInInventory( Bags.GetRandomElement() );  }
				if (Gloves.Count() > 0) { player.GetInventory().CreateInInventory( Gloves.GetRandomElement() ); }
				if (Vests.Count() > 0) { player.GetInventory().CreateInInventory( Vests.GetRandomElement() ); }
				if (Tops.Count() > 0) { player.GetInventory().CreateInInventory( Tops.GetRandomElement() ); }
				if (Pants.Count() > 0) { player.GetInventory().CreateInInventory( Pants.GetRandomElement() ); }
				if (Boots.Count() > 0) { player.GetInventory().CreateInInventory( Boots.GetRandomElement() ); }
				if (HeadGear.Count() > 0) { player.GetInventory().CreateInInventory( HeadGear.GetRandomElement() ); }

				player.GetInventory().CreateInInventory( "Battery9V" );
				
				itemEnt = player.GetInventory().CreateInInventory( "Rag" );
				itemBs = ItemBase.Cast(itemEnt);
				itemBs.SetQuantity(6);

				player.SetQuickBarEntityShortcut(itemBs, 0, true);
			}
			else
			{
				if (m_RandomizedLoadouts2)
				{
					//safe guard
					m_RandomizedLoadouts = false;//turn off other load outs just in case
				
					player.RemoveAllItems();       
					
					EntityAI itemEnt;
					EntityAI itemIn;
					ItemBase itemBs;
			
					//Add items
					EntityAI item = player.GetInventory().CreateInInventory(tops.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetHealth(itemEnt, 20);
					
					EntityAI item2 = player.GetInventory().CreateInInventory(pants.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetHealth(itemEnt, 20);
					
					EntityAI item3 = player.GetInventory().CreateInInventory(shoes.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetHealth(itemEnt, 20);
							
					itemEnt = player.GetInventory().CreateInInventory(tool.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
			
					itemEnt = player.GetInventory().CreateInInventory(medic.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
			
					itemEnt = player.GetInventory().CreateInInventory(drink.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
			
					itemEnt = player.GetInventory().CreateInInventory(food.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
					
					itemEnt = player.GetInventory().CreateInInventory(radio.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
					
					itemEnt = player.GetInventory().CreateInInventory(battery.GetRandomElement());
					itemBs = ItemBase.Cast(itemEnt);
					SetRandomHealth(itemEnt);
				} else {
					//Vanilla
					itemEnt = player.GetInventory().CreateInInventory( "Rag" );
					itemBs = ItemBase.Cast(itemEnt);							
					itemBs.SetQuantity(6);
				}
			}
		}
		else
		{
			//Vanilla
			itemEnt = player.GetInventory().CreateInInventory( "Rag" );
			itemBs = ItemBase.Cast(itemEnt);							
			itemBs.SetQuantity(6);
		}
			
		if (m_SpawnArmed)
		{
		    //Gun spawner Handle
			//SpawnGunIn( PlayerBase player, string ClassName, bool isPrimary, TstringArray Attachments, TstringArray Extras) NOTE: Set bool to 'true' IF weapon was primary
			int oRandValue = Math.RandomIntInclusive(0,2);
			switch(oRandValue.ToString())
			{
				case "0":
				SpawnGunIn( player , "fnx45", true, {"fnp45_mrdsoptic","PistolSuppressor"},{"mag_fnx45_15rnd","mag_fnx45_15rnd"} );
				break;

				case "1":
				SpawnGunIn( player , "CZ75", true, {"PistolSuppressor"} , {"Mag_CZ75_15Rnd","Mag_CZ75_15Rnd"} );
				break;

				case "2":
				SpawnGunIn( player , "makarovij70", true, {"PistolSuppressor"} , {"mag_ij70_8rnd","mag_ij70_8rnd"} );
				break;
			}
		}
	}

	void SetRandomHealth(EntityAI itemEnt)
	{
		int rndHlt = Math.RandomInt(40,100);
		itemEnt.SetHealth("","",rndHlt);
	}

	void SetHealth(EntityAI itemEnt, int health)
	{
	itemEnt.SetHealth("","",health);
	}
};