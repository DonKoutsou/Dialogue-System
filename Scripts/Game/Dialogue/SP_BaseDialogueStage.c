[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStage
{
	SP_DialogueBranch m_Owner;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Action Text", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	//------------------------------------------------------------------//
	[Attribute(defvalue: "Dialogue Text", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Branch, if present will cause branch to split instead of progressing its stage. When a branch splits, the dialogue system will only look in the entries of this branch only")]
	ref array<ref SP_DialogueBranch> m_Branch;
	
	[Attribute(desc: "Event to be recorded on notebook")]
	protected ref BaseEventContainer m_sEventString;
	
	protected bool m_bEventRecorded;
	//------------------------------------------------------------------//
	string m_sCantBePerformedReason = "(Cant Be Performed)";
	//------------------------------------------------------------------//
	bool GetEvent(out string eventString)
	{
		if (!m_sCantBePerformedReason)
			return false;
		else
		{
			eventString = m_sCantBePerformedReason;
			return true;
		}
	}
	//------------------------------------------------------------------//
	void Perform(IEntity Character, IEntity Player)
	{
		if (m_sEventString && !m_bEventRecorded)
		{
			SCR_ChimeraCharacter Char = SCR_ChimeraCharacter.Cast(Player);
			SP_CallendarComponent Callendar = Char.GetCallendar();
			if (Callendar)
			{
				Callendar.RecordEvent(m_sEventString.GetString(Player, Character));
				m_bEventRecorded = true;
			}
		}
		
	};
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
	void Init(SP_DialogueBranch Owner = null, int Index = 0)
	{
		if (Owner)
			m_Owner = Owner;
		if (!m_Branch.IsEmpty())
		{
			for (int i = 0; i < m_Branch.Count(); i++)
			{
				SP_DialogueBranch branch = SP_DialogueBranch.Cast(m_Branch[i]);
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

		string classname = source.GetClassName();
		title = string.Format("%1: %2 - %3", classname, actionName, branchesStr);
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