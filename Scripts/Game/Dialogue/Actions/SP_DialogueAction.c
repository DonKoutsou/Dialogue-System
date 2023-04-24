class SP_DialogueAction : ScriptedUserAction
{
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		DiagComp.LocateDialogueArchetype(pOwnerEntity);
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