//Dialogue Branch is used to store a menu entry. In any dialogue branch a multiple stages can be concifgured.
//In Dialogue branch the dialogues progession can be configured, at any stage a Multiple choice config can be introduces to create another branch.
//Each branch tracks its own stage
[BaseContainerProps(configRoot:true),  DialogueBranchConfigTitleAttribute()]
class SP_DialogueBranch
{
	//------------------------------------------------------------------//
	//Array containing the texts of each stage this branch will have
	[Attribute()]
	ref array<ref DialogueStageConfig> m_BranchStageConfig;
	//Stage of this config
	int DialogueBranchStage;
	
	//------------------------------------------------------------------//
	//Text that is going to be used as title for the action
	string GetActionText()
	{
		string ActText;
		//check if this config is out of texts(stage has progressed further than the available entries
		if (m_BranchStageConfig[DialogueBranchStage] && m_BranchStageConfig.GetRefCount() >= DialogueBranchStage)
		{
			//Go in config number (current stage) and look for the action text
			ActText = m_BranchStageConfig[DialogueBranchStage].GetActionText();
		}
		else
		{
			//else return emty string
			ActText = STRING_EMPTY;
		}
		
		return ActText;
	}
	//------------------------------------------------------------------//
	//Used to take the action title from SP_MultipleChoiceConfig inside the branch instead of the DialogueStageConfig
	string GetMultiActionText(int TextKey)
	{
		string ActText = m_BranchStageConfig[DialogueBranchStage].GetMultiActionText(TextKey);
		return ActText;
	}
	//------------------------------------------------------------------//
	//Text that is going to be sent in the chat
	string GetDialogueText()
	{
		string DiagText;
		//check if this config is out of texts(stage has progressed further than the available entries
		if (m_BranchStageConfig[DialogueBranchStage] && m_BranchStageConfig.GetRefCount() >= DialogueBranchStage)
		{
			//Go in config number (current stage) and look for the action text
			DiagText = m_BranchStageConfig[DialogueBranchStage].GetDialogueText();
		}
		else
		{
			//else return emty string
			DiagText = STRING_EMPTY;
		}
		return DiagText;
	}
	//------------------------------------------------------------------//
	//Used to take the dialogue text from SP_MultipleChoiceConfig inside the branch instead of the DialogueStageConfig
	string GetMultiDialogueText(int TextKey)
	{
		string Diagtext = m_BranchStageConfig[DialogueBranchStage].GetMultiDialogueText(TextKey);
		return Diagtext;
	}
	//------------------------------------------------------------------//
	DialogueStageConfig GetDialogueStageConfig()
	{
		return m_BranchStageConfig[DialogueBranchStage];
	}
	//------------------------------------------------------------------//
	//Increments stage of this branch
	void IncrementConfigStage(int incrementamount)
	{
		DialogueBranchStage = DialogueBranchStage + 1;
	}
	//------------------------------------------------------------------//
	//Checks if a SP_MultipleChoiceConfig exists in the current DialogueStageConfig
	bool CheckIfTextConfigBranches()
	{
		if(m_BranchStageConfig[DialogueBranchStage].GetMultipleChoiceConfig())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------//
	//resets stage of this branch
	void ResetBranchStage()
	{
		DialogueBranchStage = 0;
	}
	
};
[BaseContainerProps(configRoot:true), DialogueStageConfigTitleAttribute()]
class DialogueStageConfig
{
	//------------------------------------------------------------------//
	//Behavior that this choice is going to use once performed. //eg. "IncrementDialogueStage" increments the stage of the branch and progresses further down the dialogue, while "Stay" keep the menu open so player can keep using it
	[Attribute("0", UIWidgets.ComboBox, "What is going to be the behavior of this choice", "", ParamEnumArray.FromEnum(EChoiseBehavior) )]
	protected EChoiseBehavior ChoiseBehavior;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Action Text", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Dialogue Text", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	//------------------------------------------------------------------//
	[Attribute()]
	protected ref SP_MultipleChoiceConfig m_MultipleBranchConfig;
	//------------------------------------------------------------------//
	//Returns the m_MultipleBranchConfig hooked in this DialogueStageConfig.
	SP_MultipleChoiceConfig GetMultipleChoiceConfig()
	{
		if(m_MultipleBranchConfig == null)
		{
			return null;
		}
		else
			return m_MultipleBranchConfig;
	}
	//------------------------------------------------------------------//
	//Get action text from DialogueStageConfig
	string GetActionText()
	{
	 return ActionText;
	}
	//------------------------------------------------------------------//
	//Get action text from SP_MultipleChoiceConfig, text key chooses the entry. TextKey = 0 will take the first entry in SP_MultipleChoiceConfig
	string GetMultiActionText(int TextKey)
	{
		string ActText
		if(m_MultipleBranchConfig)
		{
			ActText = m_MultipleBranchConfig.GetActionText(TextKey);	
		}
		else
		{
			ActText = STRING_EMPTY;
		}
		return ActText;
	}
	//------------------------------------------------------------------//
	//Get dialogue text from DialogueStageConfig
	string GetDialogueText()
	{
	 return DialogueText;
	}
	//------------------------------------------------------------------//
	//Get dialogue text from SP_MultipleChoiceConfig, text key chooses the entry. TextKey = 0 will take the first entry in SP_MultipleChoiceConfig
	string GetMultiDialogueText(int TextKey)
	{
		string DiagText;
		if(m_MultipleBranchConfig)
		{
			DiagText = m_MultipleBranchConfig.GetDialogueText(TextKey);
		}
		else
		{
			DiagText = STRING_EMPTY;
		}
	 	return DiagText;
	}
	//------------------------------------------------------------------//
	//Getter for choice behavior Enum
	EChoiseBehavior GetChoiseBehavior()
	{
		return ChoiseBehavior;
	}
}
//---------------------------------------------------------------------------------------------------//
class DialogueStageConfigTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		// Check if there is input in m_MultipleBranchConfig
		bool hasMultiChoiceConfig = false;
		SP_MultipleChoiceConfig ChoiceConf;
		if (source.Get("m_MultipleBranchConfig", ChoiceConf))
		{
			SP_MultipleChoiceConfig multiChoiceConfig;
			source.Get("m_MultipleBranchConfig", multiChoiceConfig);
			hasMultiChoiceConfig = (multiChoiceConfig != null);
		}
		// Generate string according to enabled state
		string strEnabled;
		SP_MultipleChoiceConfig multiChoiceConfig;
		source.Get("m_MultipleBranchConfig", multiChoiceConfig);
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
		title = string.Format("Stage: %1 - %2 - %3", actionName, behaviorStr, strEnabled);


		return true;
	}
};
class DialogueBranchConfigTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		title = string.Format("Branch");


		return true;
	}
};