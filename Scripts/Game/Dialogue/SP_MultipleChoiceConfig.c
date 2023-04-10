//Used in Dialogue Archetype, in any SP_DialogueBranch
//SP_MultipleChoiceConfig is used to store the text that is going to be used in radial menu in dialogue.
//Also  marks the config it is atatched to as branching dialogue so when that action is performed radial menu will open intead of progressing dialogue
[BaseContainerProps(configRoot: true)]
class SP_MultipleChoiceConfig: ScriptAndConfig
{
	//------------------------------------------------------------------//
	//Class containing both action and dialogue text for each radial menu entry
	[Attribute(defvalue: "", desc: "Action Title and Dialogue Text for Radial menu choice", category: "Dialogue")]
	protected ref array<ref SP_DialogueBranch> m_MultiChoiceTexts;
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
	//returns the config one step above this one
	SP_MultipleChoiceConfig GetPartnerConfig()
	{
		return ParentConfig;
	}
	//------------------------------------------------------------------//
	//inherits config wich caused this one to branch
	void InheritConfig(SP_MultipleChoiceConfig Config)
	{
		ParentConfig = Config;
	}
	//------------------------------------------------------------------//
	//check if branched
	bool GetBranchState()
	{
		return IsBranchBranched;
	}
	//------------------------------------------------------------------//
	//Inherits Archetype from previus config once branched, used to ping call the ping function
	void InheritArchetype(SP_DialogueArchetype Archetype)
	{
		OriginalArchetype = Archetype;
	}
	//------------------------------------------------------------------//
	//Get action title from this config used TextID. TextID = 0 will give first entry etc.....
	string GetActionText(int TextID)
    {
        if (TextID >= 0 && TextID < m_MultiChoiceTexts.Count()) 
        {
			if (IsBranchBranched == true)
			{	
				string acttext = m_MultiChoiceTexts[BranchBranchID].GetMultiActionText(TextID);
				return acttext;
			}
			else
			{
				string acttext = m_MultiChoiceTexts[TextID].GetActionText();
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
		//check if value is out of bounds
        if (TextID >= 0 && TextID < m_MultiChoiceTexts.Count()) 
        {
			string diagtext;
			//check if this config is branched, if yes look for text in next branch for text
			if (IsBranchBranched == true)
			{
				diagtext = m_MultiChoiceTexts[BranchBranchID].GetMultiDialogueText(TextID);
			}
			//else check for SP_MultipleChoiceConfig in next branch, if there is one take text from it and branch it
			else
			{
				if (m_MultiChoiceTexts[TextID].CheckIfTextConfigBranches() == true)
				{
					diagtext = m_MultiChoiceTexts[TextID].GetDialogueText();
					BranchBranch(TextID);
				}
				else
				//else means that it shoulndt branch, so we take text from it and figure out if we should increment stage based on EChoiseBehavior
				{
					EChoiseBehavior ChoiseBehavior = m_MultiChoiceTexts[TextID].GetDialogueStageConfig().GetChoiseBehavior();
					diagtext = m_MultiChoiceTexts[TextID].GetDialogueText();
					switch(ChoiseBehavior)
					{
						case 0:
						{
							m_MultiChoiceTexts[TextID].IncrementConfigStage(1);
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
		if (IsBranchBranched == false)
		{
			IsBranchBranched = true;
			
			BranchBranchID = BranchID;
		}
		m_MultiChoiceTexts[BranchID].GetDialogueStageConfig().GetMultipleChoiceConfig().InheritArchetype(OriginalArchetype);
		m_MultiChoiceTexts[BranchID].GetDialogueStageConfig().GetMultipleChoiceConfig().InheritConfig(this);
		OriginalArchetype.Ping(this);
	}
	//------------------------------------------------------------------//
	//Unbranches this branch, resets branch stage and resets BranchID value, also pings the Archetype
	void UnbranchBranch()
	{
		if (IsBranchBranched == true)
		{
			IsBranchBranched = false;
			m_MultiChoiceTexts[BranchBranchID].ResetBranchStage();
			BranchBranchID = 0;
			OriginalArchetype.Ping(ParentConfig);
		}
		
	}
};
//-------------------------------------------------------------------------------------------------------------//
//Used as text container 
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueMultiTextConfig
{
	[Attribute(defvalue: "", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	[Attribute(defvalue: "", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
	string GetActionText()
	{
	 return ActionText;
	}
	string GetDialogueText()
	{
	 return DialogueText;
	}
}

