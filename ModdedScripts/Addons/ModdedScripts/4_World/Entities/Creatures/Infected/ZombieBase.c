modded class ZombieBase
{
	private bool m_EventZed;
	private PlayerBase lastHitSource;
	private int lootDropChance;
	bool m_walkingZeds;

	ref TStringArray m_PossibleLootDrops,m_PossibleWeaponDrops;

	//-------------------------------------------------------------
	void AttachEventHandle(TStringArray PossibleLootDrops,TStringArray PossibleWeaponDrops, int dropChance)
	{
		m_EventZed = true;
		m_PossibleLootDrops   = PossibleLootDrops;
		m_PossibleWeaponDrops = PossibleWeaponDrops;
		lootDropChance        = dropChance;
	}
	//-------------------------------------------------------------
	//						HandleDeath
	//-------------------------------------------------------------
	override bool HandleDeath(int pCurrentCommandID)
	{
		if( pCurrentCommandID == DayZInfectedConstants.COMMANDID_DEATH )
		{
			return true;
		}

		if( !IsAlive() )
		{
			if (m_EventZed)
			{
				StartCommand_Death();
				AfterDeath();
			}
			else
			{
				StartCommand_Death();
			}
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------
	//						Cleanup
	//-------------------------------------------------------------

	void AfterDeath()
	{
		//baseAttachments
		//Spawn a random loot on death
		if (lootDropChance > 0)
		{
			int RandomLootType = Math.RandomIntInclusive(0,1);
			EntityAI ItemAI;
			ItemBase ItemBS

			if (RandomLootType == 0)
			{
				ItemAI = GetGame().CreateObject( m_PossibleLootDrops.GetRandomElement(), this.GetPosition(), false, true );
				ItemBS = ItemBase.Cast(ItemAI);
			}
			else
			{
				string ClassName = m_PossibleWeaponDrops.GetRandomElement();
				ItemAI = GetGame().CreateObject( ClassName, this.GetPosition(), false, true );
				ItemBS = ItemBase.Cast(ItemAI);

				ref array<string> basAtts = new array<string>;
				GetGame().ConfigGetTextArray("cfgWeapons" + " " + ClassName + " baseAttachments", basAtts);

				for (int i = 0; i < basAtts.Count(); ++i)
				{
					ItemAI.GetInventory().CreateInInventory(basAtts.Get(i));
				}
			}
			Param1<string> Msgparam;
			Msgparam = new Param1<string>( "The Zombie Has Dropped Some loot!" );
			GetGame().RPCSingleParam(lastHitSource, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, lastHitSource.GetIdentity());
		}
	}
	//-------------------------------------------------------------
	//						Mindstate
	//-------------------------------------------------------------
	Override bool HandleMindStateChange(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		DayZInfectedCommandMove moveCommand = GetCommand_Move();
		if( moveCommand && moveCommand.IsTurning() )
			return false;
		
		int mindState = pInputController.GetMindState();
		if( m_LastMindState != mindState )
		{
			switch( mindState )
			{
			case DayZInfectedConstants.MINDSTATE_CALM:
				if( moveCommand )
					moveCommand.SetIdleState(0);
				break;

			case DayZInfectedConstants.MINDSTATE_DISTURBED:
				if( moveCommand )
					moveCommand.SetIdleState(1);
				break;
			
			case DayZInfectedConstants.MINDSTATE_CHASE:
				if(m_walkingZeds)
				{
					if( moveCommand && (m_LastMindState < DayZInfectedConstants.MINDSTATE_CHASE) )
					moveCommand.SetIdleState(2);
				break;
				}
				else
				{	//No Running zeds
					if( moveCommand )
						moveCommand.SetIdleState(1);
				break;
				}
			}
			
			m_LastMindState = mindState;
			m_AttackCooldownTime = 0.0;
		}
		return false;
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		super.EEHitBy(damageResult, damageType, source, component, ammo, modelPos);

		string sourceType = source.GetType();
		if (lastHitSource != source && GetGame().IsKindOf( sourceType, "SurvivorBase"))
		{
			lastHitSource = PlayerBase.Cast(source); //update last hit source owner
		}

		int oRandValue = Math.RandomIntInclusive(0,100);
		float DamageDealt = damageResult.GetDamage(component,""); //zoneName -- healthType

		this.SetHealth(oRandValue - DamageDealt);
	}
}