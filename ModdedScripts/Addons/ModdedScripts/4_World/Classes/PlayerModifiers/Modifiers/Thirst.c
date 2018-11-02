modded class Thirst
{
bool m_NoThirst;
    override void OnTick(PlayerBase player, float deltaT){
		if(m_NoThirst)
		{
			//Do nothing
		}
		else
		{
			player.GetMovementState(m_MovementState);
			float metabolic_speed = MiscGameplayFunctions.GetWaterMetabolicSpeed(m_MovementState.m_iMovement);
			//PrintString("metabolic_speed:"+metabolic_speed.ToString());
			float water = player.GetStatWater().Get();
			player.GetStatWater().Add( (-metabolic_speed * deltaT) );
			
			if ( water <= PlayerConstants.LOW_WATER_TRESHOLD )
			{		
				player.AddHealth("GlobalHealth", "Health", -PlayerConstants.LOW_WATER_DAMAGE_PER_SEC * deltaT );
			}

		}
	}
}