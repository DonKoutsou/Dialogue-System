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
	SCR_ListBoxElementComponent m_ListBoxElement;
    SCR_ListBoxComponent m_ListBoxComponent;
    IEntity myCallerEntity;
	IEntity myUserEntity;           
    string CharName;
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
		}
		m_CharacterName = TextWidget.Cast(m_wRoot.FindAnyWidget("CharacterName"));
		m_CharacterName.SetText(CharName);
		
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
		elComp3.m_OnClicked.Insert(myCustomFunction);
	} 
    void myCustomFunction()
    {
		SP_DialogueArchetype DArch = DiagComp.LocateCharacterArchetype(myCallerEntity);
			if (DArch.IsCharacterBranched == true)
			{
				UpdateEntries();
				DArch.IsCharacterBranched = false;
			}
			else
		{
			GetGame().GetMenuManager().CloseAllMenus();
		}
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