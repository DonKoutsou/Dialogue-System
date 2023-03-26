[BaseContainerProps(configRoot: true)]
class SP_CharacterArchetype
{
	[Attribute()]
	protected int m_sCharacterID;
	[Attribute()]
	protected ref array<ref SP_DialogueConfig> DialogueConfig;
	
	protected ref map<int, ref SP_DialogueConfig> DialogueConfigMap;
    
	
	protected SP_DialogueComponent DiagComp;
	protected int CharacterDiagStage = 1;
	protected int CharacterDiagStageB1 = 1;
	protected int CharacterDiagStageB2 = 1;
	
	bool GetCharacterID()
	{
		return m_sCharacterID;
	}
	void Init()
	{	
		DialogueConfigMap = new map<int, ref SP_DialogueConfig>();
		for (int i, count = DialogueConfig.Count(); i < count; i++)
        {
			int key = (DialogueConfig[i].GetDialogueStageKey() << 16) | (DialogueConfig[i].GetDialogueBranchKey() << 8) | DialogueConfig[i].IsthisMultiple();
        	DialogueConfigMap.Insert(key, DialogueConfig[i]);
        }
	}
	
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

	bool IncrementStage(int BranchID, int incrementamount, int MultipleChoise)
	{	
		switch	(BranchID)
		{
			case 0:
			SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStage, BranchID, MultipleChoise);
			if (config != null && config.IsInfluanceGlobal() == true)
			{
				CharacterDiagStage += incrementamount;
				CharacterDiagStageB1 += incrementamount;
				CharacterDiagStageB2 += incrementamount;
				break;
			}
			CharacterDiagStage += incrementamount;
				break;
			case 1:
			SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStageB1, BranchID, MultipleChoise);
			if (config != null && config.IsInfluanceGlobal() == true)
			{
				CharacterDiagStage += incrementamount;
				CharacterDiagStageB1 += incrementamount;
				CharacterDiagStageB2 += incrementamount;
				break;
			}
			CharacterDiagStageB1 += incrementamount;
				break;
			case 2:
			SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStageB2, BranchID, MultipleChoise);
			if (config != null && config.IsInfluanceGlobal() == true)
			{
				CharacterDiagStage += incrementamount;
				CharacterDiagStageB1 += incrementamount;
				CharacterDiagStageB2 += incrementamount;
				break;
			}
			CharacterDiagStageB2 += incrementamount;
                break;
		}	
		return true;
	}

	int GetDiagStage(int branchId)
	{
		if (branchId == 0)
			{
				return CharacterDiagStage;
			}
		else if (branchId == 1)
			{
				return CharacterDiagStageB1;
			}
		else if (branchId == 2)
			{
				return CharacterDiagStageB2;
			}
		return 0;
	}
	string GetDialogueText(int BranchID, bool MultipleChoise)
	{
		string m_stexttoshow;
		switch (BranchID)
			{
				case 0:
					SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStage, BranchID, MultipleChoise);
					if (config != null)
					{
						m_stexttoshow = config.GetDialogueText(CharacterDiagStage);
					}
					break;
				case 1:
					SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStageB1, BranchID, MultipleChoise);
					if (config != null)
					{
						m_stexttoshow = config.GetDialogueText(CharacterDiagStageB1);
					}
					break;	
				case 2:
					SP_DialogueConfig config = GetDialogueConfig(CharacterDiagStageB2, BranchID, MultipleChoise);
					if (config != null)
					{
						m_stexttoshow = config.GetDialogueText(CharacterDiagStageB2);
					}
					break;
			}	
		return m_stexttoshow;
	}
	string GetActionTitle(int BranchID, bool MultipleChoise)
	{
		SP_DialogueConfig config;
		string m_sActionTitle;
		switch (BranchID)
			{
				case 0:
					config = GetDialogueConfig(CharacterDiagStage, BranchID, MultipleChoise);
					if (config != null)
					{
						m_sActionTitle = config.GetActionText(CharacterDiagStage);
					}
					break;
				case 1:
					config = GetDialogueConfig(CharacterDiagStageB1, BranchID, MultipleChoise);
					if (config != null)
					{
						m_sActionTitle = config.GetActionText(CharacterDiagStageB1);
					}
					break;	
				case 2:
					config = GetDialogueConfig(CharacterDiagStageB2, BranchID, MultipleChoise);
					if (config != null)
					{
						m_sActionTitle = config.GetActionText(CharacterDiagStageB2);
					}
					break;	
			}
		return m_sActionTitle;
	}
};