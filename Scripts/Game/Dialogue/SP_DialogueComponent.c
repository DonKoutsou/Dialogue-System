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
modded enum ECharacterRank
{
	MEDIC,
	NAVIGATOR,
	COMMANDER
}
class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute()]
	protected ref array<ref SP_DialogueArchetype> m_CharacterArchetypeList;
	//----------------------------------------------------------------------------------------------------------------//
	protected ref map<string, ref SP_DialogueArchetype> DialogueArchetypeMap;	
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	SCR_BaseGameMode GameMode;
	//----------------------------------------------------------------------------------------------------------------//
	//Main function. Its called in SP_DialogueUI when an input is pressed. Branch ID will be different based on the input pressed
	void DoDialogue(IEntity Character, IEntity Player, int BranchID, int IncrementAmount = 1)
	{
		//------------------------------------------------------------------//
		//SenderID needed to send text to chat
		int senderID;
		//String to store the text we want to send on chat
		string 					m_DialogTexttoshow;
		//Name of character we are talking to 
		string 					senderName = GetCharacterName(Character);
		//Dialogue Archetype matching the charcter we are talking to 
		SP_DialogueArchetype 	DiagArch = LocateDialogueArchetype(Character);
		//Get branch located in found archetype using ID
		SP_DialogueBranch 		Branch = DiagArch.GetDialogueBranch(BranchID);
		//------------------------------------------------------------------//
		//For UI
		MenuBase 				myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass 			DiagUI = DialogueUIClass.Cast(myMenu);
		//------------------------------------------------------------------//
		//If CanBePerformed is false dialogue wont be executed
		if (Branch.CanBePerformed(Character, Player) == false)
		{
			DiagUI.UpdateEntries(Character, Player);
			return;
		}
		//------------------------------------------------------------------//
		//Check if branch has another branch in its current stage. If yes we will have to cause a branch after getting our text
		if (Branch.CheckIfStageBranches() == true)
		{
			//--------------------------------------//
			//If our branch should be branched it means that our character will need to be branched as well
			if(DiagArch.IsCharacterBranched() == false)
			{
				DiagArch.BranchArchetype(BranchID);
			}
			//--------------------------------------//
			//Look for the config that matches our character. Config hold info about progression of dialogue for the Specific AI we are talking to.			
			DialogueBranchInfo Conf = Branch.LocateConfig(Character);
			m_DialogTexttoshow = Branch.GetDialogueText(Character);
			SendText(m_DialogTexttoshow, m_ChatChannel, senderID, senderName);
			// Cause a branch of the config
			Conf.CauseBranch(BranchID);
			//Call OnPerform function of the branch stage
			Branch.OnPerform(Character, Player);
			//--------------------------------------//
			DiagUI.UpdateEntries(Character, Player);
			//--------------------------------------//
			//Inherit needed data to next config
			Branch.GetCurrentDialogueBranch(Character, BranchID).InheritData(DiagArch, Conf, Character);
			return;
			//--------------------------------------//
		}
		
		//--------------------------------------//
		Branch.OnPerform(Character, Player);
		DialogueBranchInfo Conf = Branch.LocateConfig(Character);
		m_DialogTexttoshow = Branch.GetDialogueText(Character);
		//--------------------------------------//
		SendText(m_DialogTexttoshow, m_ChatChannel, senderID, senderName);
		//--------------------------------------//
		int stage = Conf.GetDialogueBranchStage() + 1;
		//If a stage exists it means that dialogue can increment
		if (Branch.CheckNextStage(stage) == true)
		{
			IncrementDiagStage(Character, BranchID, IncrementAmount);
		}
		//--------------------------------------//
		DiagUI.UpdateEntries(Character, Player);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Function used for "GoBack" and "Leave" dialogue options
	void DoBackDialogue(IEntity Character, IEntity Player)
	{
		string senderName = GetCharacterName(Character);
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(Character);
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
		DiagUI.UpdateEntries(Character, Player);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Send text function for sending the provided text to chat 
	void SendText(string Text, BaseChatChannel Chanell, int SenderID, string SenderName)
	{
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(Text, m_ChatChannel, SenderID, SenderName);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Get ActionName function used in SP_DialogueUI to check if provided branch ID gives us any text.
	string GetActionName(int BranchID, IEntity Character, IEntity Player)
	{
		string m_sActionName;
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(Character);
		m_sActionName = DiagArch.GetActionTitle(BranchID, Character, Player);
		return m_sActionName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Locates Archetype using LocateDialogueArchetype function using Ientity provided.
	//Increments stage of branch in the archetype
	bool IncrementDiagStage(IEntity owner, int BranchID, int incrementamount)
	{
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(owner);
		DiagArch.IncrementStage(BranchID, incrementamount);
		return false;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER NAME
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
	//CHARACTER RANK
	ECharacterRank GetCharacterRank(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
	//CHARACTER FACTION
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
	SP_DialogueArchetype LocateDialogueArchetype(IEntity Character)
	{
		SP_DialogueArchetype CharDialogueArch;
		//using character full name atm to match Character with Archetype
		string LocCharacterName = GetCharacterName(Character);
		if (!GetArchetypeTemplate(Character))
		{
			return CharDialogueArch;
		}
		//Check if an Archetype with out character's name exists
		if (DialogueArchetypeMap.Contains(LocCharacterName))
			{
				//--------------------------------------------------------//
				//if yes assign it to CharDialogueArch so we can return it
			    CharDialogueArch = DialogueArchetypeMap[LocCharacterName];
			}
			else
			{
				//-------------------------------------------------------------------------//
				//if not find an ArchetypeTemplate, make a copy of it and instet it in DialogueArchetypeMap
				//find character template using our character entity
				CharDialogueArch = GetArchetypeTemplate(Character);
				
				//-------------------------------------------------------------------------//
				//create a new archetype and copy the stuff in it
				SP_DialogueArchetype DiagArchNew = CopyArchetype(CharDialogueArch);
				//-------------------------------------------------------------------------//
				//initialise the newly made Archetype after its filled with all data
				DiagArchNew.Init(Character);
				//-------------------------------------------------------------------------//
				//instert it int the ArchetypeMap
				DialogueArchetypeMap.Insert(LocCharacterName, DiagArchNew);
				return DiagArchNew;
				//-------------------------------------------------------------------------//
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

