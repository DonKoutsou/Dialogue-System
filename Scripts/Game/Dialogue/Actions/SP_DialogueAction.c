class SP_DialogueAction : ScriptedUserAction
{
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	
	protected int PrevWalkSpeed;
	protected vector PrevWalkDir;
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		AIControlComponent AiComp = AIControlComponent.Cast(pOwnerEntity.FindComponent(AIControlComponent));
		AIAgent Agent = AiComp.GetControlAIAgent();
		AIGroup group = AIGroup.Cast(Agent.GetParentGroup());
        string NoTalkText = "Cant talk to you now";
		CharacterControllerComponent CharCont = CharacterControllerComponent.Cast(pOwnerEntity.FindComponent(CharacterControllerComponent));
		PrevWalkSpeed = CharCont.GetMovementSpeed();
		CharCont.SetMovement(0, "0 0 0");
        if (group)
		{
			Agent = group.GetLeaderAgent();
		}
		if (Agent.HasOrders() == true)
		{
			string name = DiagComp.GetCharacterName(pOwnerEntity);
			DiagComp.SendText(NoTalkText, DiagComp.m_ChatChannel, 0, name);
			return;
		}
		//Agent.GetMovementComponent().ForceFollowPathOfEntity(pUserEntity) = true;
		if (!DiagComp.LocateDialogueArchetype(pOwnerEntity))
		{
			string name = DiagComp.GetCharacterName(pOwnerEntity);
			DiagComp.SendText(NoTalkText, DiagComp.m_ChatChannel, 0, name);
			return;
		}
		
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		DiagUI.Init(pOwnerEntity, pUserEntity);
		DiagUI.UpdateEntries(pOwnerEntity, pUserEntity);
	}
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
	}
	//------------------------------------------------------------------//
	override bool CanBeShownScript(IEntity user)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return false;
		SCR_CharacterDamageManagerComponent damageMan = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMan)
			return false;
		
		if (damageMan.GetState() == EDamageState.DESTROYED)
			return false;
		
		return super.CanBePerformedScript(user);
	}
}