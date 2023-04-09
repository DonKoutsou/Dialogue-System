class SP_DialogueAction : ScriptedUserAction
{
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		DiagUI.Init(pOwnerEntity, pUserEntity);
		DiagUI.UpdateEntries();
	}
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
	}
	//------------------------------------------------------------------//
}