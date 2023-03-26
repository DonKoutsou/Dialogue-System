[BaseContainerProps(configRoot: true)]
class SP_CharacterArchetype
{
	[Attribute()]
	protected int m_sCharacterID;
	[Attribute()]
	protected ref array<ref SP_DialogueConfig> DialogueConfig;
	
	protected SP_DialogueComponent DiagComp;
	static SP_CharacterArchetype CharacterArchetype;
	protected int CharacterDiagStage = 0;
	protected int CharacterDiagStageB1 = 0;
	protected int CharacterDiagStageB2 = 0;
	
	bool GetCharacterID()
	{
		return m_sCharacterID;
	}
	bool IncrementStage(int BranchID, int incrementamount, int MultipleChoise)
	{
		foreach (SP_DialogueConfig config: DialogueConfig)
		{
				if (config.GetDialogueBranchKey() == BranchID)
					{
						switch (BranchID)
							{
								case 0:
									if (config.GetDialogueStageKey() == CharacterDiagStage)
										{
											if(config.IsInfluanceGlobal() == true)
											{
												CharacterDiagStage += incrementamount;
										        CharacterDiagStageB1 += incrementamount;
										        CharacterDiagStageB2 += incrementamount;
												return true;
											}	
										}
									break;
								case 1:
					                if (config.GetDialogueStageKey() == CharacterDiagStageB1)
										{
											if(config.IsInfluanceGlobal() == true)
											{
												CharacterDiagStage += incrementamount;
										        CharacterDiagStageB1 += incrementamount;
										        CharacterDiagStageB2 += incrementamount;
												return true;
											}	
										}
					                break;
					            case 2:
					                if (config.GetDialogueStageKey() == CharacterDiagStageB2)
										{
											if(config.IsInfluanceGlobal() == true)
											{
												CharacterDiagStage += incrementamount;
										        CharacterDiagStageB1 += incrementamount;
										        CharacterDiagStageB2 += incrementamount;
												return true;
											}	
										}
					                break;
							} 
					}
		}
		switch (BranchID)
		{
			case 0:
				CharacterDiagStage += incrementamount;
				break;
			case 1:
                CharacterDiagStageB1 += incrementamount;
                break;
            case 2:
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
		foreach (SP_DialogueConfig config: DialogueConfig)
		{
			if(config.IsthisMultiple() == MultipleChoise)
			{
				if(config.GetDialogueBranchKey() == BranchID)
				{
					switch (BranchID)
					{
						case 0:
							if (config.GetDialogueStageKey() == CharacterDiagStage)
							{
								m_stexttoshow = config.GetDialogueText(CharacterDiagStage);
							}
							break;
						case 1:
							if (config.GetDialogueStageKey() == CharacterDiagStageB1)
							{
								m_stexttoshow = config.GetDialogueText(CharacterDiagStageB1);
							}
							break;	
						case 2:
							if (config.GetDialogueStageKey() == CharacterDiagStageB2)
							{
								m_stexttoshow = config.GetDialogueText(CharacterDiagStageB2);
							}
							break;	
					}
				}
			}	
		}
		
		return m_stexttoshow;
	}
	string GetActionTitle(int BranchID, bool MultipleChoise)
	{
		string m_sActionTitle;
		foreach (SP_DialogueConfig config: DialogueConfig)
		{
			if(config.IsthisMultiple() == MultipleChoise)
			{
				if(config.GetDialogueBranchKey() == BranchID)
				{
					switch (BranchID)
					{
						case 0:
							if (config.GetDialogueStageKey() == CharacterDiagStage)
							{
								m_sActionTitle = config.GetActionText(CharacterDiagStage);
							}
							break;
						case 1:
							if (config.GetDialogueStageKey() == CharacterDiagStageB1)
							{
								m_sActionTitle = config.GetActionText(CharacterDiagStageB1);
							}
							break;	
						case 2:
							if (config.GetDialogueStageKey() == CharacterDiagStageB2)
							{
								m_sActionTitle = config.GetActionText(CharacterDiagStageB2);
							}
							break;	
					}
				}
			}	
		}
		return m_sActionTitle;
	}
};