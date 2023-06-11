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
	PanelWidget									m_CharacterRep;
	PanelWidget									m_PlayerRep;
	ImageWidget 								m_FactionRep;
	SCR_ListBoxElementComponent 				m_ListBoxElement;
    SCR_ListBoxComponent 						m_ListBoxComponent;
	
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
	string 										rank;
	string		 								PLrank;
	FactionKey 									faction;
	FactionKey 									Plfaction;
	//----------------------------------------------------------------//
	//DialogueStystem
	SP_DialogueComponent 						DiagComp;
	SP_DialogueArchetype 						DiagArch;
	SCR_CharacterIdentityComponent				m_IDComp;
	SCR_CharacterIdentityComponent				m_CharIDComp;
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
		m_CharIDComp = SCR_CharacterIdentityComponent.Cast(myCallerEntity.FindComponent(SCR_CharacterIdentityComponent));
		m_wRoot = GetRootWidget();
		m_ListBoxOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox0")); 
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		string DiagText;
		int entryamount;
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
		CharRank = rank;
		PLrank = m_sPlRank;
		
		AssignIcons(faction, Plfaction);
		
		if(m_IDComp.GetRep() > 50)
		{
			m_PlayerRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PlayerRep.SetColor(Color.DarkGreen);
		}
		else if(m_IDComp.GetRep() <= 50 && m_IDComp.GetRep() > 20)
		{
			m_PlayerRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PlayerRep.SetColor(Color.DarkYellow);
		}
		else if(m_IDComp.GetRep() <= 20)
		{
			m_PlayerRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("ReputationColor"));
			m_PlayerRep.SetColor(Color.DarkRed);
		}
		if(m_CharIDComp.GetRep() > 50)
		{
			m_CharacterRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("CharReputationColor"));
			m_CharacterRep.SetColor(Color.DarkGreen);
		}
		else if(m_CharIDComp.GetRep() <= 50 && m_CharIDComp.GetRep() > 20)
		{
			m_CharacterRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("CharReputationColor"));
			m_CharacterRep.SetColor(Color.DarkYellow);
		}
		else if(m_CharIDComp.GetRep() <= 20)
		{
			m_CharacterRep = PanelWidget.Cast(m_wRoot.FindAnyWidget("CharReputationColor"));
			m_CharacterRep.SetColor(Color.DarkRed);
		}
		
		m_CharacterName = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterName"));
		m_CharacterName.SetText(CharName);
		
		m_CharacterRank = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterRank"));
		m_CharacterRank.SetText(CharRank);
		
		m_PlayerName = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerName0"));
		m_PlayerName.SetText(PlName);

		m_PlayerRank = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerRank0"));
		m_PlayerRank.SetText(m_sPlRank);
		
		for (int i = 0; i < 7; i++)
		{
			DiagComp.GetActionName(i, myCallerEntity, Player, DiagText);
			if (DiagText != STRING_EMPTY)
			{
				m_ListBoxComponent.AddItem(DiagText);
				CurrentBranchID = i;
				SCR_ListBoxElementComponent elComp = m_ListBoxComponent.GetElementComponent(entryamount);
				switch(i)
				{
					case 0:
					elComp.m_OnClicked.Insert(ExecuteDialogue0);
					break;
					case 1:
					elComp.m_OnClicked.Insert(ExecuteDialogue1);
					break;
					case 2:
					elComp.m_OnClicked.Insert(ExecuteDialogue2);
					break;
					case 3:
					elComp.m_OnClicked.Insert(ExecuteDialogue3);
					break;
					case 4:
					elComp.m_OnClicked.Insert(ExecuteDialogue4);
					break;
					case 5:
					elComp.m_OnClicked.Insert(ExecuteDialogue5);
					break;
					case 6:
					elComp.m_OnClicked.Insert(ExecuteDialogue6);
					break;
				}
				string entrynumber = (entryamount + 1).ToString();
				if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
				{
					elComp.SetTextNumber(GetGamepadButtonText(entryamount));
				}
				else
				{
					elComp.SetTextNumber(entrynumber);
				}
				
				DiagText = STRING_EMPTY;
				string actionname = "Dialogue" + entryamount.ToString();
				switch(i)
				{
					case 0:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue0);
					break;
					case 1:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue1);
					break;
					case 2:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue2);
					break;
					case 3:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue3);
					break;
					case 4:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue4);
					break;
					case 5:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue5);
					break;
					case 6:
					GetGame().GetInputManager().AddActionListener(actionname, EActionTrigger.DOWN, ExecuteDialogue6);
					break;
				}
				entryamount = entryamount + 1;
			}
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
	string GetDialogueActionName(int index)
	{
	    return "Dialogue" + index.ToString();
	}
	string GetGamepadButtonText(int index)
	{
	    switch (index)
	    {
	        case 1:
	            return "A";
			break;
	        case 2:
	            return "X";
			break;
	        case 3:
	            return "Y";
			break;
	        case 4:
	            return "D-Pad Up";
			break;
	        case 5:
	            return "D-Pad Down";
			break;
	        case 6:
	            return "D-Pad Left";
			break;
	        case 7:
	            return "D-Pad Right";
			break;
	        default:
	            return STRING_EMPTY;
			break;
	    }
		return STRING_EMPTY;
	}
	void AssignIcons(FactionKey factioncharacter, FactionKey factionplayer)
	{
		m_CharacterFactionIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("FactionIcon"));
		m_PlayerFactionIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("PlayerFactionIcon0"));
		if (m_CharacterFactionIcon)
		{
			switch(factioncharacter)
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
			switch(factionplayer)
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
	}
}