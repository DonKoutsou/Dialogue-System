modded enum ChimeraMenuPreset
{
    DialogueLayout
}
class DialogueUIClass: ChimeraMenuBase
{
	SP_DialogueComponent DiagComp;
	SP_DialogueArchetype DiagArch;
	IEntity TalkingChar;
	bool IsDialogueHappening = false;
    protected Widget m_wRoot;    
    protected SCR_ButtonTextComponent comp;
	OverlayWidget m_ListBoxOverlay;
	TextWidget m_CharacterName;
	TextWidget m_CharacterRank;
	ImageWidget m_CharacterFactionIcon;
	SCR_ListBoxElementComponent m_ListBoxElement;
    SCR_ListBoxComponent m_ListBoxComponent;
    IEntity myCallerEntity;
	IEntity myUserEntity;           
    string CharName;
	string CharRank;
	ECharacterRank rank;
	FactionKey faction;
	string Diag0;
	string Diag1;
	string Diag2;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
    //------------------------------------------------------------------------------------------------
    override void OnMenuOpen()
    {
    };
	void Init(IEntity Owner, IEntity User)
	{
		myCallerEntity = Owner;
		myUserEntity = User;
		m_wRoot = GetRootWidget();
		m_ListBoxOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox0")); 
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		if(myCallerEntity)
		{
			CharName = DiagComp.GetCharacterName(myCallerEntity);
			rank = DiagComp.GetCharacterRank(myCallerEntity);
			faction = DiagComp.GetCharacterFaction(myCallerEntity);
		}
		switch(rank)
		{
			case 1:
			{
				CharRank = "Private";
			}
		}
		m_CharacterFactionIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionIcon"));
		if (m_CharacterFactionIcon)
		{
		switch(faction)
		{
			case "US":
			{
				m_CharacterFactionIcon.LoadImageTexture(0, "{EB7F65DBC9392557}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
			}
			break;
			case "USSR":
			{
				m_CharacterFactionIcon.LoadImageTexture(0, "{40B12B0DF911B856}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
			}
			break;
			case "FIA":
			{
				m_CharacterFactionIcon.LoadImageTexture(0, "{FB266CDD4502D60B}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_Fia.edds");
			}
			break;
		}
		}
		m_CharacterName = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterName"));
		m_CharacterName.SetText(CharName);
		
		m_CharacterName = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterRank"));
		m_CharacterName.SetText(CharRank);
		
	} 
	void UpdateEntries()
	{
		Diag0 = DiagComp.GetActionName(0, myCallerEntity);
		Diag1 = DiagComp.GetActionName(1, myCallerEntity);
		Diag2 = DiagComp.GetActionName(2, myCallerEntity);
		m_ListBoxComponent.AddItem(Diag0);
		m_ListBoxComponent.AddItem(Diag1);
		m_ListBoxComponent.AddItem(Diag2);
		m_ListBoxComponent.AddItem("Leave");
		SCR_ListBoxElementComponent elComp0 = m_ListBoxComponent.GetElementComponent(0);
		SCR_ListBoxElementComponent elComp1 = m_ListBoxComponent.GetElementComponent(1);
		SCR_ListBoxElementComponent elComp2 = m_ListBoxComponent.GetElementComponent(2);
		SCR_ListBoxElementComponent elComp3 = m_ListBoxComponent.GetElementComponent(3);
		elComp0.m_OnClicked.Insert(DoDialogue0);
		elComp1.m_OnClicked.Insert(DoDialogue1);
		elComp2.m_OnClicked.Insert(DoDialogue2);
		{
		elComp3.m_OnClicked.Insert(myCustomFunction);
		}
	} 
    void myCustomFunction()
    {
		GetGame().GetMenuManager().CloseAllMenus();
		SP_DialogueArchetype DArch = DiagComp.LocateCharacterArchetype(myCallerEntity);
		DArch.IsCharacterBranched = false;
    }
	void BranchBack()
	{
		SP_DialogueArchetype DArch = DiagComp.LocateCharacterArchetype(myCallerEntity);
		DArch.IsCharacterBranched = false;
	}
	void DoDialogue0()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 0, 1);
	}
	void DoDialogue1()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 1, 1);
	}
	void DoDialogue2()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 2, 1);
	}
	
}