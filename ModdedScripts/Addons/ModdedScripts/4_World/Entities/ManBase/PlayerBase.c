modded class PlayerBase
{
    ref ScoreTrackerHandle KillTracker;
	
	string KillLogFileLocation = "$profile:KillActivity.txt";
	
	string GetDate()
	{
		int year, month, day, hour, minute, second;
		
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);

		string date = "[" + "Hour: " + hour.ToStringLen(2) + " -- "+ "Minute: " + minute.ToStringLen(2) +"] " + " || " + "[Month: " +month.ToStringLen(2) + " -- " + "Day: " + day.ToStringLen(2) + "]";
		return date;
	}
	
	override void OnConnect()
    {
		super.OnConnect();
		
        ref SurvivorBase ChachedSB = this;
		ChachedSB.SetPlayerID(this.GetIdentity().GetPlainId());
		ChachedSB.SetPlayerName(this.GetIdentity().GetName());
		
		string strRet;
		g_Game.GetProfileString("SessionFeed", strRet);
		if (strRet == "true")
		{
			string Message = "Player " + ChachedSB.GetPlayerName() + " Joined!";
			GetGame().ChatPlayer(1,Message); //Global Chat
		}

        KillTracker = new ScoreTrackerHandle;
        KillTracker.PlayerConnectHandler(ChachedSB.GetPlayerID(), ChachedSB.GetPlayerName());
		//-----Kill Activiy shit-----
		if ( !FileExist(KillLogFileLocation) ) 
		{
			FileHandle KillLogs = OpenFile(KillLogFileLocation, FileMode.WRITE);
			FPrintln(KillLogs,"          -------------- KILL ACTIVITY LOGS v0.1 By DaOne --------------"); //Print Start Line
			FPrintln(KillLogs,"-------------- TIME CREATED: "+ GetDate() +"--------------"); //Print Start Line
			CloseFile(KillLogs);
		}
    }
	
	override void OnDisconnect()
	{
		super.OnDisconnect();
		
		ref SurvivorBase ChachedSB = this;
		string strRet;
		g_Game.GetProfileString("SessionFeed", strRet);
		if (strRet == "true")
		{
			string Message = "Player " + ChachedSB.GetPlayerName() + " Disconnected!";
			GetGame().ChatPlayer(1,Message); //Global Chat
		}
	}
	
    override void EEKilled( Object killer )
    {
		super.EEKilled(killer);
		
		FileHandle KillLogs;
		string TimeStamp;
        ref SurvivorBase SurvivorKilled = this;
        ref SurvivorBase SurvivorKiller = killer;
        ref Man KillerDude = killer;
		
		string KillerName = SurvivorKiller.GetPlayerName();
		string KilledName = SurvivorKilled.GetPlayerName();
		float distance;
		int Rounded;
		string Message;
		
		//-------------------KillTracker-------------------
        if (GetGame().IsServer()) {
			
		   if (KillerDude.IsMan() && SurvivorKiller.GetPlayerID() != SurvivorKilled.GetPlayerID()) 
		   {
			    KillTracker.KillHandler(SurvivorKiller.GetPlayerID());  //Record Killer kill
				
				distance = vector.Distance(SurvivorKilled.GetPosition(),SurvivorKiller.GetPosition());
				Rounded = Math.Round(distance);
				
				Message = KillerName + " Killed " + KilledName + " with " + "["+ SurvivorKiller.GetHumanInventory().GetEntityInHands().GetDisplayName() +"]" + " ("+ Rounded.ToString() + "m" +")";
				GetGame().ChatPlayer(1,Message); //Global Chat
				
				if ( FileExist(KillLogFileLocation) ) 
				{
					KillLogs = OpenFile(KillLogFileLocation, FileMode.APPEND);
					TimeStamp = GetDate();
					FPrintln(KillLogs,"\n");
					FPrintln(KillLogs,"-->TIME STAMP: "+ TimeStamp +" ===>: "+Message);
					FPrintln(KillLogs,"---------------------------------------------------------");
					CloseFile(KillLogs);
				}

		   }else if (KillerDude.IsMan() && SurvivorKiller.GetPlayerID() == SurvivorKilled.GetPlayerID())  //SUICIDE
		   {
				Message = "Player: " + KilledName + " Committed Suicide";
				string cmdLine;
				if (GetGame().CommandlineGetParam("SuicideMessage",cmdLine))
				{
					GetGame().ChatPlayer(1,Message); //Global Chat
				}
				
				if ( FileExist(KillLogFileLocation) ) 
				{
					KillLogs = OpenFile(KillLogFileLocation, FileMode.APPEND);
					TimeStamp = GetDate();
					FPrintln(KillLogs,"\n");
					FPrintln(KillLogs,"-->TIME STAMP: "+ TimeStamp +" ===>: "+Message);
					FPrintln(KillLogs,"---------------------------------------------------------");
					CloseFile(KillLogs);
				}
		   }
          
		  KillTracker.DeathHandler(SurvivorKilled.GetPlayerID()); //Record Player death
        }
    }
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos);
		string SafeZoneGmodeCheck;
	    ref SurvivorBase SBPlayer = this;

		string GUID = SBPlayer.GetPlayerID();
		g_Game.GetProfileString("SafeZoneStatus"+GUID,SafeZoneGmodeCheck);
				
		//TEMP
		if (SafeZoneGmodeCheck == "true") //SafeZone
		{
			if ( damageResult != null )
			{
				SetHealth( GetMaxHealth( "", "") );
				SetHealth( "","Blood", GetMaxHealth( "", "Blood" ) );
				SetHealth( "","Shock", GetMaxHealth( "","Shock") );
			}
		}
		else
		{
			if( damageResult != null && damageResult.GetDamage(dmgZone, "Shock") > 0)
			{
				m_LastShockHitTime = GetGame().GetTime();
			}
			
			//new bleeding computation
			//---------------------------------------
			if ( damageResult != null && GetBleedingManagerServer() )
			{
				float dmg = damageResult.GetDamage(dmgZone, "Blood");
				GetBleedingManagerServer().ProcessHit(dmg, component, dmgZone, ammo, modelPos);
			}
			//---------------------------------------
		}
	}
}