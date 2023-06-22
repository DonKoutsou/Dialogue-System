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
	ref array<ref SP_DialogueBranch> m_Branch;
	//------------------------------------------------------------------//
	string m_sCantBePerformedReason = "(Cant Be Performed)";
	//------------------------------------------------------------------//
	void Perform(IEntity Character, IEntity Player){};
	//------------------------------------------------------------------//
	bool CanBePerformed(IEntity Character, IEntity Player){return true;}
	//------------------------------------------------------------------//
	bool CanBeShown(IEntity Character, IEntity Player){return true;}
	//------------------------------------------------------------------//
	void SetCannotPerformReason(string reason){m_sCantBePerformedReason = reason;}
	//------------------------------------------------------------------//
	void GetCannotPerformReason(out string CantBePReason){CantBePReason = m_sCantBePerformedReason;}
	void SetActionText(string text)
	{
		ActionText = text;
	}
	//------------------------------------------------------------------//
	bool GetActionText(IEntity Character, IEntity Player, out string acttext)
	{
		acttext = ActionText;
		if (CanBeShown(Character, Player) == false)
		{
			acttext = STRING_EMPTY;
			return false;
		}
		if (CanBePerformed(Character, Player) == false)
		{
			acttext = ActionText + " " + m_sCantBePerformedReason;
			return true;
		}
	 	return true;
	}
	void SetDialogueText(string text)
	{
		DialogueText = text;
	}
	//------------------------------------------------------------------//
	string GetStageDialogueText(IEntity Character, IEntity Player)
	{
	 return DialogueText;
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
	void InheritData(SP_DialogueArchetype Archetype, DialogueBranchInfo Config, IEntity Char)
	{
		for (int i, count = m_Branch.Count(); i < count; i++)
		{
			m_Branch[i].InheritData(Archetype, Config, Char);
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
	bool CheckIfAnyBranchesBranch(IEntity Character, out int branchedID)
	{
		for (int i, count = m_Branch.Count(); i < count; i++)
		{
				if(m_Branch[i].CheckIfBranched(Character) == true)
				{
					branchedID = i;
					return true;
				}
		}
		return false;
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
		string branchesStr;
		array<ref SP_DialogueBranch> branches;
		source.Get("m_Branch", branches);
		if (branches.Count() > 0)
		{
			branchesStr = "Stage Branches"
		}
		else
		{
			branchesStr = "Stage Increments"
		}
		title = string.Format("Stage: %1 - %2", actionName, branchesStr);
		return true;
	}
}