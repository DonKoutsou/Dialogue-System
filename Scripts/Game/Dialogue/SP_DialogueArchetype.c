//Dialogue archetype should hold all the possible dialogue a character could hold.
//A character archetype can be used by alot of different characters (eg. Seting dialogue for all low ranking officers)
//but 1 character cant/shouldn't use multiple character archetypes. (eg. Character has 1 name 1 faction 1 rank. once any of the identifiers change his dialogue will change too)
[BaseContainerProps(configRoot: true)]
class SP_DialogueArchetype: ScriptAndConfig
{
	//------------------------------------------------------------------//
	//Dialogue Identifier
	[Attribute("50", UIWidgets.ComboBox, "ID of what type of identifier is going to be used", "", ParamEnumArray.FromEnum(EArchetypeIdentifier) )]
	private EArchetypeIdentifier ArchetypeIdentifier;
	//Character Name
	[Attribute("", UIWidgets.Object, "Character full name, used to identify character dialogue. Compared to name in identitycomponent", category:"CharacterInfo")]
	private string m_sCharacterName;
	//Character Faction
	[Attribute("", UIWidgets.Object, "Character faction, used to identify character dialogue. Compared to factionkey in FactionAffiliationComponent", category:"CharacterInfo")]
	private FactionKey m_sCharacterFaction;
	//Character Rank
	[Attribute("50", UIWidgets.ComboBox, "Character rank, used to identify character dialogue. Compared to rank in SCR_CharacterRankComponent", category:"CharacterInfo", ParamEnumArray.FromEnum(ECharacterRank))]
	private ECharacterRank m_sCharacterRank;
	//------------------------------------------------------------------//
	//Different configuration containing dialogue texts
	[Attribute(desc: "Filtered selection test")]
	private ref array<ref SP_DialogueBranch> DialogueBranch;
	//------------------------------------------------------------------//
	//Map to be filled with all the configurations on Init
	protected ref map<int, ref SP_DialogueBranch> DialogueBranchMap;
    protected SP_DialogueComponent DiagComp;
	SP_DialogueArchetype OriginalArchetype;
	IEntity TalkingCharacter;
	bool IsCharacterBranched;
	int BranchedID;
	//------------------------------------------------------------------//
	//Branching bool. If a Dialogue Archetype has its IsCharacterBranched set to true it will give text from SP_MultipleChoiceConfig
	//Selecting from wich SP_DialogueBranch to take the SP_MultipleChoiceConfig happens using current dialogue stage and branch ID specified when the branching happened
	//Branch ID that is set when the Archtype gets branched
	//------------------------------------------------------------------//
	//Function used to branch this archetype. Called used a branchID wich sets from wich branch to take data
	//If branch is set to 1 Action,Dialogue text will be taken from SP_DialogueBranch with BranchID == 1 and StageID == Current Stage
	//------------------------------------------------------------------//
	//Current config is the last config used.
	bool IsCharacterBranched()
	{
		return IsCharacterBranched;
	}
	void BranchArchetype(int Branch)
	{
		if (IsCharacterBranched == false)
		{
			IsCharacterBranched = true;
			BranchedID = Branch;
		}
	}
	void UnBranchArchetype()
	{
		if (IsCharacterBranched == true)
		{
			IsCharacterBranched = false;
			BranchedID = null;
		}
	}
	int GetBranchedID()
	{
		return BranchedID;
	}
	//------------------------------------------------------------------//
	//Dialogue identifier to be used for this archetype, can be set to something generic and provide its dialogues to a variety of entities
	EArchetypeIdentifier GetIdentifier()
	{
		return ArchetypeIdentifier;
	}
	//------------------------------------------------------------------//
	//Character Name set for this Archetype, relevant only if ArchetypeIdentifier is looking for it
	string GetArchetypeTemplateName()
	{
		return m_sCharacterName;
	}
	//------------------------------------------------------------------//
	//Character Faction set for this Archetype, relevant only if ArchetypeIdentifier is looking for it
	FactionKey GetArchetypeTemplateFaction()
	{
		return m_sCharacterFaction;
	}
	//------------------------------------------------------------------//
	//Character Rank set for this Archetype, relevant only if ArchetypeIdentifier is looking for it
	ECharacterRank GetArchetypeTemplateRank()
	{
		return m_sCharacterRank;
	}
	array<ref SP_DialogueBranch> GetDialogueBranchArray()
	{
		return DialogueBranch;
	}
	map<int, ref SP_DialogueBranch> GetDialogueBranchMap()
	{
		return DialogueBranchMap;
	}
	//------------------------------------------------------------------//
	//Mapping all configrations existing uder this character archetype
	void Init(IEntity Character)
	{
		OriginalArchetype = this;
		DialogueBranchMap = new map<int, ref SP_DialogueBranch>();
		for (int i = 0, count = DialogueBranch.Count(); i < count; i++)
        {
			//using 2 values to create key, branch and stage IDs
			int key = (i);
        	DialogueBranchMap.Insert(key, DialogueBranch[i]);
        }
		TalkingCharacter = Character;
	}
	//------------------------------------------------------------------//
	//Find the Config you are looking for using the map made above
	SP_DialogueBranch GetDialogueBranch(IEntity Character, int BranchKey)
    {
		int key = (BranchKey);
        SP_DialogueBranch branch;
		if (IsCharacterBranched == true)
		{
			if (!DialogueBranchMap.Find(BranchedID, branch))
			{
	        	return null;
	    	}
			return branch.GetCurrentDialogueBranch(Character, BranchKey);
		}
		if (!DialogueBranchMap.Find(key, branch))
			{
	        	return null;
	    	}
        return branch;
    }
	//------------------------------------------------------------------//
	//Checks if a SP_MultipleChoiceConfig is hooked on this branch, used to initiate radial menu insead of completing dialogue
	bool CheckIfDialogueBranches(SP_DialogueBranch DialogueBranch)
	{
		
		if (DialogueBranch.CheckIfStageBranches() == false)
		{
			return false;
		}
		else
			return true;
	}
	//------------------------------------------------------------------//
	//Check if IsInfluanceGlobal is true wich means that all branches should progress, if not progres only the branch provided
	bool IncrementStage(int BranchID, int incrementamount)
	{	
		SP_DialogueBranch branch = GetDialogueBranch(TalkingCharacter, BranchID);
		branch.IncrementBranchStage(incrementamount);
		return true;
	}
	//------------------------------------------------------------------//
	//Find correct branch using currect stage and take dialogue text from it 
	string GetDialogueText(int BranchID)
	{
		string m_stexttoshow;
		SP_DialogueBranch branch = GetDialogueBranch(TalkingCharacter, BranchID);
		m_stexttoshow = branch.GetDialogueText(TalkingCharacter, BranchID);	
		return m_stexttoshow;
	}
	//------------------------------------------------------------------//
	//Find correct branch using current stage and take action title from it 
	string GetActionTitle(int BranchID)
	{
		string m_sActionTitle;
		SP_DialogueBranch Branch = GetDialogueBranch(TalkingCharacter, BranchID);
		if (Branch)
		{
			m_sActionTitle = Branch.GetActionText(TalkingCharacter, BranchID);
		}
		else
		{
			m_sActionTitle = STRING_EMPTY;
		}
		return m_sActionTitle;
	}
	//------------------------------------------------------------------//
	// constructor for when creating new Archetype
	void SP_DialogueArchetype(SP_DialogueArchetype original, bool isNew = false)
    {
		if (isNew) 
		{
        ArchetypeIdentifier = original.GetIdentifier();
        m_sCharacterName = original.GetArchetypeTemplateName();
        m_sCharacterRank = original.GetArchetypeTemplateRank();
        m_sCharacterFaction = original.GetArchetypeTemplateFaction();
        DialogueBranch = original.GetDialogueBranchArray();
		DialogueBranchMap = original.GetDialogueBranchMap();
		}
    }
};