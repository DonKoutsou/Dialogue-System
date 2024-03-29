class DialogueUIClass: ChimeraMenuBase
{ 
	//----------------------------------------------------------------//
	//UI Widgets
	protected Widget m_wRoot; 
	OverlayWidget m_ListBoxOverlay;
	OverlayWidget m_ListBoxOverlayHistory;
	TextWidget m_CharacterName;
	TextWidget m_PlayerName;
	SCR_ListBoxElementComponent m_ListBoxElement;
 	DS_ListBoxComponent m_ListBoxComponent;
	SCR_ListBoxComponent m_ListBoxComponentHistory;
	
    //----------------------------------------------------------------//
	//PlayerCharacter
	IEntity 										myUserEntity;           
	//----------------------------------------------------------------//
	//Charactaer we are talking to
	IEntity 										myCallerEntity;
	string 											CharName;
	//----------------------------------------------------------------//
	//DialogueStystem
	DS_DialogueComponent 						DiagComp;
	DS_DialogueArchetype 						DiagArch;
	SCR_CharacterIdentityComponent	m_IDComp;
	SCR_CharacterIdentityComponent	m_CharIDComp;
	protected BaseGameMode 					GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
	int 														CurrentBranchID;

    //------------------------------------------------------------------------------------------------//
	override void OnMenuUpdate(float tDelta)
	{
		bool ctxActive = GetGame().GetInputManager().IsContextActive("DialogueMenuContext");
		m_OnTextEditContextChange.Invoke(ctxActive);

		// Invoke OnUpdate
		m_OnUpdate.Invoke(tDelta);
		GetGame().GetInputManager().ActivateContext("DialogueMenuContext");
		super.OnMenuUpdate(tDelta);
	}
	void RemoveListeners()
	{
		for (int i = 0; i < m_ListBoxComponent.GetItemCount(); i++)
		{
			GetGame().GetInputManager().RemoveActionListener(string.Format("Dialogue%1", i), EActionTrigger.DOWN, DS_ListBoxElementComponent.Cast(m_ListBoxComponent.GetElementComponent(i)).OnKeyPressed);
		}
		GetGame().GetInputManager().RemoveActionListener("DialogueBack", EActionTrigger.DOWN, DoDialogueBack);
		GetGame().GetInputManager().RemoveActionListener("DialogueBack", EActionTrigger.DOWN, LeaveFunction);
	}
	void Init(IEntity Owner, IEntity User)
	{
		myCallerEntity = Owner;
		myUserEntity = User;
		m_wRoot = GetRootWidget();
		m_ListBoxOverlay = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox0")); 
		//m_ListBoxOverlayHistory = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ListBox1")); 
		m_ListBoxComponent = DS_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(DS_ListBoxComponent));
		//m_ListBoxComponentHistory = SCR_ListBoxComponent.Cast(m_ListBoxOverlayHistory.FindHandler(SCR_ListBoxComponent));
		DiagComp = DS_DialogueComponent.Cast(GameMode.FindComponent(DS_DialogueComponent));
		m_IDComp = SCR_CharacterIdentityComponent.Cast(User.FindComponent(SCR_CharacterIdentityComponent));
		m_CharIDComp = SCR_CharacterIdentityComponent.Cast(myCallerEntity.FindComponent(SCR_CharacterIdentityComponent));
	}
	//------------------------------------------------------------------------------------------------//
	void UpdateEntries(IEntity Character, IEntity Player, bool AllowLeave = 1)
	{
		Init(Character, Player);
		string DiagText;
		int entryamount;
		if(myCallerEntity)
		{
			CharName = DiagComp.GetCharacterName(myCallerEntity);
		}

		array <string> a_texthistory = new array <string>();
		array <string> a_PLtexthistory = new array <string>();
		DiagComp.GetTextHistory(a_texthistory, a_PLtexthistory);
	
		for (int i = 0; i < 7; i++)
		{
			DiagComp.GetActionName(i, myCallerEntity, Player, DiagText);
			
			if (DiagText != STRING_EMPTY)
			{
				
				
				
				m_ListBoxComponent.AddItem(DiagText);
				CurrentBranchID = i;
				DS_ListBoxElementComponent elComp = DS_ListBoxElementComponent.Cast(m_ListBoxComponent.GetElementComponent(entryamount));
				elComp.branch = i;
				elComp.m_OnClicked.Insert(ExecuteDialogue);
				string entrynumber = (entryamount + 1).ToString();
				if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
				{
					elComp.SetTextNumber(GetGamepadButtonText(entryamount));
				}
				else
				{
					elComp.SetTextNumber(entrynumber);
				}
				
				bool canbeperf;
				DiagComp.GetCanBePerformed(i, myCallerEntity, Player, canbeperf);
				elComp.SetCanBePerformed(canbeperf);
				
				DiagText = STRING_EMPTY;
				GetGame().GetInputManager().AddActionListener(string.Format("Dialogue%1", entryamount.ToString()), EActionTrigger.DOWN, elComp.OnKeyPressed);
				entryamount = entryamount + 1;
			}
		}
		if (!AllowLeave)
			return;
		//Check if Archtype is branched an choose to create a Leave button or a Go Back button
		DS_DialogueArchetype DArch = DiagComp.LocateDialogueArchetype(myCallerEntity, Player);
		if (DArch.IsCharacterBranched == true)
		{
			m_ListBoxComponent.AddItem("Go Back");
			DS_ListBoxElementComponent elComp7 = DS_ListBoxElementComponent.Cast(m_ListBoxComponent.GetElementComponent(entryamount));
			elComp7.m_OnClicked.Insert(DoDialogueBack);
			if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp7.SetTextNumber("B");
			}
			else
			{
				elComp7.SetTextNumber("[TAB]");
			}
			GetGame().GetInputManager().AddActionListener("DialogueBack", EActionTrigger.DOWN, DoDialogueBack);
			return;
		}
		m_ListBoxComponent.AddItem("Leave");
		DS_ListBoxElementComponent elComp7 = DS_ListBoxElementComponent.Cast(m_ListBoxComponent.GetElementComponent(entryamount));
		elComp7.m_OnClicked.Insert(LeaveFunction);
		if(GetGame().GetInputManager().GetLastUsedInputDevice() == EInputDeviceType.GAMEPAD)
			{
				elComp7.SetTextNumber("B");
			}
			else
			{
				elComp7.SetTextNumber("[TAB]");
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
	void ExecuteDialogue(SCR_ListBoxElementComponent ListboxElement)
	{
		DS_ListBoxElementComponent listbox = DS_ListBoxElementComponent.Cast(ListboxElement);
		RemoveListeners();
		string diagname = DiagComp.DoDialogue(myCallerEntity, myUserEntity, listbox.branch);
	}
	//------------------------------------------------------------------------------------------------//
	void DoDialogueBack()
	{
		RemoveListeners();
		DiagComp.DoBackDialogue(myCallerEntity, myUserEntity);
	}
	
	//------------------------------------------------------------------------------------------------//
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
}