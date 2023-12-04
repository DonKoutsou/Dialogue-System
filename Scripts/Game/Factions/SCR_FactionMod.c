//------------------------------------------------------------------------------------------------
modded class SCR_Faction
{
	[Attribute("VOICE_AHOJ")]
	string m_sGreetName;

	string GetGreetName()
	{
		return m_sGreetName;
	}
};
