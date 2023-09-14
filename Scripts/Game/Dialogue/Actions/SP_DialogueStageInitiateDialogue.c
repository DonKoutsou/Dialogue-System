[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageInitiateDialogue : DialogueStage
{
	//------------------------------------------------------------------//
	IEntity Target;
	string TargetName;
	int m_iIndex;
	protected SP_DialogueComponent DiagComp;
	protected BaseGameMode GameMode;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		if (!Target)
		{
			Target = SP_DialogueComponent.a_PLcontactList[m_iIndex];
			if (Target)
			{
				TargetName = SP_DialogueComponent.GetCharacterName(Target);
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
		
		SCR_AIConverseBehavior action = new SCR_AIConverseBehavior(utility, null, Player.GetOrigin(), true);
		
		
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
		
	};
	//------------------------------------------------------------------//
	override bool GetActionText(IEntity Character, IEntity Player, out string acttext)
	{
		if (!CanBeShown(Character, Player))
			return false;
		if (!Target)
		{
			Target = SP_DialogueComponent.a_PLcontactList[m_iIndex];
			if (Target)
			{
				TargetName = SP_DialogueComponent.GetCharacterName(Target);
			}
		}
		if (!TargetName)
				return false;
		acttext = TargetName;
		if (CanBePerformed(Character, Player) == false)
		{
			acttext = acttext + " " + m_sCantBePerformedReason;
			return true;
		}
		return true;
	}
	override bool CanBeShown(IEntity Character, IEntity Player)
	{
		if (!SP_DialogueComponent.a_PLcontactList.IsEmpty() && SP_DialogueComponent.a_PLcontactList.Count() > m_iIndex)
			return true; 
		return false;
	}
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		SCR_GadgetManagerComponent gadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(Target);
		if (!gadgetManager)
			return false;

		IEntity item = gadgetManager.GetGadgetByType(EGadgetType.RADIO);
		if (!item)
		{
			SetCannotPerformReason("|Cant reach. Character missing radio.|");
			return false;
		}
			
		return true;
	}
	override void Init(SP_DialogueBranch Owner = null, int Index = 0)
	{
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		super.Init(Owner, Index);
		if (Owner)
		{
			m_iIndex = Index;
			
		}
		
	}

};