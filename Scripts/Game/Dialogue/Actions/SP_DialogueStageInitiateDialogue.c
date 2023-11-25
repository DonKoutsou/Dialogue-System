[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageInitiateDialogue : DS_BaseDialogueStageAction
{
	//------------------------------------------------------------------//
	IEntity Target;
	string TargetName;
	int m_iIndex;
	protected DS_DialogueComponent DiagComp;
	protected BaseGameMode GameMode;
	
	override void Perform(IEntity Character, IEntity Player)
	{
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
	/*override bool CanBeShown(IEntity Character, IEntity Player)
	{
		if (!DS_DialogueComponent.a_PLcontactList.IsEmpty() && DS_DialogueComponent.a_PLcontactList.Count() > m_iIndex)
			return true; 
		return false;
	}*/
	
	override void Init(DS_DialogueBranch Owner = null, int Index = 0)
	{
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
		DiagComp = DS_DialogueComponent.Cast(GameMode.FindComponent(DS_DialogueComponent));
		super.Init(Owner, Index);
		if (Owner)
		{
			m_iIndex = Index;
			
		}
	}

};
