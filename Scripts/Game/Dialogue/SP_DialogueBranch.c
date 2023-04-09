//Dialogue Configuration Entry, contains dialogue and action name for specified branch and stage.
//If the dialogue branch ID and stage ID match this dialogue it should show up on the specified character archetype.
[BaseContainerProps(configRoot: true)]
class SP_DialogueBranch: ScriptAndConfig
{
	//------------------------------------------------------------------//
	//if this configuration should progress the stage of all the branches on their character archetype
	[Attribute(defvalue: "", desc: "Should this dialogue progress all branches when executed?", category: "Dialogue",  )]
	protected bool m_bGlobalStageInfluance;
	[Attribute(defvalue: "", desc: "Action Title and Dialogue Text for Radial menu choice", category: "Dialogue")]
	ref array<ref DialogueTextConfig> m_DialogueTexts;
	int DialogueBranchStage;
	void ResetBranchStage()
	{
		DialogueBranchStage = 0;
	}
	//------------------------------------------------------------------//
	//Text that is going to be used as title for the action
	string GetActionText()
	{
		string ActText;
		if (m_DialogueTexts.GetRefCount() >= DialogueBranchStage)
		{
			ActText = m_DialogueTexts[DialogueBranchStage].GetActionText();
		}
		else
		{
			ActText = STRING_EMPTY;
		}
		
		return ActText;
	}
	string GetMultiActionText(int TextKey)
	{
		string ActText = m_DialogueTexts[DialogueBranchStage].GetMultiActionText(TextKey);
		return ActText;
	}
	string GetMultiDialogueText(int TextKey)
	{
		string Diagtext = m_DialogueTexts[DialogueBranchStage].GetMultiDialogueText(TextKey);
		return Diagtext;
	}
	//------------------------------------------------------------------//
	//Text that is going to be sent to chat
	string GetDialogueText()
	{
		string DiagText;
		if ( m_DialogueTexts.GetRefCount() >= DialogueBranchStage)
		{
			DiagText = m_DialogueTexts[DialogueBranchStage].GetDialogueText();
		}
		else
		{
			DiagText = STRING_EMPTY;
		}
		return DiagText;
	}
	//------------------------------------------------------------------//
	//Boolean about if this configuration should progress all branches when used
	bool IsInfluanceGlobal()
	{
		return m_bGlobalStageInfluance;
	}
	DialogueTextConfig GetDialogueTextConfig()
	{
		return m_DialogueTexts[DialogueBranchStage];
	}
	void IncrementConfigStage(int incrementamount)
	{
		DialogueBranchStage = DialogueBranchStage + 1;
	}
	bool CheckIfTextConfigBranches()
	{
		if(m_DialogueTexts[DialogueBranchStage].GetMultipleChoiceConfig())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
};
class DialogueTextConfig: ScriptAndConfig
{
	[Attribute(defvalue: "", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	[Attribute(defvalue: "", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	[Attribute()]
	protected ref SP_MultipleChoiceConfig m_MultipleChoiceConfig;
	//------------------------------------------------------------------//
	//Getter for radial configuration file marking this config as branching and that it should open radial menu
	SP_MultipleChoiceConfig GetMultipleChoiceConfig()
	{
		if(m_MultipleChoiceConfig == null)
		{
			return null;
		}
		else
			return m_MultipleChoiceConfig;
		
	}
	string GetActionText()
	{
	 return ActionText;
	}
	string GetMultiActionText(int TextKey)
	{
		string ActText
		if(m_MultipleChoiceConfig)
		{
			ActText = m_MultipleChoiceConfig.GetActionText(TextKey);	
		}
		else
		{
			ActText = STRING_EMPTY;
		}
		
		return ActText;
	}
	string GetDialogueText()
	{
	 return DialogueText;
	}
	string GetMultiDialogueText(int TextKey)
	{
		string DiagText;
		if(m_MultipleChoiceConfig)
		{
			DiagText = m_MultipleChoiceConfig.GetDialogueText(TextKey);
		}
		else
		{
			DiagText = STRING_EMPTY;
		}
	 	return DiagText;
	}
}
