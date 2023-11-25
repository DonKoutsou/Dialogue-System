[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageCheckIDAction : DS_BaseDialogueStageAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		array<BaseInfoDisplay> infoDisplays = {};
		GetGame().GetPlayerController().GetHUDManagerComponent().GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays : infoDisplays)
		{
			SCR_IDWidget casualtyInspectDisplay = SCR_IDWidget.Cast(baseInfoDisplays);
			if (!casualtyInspectDisplay)
				continue;

			casualtyInspectDisplay.SetTarget(Character);
			casualtyInspectDisplay.ShowInspectCasualtyWidget(Character);
			break;
		}
		super.Perform(Character, Player);
	};
};