//Dialogue Branch is used to store a menu entry. In any dialogue branch a multiple stages can be concifgured.
//In Dialogue branch the dialogues progession can be configured, at any stage a Multiple choice config can be introduces to create another branch.
//Each branch tracks its own stage
[BaseContainerProps(configRoot:true),  DialogueBranchConfigTitleAttribute()]
class SP_DialogueBranch
{
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Stage, Depending on the stage of the branch the apropriate stage will be selected. Stage = 0 means first entry etc...")]
	ref array<ref DialogueStage> 						m_BranchStages;
	ref DialogueBranchInfo 								BranchInfoConfig;
	protected ref map<string, ref DialogueBranchInfo> 	BranchInfoConfigMap;
	
	//Text that is going to be used as title for the action
	void OnPerform(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);;
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		if(m_BranchStages.Count() >= Bstage)
		{
			m_BranchStages[Bstage].Perform(Character, Player);
		}
	};
	//------------------------------------------------------------------//
	bool CheckCondition(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo CondConf = LocateConfig(Character);
		return CondConf.CheckCondition(Character, Player);
	}
	bool CanBePerformed(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		return m_BranchStages[Bstage].CanBePerformed(Character, Player);
	}
	//------------------------------------------------------------------//
	bool CanBeShown(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		return m_BranchStages[Bstage].CanBeShown(Character, Player);
	}
	//------------------------------------------------------------------//
	bool GetActionText(IEntity Character, IEntity Player, out string ActText)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		if(m_BranchStages.Count() >= Bstage)
		{
			m_BranchStages[Bstage].GetActionText(Character, Player, ActText);
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------//
	//Text that is going to be sent in the chat
	void GetDialogueText(IEntity Character, IEntity Player, out string DiagText)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		if(m_BranchStages.Count() >= Bstage)
		{
			//if (m_BranchStages[Bstage].CheckIfStageBranches(Character, Player);
			
			DiagText = m_BranchStages[Bstage].GetStageDialogueText(Character, Player);
		}
	}
	//------------------------------------------------------------------//
	bool CheckIfBranched(IEntity Char)
	{
		DialogueBranchInfo Conf = LocateConfig(Char);
		return Conf.CheckifBranched();
	}
	//------------------------------------------------------------------//
	//Increments stage of this branch
	void IncrementBranchStage(int incrementamount, IEntity Character)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		Conf.IncrementStage(incrementamount);
	}
	//------------------------------------------------------------------//
	//Checks if a SP_MultipleChoiceConfig exists in the current DialogueStage
	bool CheckIfStageBranches(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		DialogueStage Diagstage;
		if (m_BranchStages.Count() >= Bstage)
		{
			Diagstage = m_BranchStages[Bstage];
		}
		
		if(Diagstage && Diagstage.CheckIfStageCanBranch(Character, Player))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------//
	void CauseBranch(int BranchID, IEntity Character)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		DialogueStage DiagStage;
		GetDialogueStage(Character, DiagStage);
		DiagStage.InheritData(Conf.OriginalArchetype, Conf, Character);
		Conf.CauseBranch(BranchID);
	}
	void UnBranch(IEntity Character)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		DialogueStage DiagStage;
		GetDialogueStage(Character, DiagStage);
		DiagStage.InheritData(Conf.OriginalArchetype, Conf, Character);
		Conf.Unbranch();
	}
	
	//------------------------------------------------------------------//
	void GetParent(IEntity Character, out DialogueBranchInfo Parent)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		Conf.GetParentConfig(Parent);
	}
	//------------------------------------------------------------------//
	bool GetDialogueStage(IEntity Character, out DialogueStage DiagStage)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		if(m_BranchStages.Count() >= Bstage)
		{
			DiagStage = m_BranchStages[Bstage];
			return true;
		}
		return false;
	}
	bool CheckNextStage(int stageID)
	{
		if (m_BranchStages.Count() >= stageID + 1)
		{
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------//
	//Return the current Branch. Meaning, it keeps looking deeper for a branch that doesent branch further (IsBranchBranched = false)
	void GetCurrentDialogueBranch(IEntity Character, int BranchID, out SP_DialogueBranch branch)
	{
		//Get the config for this dialogue branch
		DialogueBranchInfo Conf = LocateConfig(Character);
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		branch = this;
		if (Conf.CheckifBranched() == true)
		{
			int BranchedID;
			Conf.GetBranchedID(BranchedID);
			
			bool BranchState = m_BranchStages[Bstage].CheckIfAnyBranchesBranch(Character, BranchedID);
			if (BranchState == true)
			{
				branch = m_BranchStages[Bstage].GetBranch(BranchedID);
				if (branch)
				{
					branch.GetCurrentDialogueBranch(Character, BranchID, branch);
				}
			}
			BranchState = m_BranchStages[Bstage].CheckIfAnyBranchesBranch(Character, BranchID);
			if(BranchState == false)
			{
				branch = m_BranchStages[Bstage].GetBranch(BranchID);
				if(branch)
				{
					branch.GetCurrentDialogueBranch(Character, BranchID, branch);
				}
			}
		}
	}
	//------------------------------------------------------------------//
	DialogueBranchInfo LocateConfig(IEntity Character)
	{
		DialogueBranchInfo config;
		if(!BranchInfoConfigMap)
		{
			BranchInfoConfigMap = new map<string, ref DialogueBranchInfo>();
		}
		SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		string key = DiagComp.GetCharacterName(Character);
		if (BranchInfoConfigMap.Find(key, config))
		{
			return config;
		}
		else
		{
			config = CopyConfig(BranchInfoConfig);
			BranchInfoConfigMap.Insert(key, config);
			return config;
		}
	}
	//------------------------------------------------------------------//
	DialogueBranchInfo CopyConfig(DialogueBranchInfo OriginalConfig)
	{
		DialogueBranchInfo DiagConfigCopy = new DialogueBranchInfo(OriginalConfig, true);
		return DiagConfigCopy;
	}
	//------------------------------------------------------------------//
	void InheritData(SP_DialogueArchetype Archetype, DialogueBranchInfo ParentConfig, IEntity Char)
	{
		DialogueBranchInfo Conf = LocateConfig(Char);
		Conf.SetParent(ParentConfig);
		Conf.SetOriginalArch(Archetype);
	}
};
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueBranchInfo
{
	//------------------------------------------------------------------//
	//Archetype wich this cofig belongs to
	SP_DialogueArchetype OriginalArchetype;
	//------------------------------------------------------------------//
	//SP_MultipleChoiceConfig that come before this one in a hierarchy. Used for DoBackDialogue in SP_DialogueComponent
	DialogueBranchInfo ParentConfig;
	//------------------------------------------------------------------//
	//Branching info for this config. Once config gets branched the branch that caused it has its ID saved in BranchBranchID
	bool IsBranchBranched;
	int BranchBranchID;
	int DialogueBranchStage;
	//------------------------------------------------------------------//
	void DialogueBranchInfo(DialogueBranchInfo original, bool isNew = false){};
	//------------------------------------------------------------------//
	void IncrementStage(int amount){DialogueBranchStage = DialogueBranchStage + 1;}
	//------------------------------------------------------------------//
	void SetParent(DialogueBranchInfo BranchConf){ParentConfig = BranchConf;}
	//------------------------------------------------------------------//
	void SetOriginalArch(SP_DialogueArchetype DiagArch){OriginalArchetype = DiagArch;}
	//------------------------------------------------------------------//
	void GetDialogueBranchStage(out int BStage){BStage = DialogueBranchStage;}
	//------------------------------------------------------------------//
	bool CheckifBranched(){return  IsBranchBranched;}
	//------------------------------------------------------------------//
	void GetBranchedID(out int bID){bID = BranchBranchID;}
	//------------------------------------------------------------------//
	void GetParentConfig(out DialogueBranchInfo ParentC){ParentC = ParentConfig;}
	//------------------------------------------------------------------//
	void CauseBranch(int BranchID)
	{
		if(IsBranchBranched == false)
		{
			IsBranchBranched = true;
			BranchBranchID = BranchID;
		}
	}
	//------------------------------------------------------------------//
	void Unbranch()
	{
		if (IsBranchBranched == true)
		{
			IsBranchBranched = false;
		}
	}
	bool CheckCondition(IEntity Character, IEntity Player)
	{
		return true;
	};
	void SetupCondition()
	{
		
	};
}
class DialogueBranchConfigTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		array <ref DialogueStage> Stages;
		source.Get("m_BranchStages", Stages);
		string texttoshow;
		int StageAmount;
		string Diagtext;
		for (int i, count = Stages.Count(); i < count; i++)
		{
			if (Stages[i].m_Branch.Count() > 0)
			{
				texttoshow = "| Branches at stage" + " " + i;
			}
			StageAmount = StageAmount + 1;
			if (!Diagtext)
			{
				Diagtext = Stages[i].ActionText;
			}
		}
		title = string.Format("Branch: %1 stages | %2 %3", StageAmount ,Diagtext, texttoshow);
		return true;
	}
};