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
  ref array <ref DS_BaseDialogueStageAction> a_DialogueActions;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc : "Condition for action to be performable ")]
  ref array <ref DS_BaseDialogueStageActionCondition> a_DialogueActionConditions;
	
	//------------------------------------------------------------------//
	
	[Attribute(desc : "Should hide option if not performable")]
	bool m_bHideIfCantBePerformed;
	
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Branch, if present will cause branch to split instead of progressing its stage. When a branch splits, the dialogue system will only look in the entries of this branch only")]
	ref array<ref DS_DialogueBranch> m_Branch;
	
	//------------------------------------------------------------------//
	DS_DialogueBranch m_Owner;
	int m_iIndex;
	
	int m_iTimesPerformed;
	string m_sCantBePerformedReason;
	string m_sCantBePerformedDialogue;
	void Perform(IEntity Character, IEntity Player)
	{
		m_iTimesPerformed += 1;
		if (a_DialogueActions)
		{
			foreach (DS_BaseDialogueStageAction action : a_DialogueActions)
			{
				action.Perform(Character, Player);
			}
		}
	};
	
	//------------------------------------------------------------------//
	
	bool CanBePerformed(IEntity Character, IEntity Player)
	{
		if (a_DialogueActionConditions)
		{
			foreach (DS_BaseDialogueStageActionCondition Condition : a_DialogueActionConditions)
			{
				if (!Condition.CanBePerformed(Character, Player))
				{
					Condition.GetCannotPerformReason(m_sCantBePerformedReason);
					Condition.GetCannotPerformDialogue(m_sCantBePerformedDialogue);
					return false;
				}
					
			}
			return true;
		} 
		else 
			return true;
	}
	//------------------------------------------------------------------//
	
	bool CanBeShown(IEntity Character, IEntity Player)
	{
		if (!CanBePerformed(Character, Player))
			return !m_bHideIfCantBePerformed; 
		return true;
	}
	
	//------------------------------------------------------------------//
	
	void GetCannotPerformReason(out string CantBePReason)
	{
		CantBePReason = m_sCantBePerformedReason;
	}
	void GetCannotPerformDialogue(out string CanBePDialogue)
	{
		CanBePDialogue = m_sCantBePerformedDialogue;
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
		if (a_DialogueActions)
		{
			foreach (DS_BaseDialogueStageAction action : a_DialogueActions)
			{
				action.Init(this, Index);
			}
		}
		if (a_DialogueActionConditions)
		{
			foreach (DS_BaseDialogueStageActionCondition condition : a_DialogueActionConditions)
			{
				condition.Init(this, Index);
			}
		}
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
		ref array<ref DS_DialogueBranch> branches;
		source.Get("m_Branch", branches);
		if (branches && branches.Count() > 0)
		{
			branchesStr = "Stage Branches"
		}
		else
		{
			branchesStr = "Stage Increments"
		}
		ref array<ref DS_BaseDialogueStageAction> actions;
		source.Get("a_DialogueActions", actions);
		string Actionclassname;
		if (actions)
		{
			foreach (DS_BaseDialogueStageAction action : actions)
			{
				string name = action.ToString();
				name = name.Substring(16, name.Length() - 36);
				Actionclassname = Actionclassname + name;
			}
		}
		else
		{
			Actionclassname = "No actions configured for this stage."
		}
		array<ref DS_BaseDialogueStageActionCondition> conditions;
		source.Get("a_DialogueActionConditions", conditions);
		string Conditionclassname;
		if (conditions)
		{
			foreach (DS_BaseDialogueStageActionCondition condition : conditions)
			{
				string name = condition.ToString();
				name = name.Substring(16, name.Length() - 36);
				Conditionclassname = Conditionclassname + name;
			}
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