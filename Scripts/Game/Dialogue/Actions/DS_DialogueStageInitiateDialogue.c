[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageInitiateDialogue : DS_BaseDialogueStageAction
{
	//------------------------------------------------------------------//
	IEntity Target;
	string TargetName;
	
	protected DS_DialogueComponent DiagComp;
	protected BaseGameMode GameMode;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
		DiagComp = DS_DialogueComponent.Cast(GameMode.FindComponent(DS_DialogueComponent));
		if (!Target)
		{
			Target = DS_DialogueComponent.a_PLcontactList[m_iIndex];
			if (Target)
			{
				TargetName = DS_DialogueComponent.GetCharacterName(Target);
			}
		}
		//DiagComp.Escape(Character, Player);
		AIControlComponent comp = AIControlComponent.Cast(Target.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		
		SCR_AIConverseBehavior action = new SCR_AIConverseBehavior(utility, null, Player, true);
		
		
		string NoTalkText = "Cant talk to you now";
		FactionKey SenderFaction = DiagComp.GetCharacterFaction(Target).GetFactionKey();
		BaseChatChannel Channel;
		string name = DiagComp.GetCharacterName(Target);
		if (!DiagComp.LocateDialogueArchetype(Target, Player))
		{
			DiagComp.SendText(NoTalkText, Channel, 0, name, DiagComp.GetCharacterRankName(Target));
			return;
		}
		if (!utility.FindActionOfType(SCR_AIFollowBehavior))
			utility.AddAction(action);
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		//GetGame().GetInputManager().ActivateContext("DialogueMenuContext");
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		//DiagComp.IntroducitonSting(Character, Player);
		
		DiagUI.Init(Target, Player);
		DiagUI.UpdateEntries(Target, Player);
		super.Perform(Character, Player);
	};

};
