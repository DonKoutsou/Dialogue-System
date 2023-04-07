[BaseContainerProps(configRoot: true)]
class SP_Dialogues : ScriptAndConfig
{
	[Attribute("", UIWidgets.Object, "")]
	ref array<ref SP_DiagMenuCategory> m_aDialogueCategories;
	void UpdateDialogueEntries()
	{
		for (int i = 0, count = m_aDialogueCategories.Count(); i < count; i++)
        	{
			SP_DiagMenuCategory DiagCat = SP_DiagMenuCategory.Cast(m_aDialogueCategories[i]);
			DiagCat.Update();
			}
	}
}

class SP_DiagMenuCategory : BaseSelectionMenuCategory
{
	[Attribute("", UIWidgets.EditBox, "")]
	protected string m_sDiagCategoryName;
	[Attribute("", UIWidgets.Object, "")]
	protected ref array<ref BaseSelectionMenuEntry> m_aDiagCategoryContent;
	//------------------------------------------------------------------------------------------------
	override void AddElementToCategory(BaseSelectionMenuEntry element)
	{
		m_aDiagCategoryContent.Insert(element);
	}

	//------------------------------------------------------------------------------------------------
	override array<ref BaseSelectionMenuEntry> GetCategoryElements()
	{
		return m_aDiagCategoryContent;
	}
	void Update()
	{
		for (int i = 0, count = m_aDiagCategoryContent.Count(); i < count; i++)
        	{
			SP_DialogueSelectionMenuEntry DiagMenuEntry = SP_DialogueSelectionMenuEntry.Cast(m_aDiagCategoryContent[i]);
			DiagMenuEntry.UpdateVisuals();
			}
		
	}
}

class SP_DialogueSelectionMenuEntry : SCR_BaseGroupEntry
{
	
	[Attribute(desc: "DialogueBranchKey, Key used to define wich action should use this config", category: "Dialogue",  )]			//TODO: make config, memory
	ref DialogueBranchKey m_iDialogueBranchKey;
	[Attribute( defvalue: "", desc: "ID used to collect texts for DialogueComponent",  )]			//TODO: make config, memory
	protected int EntryID;
	[Attribute()]
	bool IsExit;
	
	string m_sPageName;
	protected string m_sActionText;
	protected string m_sDialogueText;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	protected SP_DialogueComponent DiagComp;
	protected SCR_RadialMenuComponent RadComp;
	protected SCR_RadialMenuHandler RadHand;
	protected SP_DialogueConfig DiagConf;
	//------------------------------------------------------------------//
	void SetDialogueConfig(SP_DialogueConfig DConf)
	{
		DiagConf = DConf;
		return;
	}
	override void UpdateVisuals()
	{
		
		RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		RadHand = SCR_RadialMenuHandler.Cast(RadComp.GetRadialMenuHandler());
		SP_RadialMenuDiags RadDiag = SP_RadialMenuDiags.Cast(RadHand);
		IEntity owner = RadDiag.GetTalkingChar();
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		m_sDialogueText = DiagComp.GetActionName(m_iDialogueBranchKey.GetBranchKey(), owner);
		if (IsExit == true)
		{
			m_sDialogueText = "Leave";
		}
		if (m_EntryComponent)
			m_EntryComponent.SetLabelText(m_sDialogueText);
	}
	IEntity GetTalkingCharacter()
	{
		RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		RadHand = SCR_RadialMenuHandler.Cast(RadComp.GetRadialMenuHandler());
		SP_RadialMenuDiags RadDiag = SP_RadialMenuDiags.Cast(RadHand);
		IEntity owner = RadDiag.GetTalkingChar();
		return owner;
	}
	protected override event void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
	{
		RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		SP_RadialMenuDiags RadMenuDiags = SP_RadialMenuDiags.Cast(RadComp.GetRadialMenuHandler());
		
		RadHand = SCR_RadialMenuHandler.Cast(RadComp.GetRadialMenuHandler());
		SP_RadialMenuDiags RadDiag = SP_RadialMenuDiags.Cast(RadHand);
		IEntity owner = RadDiag.GetTalkingChar();
		super.OnPerform(user, sourceMenu);
		if (IsExit == true)
		{
			SP_DialogueArchetype DArch = DiagComp.LocateCharacterArchetype(owner);
			if (DArch.IsCharacterBranched == true)
			{
				DArch.IsCharacterBranched = false;
				RadDiag.UpdateDiag();
				return;
			}
			else
			{
				RadMenuDiags.Close(user);
				RadMenuDiags.OpenMenu(user, false);
				
				DiagComp.LocateCharacterArchetype(RadDiag.GetTalkingChar()).ResetStage(m_iDialogueBranchKey.GetBranchKey());
				return;
			}
			
		}
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		
		if (DiagComp)
		{
			IEntity talkingchar = RadMenuDiags.GetTalkingChar();
			//SendText function on dialogue component, sending all gathered data from action
			//DiagComp.DoRadialDialogue(owner, talkingchar, m_iDialogueBranchKey, 1, EntryID);
			DiagComp.DoDialogue(owner, talkingchar, m_iDialogueBranchKey.GetBranchKey(), 1);
		}
		//RadDiag.PageSetup();
		UpdateVisuals();
		return;
	}
	override bool GetEntryNameScript(out string outName)
	{
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		RadHand = SCR_RadialMenuHandler.Cast(RadComp.GetRadialMenuHandler());
		SP_RadialMenuDiags RadDiag = SP_RadialMenuDiags.Cast(RadHand);
		IEntity owner = RadDiag.GetTalkingChar();
		
		if (DiagComp)
		{
			outName = DiagComp.GetActionName(m_iDialogueBranchKey.GetBranchKey(), owner);
			m_sDialogueText = outName;
		}
		if (outName == STRING_EMPTY)
			{
				return false;
			}
			else
				return true;
		//RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		//RadHand = SCR_RadialMenuHandler.Cast(RadComp.GetRadialMenuHandler());
		//SP_RadialMenuDiags RadDiag = SP_RadialMenuDiags.Cast(RadHand);
		//IEntity owner = RadDiag.GetTalkingChar();
		//DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		//outName = DiagComp.GetActionName(m_iDialogueBranchKey, owner);
		//if(!outName)
		//{
		//	return false;
		//}
		//return true;
	}
	override string GetName() 
	{
		 return m_sDialogueText;
	}
	override bool GetEntryDescriptionScript(out string outDescription)
	{
		outDescription = m_sDialogueText;
		return true;
	}
	//------------------------------------------------------------------//
	int GetDialogueBranchKey()
	{
		return m_iDialogueBranchKey;
	}
	//------------------------------------------------------------------//
	string GetActionText(int stageid)
	{
		string actiontext;
		actiontext = m_sDialogueText;
		
		return actiontext;
	}
};
enum EChoiseBehavior
	{
		"Accept&Close",
		"AlrernateDialogue",
		"Stay"
	};