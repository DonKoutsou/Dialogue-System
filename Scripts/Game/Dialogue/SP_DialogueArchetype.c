//Dialogue archetype should hold all the possible dialogue a character could hold.
//A character archetype can be used by alot of different characters (eg. Seting dialogue for all low ranking officers)
//but 1 character cant/shouldn't use multiple character archetypes. (eg. Character has 1 name 1 faction 1 rank. once any of the identifiers change his dialogue will change too)
[BaseContainerProps(configRoot: true)]
class SP_DialogueArchetype: ScriptAndConfig
{
	//------------------------------------------------------------------//
	//Dialogue Identifier
	[Attribute("50", UIWidgets.ComboBox, desc : "ID of what type of identifier is going to be used", "", ParamEnumArray.FromEnum(EArchetypeIdentifier) )]
	private EArchetypeIdentifier ArchetypeIdentifier;

	//Character Name
	[Attribute("", UIWidgets.Object, desc : "Character full name, used to identify character dialogue. Compared to name in identitycomponent", category:"CharacterInfo")]
	private string m_sCharacterName;
	//Character Faction
	[Attribute("", UIWidgets.Object, desc : "Character faction, used to identify character dialogue. Compared to factionkey in FactionAffiliationComponent", category:"CharacterInfo")]
	private FactionKey m_sCharacterFaction;
	//Character Rank
	[Attribute("50", UIWidgets.ComboBox, desc : "Character rank, used to identify character dialogue. Compared to rank in SCR_CharacterRankComponent", category:"CharacterInfo", ParamEnumArray.FromEnum(SCR_ECharacterRank))]
	private SCR_ECharacterRank m_sCharacterRank;
	//Factions this archetype is for
	[Attribute("FactionKey", desc : "Faction the player needs to be to get access to this archetype", category:"CharacterInfo")]
	private ref array<string> m_aArchetypeFactionMatch;
	//------------------------------------------------------------------//
	[Attribute("DialogueBranch")]
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
	void GetDialogueBranchMap(out map<int, ref SP_DialogueBranch> DialogueMap){DialogueMap = DialogueBranchMap;}
	//------------------------------------------------------------------//
	void GetBranchedID(out int BID){BID = BranchedID;}
	//------------------------------------------------------------------//
	void GetIdentifier(out EArchetypeIdentifier ArchID){ArchID = ArchetypeIdentifier};
	//------------------------------------------------------------------//
	void GetArchetypeTemplateName(out string ArchName){ArchName = m_sCharacterName};
	//------------------------------------------------------------------//
	void GetArchetypeTemplateFaction(out FactionKey ArchFact){ArchFact = m_sCharacterFaction};
	//------------------------------------------------------------------//
	void GetArchetypeTemplateRank(out SCR_ECharacterRank ChRank){ChRank = m_sCharacterRank};
	//------------------------------------------------------------------//
	void GetArchtypeFactionMatch(out array<string> FactMatch) {FactMatch = m_aArchetypeFactionMatch;}
	//------------------------------------------------------------------//
	void GetDialogueBranchArray(out array<ref SP_DialogueBranch> Branches){Branches = DialogueBranch;}
	//------------------------------------------------------------------//
	//Bool returning IsCharacterBranched, used to check if character is branched. True = Branched - False = Unbranched
	bool IsCharacterBranched(){return IsCharacterBranched;}
	//------------------------------------------------------------------//
	//Branches Archetype, setting IsCharacterBranched to true if its not already, and sets ID of branch that dialogue should follow, if branch 0 gets branched we will want to look there for dialogue, this is where the ID is used.
	//If dialogue branch 1 id branched, when we go look for dialogue in the archetype will will only look inside branch 1.
	void BranchArchetype(int Branch)
	{
		if (IsCharacterBranched == false)
		{
			IsCharacterBranched = true;
			BranchedID = Branch;
		}
	}
	//------------------------------------------------------------------//
	//Unbranches Archetype, setting IsCharacterBranched to false if its not already, resets branch ID
	void UnBranchArchetype()
	{
		if (IsCharacterBranched == true)
		{
			IsCharacterBranched = false;
			BranchedID = null;
		}
	}
	//------------------------------------------------------------------//
	//Find Branch using Branch ID
	//If character is branched we look for a branch using our BranchedID that we got once this Archetype got branched and we keep looking deeper using GetCurrentDialogueBranch
	bool GetDialogueBranch(int BranchKey, out SP_DialogueBranch branch)
    {
		if (IsCharacterBranched == true)
		{
			if (!DialogueBranchMap.Find(BranchedID, branch))
			{
	        	return false;
	    	}
			branch.GetCurrentDialogueBranch(TalkingCharacter, BranchKey, branch);
			return true;
		}
		if (!DialogueBranchMap.Find(BranchKey, branch))
			{
	        	return false;
	    	}
		if(branch)
		{
			return true;
		}
        return false;
    }
	//------------------------------------------------------------------//
	//Find correct branch and get dialogue text from it
	bool GetDialogueText(int BranchID, IEntity Character, IEntity Player, out string DiagText)
	{
		SP_DialogueBranch branch;
		GetDialogueBranch(BranchID, branch);
		if (branch)
		{
			branch.GetDialogueText(TalkingCharacter, Player, DiagText);
			return true;
		}
		else
		{
			DiagText = STRING_EMPTY;
			return false;
		}
		return false;
	}
	//------------------------------------------------------------------//
	//Find correct branch using current stage and take action title from it 
	bool GetActionTitle(int BranchID, IEntity Character, IEntity Player, out string ActText)
	{
		SP_DialogueBranch Branch;
		GetDialogueBranch(BranchID, Branch);
		if (Branch)
		{
			Branch.GetActionText(Character, Player, ActText);
			return true;
		}
		else
		{
			ActText = STRING_EMPTY;
			return false;
		}
		return false;
	}
	//------------------------------------------------------------------//
	//Find branch using branchID and increment its stage
	bool IncrementStage(int BranchID, int incrementamount, IEntity Character)
	{	
		SP_DialogueBranch branch
		GetDialogueBranch(BranchID, branch);
		branch.IncrementBranchStage(incrementamount, Character);
		return true;
	}
	//------------------------------------------------------------------//
	
	//------------------------------------------------------------------//
	// constructor for when creating new Archetype
	void SP_DialogueArchetype(SP_DialogueArchetype original, bool isNew = false)
  {
		if (isNew) 
		{
        original.GetIdentifier(ArchetypeIdentifier);
        original.GetArchetypeTemplateName(m_sCharacterName);
        original.GetArchetypeTemplateRank(m_sCharacterRank);
        original.GetArchetypeTemplateFaction(m_sCharacterFaction);
        original.GetDialogueBranchArray(DialogueBranch);
		original.GetDialogueBranchMap(DialogueBranchMap);
		}
  }
	//Mapping all configrations existing uder this character archetype
	//When using ArchetypeTemplate to create new archetype for specific AI, it is Initialised
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
};