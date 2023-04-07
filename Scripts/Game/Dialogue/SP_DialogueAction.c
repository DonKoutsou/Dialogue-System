class SP_DialogueAction : ScriptedUserAction
{
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected SCR_RadialMenuComponent RadComp
	protected SP_RadialMenuDiags RadialMenuDiags;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		if (DiagComp)
		{
			//DoDialogue function on dialogue component, sending all gathered data from action
			DiagComp.StartRadialMenu(pOwnerEntity, pUserEntity);
		}
		return;
	}
	override bool CanBeShownScript(IEntity user)
	{
		if (RadialMenuDiags.IsDialogueHappening == true)
		{
			return false;
		}
		else
		{
			return true;
		}
		
	}
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		RadComp = SCR_RadialMenuComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_RadialMenuComponent));
		RadialMenuDiags = SP_RadialMenuDiags.Cast(RadComp.GetRadialMenuHandler());
	}
	//------------------------------------------------------------------//
}