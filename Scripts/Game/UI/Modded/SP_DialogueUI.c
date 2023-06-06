modded enum EModularButtonEventHandler
{
	BUTTON_1		= 1<<6,
	BUTTON_2		= 1<<7,
	BUTTON_3		= 1<<8,
	BUTTON_4		= 1<<9
};
class DialogueUIClass: ChimeraMenuBase
{ 
	//----------------------------------------------------------------//
	//UI Widgets
	protected Widget 							m_wRoot; 
	OverlayWidget 								m_ListBoxOverlay;
	TextWidget 									m_CharacterName;
	TextWidget 									m_PlayerName;
	TextWidget 									m_CharacterRank;
	TextWidget 									m_PlayerRank;
	ImageWidget 								m_CharacterFactionIcon;
	ImageWidget 								m_PlayerFactionIcon;
	ImageWidget 								m_FactionRep;
	SCR_ListBoxElementComponent 				m_ListBoxElement;
    SCR_ListBoxComponent 						m_ListBoxComponent;
	PanelWidget									m_PanelWidget;
    //----------------------------------------------------------------//
	//PlayerCharacter
	IEntity 									myUserEntity;           
	//----------------------------------------------------------------//
	//Charactaer we are talking to
	IEntity 									myCallerEntity;
	string 										CharName;
	string 										PlName;
	string 										CharRank;
	string 										m_sPlRank;
	SCR_ECharacterRank 							rank;
	SCR_ECharacterRank 							PLrank;
	FactionKey 									faction;
	FactionKey 									Plfaction;
	//----------------------------------------------------------------//
	//DialogueStystem
	SP_DialogueComponent 						DiagComp;
	SP_DialogueArchetype 						DiagArch;
	SCR_CharacterIdentityComponent				m_IDComp;
	protected SCR_BaseGameMode 					GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	int 										CurrentBranchID;
	ref ScriptInvoker 							m_OnKeyDown = new ScriptInvoker();
    //------------------------------------------------------------------------------------------------//
	override void OnMenuUpdate(float tDelta)
	{
		GetGame().GetInputManager().ActivateContext("DialogueMenu", 0);
	}
	void RemoveListeners()
	{
		GetGame().GetInputManager().RemoveActionListener("Dialogue0", EActionTrigger.DOWN, ExecuteDialogue0);
		GetGame().GetInputManager().RemoveActionListener("Dialogue1", EActionTrigger.DOWN, ExecuteDialogue1);
		GetGame().GetInputManager().RemoveActionListener("Dialogue2", EActionTrigger.DOWN, ExecuteDialogue2);
		GetGame().GetInputManager().RemoveActionListener("Dialogue3", EActionTrigger.DOWN, ExecuteDialogue3);
		GetGame().GetInputManager().RemoveActionListener("Dialogue4", EActionTrigger.DOWN, ExecuteDialogue4);
		GetGame().GetInputManager().RemoveActionListener("Dialogue5", EActionTrigger.DOWN, ExecuteDialogue5);
		GetGame().GetInputManager().RemoveActionListener("Dialogue6", EActionTrigger.DOWN, ExecuteDialogue6);
		GetGame().GetInputManager().RemoveActionListener("DialogueBack", EActionTrigger.DOWN, DoDialogueBack);
		GetGame().GetInputManager().RemoveActionListener("DialogueBack", EActionTrigger.DOWN, LeaveFunction);
	}
	void Init(IEntity Owner, IEntity User)
	{
		myCallerEntity = Owner;
		myUserEntity = User;
		m_wRoot = GetRootWidget();
		m_ListBoxOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox0")); 
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		m_IDComp = SCR_CharacterIdentityComponent.Cast(User.FindComponent(SCR_CharacterIdentityComponent));
	}
	//------------------------------------------------------------------------------------------------//
	void UpdateEntries(IEntity Character, IEntity Player)
	{
		myCallerEntity = Character;
		myUserEntity = Player;
		m_IDComp = SCR_CharacterIdentityComponent.Cast(myUserEntity.FindComponent(SCR_CharacterIdentityComponent));
		m_wRoot = GetRootWidget();
		m_ListBoxOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox0")); 
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		string DiagText;
		int entryamount;
		//Check if any there arent inputs comming form GetActionName, if not do not create the item
		if(myCallerEntity)
		{
			CharName = DiagComp.GetCharacterName(myCallerEntity);
			rank = DiagComp.GetCharacterRank(myCallerEntity);
			faction = DiagComp.GetCharacterFaction(myCallerEntity).GetFactionKey();
		}
		if(myUserEntity)
		{
			PlayerManager mngr = GetGame().GetPlayerManager();
			int pid = mngr.GetPlayerIdFromControlledEntity(myUserEntity);
			PlName = mngr.GetPlayerName(pid);
			PLrank = DiagComp.GetCharacterRank(myUserEntity);
			Plfaction = DiagComp.GetCharacterFaction(myUserEntity).GetFactionKey(); 
		}
		SCR_Faction SCRFact = SCR_Faction.Cast(DiagComp.GetCharacterFaction(myCallerEntity));
		int FactionRep;
		SCRFact.GetFactionRep(DiagComp.GetCharacterFaction(myUserEntity), FactionRep);
		if(FactionRep > 50)
		{
			m_FactionRep = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionRep()"));
			m_FactionRep.SetColor(Color.DarkGreen);
		}
		else if(FactionRep <= 50 &&FactionRep > 20)
		{
			m_FactionRep = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionRep()"));
			m_FactionRep.SetColor(Color.DarkYellow);
		}
		else if(FactionRep <= 20)
		{
			m_FactionRep = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionRep()"));
			m_FactionRep.SetColor(Color.DarkRed);
		}
		switch(rank)
		{
			case 1:
			{
				CharRank = "Private";
			}
			break;
			case 8:
			{
				CharRank = "GENERAL";
			}
			break;
			case 22:
			{
				CharRank = "Commander";
			}
			break;
		}
		switch(PLrank)
		{
			case 1:
			{
				m_sPlRank = "Private";
			}
			break;
			case 8:
			{
				m_sPlRank = "GENERAL";
			}
			break;
			case 22:
			{
				m_sPlRank = "Commander";
			}
			break;
		}
		m_CharacterFactionIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionIcon"));
		m_PlayerFactionIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerFactionIcon0"));
		if (m_CharacterFactionIcon)
		{
			switch(faction)
			{
				case "US":
					m_CharacterFactionIcon.LoadImageTexture(0, "{EB7F65DBC9392557}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "USSR":
					m_CharacterFactionIcon.LoadImageTexture(0, "{40B12B0DF911B856}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "FIA":
					m_CharacterFactionIcon.LoadImageTexture(0, "{FB266CDD4502D60B}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_Fia.edds");
				break;
				case "BANDITS":
					m_CharacterFactionIcon.LoadImageTexture(0, "{855625FE8D6A87A8}UI/Textures/DamageState/damageState-dead.edds");
				break;
				case "SPEIRA":
					m_CharacterFactionIcon.LoadImageTexture(0, "{CEAC8771342689C5}Assets/Data/Faction_SPEIRA.edds");
				break;	
			}
		}
		if (m_PlayerFactionIcon)
		{
			switch(Plfaction)
			{
				case "US":
					m_PlayerFactionIcon.LoadImageTexture(0, "{EB7F65DBC9392557}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "USSR":
					m_PlayerFactionIcon.LoadImageTexture(0, "{40B12B0DF911B856}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "FIA":
					m_PlayerFactionIcon.LoadImageTexture(0, "{FB266CDD4502D60B}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_Fia.edds");
				break;
				case "BANDITS":
					m_PlayerFactionIcon.LoadImageTexture(0, "{855625FE8D6A87A8}UI/Textures/DamageState/damageState-dead.edds");
				break;
				case "SPEIRA":
					m_PlayerFactionIcon.LoadImageTexture(0, "{CEAC8771342689C5}Assets/Data/Faction_SPEIRA.edds");
				break;	
			}
		}
		if(m_IDComp.GetRep() > 50)
		{
			m_PanelWidget = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PanelWidget.SetColor(Color.DarkGreen);
		}
		else if(m_IDComp.GetRep() <= 50 && m_IDComp.GetRep() > 20)
		{
			m_PanelWidget = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PanelWidget.SetColor(Color.DarkYellow);
		}
		else if(m_IDComp.GetRep() <= 20)
		{
			m_PanelWidget = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PanelWidget.SetColor(Color.DarkRed);
		}
		m_CharacterName = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterName"));
		m_CharacterName.SetText(CharName);
		
		m_CharacterRank = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterRank"));
		m_CharacterRank.SetText(CharRank);
		
		m_PlayerName = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerName0"));
		m_PlayerName.SetText(PlName);

		m_PlayerRank = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerRank0"));
		m_PlayerRank.SetText(m_sPlRank);
		
		DiagComp.GetActionName(0, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 0;
			SCR_ListBoxElementComponent elComp0 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp0.m_OnClicked.Insert(ExecuteDialogue0);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp0.SetTextNumber("A");
			}
			else
			{
				elComp0.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue0", EActionTrigger.DOWN, ExecuteDialogue0);
		}
		DiagComp.GetActionName(1, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 1;
			SCR_ListBoxElementComponent elComp1 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp1.m_OnClicked.Insert(ExecuteDialogue1);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp1.SetTextNumber("X");
			}
			else
			{
				elComp1.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue1", EActionTrigger.DOWN, ExecuteDialogue1);
		}
		DiagComp.GetActionName(2, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 2;
			SCR_ListBoxElementComponent elComp2 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp2.m_OnClicked.Insert(ExecuteDialogue2);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp2.SetTextNumber("Y");
			}
			else
			{
				elComp2.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue2", EActionTrigger.DOWN, ExecuteDialogue2);
		}
		DiagComp.GetActionName(3, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 3;
			SCR_ListBoxElementComponent elComp3 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp3.m_OnClicked.Insert(ExecuteDialogue3);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp3.SetTextNumber("D-Pad Up");
			}
			else
			{
				elComp3.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue3", EActionTrigger.DOWN, ExecuteDialogue3);
		}
		DiagComp.GetActionName(4, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 4;
			SCR_ListBoxElementComponent elComp4 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp4.m_OnClicked.Insert(ExecuteDialogue4);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp4.SetTextNumber("D-Pad Down");
			}
			else
			{
				elComp4.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue4", EActionTrigger.DOWN, ExecuteDialogue4);
		}
		DiagComp.GetActionName(5, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 5;
			SCR_ListBoxElementComponent elComp5 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp5.m_OnClicked.Insert(ExecuteDialogue5);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp5.SetTextNumber("D-Pad Left");
			}
			else
			{
				elComp5.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue5", EActionTrigger.DOWN, ExecuteDialogue5);
		}
		DiagComp.GetActionName(6, myCallerEntity, Player, DiagText);
		if (DiagText != STRING_EMPTY)
		{
			m_ListBoxComponent.AddItem(DiagText);
			CurrentBranchID = 6;
			SCR_ListBoxElementComponent elComp6 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp6.m_OnClicked.Insert(ExecuteDialogue6);
			string entrynumber = (entryamount + 1).ToString();
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp6.SetTextNumber("D-Pad Right");
			}
			else
			{
				elComp6.SetTextNumber(entrynumber);
			}
			entryamount = entryamount + 1;
			DiagText = STRING_EMPTY;
			GetGame().GetInputManager().AddActionListener("Dialogue6", EActionTrigger.DOWN, ExecuteDialogue6);
		}
		//Check if Archtype is branched an choose to create a Leave button or a Go Back button
		SP_DialogueArchetype DArch = DiagComp.LocateDialogueArchetype(myCallerEntity, Player);
		if (DArch.IsCharacterBranched == true)
		{
			m_ListBoxComponent.AddItem("Go Back");
			SCR_ListBoxElementComponent elComp7 = m_ListBoxComponent.GetElementComponent(entryamount);
			elComp7.m_OnClicked.Insert(DoDialogueBack);
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp7.SetTextNumber("B");
			}
			else
			{
				elComp7.SetTextNumber("[BACKSPACE]");
			}
			GetGame().GetInputManager().AddActionListener("DialogueBack", EActionTrigger.DOWN, DoDialogueBack);
			return;
		}
		m_ListBoxComponent.AddItem("Leave");
		SCR_ListBoxElementComponent elComp7 = m_ListBoxComponent.GetElementComponent(entryamount);
		elComp7.m_OnClicked.Insert(LeaveFunction);
		if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp7.SetTextNumber("B");
			}
			else
			{
				elComp7.SetTextNumber("[BACKSPACE]");
			}
		GetGame().GetInputManager().AddActionListener("DialogueBack", EActionTrigger.DOWN, LeaveFunction);
	}
	//------------------------------------------------------------------------------------------------//
	//Function called to close menu
    void LeaveFunction()
    {
		RemoveListeners();
		DiagComp.DoBackDialogue(myCallerEntity, myUserEntity);
		DiagComp.ReleaseAI(myCallerEntity, myUserEntity);
		GetGame().GetMenuManager().CloseAllMenus();
    }
	override void OnMenuClose()
	{
		RemoveListeners();
	}
	//------------------------------------------------------------------------------------------------//
	//DoDialogue function wich branch ID 0
	void ExecuteDialogue0()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 0);
	}
	//------------------------------------------------------------------------------------------------//
	void ExecuteDialogue1()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 1);
	}
	//------------------------------------------------------------------------------------------------//
	void ExecuteDialogue2()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 2);
	}
	void ExecuteDialogue3()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 3);
	}
	void ExecuteDialogue4()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 4);
	}
	void ExecuteDialogue5()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 5);
	}
	void ExecuteDialogue6()
	{
		RemoveListeners();
		DiagComp.DoDialogue(myCallerEntity, myUserEntity, 6);
	}
	//------------------------------------------------------------------------------------------------//
	void DoDialogueBack()
	{
		RemoveListeners();
		DiagComp.DoBackDialogue(myCallerEntity, myUserEntity);
	}
	//------------------------------------------------------------------------------------------------//
	override event bool OnKeyDown(Widget w, int x, int y, int key)
	{
		if (key == 1)
		{
			OnClick(w, x, y, 0);
			m_OnKeyDown.Invoke(this);
			return true;
		}
		return false;
	}
}