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
	
	protected int atcionstoshow;
	protected SP_DialogueComponent DiagComp;
	protected ActionsManagerComponent actmngr
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	protected ref array<BaseUserAction> actions;

	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		if (DiagComp)
		{
			DiagComp.SendText(pOwnerEntity, pUserEntity, m_ChatChannel, CharID, ActionBranchID, m_bMultipleChoise);
			//sends text to chat bar, CharID, and Branch id are used to select correct dialogue config
			DiagComp.IncrementDiagStage(CharID, ActionBranchID, 1, m_bMultipleChoise);
			//makes sure all the places where stage should be updated is updated
		}
		
		return;
	}
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