class SP_RadialMenuDiags : SCR_RadialMenuHandler
{
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute("", UIWidgets.Object, "Diag config")]
	protected ref SP_Dialogues m_DialogueConfig;
	//----------------------------------------------------------------------------------------------------------------//
	IEntity TalkingChar;
	bool IsDialogueHappening = false;
	
	protected static ResourceName m_EntryLayout = "{D263AE2495703ECD}UI/layouts/Common/RadialMenu/RadialEntryElementTextOnly.layout";
	//pseudo-Init sent from action
	//setting up bool wich is used to show and hide initial Talk action

	void Start(IEntity owner, IEntity user)
	{
		IsDialogueHappening = true;
		TalkingChar = owner;
		PageSetup();
		Open(user);
		OpenMenu(user, true);
		UpdateDiag();
	}
	//Gets character you are talking to. 
	//Start void is basicly an init when radial menu is opened from dialogue action, it sets Talking character as the owner of action wich is the character you are talking to.
	IEntity GetTalkingChar()
	{
		return TalkingChar;
	}
	override void OpenMenu(IEntity owner, bool isOpen)
	{
		super.OpenMenu(owner, isOpen);
		
		if (isOpen)
		{
			m_pSource = SCR_PlayerController.GetLocalMainEntity();
			m_ActualCategory = m_RootCategory;
			PageSetup();
		}
	}
	override protected void Init(IEntity owner)
	{	
		super.Init(owner);
		
		m_pOwner = owner;
	}
	override void PageSetup()
	{
		m_aMenuPages.Clear();
		ClearEntries();
		
		foreach (SP_DiagMenuCategory DialogueCategory : m_DialogueConfig.m_aDialogueCategories)
		{
			SCR_MenuPage newPage = new SCR_MenuPage(DialogueCategory.GetCategoryName());
			m_aMenuPages.Insert(newPage);
			foreach (BaseSelectionMenuEntry entry : DialogueCategory.GetCategoryElements())
			{
				SP_DialogueSelectionMenuEntry DiagEntry = SP_DialogueSelectionMenuEntry.Cast(entry);
				DiagEntry.SetEntryLayout(m_EntryLayout);
				DiagEntry.m_sPageName = DialogueCategory.GetCategoryName();
				DialogueCategory.UpdateVisuals();
				newPage.AddEntry(DiagEntry);
			}
		}
		SetPage(0);
		UpdateEntries();
	}
	void UpdateDiag()
	{
		UpdateEntries();
	}
}