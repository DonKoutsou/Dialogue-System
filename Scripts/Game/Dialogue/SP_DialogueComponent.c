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
modded enum SCR_ECharacterRank
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
	//Channels
	[Attribute()]
	ref BaseChatChannel m_ChatChannelFIA;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelUSSR;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelUS;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelBANDITS;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelSPEIRA;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelANOUNCER;
	//----------------------------------------------------------------------------------------------------------------//
	BaseGameMode GameMode;
	
	void Escape(IEntity Char, IEntity Player)
	{
		DoBackDialogue(Char, Player);
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(Char, Player);
		while(DiagArch.IsCharacterBranched == true)
		{
			DoBackDialogue(Char, Player);
		}
		DoBackDialogue(Char, Player);
		ReleaseAI(Char, Player);
		GetGame().GetMenuManager().CloseAllMenus();
	}
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
		//Faction
		FactionKey				senderFaction = GetCharacterFaction(Character).GetFactionKey();
		//Dialogue Archetype matching the charcter we are talking to 
		SP_DialogueArchetype 	DiagArch = LocateDialogueArchetype(Character, Player);
		//Get branch located in found archetype using ID
		SP_DialogueBranch 		Branch;
		DiagArch.GetDialogueBranch(BranchID, Branch);
		//------------------------------------------------------------------//
		//For UI
		MenuBase 				myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		DialogueUIClass 		DiagUI = DialogueUIClass.Cast(myMenu);
		//------------------------------------------------------------------//
		BaseChatChannel Channel;
		//If CanBePerformed is false dialogue wont be executed
		if (Branch.CanBePerformed(Character, Player) == false)
		{
			DiagUI.UpdateEntries(Character, Player);
			return;
		}
		switch (senderFaction)
			{
				case "FIA":
					Channel = m_ChatChannelFIA;
				break;
				case "USSR":
					Channel = m_ChatChannelUSSR;
				break;
				case "BANDITS":
					Channel = m_ChatChannelBANDITS;
				break;
				case "SPEIRA":
					Channel = m_ChatChannelSPEIRA;
				break;
				case "US":
					Channel = m_ChatChannelUS;
				break;
			}
		//------------------------------------------------------------------//
		//Check if branch has another branch in its current stage. If yes we will have to cause a branch after getting our text
		if (Branch.CheckIfStageBranches(Character, Player) == true)
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
			Branch.GetDialogueText(Character, Player, m_DialogTexttoshow);
			SendText(m_DialogTexttoshow, Channel, senderID, senderName, GetCharacterRankName(Character));
			// Cause a branch of the config
			Branch.CauseBranch(BranchID, Character);
			//Call OnPerform function of the branch stage
			Branch.OnPerform(Character, Player);
			//--------------------------------------//
			DiagUI.UpdateEntries(Character, Player);
			//--------------------------------------//
			return;
			//--------------------------------------//
		}
		//--------------------------------------//
		Branch.OnPerform(Character, Player);
		DialogueBranchInfo Conf = Branch.LocateConfig(Character);
		Branch.GetDialogueText(Character, Player, m_DialogTexttoshow);
		//--------------------------------------//
		SendText(m_DialogTexttoshow, Channel, senderID, senderName, GetCharacterRankName(Character));
		
		//--------------------------------------//
		int Bstage;
		Conf.GetDialogueBranchStage(Bstage);
		//If a stage exists it means that dialogue can increment
		if (Branch.CheckNextStage(Bstage + 1) == true)
		{
			IncrementDiagStage(Character, Player, BranchID, IncrementAmount);
		}
		//--------------------------------------//
		DiagUI.UpdateEntries(Character, Player);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Function used for "GoBack" and "Leave" dialogue options
	void DoBackDialogue(IEntity Character, IEntity Player)
	{
		string senderName = GetCharacterName(Character);
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(Character, Player);
		string m_DialogTexttoshow = "Go Back";
		int BranchID;
		DiagArch.GetBranchedID(BranchID);
		SP_DialogueBranch Branch;
		DiagArch.GetDialogueBranch(0, Branch);
		DialogueBranchInfo ParentBranch;
		Branch.GetParent(Character, ParentBranch);
		if (!Branch)
		{
			DiagArch.UnBranchArchetype();
		}
		else if (Branch && ParentBranch && ParentBranch.CheckifBranched() == true)
		{
			DialogueBranchInfo Conf = Branch.LocateConfig(Character);
			DialogueBranchInfo PConf;
			Conf.GetParentConfig(PConf);
			PConf.Unbranch();
			DialogueBranchInfo PofPConf;
			PConf.GetParentConfig(PofPConf);
			if (!PofPConf)
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
		PlayDialogueSound();
	}
	void ReleaseAI(IEntity Character, IEntity Player)
	{
		AIControlComponent comp = AIControlComponent.Cast(Character.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		SCR_AIConverseBehavior act = SCR_AIConverseBehavior.Cast(utility.FindActionOfType(SCR_AIConverseBehavior));
		act.SetActionState(EAIActionState.FAILED);
		//utility.SetStateAllActionsOfType(SCR_AIConverseBehavior, EAIActionState.FAILED, false);
	}
	//----------------------------------------------------------------------------------------------------------------//
	void DoAnouncerDialogue(string Text)
	{
		//------------------------------------------------------------------//
		//SenderID needed to send text to chat
		int senderID;
		//String to store the text we want to send on chat
		string 					m_DialogTexttoshow;
		//Name of character we are talking to 
		string 					senderName = "Anouncer";
		string					senderRank = "PC";
		//--------------------------------------//
		m_DialogTexttoshow = Text;
		//--------------------------------------//
		SendText(m_DialogTexttoshow, m_ChatChannelANOUNCER, senderID, senderName, senderRank);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Send text function for sending the provided text to chat 
	void SendText(string Text, BaseChatChannel Chanell, int SenderID, string SenderName, string rank)
	{
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(Text, Chanell, SenderID, rank + " " + SenderName);
		PlayDialogueSound();
	}
	void PlayDialogueSound()
	{
		IEntity player = GetGame().GetPlayerController().GetControlledEntity();
		if(!player)
		{
			return;
		}
		CharacterSoundComponent SoundC = CharacterSoundComponent.Cast(player.FindComponent(CharacterSoundComponent));
		SoundC.SoundEvent("SOUND_RADIO_CHANGEFREQUENCY_ERROR");
	}
	
	//----------------------------------------------------------------------------------------------------------------//
	//Get ActionName function used in SP_DialogueUI to check if provided branch ID gives us any text.
	void GetActionName(int BranchID, IEntity Character, IEntity Player, out string ActName)
	{
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(Character, Player);
		DiagArch.GetActionTitle(BranchID, Character, Player, ActName);
	}
	//----------------------------------------------------------------------------------------------------------------//
	//Locates Archetype using LocateDialogueArchetype function using Ientity provided.
	//Increments stage of branch in the archetype
	bool IncrementDiagStage(IEntity owner, IEntity Player, int BranchID, int incrementamount)
	{
		SP_DialogueArchetype DiagArch = LocateDialogueArchetype(owner, Player);
		DiagArch.IncrementStage(BranchID, incrementamount, owner);
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
			Identity ID = IdentityComponent.GetIdentity();
			if(ID)
			{
				CharacterFullName = ID.GetFullName();
			}
		}
		return CharacterFullName;
	}
	//CHARACTER RANK
	SCR_ECharacterRank GetCharacterRank(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
	string GetCharacterRankName(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		string CharacterRank;
		if(RankComponent)
		{
			CharacterRank = RankComponent.GetCharacterRankNameShort(Character);
		}
		return CharacterRank;
	}
	//CHARACTER FACTION
	Faction GetCharacterFaction(IEntity Character)
	{
		FactionAffiliationComponent FactionComponent = FactionAffiliationComponent.Cast(Character.FindComponent(FactionAffiliationComponent));
		if(FactionComponent)
		{
			return FactionComponent.GetAffiliatedFaction();
		}
		else
			return null;	
	}
	//----------------------------------------------------------------------------------------------------------------//
	SP_DialogueArchetype GetArchetypeTemplate(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		string senderName = GetCharacterName(pOwnerEntity);
		SCR_ECharacterRank senderRank = GetCharacterRank(pOwnerEntity);
		FactionKey senderFaction = GetCharacterFaction(pOwnerEntity).GetFactionKey();
		
		FactionKey playerFaction = GetCharacterFaction(pUserEntity).GetFactionKey();
		
		SP_DialogueArchetype DiagArch;
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			array <string> factions;
			m_CharacterArchetypeList[i].GetArchtypeFactionMatch(factions);
			if (factions.Contains(playerFaction) == true)
			{
				EArchetypeIdentifier Archid;
				m_CharacterArchetypeList[i].GetIdentifier(Archid);
				string ArchTempN;
				m_CharacterArchetypeList[i].GetArchetypeTemplateName(ArchTempN);
				SCR_ECharacterRank ArchRank;
				m_CharacterArchetypeList[i].GetArchetypeTemplateRank(ArchRank);
				FactionKey ArchFaction;
				m_CharacterArchetypeList[i].GetArchetypeTemplateFaction(ArchFaction);
				
				switch (Archid) 
				{
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 0 means we look for Character Name
			    case 0:
					if (ArchTempN == senderName)
					{
						DiagArch = m_CharacterArchetypeList[i];
						return DiagArch;	
					}
					break;
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 1 means we look for Character Rank
			    case 1:
					if (ArchRank == senderRank)
					{
						DiagArch = m_CharacterArchetypeList[i];
						return DiagArch;
					}
					break;
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 2 means we look for Character Faction
				case 2:
					if (ArchFaction == senderFaction)
					{
						DiagArch = m_CharacterArchetypeList[i];
						return DiagArch;
					}
					break;
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 3 means we look for Character Faction and Rank
				case 3:
					if (ArchFaction == senderFaction && ArchRank == senderRank)
					{
						DiagArch = m_CharacterArchetypeList[i];
						return DiagArch;
					}
					break;
				}
			}
			//-----------------------------------------------------------------------------------------------------------//
		}
		return DiagArch;
	}
	//-----------------------------------------------------------------------------------------------------------//
	// locate if there is already an Archetype instace for this specific charater and if not initiates the creation of one
	SP_DialogueArchetype LocateDialogueArchetype(IEntity Owner, IEntity User)
	{
		SP_DialogueArchetype CharDialogueArch;
		//using character full name atm to match Owner with Archetype
		string LocCharacterName = GetCharacterName(Owner);
		if (!GetArchetypeTemplate(Owner, User))
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
				CharDialogueArch = GetArchetypeTemplate(Owner, User);
				
				//-------------------------------------------------------------------------//
				//create a new archetype and copy the stuff in it
				SP_DialogueArchetype DiagArchNew = CopyArchetype(CharDialogueArch);
				//-------------------------------------------------------------------------//
				//initialise the newly made Archetype after its filled with all data
				DiagArchNew.Init(Owner);
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
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
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
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
}
//----------------------------------------------------------------------------------------------------------------//

