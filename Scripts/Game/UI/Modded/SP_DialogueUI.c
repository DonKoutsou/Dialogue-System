modded enum ChimeraMenuPreset
{
    DialogueLayout
}
class DialogueUIClass: ChimeraMenuBase
{ 
	//-----------------------------------------------//
	//UI Widgets
	protected Widget m_wRoot; 
	OverlayWidget m_ListBoxOverlay;
	TextWidget m_CharacterName;
	TextWidget m_CharacterRank;
	ImageWidget m_CharacterFactionIcon;
	SCR_ListBoxElementComponent m_ListBoxElement;
    SCR_ListBoxComponent m_ListBoxComponent;
    //-----------------------------------------------//
	//PlayerCharacter
	IEntity myUserEntity;           
	//-----------------------------------------------//
	//Charactaer we are talking to
	IEntity myCallerEntity;
	string CharName;
	string CharRank;
	ECharacterRank rank;
	FactionKey faction;
	//-----------------------------------------------//
	//DialogueStystem
	SP_DialogueComponent DiagComp;
	SP_DialogueArchetype DiagArch;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
    //------------------------------------------------------------------------------------------------//
	
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
		string Diag0 = DiagComp.GetActionName(0, myCallerEntity);
		string Diag1 = DiagComp.GetActionName(1, myCallerEntity);
		string Diag2 = DiagComp.GetActionName(2, myCallerEntity);
		string Diag3 = DiagComp.GetActionName(3, myCallerEntity);
		int entryamount;
		//Check if any there arent inputs comming form GetActionName, if not do not create the item
		if (Diag0 != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(Diag0);
			SCR_ListBoxElementComponent elComp0 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp0.m_OnClicked.Insert(DoDialogue0);
			string entrynumber = (entryamount + 1).ToString();
			elComp0.SetTextNumber(entrynumber);
			entryamount = entryamount + 1;
		}
		if (Diag1 != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(Diag1);
			SCR_ListBoxElementComponent elComp1 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp1.m_OnClicked.Insert(DoDialogue1);
			string entrynumber = (entryamount + 1).ToString();
			elComp1.SetTextNumber(entrynumber);
			entryamount = entryamount + 1;
		}
		if (Diag2 != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(Diag2);
			SCR_ListBoxElementComponent elComp2 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp2.m_OnClicked.Insert(DoDialogue2);
			string entrynumber = (entryamount + 1).ToString();
			elComp2.SetTextNumber(entrynumber);
			entryamount = entryamount + 1;
		}
		if (Diag3 != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(Diag3);
			SCR_ListBoxElementComponent elComp3 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp3.m_OnClicked.Insert(DoDialogue3);
			string entrynumber = (entryamount + 1).ToString();
			elComp3.SetTextNumber(entrynumber);
			entryamount = entryamount + 1;
		}
		//Check if Archtype is branched an choose to create a Leave button or a Go Back button
		SP_DialogueArchetype DArch = DiagComp.LocateCharacterArchetype(myCallerEntity);
		if (DArch.IsCharacterBranched == true)
		{
			m_ListBoxComponent.AddItem("Go Back");
			SCR_ListBoxElementComponent elComp4 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp4.m_OnClicked.Insert(DoDialogueBack);
			string entrynumber = (entryamount + 1).ToString();
			elComp4.SetTextNumber(entrynumber);
			return;
		}
		m_ListBoxComponent.AddItem("Leave");
		SCR_ListBoxElementComponent elComp4 = m_ListBoxComponent.GetElementComponent(entryamount);
		elComp4.m_OnClicked.Insert(LeaveFunction);
		string entrynumber = (entryamount + 1).ToString();
		elComp4.SetTextNumber(entrynumber);
	}
	//Function called to close menu
    void LeaveFunction()
    {
		GetGame().GetMenuManager().CloseAllMenus();
    }
	//DoDialogue function wich branch ID 0
	void DoDialogue0()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 0, 1);
	}
	//DoDialogue function wich branch ID 1
	void DoDialogue1()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 1, 1);
	}
	//DoDialogue function wich branch ID 2
	void DoDialogue2()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 2, 1);
	}
	//DoDialogue function wich branch ID 3
	void DoDialogue3()
	{
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 3, 1);
	}
	void DoDialogueBack()
	{
		DiagComp.DoBackDialogue(myCallerEntity, myUserEntity);
	}
}