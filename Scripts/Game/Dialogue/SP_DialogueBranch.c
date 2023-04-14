//Dialogue Branch is used to store a menu entry. In any dialogue branch a multiple stages can be concifgured.
//In Dialogue branch the dialogues progession can be configured, at any stage a Multiple choice config can be introduces to create another branch.
//Each branch tracks its own stage
[BaseContainerProps(configRoot:true),  DialogueBranchConfigTitleAttribute()]
class SP_DialogueBranch
{
	//------------------------------------------------------------------//
	[Attribute(desc: "Dialogue Stage, Depending on the stage of the branch the apropriate stage will be selected. Stage = 0 means first entry etc...")]
	ref array<ref DialogueStage> m_BranchStages;
	
	ref DialogueBranchInfo BranchInfoConfig;
	protected ref map<string, ref DialogueBranchInfo> BranchInfoConfigMap;
	IEntity TalkingCharacter;
	//------------------------------------------------------------------//
	//Text that is going to be used as title for the action
	void OnPerform(IEntity Character, IEntity Player)
	{
		DialogueBranchInfo Conf = LocateConfig(Character);
		if(m_BranchStages.Count() >= Conf.GetDialogueBranchStage())
		{
			m_BranchStages[Conf.GetDialogueBranchStage()].Perform(Character, Player);
		}
	};
	//------------------------------------------------------------------//
	void CanBeShown()
	{
	
	};
	//------------------------------------------------------------------//
	string GetActionText(IEntity Character)
	{
		string ActText;
		TalkingCharacter = Character;
		DialogueBranchInfo Conf = LocateConfig(TalkingCharacter);
		if(m_BranchStages.Count() >= Conf.GetDialogueBranchStage())
		{
			ActText = m_BranchStages[Conf.GetDialogueBranchStage()].GetActionText();
		}
		return ActText;
	}
	//------------------------------------------------------------------//
	//Text that is going to be sent in the chat
	string GetDialogueText(IEntity Character)
	{
		string DiagText;
		TalkingCharacter = Character;
		DialogueBranchInfo Conf = LocateConfig(TalkingCharacter);
		if(m_BranchStages.Count() >= Conf.GetDialogueBranchStage())
		{
		DiagText = m_BranchStages[Conf.GetDialogueBranchStage()].GetDialogueText();
		}
		return DiagText;
	}
	//------------------------------------------------------------------//
	//Increments stage of this branch
	void IncrementBranchStage(int incrementamount)
	{
		LocateConfig(TalkingCharacter).IncrementStage(incrementamount);
	}
	//------------------------------------------------------------------//
	//Checks if a SP_MultipleChoiceConfig exists in the current DialogueStage
	bool CheckIfStageBranches()
	{
		int currentstage = LocateConfig(TalkingCharacter).GetDialogueBranchStage();
		
		if(m_BranchStages[currentstage].CheckIfStageCanBranch() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------//
	void CauseBranch(int BranchID)
	{
		LocateConfig(TalkingCharacter).CauseBranch(BranchID);
	}
	//------------------------------------------------------------------//
	bool CheckIfBranched(IEntity Character)
	{
		return LocateConfig(Character).CheckifBranched();
	}
	//------------------------------------------------------------------//
	DialogueBranchInfo GetParent()
	{
		DialogueBranchInfo Parent = LocateConfig(TalkingCharacter).GetParentConfig();
		if (Parent)
		{
			return Parent;
		}
		else
		{
			return null;
		}
	}
	//------------------------------------------------------------------//
	DialogueStage GetDialogueStage()
	{
		DialogueBranchInfo Conf = LocateConfig(TalkingCharacter);
		if(m_BranchStages.Count() >= Conf.GetDialogueBranchStage())
		{
			return m_BranchStages[Conf.GetDialogueBranchStage()];
		}
		return null;
	}
	//------------------------------------------------------------------//
	//Return the current Branch. Meaning, it keeps looking deeper for a branch that doesent branch further (IsBranchBranched = false)
	SP_DialogueBranch GetCurrentDialogueBranch(IEntity Character, int BranchID)
	{
		TalkingCharacter = Character;
		SP_DialogueBranch branch;
		//Get the config for this dialogue branch
		DialogueBranchInfo Conf = LocateConfig(TalkingCharacter);
		//Get the stage valufe of this branch, use this to get the correct m_BranchStages
		int DiagStage = Conf.GetDialogueBranchStage();
		//Define the branch that we return as this one, if any of the conditions we check for later is true that will be updated, but if not, means that there is no branch that branches further so this is the one that we will return
		branch = this;
		//We check if this branch has IsBranchBranched = true
		if (Conf.CheckifBranched() == true)
		{
			//if its branched it means if has a Branched ID, we take it to use int
			//If a branch is branched we need to look further down on the corrent branch, branched ID is used for that.
			int BranchedID = Conf.GetBranchedID();
			//Go in m_BranchStages Get one of the branches nested there using the BranchedID that this branch has
			//Check if that branch has IsBranchBranched = true or false
			//IF true it means that we need to go deeper using our BranchedID, if false it means that we are on the branch we want, so we take branch using branch ID
			bool BranchState = m_BranchStages[DiagStage].GetBranch(BranchedID).CheckIfBranched(Character);
			if (BranchState == true)
			{
				branch = m_BranchStages[DiagStage].GetBranch(BranchedID);
				if (branch)
				{
					branch = branch.GetCurrentDialogueBranch(Character, BranchID);
				}
				
			}
			else
			{
				branch = m_BranchStages[DiagStage].GetBranch(BranchID);
				if(branch)
				{
					branch = branch.GetCurrentDialogueBranch(Character, BranchID);
				}
				
			}
		}
		return branch;
	}
	//------------------------------------------------------------------//
	DialogueBranchInfo LocateConfig(IEntity Character)
	{
		if(!BranchInfoConfigMap)
		{
			BranchInfoConfigMap = new map<string, ref DialogueBranchInfo>();
		}
		DialogueBranchInfo config;
		SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		string key = DiagComp.GetCharacterName(Character);
		if (BranchInfoConfigMap.Find(key, config))
		{
			return config;
		}
		else
		{
			DialogueBranchInfo configNew = CopyConfig(BranchInfoConfig);
			BranchInfoConfigMap.Insert(key, configNew);
			return configNew;
		}
	}
	//------------------------------------------------------------------//
	DialogueBranchInfo CopyConfig(DialogueBranchInfo OriginalConfig)
	{
		DialogueBranchInfo DiagConfigCopy = new DialogueBranchInfo(OriginalConfig, true);
		return DiagConfigCopy;
	}
	//------------------------------------------------------------------//
	void InheritData(SP_DialogueArchetype Archetype, DialogueBranchInfo Config)
	{
		LocateConfig(TalkingCharacter).ParentConfig = Config;
		LocateConfig(TalkingCharacter).OriginalArchetype = Archetype;
	}
};
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
	void InheritData(SP_DialogueArchetype Archetype, DialogueBranchInfo Config)
	{
		if(m_Branch && m_Branch.Count() > 0)
			{
				foreach (int i, SP_DialogueBranch DiagBranch: m_Branch)
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
//Used as text container 
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
	int BranchBranchID
	int DialogueBranchStage;
	//------------------------------------------------------------------//
	void DialogueBranchInfo(DialogueBranchInfo original, bool isNew = false)
    {
    };
	void IncrementStage(int amount)
	{
		DialogueBranchStage = DialogueBranchStage + 1;
	}
	int GetDialogueBranchStage()
	{
		return DialogueBranchStage;
	}
	bool CheckifBranched()
	{
		return  IsBranchBranched;
	}
	void CauseBranch(int BranchID)
	{
		if(IsBranchBranched == false)
		{
			IsBranchBranched = true;
			BranchBranchID = BranchID;
		}
	}
	int GetBranchedID()
	{
		return BranchBranchID;
	}
	DialogueBranchInfo GetParentConfig()
	{
		return ParentConfig;
	}
	void Unbranch()
	{
		if (IsBranchBranched == true)
		{
			IsBranchBranched = false;
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
		string behaviorStr;
		EChoiseBehavior behavior;
		source.Get("ChoiseBehavior", behavior);
		behaviorStr = typename.EnumToString(EChoiseBehavior, behavior);
		title = string.Format("Stage: %1 - %2", actionName, behaviorStr,);
		return true;
	}
}

class DialogueBranchConfigTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		title = string.Format("Branch");


		return true;
	}
};