[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageSetCharacterToFollowAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		super.Perform(Character, Player);
		AIControlComponent comp = AIControlComponent.Cast(Character.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		SCR_AIFollowBehavior action = new SCR_AIFollowBehavior(utility, null, Player);
		utility.AddAction(action);
		//if player throw popup
		if (GetGame().GetPlayerController().GetControlledEntity() == Character)
		{
			SCR_HintManagerComponent.GetInstance().ShowCustom(string.Format("%1 started to follow you", DS_DialogueComponent.GetCharacterName(Character)));
		}	
		
	};
};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageStopFollowingAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		super.Perform(Character, Player);
		AIControlComponent comp = AIControlComponent.Cast(Character.FindComponent(AIControlComponent));
		AIAgent agent = comp.GetAIAgent();
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		SCR_AIFollowBehavior act = SCR_AIFollowBehavior.Cast(utility.FindActionOfType(SCR_AIFollowBehavior));
		if (act)
			act.SetActiveFollowing(false);
		//if player throw popup
		SCR_HintManagerComponent.GetInstance().ShowCustom(string.Format("%1 has stopped following you.", DS_DialogueComponent.GetCharacterName(Character)));
	};
};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageEscapeDialogueAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		DS_DialogueComponent.GetInstance().Escape(Character, Player);
	};
};




