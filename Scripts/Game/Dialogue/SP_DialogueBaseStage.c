[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class SP_DialogueBaseStage
{
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Action Text", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Dialogue Text", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Branch, if present will cause branch to split instead of progressing its stage. When a branch splits, the dialogue system will only look in the entries of this branch only")]
	private ref array<ref SP_DialogueEntry> m_Branch;
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
	//------------------------------------------------------------------//
	void InheritData(SP_DialogueArchetype Archetype, DialogueEntryInfo Config)
	{
		if(m_Branch && m_Branch.Count() > 0)
			{
				foreach (int i, SP_DialogueEntry DiagBranch: m_Branch)
				{
					DiagBranch.InheritData(Archetype, Config)
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
	//Returns branch using ID. 
	SP_DialogueEntry GetBranch(int BranchID)
	{
		if(m_Branch[BranchID])
		{
			return m_Branch[BranchID];
		}
		return null;
	}
	//------------------------------------------------------------------//
	//Returns dialogue text
	string GetDialogueText()
	{
	 return DialogueText;
	}
};
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