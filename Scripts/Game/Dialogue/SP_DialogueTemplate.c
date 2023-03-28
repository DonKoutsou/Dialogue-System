[BaseContainerProps(configRoot: true)]
class SP_DialogueTemplate
{
	[Attribute("50", UIWidgets.ComboBox, "ID of what type of identifier is going to be used", "", ParamEnumArray.FromEnum(EDiagIdentifier) )]
	protected EDiagIdentifier DialogueIdentifier;
	
	[Attribute()]
	protected int m_sCharacterID;
	
	[Attribute()]
	protected string m_sCharacterName;
	
	[Attribute()]
	protected FactionKey m_sCharacterFaction;
	
	[Attribute("50", UIWidgets.ComboBox, "Specify a rank if you want to use character rank as identifier", "", ParamEnumArray.FromEnum(ECharacterRank) )]
	protected ECharacterRank m_sCharacterRank;

	
	[Attribute()]
	protected ref array<ref SP_DialogueBranch> DialogueBranch;
	
	
	
	
	//------------------------------------------------------------------//
    protected SP_DialogueComponent DiagComp;
	
	//------------------------------------------------------------------//
	
	EDiagIdentifier GetIdentifier()
	{
		return DialogueIdentifier;
	}
	//------------------------------------------------------------------//
	bool GetCharacterID()
	{
		return m_sCharacterID;
	}
	//------------------------------------------------------------------//
	string GetCharacterName()
	{
		return m_sCharacterName;
	}
	//------------------------------------------------------------------//
	FactionKey GetCharacterFaction()
	{
		return m_sCharacterFaction;
	}
	//------------------------------------------------------------------//
	ECharacterRank GetCharacterRank()
	{
		return m_sCharacterRank;
	}
	//------------------------------------------------------------------//
	void Init()
	{	
		//DialogueStageMap = new map<int, ref SP_DialogueConfig>();
		//for (int i, count = DialogueStage.Count(); i < count; i++)
        //{
		//	int key = (DialogueStage[i].GetDialogueStageKey() << 16) | (DialogueStage[i].GetDialogueBranchKey() << 8) | DialogueStage[i].IsthisMultiple();
        //	DialogueConfigMap.Insert(key, DialogueStage[i]);
       	//}
		
	}
	//------------------------------------------------------------------//
	SP_DialogueBranch GetDialogueBranch(int BranchKey)
    {
        return DialogueBranch[BranchKey];
    }
	//------------------------------------------------------------------//
	bool IncrementStage(int BranchID, int StageID, int incrementamount, int MultipleChoise)
	{	
		SP_DialogueStage Stage = SP_DialogueStage.Cast(DialogueBranch[BranchID].GetDialogueStage());
		if (Stage != null && Stage.IsInfluanceGlobal() == true)
		{
			for (int i, count = DialogueBranch.Count(); i < count; i++)
        	{	
				if (DialogueBranch[i].GetBranchStage() == StageID)
				{
					DialogueBranch[i].IncrementBranchStage(incrementamount);
					return true;
				}
        	}
		}
		else
			DialogueBranch[BranchID].IncrementBranchStage(incrementamount);
		return true;
	}
	//------------------------------------------------------------------//
	string GetDialogueText(int BranchID, int StageID, bool MultipleChoise)
	{
		string m_stexttoshow = DialogueBranch[BranchID].GetDialogueText(StageID);
		return m_stexttoshow;
	}
	//------------------------------------------------------------------//
	string GetActionTitle(int BranchID, int StageID, bool MultipleChoise)
	{
		string m_sActionTitle = DialogueBranch[BranchID].GetDialogueActionTitle(StageID);
		return m_sActionTitle;
	}
};