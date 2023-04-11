//Used in Dialogue Archetype, in any SP_DialogueBranch
//SP_MultipleChoiceConfig is used to store the text that is going to be used in radial menu in dialogue.
//Also  marks the config it is atatched to as branching dialogue so when that action is performed radial menu will open intead of progressing dialogue
[BaseContainerProps(configRoot: true), DialogueBranchConfigTitleAttribute()]
class SP_MultipleChoiceConfig: ScriptAndConfig
{
	[Attribute()]
	ref DialogueMultiTextConfig MultipleChoiceConfig;
	protected ref map<string, ref DialogueMultiTextConfig> DialogueMultiTextConfigMap;
	IEntity TalkingCharacter;
	//------------------------------------------------------------------//
	int GetBranchID()
	{
		return LocateConfig(TalkingCharacter).BranchBranchID;
	}
	//------------------------------------------------------------------//
	SP_DialogueArchetype GetOriginalArchetype()
	{
		return LocateConfig(TalkingCharacter).OriginalArchetype;
	}
	//------------------------------------------------------------------//
	array<ref SP_DialogueBranch> GetBranches()
	{
		return LocateConfig(TalkingCharacter).m_Branches;
	}
	//------------------------------------------------------------------//
	//returns the config one step above this one
	SP_MultipleChoiceConfig GetParentConfig()
	{
		return LocateConfig(TalkingCharacter).ParentConfig;
	}
	//------------------------------------------------------------------//
	//inherits config wich caused this one to branch
	void InheritConfig(SP_MultipleChoiceConfig Config)
	{
		LocateConfig(TalkingCharacter).ParentConfig = Config;
	}
	void InheritCharacter(IEntity Character)
	{
		TalkingCharacter = Character;
	}
	//------------------------------------------------------------------//
	//check if branched
	bool GetBranchState()
	{
		return LocateConfig(TalkingCharacter).IsBranchBranched;
	}
	//Inherits Archetype from previus config once branched, used to ping call the ping function
	void InheritArchetype(SP_DialogueArchetype Archetype)
	{
		LocateConfig(TalkingCharacter).OriginalArchetype = Archetype;
	}
	//------------------------------------------------------------------//
	//Get action title from this config used TextID. TextID = 0 will give first entry etc.....
	string GetActionText(int TextID)
    {
		DialogueMultiTextConfig Conf = LocateConfig(TalkingCharacter);
        if (TextID < Conf.m_Branches.Count()) 
        {
			if (Conf.IsBranchBranched == true)
			{	
				string acttext = Conf.m_Branches[GetBranchID()].GetMultiActionText(TextID);
				return acttext;
			}
			else
			{
				string acttext = Conf.m_Branches[TextID].GetActionText();
				return acttext;
			}
        }
        else 
        {
            return STRING_EMPTY;
        }
    }
	//------------------------------------------------------------------//
	//Get dialogue besed on ID provided, also used to check if something needs to be branched
	string GetDialogueText(int TextID)
    {
		DialogueMultiTextConfig Conf = LocateConfig(TalkingCharacter);
		//check if value is out of bounds
        if (TextID >= 0 && TextID < Conf.m_Branches.Count()) 
        {
			string diagtext;
			//check if this config is branched, if yes look for text in next branch for text
			if (Conf.IsBranchBranched == true)
			{
				diagtext = Conf.m_Branches[GetBranchID()].GetMultiDialogueText(TextID);
			}
			//else check for SP_MultipleChoiceConfig in next branch, if there is one take text from it and branch it
			else
			{
				if (Conf.m_Branches[TextID].CheckIfTextConfigBranches() == true)
				{
					diagtext = Conf.m_Branches[TextID].GetDialogueText();
					BranchBranch(TextID);
				}
				else
				//else means that it shoulndt branch, so we take text from it and figure out if we should increment stage based on EChoiseBehavior
				{
					EChoiseBehavior ChoiseBehavior = Conf.m_Branches[TextID].GetDialogueStageConfig().GetChoiseBehavior();
					diagtext = Conf.m_Branches[TextID].GetDialogueText();
					switch(ChoiseBehavior)
					{
						case 0:
						{
							Conf.m_Branches[TextID].IncrementConfigStage(1);
						}
						break;
						case 1:
						{
							
						};
						break;
					}
				}	
			}	
			return diagtext; 
        }
        else
        {
            return STRING_EMPTY;
        }
    }
	//------------------------------------------------------------------//
	//Branches this branch, sets the branchID and passes the owner Archetype and itself on the next config, finally pings the archetype
	void BranchBranch(int BranchID)
	{
		DialogueMultiTextConfig Conf = LocateConfig(TalkingCharacter);
		if (Conf.IsBranchBranched == false)
		{
			Conf.IsBranchBranched = true;
			
			Conf.BranchBranchID = BranchID;
		}
		Conf.m_Branches[BranchID].GetDialogueStageConfig().GetMultipleChoiceConfig().InheritCharacter(TalkingCharacter);
		Conf.m_Branches[BranchID].GetDialogueStageConfig().GetMultipleChoiceConfig().InheritArchetype(Conf.OriginalArchetype);
		Conf.m_Branches[BranchID].GetDialogueStageConfig().GetMultipleChoiceConfig().InheritConfig(this);
		Conf.OriginalArchetype.Ping(this);
	}
	//------------------------------------------------------------------//
	//Unbranches this branch, resets branch stage and resets BranchID value, also pings the Archetype
	void UnbranchBranch()
	{
		DialogueMultiTextConfig Conf = LocateConfig(TalkingCharacter);
		if (Conf.IsBranchBranched == true)
		{
			Conf.IsBranchBranched = false;
			Conf.m_Branches[Conf.BranchBranchID].ResetBranchStage();
			Conf.BranchBranchID = 0;
			Conf.OriginalArchetype.Ping(Conf.ParentConfig);
		}
		
	}
	// constructor for when creating new MultiBranch instance
	
	DialogueMultiTextConfig LocateConfig(IEntity Character)
	{
		if(!DialogueMultiTextConfigMap)
		{
			DialogueMultiTextConfigMap = new map<string, ref DialogueMultiTextConfig>();
		}
		DialogueMultiTextConfig config;
		SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		string key = DiagComp.GetCharacterName(Character);
		if (DialogueMultiTextConfigMap.Find(key, config))
		{
			return config;
		}
		else
		{
			DialogueMultiTextConfig configNew = CopyConfig(MultipleChoiceConfig);
			DialogueMultiTextConfigMap.Insert(key, configNew);
			return configNew;
		}
	}
	DialogueMultiTextConfig CopyConfig(DialogueMultiTextConfig OriginalConfig)
	{
		DialogueMultiTextConfig DiagConfigCopy = new DialogueMultiTextConfig(OriginalConfig, true);
		return DiagConfigCopy;
	}
};
//-------------------------------------------------------------------------------------------------------------//
//Used as text container 
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueMultiTextConfig
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
	SP_MultipleChoiceConfig ParentConfig;
	//------------------------------------------------------------------//
	//Branching info for this config. Once config gets branched the branch that caused it has its ID saved in BranchBranchID
	bool IsBranchBranched;
	int BranchBranchID
	//------------------------------------------------------------------//
	void DialogueMultiTextConfig(DialogueMultiTextConfig original, bool isNew = false)
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
}