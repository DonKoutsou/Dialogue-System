enum EArchetypeIdentifier
	{
		Name,
		Rank,
		"FactionKey",
		"Faction and Rank"
	};
enum EChoiseBehavior
	{
		IncrementDialogueStage,
		Stay,
		IncrementDialogueStageandGoBack
	};

class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute()]
	protected ref array<ref SP_DialogueArchetype> m_CharacterArchetypeList;
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	//----------------------------------------------------------------------------------------------------------------//
	//Dialogue System
	protected ref map<string, ref SP_DialogueArchetype> DialogueArchetypeMap;
	SCR_BaseGameMode GameMode;
	
	
	
	//------------------------------------------------------------------//
	//Main function used to send text to chat. 
	//Has the logic for branching the dialogue Archetype(Branching cuases UI entries to get text and button logic from SP_MultipleChoiceConfig
	void DoDialogue(IEntity Character, IEntity Player, int BranchID, int IncrementAmount)
	{
		//Get name of character that will send message to chat
		string senderName = GetCharacterName(Character);
		int senderID;
		SP_DialogueArchetype DiagArch = LocateCharacterArchetype(Character);
		string m_DialogTexttoshow;
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		//check if this achetype is branched. if yes get text from SP_MultipleChoiceConfig and return before incrementing stage
		SP_DialogueBranch Branch = DiagArch.GetDialogueBranch(BranchID);
		if (Branch.CheckIfStageBranches() == true)
		{
			if(DiagArch.IsCharacterBranched() == false)
			{
				DiagArch.BranchArchetype(BranchID);
			}
			DialogueBranchInfo Conf = Branch.LocateConfig(Character);
			m_DialogTexttoshow = DiagArch.GetDialogueText(BranchID);
			SendText(m_DialogTexttoshow, m_ChatChannel, senderID, senderName);
			Conf.CauseBranch(BranchID);
			Branch.GetCurrentDialogueBranch(Character, BranchID).InheritData(DiagArch, Conf);
			DiagUI.Init(Character, Player);
			DiagUI.UpdateEntries();
			Branch.OnPerform(Character, Player);
			return;
		}
		m_DialogTexttoshow = DiagArch.GetDialogueText(BranchID);
		SendText(m_DialogTexttoshow, m_ChatChannel, senderID, senderName);
		Branch.OnPerform(Character, Player);
		IncrementDiagStage(Character, BranchID, IncrementAmount);
		DiagUI.Init(Character, Player);
		DiagUI.UpdateEntries();
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Function used for leaving a branch and going back to initial menu, and also leaving branch to go back to another branch
	//Unbranches character and updates UI
	void DoBackDialogue(IEntity Character, IEntity Player)
	{
		string senderName = GetCharacterName(Character);
		SP_DialogueArchetype DiagArch = LocateCharacterArchetype(Character);
		string m_DialogTexttoshow = "Go Back";
		SP_DialogueBranch Branch = DiagArch.GetDialogueBranch(DiagArch.GetBranchedID());
		DialogueBranchInfo ParentBranch = Branch.GetParent();
		if (!Branch)
		{
			DiagArch.UnBranchArchetype();
		}
		else if (Branch && ParentBranch && ParentBranch.CheckifBranched() == true)
		{
			DialogueBranchInfo m_BranchStages = Branch.LocateConfig(Character);
			m_BranchStages.GetParentConfig().Unbranch();
			if (!ParentBranch.GetParentConfig())
			{
				DiagArch.UnBranchArchetype();
			}
		}
		else if (Branch && ParentBranch && ParentBranch.CheckifBranched() == false)
		{
			DiagArch.UnBranchArchetype();
		}
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		DiagUI.Init(Character, Player);
		DiagUI.UpdateEntries();
	}
	//----------------------------------------------------------------------------------------------------------------//
	// Used to send text to chat, used on DoDialogue and DoBackDialogue
	void SendText(string Text, BaseChatChannel Chanell, int SenderID, string SenderName)
	{
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(Text, m_ChatChannel, SenderID, SenderName);
	}
	//----------------------------------------------------------------------------------------------------------------//
	// returns string from dialogue configuration under field Action Text, ment to be used for text shown on the action
	string GetActionName(int BranchID, IEntity Owner)
	{
		string m_sActionName;
		SP_DialogueArchetype DiagArch = LocateCharacterArchetype(Owner);
		m_sActionName = DiagArch.GetActionTitle(BranchID);
		return m_sActionName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//progress dialogue stage int, need to figure out where/how to store the stage
	bool IncrementDiagStage(IEntity owner, int BranchID, int incrementamount)
	{
		//find correct archetype
		SP_DialogueArchetype DiagArch = LocateCharacterArchetype(owner);
		//Increment the stage of the correct branch
		DiagArch.IncrementStage(BranchID, incrementamount);
		return false;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Getters for character info for identifiers, character name also used on sent text
	//Character full name
	string GetCharacterName(IEntity Character)
	{
		SCR_CharacterIdentityComponent IdentityComponent = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		string CharacterFullName;
		if(IdentityComponent)
		{
			return IdentityComponent.GetCharacterFullName();
		}
		else
			return STRING_EMPTY;
	}
	//Character rank
	ECharacterRank GetCharacterRank(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
	//Character Faction
	FactionKey GetCharacterFaction(IEntity Character)
	{
		FactionAffiliationComponent FactionComponent = FactionAffiliationComponent.Cast(Character.FindComponent(FactionAffiliationComponent));
		if(FactionComponent)
		{
			return FactionComponent.GetAffiliatedFaction().GetFactionKey();
		}
		else
			return STRING_EMPTY;	
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Using Dialogue ID (EDiagIdentifier) figure out wich archetype matches provided character.
	SP_DialogueArchetype GetArchetypeTemplate(IEntity pOwnerEntity)
	{
		string senderName;
			SP_DialogueArchetype DiagArch;
			ECharacterRank senderRank;
			FactionKey senderFaction;
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			EArchetypeIdentifier Archid = m_CharacterArchetypeList[i].GetIdentifier();
			switch (Archid) 
			{
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 0 means we look for Character Name
		    case 0:
				senderName = GetCharacterName(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeTemplateName() == senderName)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 1 means we look for Character Rank
		    case 1:
				senderRank = GetCharacterRank(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeTemplateRank() == senderRank)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 2 means we look for Character Faction
			case 2:
				senderFaction = GetCharacterFaction(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeTemplateFaction() == senderFaction)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 3 means we look for Character Faction and Rank
			case 3:
				senderFaction = GetCharacterFaction(pOwnerEntity);
				senderRank = GetCharacterRank(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeTemplateFaction() == senderFaction && m_CharacterArchetypeList[i].GetArchetypeTemplateRank() == senderRank)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			}
		}
	return DiagArch;
	}
	//-----------------------------------------------------------------------------------------------------------//
	// locate if there is already an Archetype instace for this specific charater and if not initiates the creation of one
	SP_DialogueArchetype LocateCharacterArchetype(IEntity Character)
	{
		SP_DialogueArchetype CharDialogueArch;
		//using character full name atm to match Character with Archetype
		string LocCharacterName = GetCharacterName(Character);
		//Check if an Archetype with out character's name exists
		if (DialogueArchetypeMap.Contains(LocCharacterName))
			{
				//if yes assign it to CharDialogueArch so we can return it
			    CharDialogueArch = DialogueArchetypeMap[LocCharacterName];
			}
			else
			{
				//if not find an ArchetypeTemplate, make a copy of it and instet it in DialogueArchetypeMap
				//find character template using our character entity
				CharDialogueArch = GetArchetypeTemplate(Character);
				//create a new archetype and copy the stuff in it
				SP_DialogueArchetype DiagArchNew = CopyArchetype(CharDialogueArch);
				//initialise the newly made Archetype after its filled with all data
				DiagArchNew.Init(Character);
				//instert it int the ArchetypeMap
				DialogueArchetypeMap.Insert(LocCharacterName, DiagArchNew);
				return DiagArchNew;
			}
	return CharDialogueArch;
	}
	//-----------------------------------------------------------------------------------------------------------//
	//takes all info requred from Archetype and returns a newly made Archetype with the copied info
	SP_DialogueArchetype CopyArchetype(SP_DialogueArchetype OriginalArchetype)
	{
		SP_DialogueArchetype DiagArchCopy = new SP_DialogueArchetype(OriginalArchetype, true);
		return DiagArchCopy;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//initialise configuration to crate map of configuration's contents
	override void EOnInit(IEntity owner)
	{
		GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		foreach (SP_DialogueArchetype config: m_CharacterArchetypeList)
		{
			config.Init(owner);
		}
		DialogueArchetypeMap = new map<string, ref SP_DialogueArchetype>;
	}
	//----------------------------------------------------------------------------------------------------------------//
	// set masks;
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
		SetEventMask(owner, EntityEvent.POSTFIXEDFRAME);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
}
//----------------------------------------------------------------------------------------------------------------//

