[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DS_DialogueStage
{
	
	[Attribute(desc: "Action Title", category: "Dialogue")]
	ref DS_BaseDialogueText m_sActionText;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc: "Dialogue Text", category: "Dialogue")]
  ref DS_BaseDialogueText m_sDialogueText;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc : "Action that will be performed ")]
  ref DS_BaseDialogueStageAction m_DialogueAction;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc : "Condition for action to be performable ")]
  ref DS_BaseDialogueStageActionCondition m_DialogueActionCondition;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc : "Should hide option if not performable")]
	bool m_bHideIfCantBePerformed;
	
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Branch, if present will cause branch to split instead of progressing its stage. When a branch splits, the dialogue system will only look in the entries of this branch only")]
	ref array<ref DS_DialogueBranch> m_Branch;
	
	//------------------------------------------------------------------//
	DS_DialogueBranch m_Owner;
	int m_iIndex;
	
	void Perform(IEntity Character, IEntity Player){if (m_DialogueAction) {m_DialogueAction.Perform(Character, Player)}};
	
	//------------------------------------------------------------------//
	
	bool CanBePerformed(IEntity Character, IEntity Player)
	{
		if (m_DialogueActionCondition)
		{
			return m_DialogueActionCondition.CanBePerformed(Character, Player);
		} 
		else 
			return true;
	}
	//------------------------------------------------------------------//
	
	bool CanBeShown(IEntity Character, IEntity Player)
	{
		if (!CanBePerformed(Character, Player))
			return m_bHideIfCantBePerformed; 
		return true;
	}
	
	//------------------------------------------------------------------//
	
	void GetCannotPerformReason(out string CantBePReason)
	{
		m_DialogueActionCondition.GetCannotPerformReason(CantBePReason);
	}
	
	//void SetActionText(string text)
	//{
	//	m_sActionText.SetText(text);
	//}
	
	//------------------------------------------------------------------//
	
	bool GetActionText(IEntity Character, IEntity Player, out string acttext)
	{
		acttext = m_sActionText.GetText(Character, Player);
		if (!CanBeShown(Character, Player))
		{
			acttext = STRING_EMPTY;
			return false;
		}
		if (!CanBePerformed(Character, Player))
		{
			string CantPerformReason;
			GetCannotPerformReason(CantPerformReason);
			acttext = m_sActionText.GetText(Character, Player) + " " + (CantPerformReason);
			return true;
		}
	 	return true;
	}
	
	//------------------------------------------------------------------//
	
	//void SetDialogueText(string text)
	//{
	//	m_sDialogueText.SetText(text);
	//}
	
	//------------------------------------------------------------------//
	string GetStageDialogueText(IEntity Character, IEntity Player)
	{
	 return m_sDialogueText.GetText(Character, Player);
	}
	
	//------------------------------------------------------------------//
	
	DS_DialogueBranch GetBranch(int BranchID)
	{
		if(m_Branch.Count() >= BranchID + 1)
		{
			return m_Branch[BranchID];
		}
		return null;
	}
	
	//------------------------------------------------------------------//
	
	void InheritData(DS_DialogueArchetype Archetype, DialogueBranchInfo Config, IEntity Char)
	{
		for (int i, count = m_Branch.Count(); i < count; i++)
		{
			m_Branch[i].InheritData(Archetype, Config, Char);
		}
	}
	
	//------------------------------------------------------------------//
	
	bool CheckIfStageCanBranch(IEntity Character, IEntity Player)
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
	
	//------------------------------------------------------------------//
	
	void Init(DS_DialogueBranch OwnerBranch, int Index)
	{
		if (OwnerBranch)
			m_Owner = OwnerBranch;
		m_iIndex = Index;
		if (m_DialogueAction)
			m_DialogueAction.Init(this, Index);
		if (m_sActionText)
			m_sActionText.Init(this, Index);
		if (m_sDialogueText)
			m_sDialogueText.Init(this, Index);
		if (!m_Branch.IsEmpty())
		{
			for (int i = 0; i < m_Branch.Count(); i++)
			{
				DS_DialogueBranch branch = DS_DialogueBranch.Cast(m_Branch[i]);
				branch.Init(this, i);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------//
class DialogueStageTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		// Get ActionName
		DS_BaseDialogueText actionName;
		source.Get("m_sActionText", actionName);
		string ActionText;
		if (actionName)
		{
			ActionText = actionName.ToString();
			ActionText = ActionText.Substring(3, ActionText.Length() - 23);
		}
		else
		{
			ActionText = "No action text configured for this stage."
		}
		
		// Get DialogueText
		DS_BaseDialogueText DiagName;
		source.Get("m_sDialogueText", DiagName);
		string DialogueText;
		if (DiagName)
		{
			DialogueText = DiagName.ToString();
			DialogueText = DialogueText.Substring(3, DialogueText.Length() - 23);
		}
		else
		{
			DialogueText = "No dialogue text configured for this stage."
		}
		
		// Get selected behavior from EChoiseBehavior enum
		string branchesStr;
		array<ref DS_DialogueBranch> branches;
		source.Get("m_Branch", branches);
		if (branches.Count() > 0)
		{
			branchesStr = "Stage Branches"
		}
		else
		{
			branchesStr = "Stage Increments"
		}
		Managed Action;
		source.Get("m_DialogueAction", Action);
		string Actionclassname;
		if (Action)
		{
			Actionclassname = Action.ToString();
			Actionclassname = Actionclassname.Substring(16, Actionclassname.Length() - 36);
		}
		else
		{
			Actionclassname = "No actions configured for this stage."
		}
		Managed Condition;
		source.Get("m_DialogueActionCondition", Condition);
		string Conditionclassname;
		if (Condition)
		{
			Conditionclassname = Condition.ToString();
			Conditionclassname = Conditionclassname.Substring(16, Conditionclassname.Length() - 36);
		}
		else
		{
			Conditionclassname = "No conditions configured for this stage."
		}
		string classname = source.GetClassName();
		title = string.Format("A : %1 - D : %2 || %3 || ACTION: %4 || CONDITION : %5", ActionText, DialogueText, branchesStr, Actionclassname, Conditionclassname);
		return true;
	}
}
//---------------------------------------------------------------------------------------------------//
class DialogueStageActionTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		string classname = source.GetClassName();
		title = classname;
		return true;
	}
}
[BaseContainerProps(configRoot:true)]
class BaseEventContainer
{
	string m_stext;
	string GetString(IEntity Player, IEntity Char){return m_stext;}
}
[BaseContainerProps(configRoot:true)]
class EventStringContainer : BaseEventContainer
{
	[Attribute(desc: "text override ")]
	string text;
	
	override string GetString(IEntity Player, IEntity Char){return text;}
}