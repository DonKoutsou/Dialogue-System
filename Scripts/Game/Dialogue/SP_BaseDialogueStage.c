[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStage
{
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Action Text", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Dialogue Text", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Branch, if present will cause branch to split instead of progressing its stage. When a branch splits, the dialogue system will only look in the entries of this branch only")]
	private ref array<ref SP_DialogueBranch> m_Branch;
	//------------------------------------------------------------------//
	//Get action text from DialogueStage
	void Perform(IEntity Character, IEntity Player)
	{
	};
	//------------------------------------------------------------------//
	string GetActionText()
	{
	 return ActionText;
	}
	void InheritData(SP_DialogueArchetype Archetype, DialogueBranchInfo Config, IEntity Char)
	{
		if(m_Branch && m_Branch.Count() > 0)
			{
				foreach (int i, SP_DialogueBranch DiagBranch: m_Branch)
				{
					DiagBranch.InheritData(Archetype, Config,Char )
				}
			}
	}
	
	//------------------------------------------------------------------//
	bool CheckIfStageCanBranch()
	{
		if (m_Branch.Count() > 0)
		{
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------//
	SP_DialogueBranch GetBranch(int BranchID)
	{
		if(m_Branch.Count() >= BranchID + 1)
		{
			return m_Branch[BranchID];
		}
		return null;
	}
	//------------------------------------------------------------------//
	//Get action text from SP_MultipleChoiceConfig, text key chooses the entry. TextKey = 0 will take the first entry in SP_MultipleChoiceConfig
	string GetStageBranchActionText(int TextKey, IEntity Character)
	{
		string ActText
		if(m_Branch[TextKey])
		{
			ActText = m_Branch[TextKey].GetActionText(Character);	
		}
		else
		{
			ActText = STRING_EMPTY;
		}
		return ActText;
	}
	//------------------------------------------------------------------//
	//Get dialogue text from DialogueStage
	string GetDialogueText()
	{
	 return DialogueText;
	}
	//------------------------------------------------------------------//
	//Get dialogue text from SP_MultipleChoiceConfig, text key chooses the entry. TextKey = 0 will take the first entry in SP_MultipleChoiceConfig
	string GetStageBranchDialogueText(int TextKey, IEntity Character)
	{
		string DiagText;
		if(m_Branch[TextKey])
		{
			DiagText = m_Branch[TextKey].GetDialogueText(Character);
		}
		else
		{
			DiagText = STRING_EMPTY;
		}
	 	return DiagText;
	}
}
//---------------------------------------------------------------------------------------------------//
class DialogueStageTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		// Get ActionName
		string actionName;
		source.Get("ActionText", actionName);

		// Get selected behavior from EChoiseBehavior enum
		string behaviorStr;
		EChoiseBehavior behavior;
		source.Get("ChoiseBehavior", behavior);
		behaviorStr = typename.EnumToString(EChoiseBehavior, behavior);
		title = string.Format("Stage: %1 - %2", actionName, behaviorStr,);
		return true;
	}
}