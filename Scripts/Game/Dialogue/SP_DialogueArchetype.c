[BaseContainerProps(configRoot: true)]
class SP_DialogueArchetype
{
	[Attribute("50", UIWidgets.ComboBox, "ID of what type of identifier is going to be used", "", ParamEnumArray.FromEnum(EDiagIdentifier) )]
	protected EDiagIdentifier DialogueIdentifier;
	
	[Attribute()]
	protected int m_sCharacterID;
	
	[Attribute()]
	protected string m_sCharacterName;
	
	[Attribute()]
	protected FactionKey m_sCharacterFaction;
	
	[Attribute("50", UIWidgets.ComboBox, "Specify a rank if you want to use character rank as identifier", "", ParamEnumArray.FromEnum(ECharacterRank) )]
	protected ECharacterRank m_sCharacterRank;

	[Attribute()]
	protected ref array<ref SP_DialogueConfig> DialogueConfig;
	
	[Attribute()]
	protected ref array<ref int> CharacterDiagBranchStage;
	
	//------------------------------------------------------------------//
	
	protected ref map<int, ref SP_DialogueConfig> DialogueConfigMap;
    protected SP_DialogueComponent DiagComp;
	
	//------------------------------------------------------------------//
	
	EDiagIdentifier GetIdentifier()
	{
		return DialogueIdentifier;
	}
	//------------------------------------------------------------------//
	bool GetCharacterID()
	{
		return m_sCharacterID;
	}
	//------------------------------------------------------------------//
	string GetCharacterName()
	{
		return m_sCharacterName;
	}
	//------------------------------------------------------------------//
	FactionKey GetCharacterFaction()
	{
		return m_sCharacterFaction;
	}
	//------------------------------------------------------------------//
	ECharacterRank GetCharacterRank()
	{
		return m_sCharacterRank;
	}
	//------------------------------------------------------------------//
	void Init()
	{
		DialogueConfigMap = new map<int, ref SP_DialogueConfig>();
		for (int i, count = DialogueConfig.Count(); i < count; i++)
        {
			int key = (DialogueConfig[i].GetDialogueStageKey() << 16) | (DialogueConfig[i].GetDialogueBranchKey() << 8) | DialogueConfig[i].IsthisMultiple();
        	DialogueConfigMap.Insert(key, DialogueConfig[i]);
        }
		
	}
	//------------------------------------------------------------------//
	SP_DialogueConfig GetDialogueConfig(int StageKey, int BranchKey, bool MultiBool)
    {
		int key = (StageKey << 16) | (BranchKey << 8) | MultiBool;
        SP_DialogueConfig config;
		if (!DialogueConfigMap.Find(key, config))
		{
        	return null;
    	}
		
        return config;
    }
	//------------------------------------------------------------------//
	bool IncrementStage(int BranchID, int incrementamount, int MultipleChoise)
	{	
		SP_DialogueConfig config = GetDialogueConfig(CharacterDiagBranchStage[BranchID], BranchID, MultipleChoise);
		if (config != null && config.IsInfluanceGlobal() == true)
		{
			for (int i, count = CharacterDiagBranchStage.Count(); i < count; i++)
        	{	
				CharacterDiagBranchStage[i] = CharacterDiagBranchStage[i] + 1;	
        	}
		}
		else
			CharacterDiagBranchStage[BranchID] = CharacterDiagBranchStage[BranchID] + 1;
		return true;
	}
	//------------------------------------------------------------------//
	string GetDialogueText(int BranchID, bool MultipleChoise)
	{
		string m_stexttoshow;
		for (int i, count = CharacterDiagBranchStage.Count(); i < count; i++)
		{
			SP_DialogueConfig config = GetDialogueConfig(CharacterDiagBranchStage[BranchID], BranchID, MultipleChoise);
			if (config != null)
				{
					m_stexttoshow = config.GetDialogueText(CharacterDiagBranchStage[BranchID]);
				}
		}	
		return m_stexttoshow;
	}
	//------------------------------------------------------------------//
	string GetActionTitle(int BranchID, bool MultipleChoise)
	{
		string m_sActionTitle;
		for (int i, count = CharacterDiagBranchStage.Count(); i < count; i++)
		{
			SP_DialogueConfig config = GetDialogueConfig(CharacterDiagBranchStage[BranchID], BranchID, MultipleChoise);
			if (config != null)
				{
					m_sActionTitle = config.GetActionText(CharacterDiagBranchStage[BranchID]);
				}
		}
		return m_sActionTitle;
	}
};