class SCR_AIGetMovementOfEntity : AITaskScripted
{
	static const string ENTITY_PORT = "Entity";
	static const string MOVEMENZT_TYPE_PORT		= "MovementType";
		
	protected override bool VisibleInPalette()
	{
		return true;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------------------------
	protected static ref TStringArray s_aVarsIn = {
		ENTITY_PORT
	};
	override TStringArray GetVariablesIn()
    {
        return s_aVarsIn;
    }
	
	//-----------------------------------------------------------------------------------------------------------------------------------------
	protected static ref TStringArray s_aVarsOut = {
		MOVEMENZT_TYPE_PORT,
	};
	override TStringArray GetVariablesOut()
    {
        return s_aVarsOut;
    }

	//-----------------------------------------------------------------------------------------------------------------------------------------
	override ENodeResult EOnTaskSimulate(AIAgent owner, float dt)
	{
		IEntity Ent;
		GetVariableIn(ENTITY_PORT, Ent);
		if(!Ent)
		{
			NodeError(this, owner, "Invalid Entity Provided!");
			return ENodeResult.FAIL;
		}
		ChimeraCharacter char = ChimeraCharacter.Cast(Ent);
		int Movement = Math.Max(char.GetCharacterController().GetCurrentMovementPhase(), 1);
		SetVariableOut(MOVEMENZT_TYPE_PORT, Movement);
		return ENodeResult.SUCCESS;
	}	
};