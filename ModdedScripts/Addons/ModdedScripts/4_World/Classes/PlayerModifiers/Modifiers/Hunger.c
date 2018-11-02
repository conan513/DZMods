modded class Hunger
{
	bool m_NoHunger;
    override void OnTick(PlayerBase player, float deltaT)
	{
			
		if(m_NoHunger)
		{
			//do nothing
		}
		else
		{
			player.GetMovementState(m_MovementState);
			float energy = player.GetStatEnergy().Get();
			float metabolic_speed = MiscGameplayFunctions.GetEnergyMetabolicSpeed(m_MovementState.m_iMovement);
			//PrintString(metabolic_speed.ToString());
			
			float energy_delta  = Math.AbsInt(player.GetStatEnergy().Get() - m_LastEnergyLevel);
			if (energy <  m_LastEnergyLevel) energy_delta = -energy_delta;
			m_LastEnergyLevel = player.GetStatEnergy().Get();
			
			player.GetStatEnergy().Add( -metabolic_speed * deltaT );
			if ( energy <= PlayerConstants.LOW_ENERGY_TRESHOLD )
			{
				player.AddHealth("GlobalHealth", "Health", -PlayerConstants.LOW_ENERGY_DAMAGE_PER_SEC * deltaT );
			}
		}
	}
}