//Dialogue Configuration Entry, contains dialogue and action name for specified branch and stage.
//If the dialogue branch ID and stage ID match this dialogue it should show up on the specified character archetype.
[BaseContainerProps(configRoot:true)]
class SP_DialogueBranch
{
	//------------------------------------------------------------------//
	//if this configuration should progress the stage of all the branches on their character archetype
	[Attribute(defvalue: "", desc: "Should this dialogue progress all branches when executed?", category: "Dialogue",  )]
	protected bool m_bGlobalStageInfluance;
	[Attribute(desc: "Filtered selection test")]
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
		if (m_DialogueTexts[DialogueBranchStage] && m_DialogueTexts.GetRefCount() >= DialogueBranchStage)
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
		if (m_DialogueTexts[DialogueBranchStage] && m_DialogueTexts.GetRefCount() >= DialogueBranchStage)
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
[BaseContainerProps(configRoot:true), DialogueTextConfigTitleAttribute()]
class DialogueTextConfig
{
	//Behavior that this choice is going to use once performed. //eg. "Accept&close" takes the selection made by player and then closes radial menu, while "Stay" keep the menu open so player can keep using it
	[Attribute("0", UIWidgets.ComboBox, "What is going to be the behavior of radial menu choise", "", ParamEnumArray.FromEnum(EChoiseBehavior) )]
	protected EChoiseBehavior ChoiseBehavior;
	[Attribute(defvalue: "Action Text", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	[Attribute(defvalue: "Dialogue Text", desc: "Dialogue Text", category: "Dialogue")]
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
	//Getter for choice behavior Enum
	EChoiseBehavior GetChoiseBehavior()
	{
		return ChoiseBehavior;
	}
}
class DialogueTextConfigTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		// Check if there is input in m_MultipleChoiceConfig
		bool hasMultiChoiceConfig = false;
		SP_MultipleChoiceConfig ChoiceConf;
		if (source.Get("m_MultipleChoiceConfig", ChoiceConf))
		{
			SP_MultipleChoiceConfig multiChoiceConfig;
			source.Get("m_MultipleChoiceConfig", multiChoiceConfig);
			hasMultiChoiceConfig = (multiChoiceConfig != null);
		}
		// Generate string according to enabled state
		string strEnabled;
		SP_MultipleChoiceConfig multiChoiceConfig;
		source.Get("m_MultipleChoiceConfig", multiChoiceConfig);
		if (multiChoiceConfig)
			strEnabled = "[X]";
		else
			strEnabled = "[  ]";

		// Get ActionName
		string actionName;
		source.Get("ActionText", actionName);

		// Get selected behavior from EChoiseBehavior enum
		string behaviorStr;
		EChoiseBehavior behavior;
		source.Get("ChoiseBehavior", behavior);
		behaviorStr = typename.EnumToString(EChoiseBehavior, behavior);
		title = string.Format("Branch: %1 : %2 : %3", actionName, behaviorStr, strEnabled);


		return true;
	}
};
