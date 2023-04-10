//Used in Dialogue Archetype, in any SP_DialogueBranch
//SP_MultipleChoiceConfig is used to store the text that is going to be used in radial menu in dialogue.
//Also  marks the config it is atatched to as branching dialogue so when that action is performed radial menu will open intead of progressing dialogue

[BaseContainerProps(configRoot: true)]
class SP_MultipleChoiceConfig: ScriptAndConfig
{
	
	//Class containing both action and dialogue text for each radial menu entry
	[Attribute(defvalue: "", desc: "Action Title and Dialogue Text for Radial menu choice", category: "Dialogue")]
	protected ref array<ref SP_DialogueBranch> m_MultiChoiceTexts;
	protected SP_DialogueComponent DiagComp;
	protected SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	SP_DialogueArchetype OriginalArchetype;
	SP_MultipleChoiceConfig ParentConfig;
	bool IsBranchBranched;
	int BranchBranchID
	SP_MultipleChoiceConfig GetPartnetConfg()
	{
		return ParentConfig;
	}
	void InheritConfig(SP_MultipleChoiceConfig Config)
	{
		ParentConfig = Config;
	}
	bool GetBranchState()
	{
		return IsBranchBranched;
	}
	void InheritArchetype(SP_DialogueArchetype Archetype)
	{
		OriginalArchetype = Archetype;
	}
	string GetActionText(int TextID)
    {
        if (TextID >= 0 && TextID < m_MultiChoiceTexts.Count()) 
        {
			if (IsBranchBranched == true)
			{	
				string acttext = m_MultiChoiceTexts[BranchBranchID].GetMultiActionText(TextID);
				return acttext;
			}
			else
			{
				string acttext = m_MultiChoiceTexts[TextID].GetActionText();
				return acttext;
			}
        }
        else 
        {
            return STRING_EMPTY;
        }
    }
	void BranchBranch(int BranchID)
	{
		if (IsBranchBranched == false)
		{
			IsBranchBranched = true;
			
			BranchBranchID = BranchID;
		}
		m_MultiChoiceTexts[BranchID].GetDialogueTextConfig().GetMultipleChoiceConfig().InheritArchetype(OriginalArchetype);
		m_MultiChoiceTexts[BranchID].GetDialogueTextConfig().GetMultipleChoiceConfig().InheritConfig(this);
		OriginalArchetype.Ping(this);
	}
	void UnbranchBranch()
	{
		if (IsBranchBranched == true)
		{
			IsBranchBranched = false;
			m_MultiChoiceTexts[BranchBranchID].ResetBranchStage();
			BranchBranchID = STRING_EMPTY;
			OriginalArchetype.Ping(ParentConfig);
		}
		
	}
	
	//Get dialogue besed on ID provided
	string GetDialogueText(int TextID)
    {
        if (TextID >= 0 && TextID < m_MultiChoiceTexts.Count()) 
        {
			string diagtext;
			
			if (IsBranchBranched == true)
			{
				diagtext = m_MultiChoiceTexts[BranchBranchID].GetMultiDialogueText(TextID);
			}
			
			else
			{
				if (m_MultiChoiceTexts[TextID].CheckIfTextConfigBranches() == true)
				{
					diagtext = m_MultiChoiceTexts[TextID].GetDialogueText();
					BranchBranch(TextID);
				}
				else
				{
					EChoiseBehavior ChoiseBehavior = m_MultiChoiceTexts[TextID].GetDialogueTextConfig().GetChoiseBehavior();
					diagtext = m_MultiChoiceTexts[TextID].GetDialogueText();
					switch(ChoiseBehavior)
					{
						case 0:
						{
							m_MultiChoiceTexts[TextID].IncrementConfigStage(1);
						}
						break;
						case 1:
						{
							
						};
						break;
					}
					
				}
				
				
					
			}
			
				
			return diagtext;
            
        }
        else
        {
            return STRING_EMPTY;
        }
    }
	
	
};
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueMultiTextConfig
{
	[Attribute(defvalue: "", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	[Attribute(defvalue: "", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	string GetActionText()
	{
	 return ActionText;
	}
	string GetDialogueText()
	{
	 return DialogueText;
	}
}

