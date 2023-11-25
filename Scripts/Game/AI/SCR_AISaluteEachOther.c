class SCR_AISaluteEachother: AITaskScripted
{
	protected static const string ENTITY_PORT = "EntityIn";
	
	//-----------------------------------------------------------------------------------------------
	override bool VisibleInPalette() {return true;}
	
	//-----------------------------------------------------------------------------------------------
	protected static ref TStringArray s_aVarsIn = {
		ENTITY_PORT
	};
	override array<string> GetVariablesIn()
	{
		return s_aVarsIn;
	}
	
	//-----------------------------------------------------------------------------------------------
	override ENodeResult EOnTaskSimulate(AIAgent owner, float dt)
	{
		
		
		ChimeraCharacter Chimera = ChimeraCharacter.Cast(owner.GetControlledEntity());
		CharacterControllerComponent Controller = Chimera.GetCharacterController();
		
		//SCR_CharacterCommandHandlerComponent handler = SCR_CharacterCommandHandlerComponent.Cast(Controller.GetAnimationComponent().GetCommandHandler());
		//if (handler.IsLoitering())
		//	return ENodeResult.SUCCESS;
		
		IEntity ent;
		
		if (!GetVariableIn(ENTITY_PORT,ent))
			return ENodeResult.FAIL;
		
		ChimeraCharacter DChimera = ChimeraCharacter.Cast(ent);
		CharacterControllerComponent DController = DChimera.GetCharacterController();
		
		Controller.TryStartCharacterGesture(15, 2000);
		DController.TryStartCharacterGesture(15, 2000);
		
		return ENodeResult.SUCCESS;
  };
};