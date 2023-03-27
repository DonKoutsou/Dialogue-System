class SP_DialogueAction : ScriptedUserAction
{	

	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	[Attribute()]
	int CharID;
	[Attribute()]
	int ActionBranchID;
	[Attribute()]
	protected bool m_bMultipleChoise;
	protected SP_DialogueComponent DiagComp;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());

	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		if (DiagComp)
		{
			//SendText function on dialogue component, sending all gathered data from action
			DiagComp.SendText(pOwnerEntity, pUserEntity, m_ChatChannel, CharID, ActionBranchID, m_bMultipleChoise);
			//Makes sure to increment the stage of each branch
			DiagComp.IncrementDiagStage(CharID, ActionBranchID, 1, m_bMultipleChoise);
		}
		
		return;
	}
	//if string that comes back from config is empty means that dialogue is finished so hide action
	override bool CanBeShownScript(IEntity user)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		string outName;
		if (DiagComp)
		{
			outName = DiagComp.GetActionName(CharID, ActionBranchID, m_bMultipleChoise);	
		}
		if (outName == STRING_EMPTY)
			{
				return false;
			}
			else
				return true;
	}
	//looks for the name of the action in the dialogue config
	override event bool GetActionNameScript(out string outName)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		
		if (DiagComp)
		{
			outName = DiagComp.GetActionName(CharID, ActionBranchID, m_bMultipleChoise);
			
		}
		if (outName == STRING_EMPTY)
			{
				return false;
			}
			else
				return true;
	}
	
	
}