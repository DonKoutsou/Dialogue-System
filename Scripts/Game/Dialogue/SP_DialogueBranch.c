//Dialogue Branch is used to store a menu entry. In any dialogue branch a multiple stages can be concifgured.
//In Dialogue branch the dialogues progession can be configured, at any stage a Multiple choice config can be introduces to create another branch.
//Each branch tracks its own stage
[BaseContainerProps(configRoot:true),  DialogueBranchConfigTitleAttribute()]
class SP_DialogueBranch
{
	//------------------------------------------------------------------//
	[Attribute()]
	ref array<ref DialogueStageConfig> m_BranchStageConfig;
	
	ref DialogueBranchInfoConfig BranchInfoConfig;
	protected ref map<string, ref DialogueBranchInfoConfig> BranchInfoConfigMap;
	IEntity TalkingCharacter;
	//------------------------------------------------------------------//
	//Text that is going to be used as title for the action
	void OnPerform(IEntity Character, IEntity Player)
	{
		DialogueBranchInfoConfig Conf = LocateConfig(Character);
		m_BranchStageConfig[Conf.GetDialogueBranchStage()].Perform(Character, Player)
	};
	void CanBeShown()
	{
	
	};
	DialogueStageConfig GetDialogueStage()
	{
		DialogueBranchInfoConfig Conf = LocateConfig(TalkingCharacter);
		return m_BranchStageConfig[Conf.GetDialogueBranchStage()];
		
	}
	SP_DialogueBranch GetCurrentDialogueBranch(IEntity Character, int BranchID)
	{
		TalkingCharacter = Character;
		SP_DialogueBranch branch;
		DialogueBranchInfoConfig Conf = LocateConfig(TalkingCharacter);
		
		branch = this;
		if (Conf.CheckifBranched() == true)
		{
			branch = m_BranchStageConfig[Conf.GetDialogueBranchStage()].GetBranch(BranchID).GetCurrentDialogueBranch(Character, BranchID);
		}
		return branch;
	}
	string GetActionText(IEntity Character, int KeyID)
	{
		string ActText;
		TalkingCharacter = Character;
		DialogueBranchInfoConfig Conf = LocateConfig(TalkingCharacter);
		if (Conf.CheckifBranched() == false)
		{
			ActText = m_BranchStageConfig[Conf.GetDialogueBranchStage()].GetActionText();
		}
		else
		{
			ActText = m_BranchStageConfig[Conf.GetDialogueBranchStage()].GetStageBranchActionText(KeyID, TalkingCharacter);
		}
		
		return ActText;
	}
	//------------------------------------------------------------------//
	//Text that is going to be sent in the chat
	string GetDialogueText(IEntity Character, int KeyID)
	{
		string DiagText;
		TalkingCharacter = Character;
		DialogueBranchInfoConfig Conf = LocateConfig(TalkingCharacter);
		if (Conf.CheckifBranched() == false)
		{
			DiagText = m_BranchStageConfig[Conf.GetDialogueBranchStage()].GetDialogueText();
		}
		else
		{
			DiagText = m_BranchStageConfig[Conf.GetDialogueBranchStage()].GetStageBranchDialogueText(KeyID, TalkingCharacter);
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
	//Checks if a SP_MultipleChoiceConfig exists in the current DialogueStageConfig
	bool CheckIfStageBranches()
	{
		int currentstage = LocateConfig(TalkingCharacter).GetDialogueBranchStage();
		
		if(m_BranchStageConfig[currentstage].CheckIfStageCanBranch() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool CheckIfBranched(IEntity Character)
	{
		return LocateConfig(Character).CheckifBranched();
	}
	bool CheckIfParentBranched()
	{
		return LocateConfig(TalkingCharacter).GetParentConfig().CheckifBranched();
	}
	void CauseBranch(SP_DialogueArchetype Archetype, DialogueBranchInfoConfig Config)
	{
		LocateConfig(TalkingCharacter).CauseBranch(Archetype, Config);
	}
	//------------------------------------------------------------------//
	DialogueBranchInfoConfig LocateConfig(IEntity Character)
	{
		if(!BranchInfoConfigMap)
		{
			BranchInfoConfigMap = new map<string, ref DialogueBranchInfoConfig>();
		}
		DialogueBranchInfoConfig config;
		SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		string key = DiagComp.GetCharacterName(Character);
		if (BranchInfoConfigMap.Find(key, config))
		{
			return config;
		}
		else
		{
			DialogueBranchInfoConfig configNew = CopyConfig(BranchInfoConfig);
			BranchInfoConfigMap.Insert(key, configNew);
			return configNew;
		}
	}
	DialogueBranchInfoConfig CopyConfig(DialogueBranchInfoConfig OriginalConfig)
	{
		DialogueBranchInfoConfig DiagConfigCopy = new DialogueBranchInfoConfig(OriginalConfig, true);
		return DiagConfigCopy;
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
	[Attribute(desc: "Filtered selection test")]
	private ref array<ref SP_DialogueBranch> m_StageBranch;
	//------------------------------------------------------------------//
	//Get action text from DialogueStageConfig
	void Perform(IEntity Character, IEntity Player)
	{
		string bullet;
		bullet = Character.GetName();
	};
	string GetActionText()
	{
	 return ActionText;
	}
	bool CheckIfStageCanBranch()
	{
		if (m_StageBranch != null)
		{
			return true;
		}
		return false;
	}
	SP_DialogueBranch GetBranch(int BranchID)
	{
		return m_StageBranch[BranchID];
	}
	//------------------------------------------------------------------//
	//Get action text from SP_MultipleChoiceConfig, text key chooses the entry. TextKey = 0 will take the first entry in SP_MultipleChoiceConfig
	string GetStageBranchActionText(int TextKey, IEntity Character)
	{
		string ActText
		if(m_StageBranch[TextKey])
		{
			ActText = m_StageBranch[TextKey].GetActionText(Character, TextKey);	
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
	string GetStageBranchDialogueText(int TextKey, IEntity Character)
	{
		string DiagText;
		if(m_StageBranch[TextKey])
		{
			DiagText = m_StageBranch[TextKey].GetDialogueText(Character, TextKey);
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
//Used as text container 
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueBranchInfoConfig
{
	//------------------------------------------------------------------//
	//Class containing both action and dialogue text for each radial menu entry
	[Attribute(defvalue: "", desc: "Action Title and Dialogue Text for Radial menu choice", category: "Dialogue")]
	ref array<ref SP_DialogueBranch> m_Branches;
	//------------------------------------------------------------------//
	//Archetype wich this cofig belongs to
	SP_DialogueArchetype OriginalArchetype;
	//------------------------------------------------------------------//
	//SP_MultipleChoiceConfig that come before this one in a hierarchy. Used for DoBackDialogue in SP_DialogueComponent
	DialogueBranchInfoConfig ParentConfig;
	//------------------------------------------------------------------//
	//Branching info for this config. Once config gets branched the branch that caused it has its ID saved in BranchBranchID
	bool IsBranchBranched;
	int BranchBranchID
	int DialogueBranchStage;
	//------------------------------------------------------------------//
	void DialogueBranchInfoConfig(DialogueBranchInfoConfig original, bool isNew = false)
    {
		if (isNew) 
		{
        OriginalArchetype = original.OriginalArchetype;
        m_Branches = original.m_Branches;
        ParentConfig = original.ParentConfig;
        IsBranchBranched = original.IsBranchBranched;
        BranchBranchID = original.BranchBranchID;
		}
    }
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
	void CauseBranch(SP_DialogueArchetype Archetype, DialogueBranchInfoConfig Config)
	{
		if(IsBranchBranched == false)
		{
			IsBranchBranched = true;
			OriginalArchetype = Archetype;
			ParentConfig = Config;
			BranchBranchID = DialogueBranchStage;
		}
	}
	DialogueBranchInfoConfig GetParentConfig()
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
class DialogueStageConfigTitleAttribute : BaseContainerCustomTitle
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