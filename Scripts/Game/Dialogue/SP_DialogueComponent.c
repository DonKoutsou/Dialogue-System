enum EArchetypeIdentifier
{
	Name,
	Rank,
	Faction_Key,
	Faction_and_Rank,
	Radio
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
	[Attribute(desc : "Used when figuring out strings for locations")]
	protected ref SCR_MapLocationQuadHint m_aWorldDirections;
	
	static protected ref SCR_MapLocationQuadHint m_WorldDirections;
	
	//containing text history of all characters player has talked to
	ref map <string, ref array <string>> texthistorymap;
	ref map <string, ref array <string>> PLtexthistorymap;
	//containing text history of character currently talking too
	ref array <string>				a_texthistory;
	ref array <string>				a_PLtexthistory;
	//all characters player talked too
	static ref array <IEntity> a_PLcontactList;
	//----------------------------------------------------------------------------------------------------------------//
	static SCR_GameModeCampaign GameMode;
	
	static SP_DialogueComponent GetInstance()
	{
		if (!GameMode)
			return null;
		return SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
	};
	void GetContactList(out array <IEntity> ContactList)
	{
		ContactList.Copy(a_PLcontactList);
	}
	void AddToContactList(IEntity Char)
	{
		if (!a_PLcontactList.Contains(Char))
			a_PLcontactList.Insert(Char)
	}
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
	string DoDialogue(IEntity Character, IEntity Player, int BranchID, int IncrementAmount = 1)
	{
		//------------------------------------------------------------------//
		//SenderID needed to send text to chat
		int senderID;
		string actiontext;
		//String to store the text we want to send on chat
		string 					m_DialogTexttoshow;
		//Name of character we are talking to 
		string 					senderName = GetCharacterName(Character);
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
			return "Cant do dat.";
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
			
			Branch.GetActionText(Character, Player, actiontext);
			Branch.GetDialogueText(Character, Player, m_DialogTexttoshow);
			AddLinesToHistory(m_DialogTexttoshow, actiontext);
			Branch.OnPerform(Character, Player);
			//SendText(m_DialogTexttoshow, Channel, senderID, senderName, GetCharacterRankName(Character));
			// Cause a branch of the config
			Branch.CauseBranch(BranchID, Character);
			//Call OnPerform function of the branch stage
			
			//--------------------------------------//
			DiagUI.UpdateEntries(Character, Player);
			array<BaseInfoDisplay> infoDisplays = {};
			GetGame().GetPlayerController().GetHUDManagerComponent().GetInfoDisplays(infoDisplays);
			foreach (BaseInfoDisplay baseInfoDisplays : infoDisplays)
			{
				SCR_DialogueWidget DialogueDisplay = SCR_DialogueWidget.Cast(baseInfoDisplays);
				if (!DialogueDisplay)
					continue;
	
				DialogueDisplay.SetTarget(Character);
				DialogueDisplay.SetText(m_DialogTexttoshow);
				DialogueDisplay.ShowInspectCasualtyWidget(Character);
			}
			//--------------------------------------//
			return m_DialogTexttoshow;
			//--------------------------------------//
		}
		//--------------------------------------//
		
		DialogueBranchInfo Conf = Branch.LocateConfig(Character);
		Branch.GetActionText(Character, Player, actiontext);
		Branch.OnPerform(Character, Player);
		Branch.GetDialogueText(Character, Player, m_DialogTexttoshow);
		//--------------------------------------//
		//SendText(m_DialogTexttoshow, Channel, senderID, senderName, GetCharacterRankName(Character));
		AddLinesToHistory(m_DialogTexttoshow, actiontext);

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
		PlayDialogueSound();
		array<BaseInfoDisplay> infoDisplays = {};
		GetGame().GetPlayerController().GetHUDManagerComponent().GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays : infoDisplays)
		{
			SCR_DialogueWidget DialogueDisplay = SCR_DialogueWidget.Cast(baseInfoDisplays);
			if (!DialogueDisplay)
				continue;

			DialogueDisplay.SetTarget(Character);
			DialogueDisplay.SetText(m_DialogTexttoshow);
			DialogueDisplay.ShowInspectCasualtyWidget(Character);
		}
		return m_DialogTexttoshow;
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
	
	//Function used when leaving dialogue, finds converse action and completes it
	void ReleaseAI(IEntity Character, IEntity Player)
	{
		//archive history
		ArchiveTextHistory(Character);
		
		//find SCR_AIUtilityComponent and SCR_AIConverseBehavior and complete them
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
		if (act)
			act.SetActiveConversation(false);
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
		SendText(m_DialogTexttoshow, null, senderID, senderName, senderRank);
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
		SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_RADIO_CHANGEFREQUENCY_ERROR);
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
	SP_DialogueArchetype GetArchetypeTemplate(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_CharacterIdentityComponent id = SCR_CharacterIdentityComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterIdentityComponent));
		if (id.HasArchetype())
			return id.GetArchetype();
		
		string senderName = pOwnerEntity.GetName();
		
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
					}
					break;
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 2 means we look for Character Faction
				case 2:
					if (ArchFaction == senderFaction)
					{
						DiagArch = m_CharacterArchetypeList[i];
					}
					break;
				//-----------------------------------------------------------------------------------------------------------//
				// diagid 3 means we look for Character Faction and Rank
				case 3:
					if (ArchFaction == senderFaction && ArchRank == senderRank)
					{
						DiagArch = m_CharacterArchetypeList[i];
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
		
		if (!GetArchetypeTemplate(Owner, User))
		{
			return CharDialogueArch;
		}
		//using character full name atm to match Owner with Archetype
		string LocCharacterName = GetCharacterName(Owner);
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
		if (!GetGame().InPlayMode())
			return;
		GameMode = SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
		foreach (SP_DialogueArchetype config: m_CharacterArchetypeList)
		{
			config.Init(owner);
		}
		DialogueArchetypeMap = new map<string, ref SP_DialogueArchetype>;
		m_WorldDirections = m_aWorldDirections;
		if	(!a_texthistory)
			a_texthistory = new array <string>();
		if	(!a_PLtexthistory)
			a_PLtexthistory = new array <string>();
		if	(!texthistorymap)
			texthistorymap = new map <string,ref array <string>>();
		if	(!PLtexthistorymap)
			PLtexthistorymap = new map <string,ref array <string>>();
		if	(!a_PLcontactList)
			a_PLcontactList = new array <IEntity>();

	}
	
	//----------------------------------------------------------------------------------------------------------------//
	// set masks;
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
	
	////////////////////////////////////////////Text history/////////////////////////////////////////
	
	// texthistorymap contains line character has told to character
	// PLtexthistorymap contains lines the player has told to character
	
	//add entry of character to map texthistorymap, checks if there isnt one already
	//also sets character as current character meaning the a_texthistory and a_PLtexthistory will be filled witht this character's lines if this function is called
	void RegisterCharInHistory(IEntity Owner)
	{
		//if character doesent exist in map add him
		if (!texthistorymap.Get(Owner.GetID().ToString()))
		{
			//create array that will be added to map alongside character
			array <string>	texthistory = {};
			texthistorymap.Insert(Owner.GetID().ToString(), texthistory);
		}
		//if character doesent exist in pl map add him
		array <string>	PLtexthistory = {};
		if (!PLtexthistorymap.Get(Owner.GetID().ToString()))
		{
			PLtexthistorymap.Insert(Owner.GetID().ToString(), PLtexthistory);
		}
		//check current active arrays is the one of this character
		if (a_texthistory != texthistorymap.Get(Owner.GetID().ToString()))
		{
			a_texthistory = texthistorymap.Get(Owner.GetID().ToString());
		}
		if (a_PLtexthistory != PLtexthistorymap.Get(Owner.GetID().ToString()))
		{
			a_PLtexthistory = PLtexthistorymap.Get(Owner.GetID().ToString());
		}
	}
	
	void AddLinesToHistory(string CharLine, string PlayerLine)
	{
		a_texthistory.Insert(CharLine);
		if (PlayerLine)
			a_PLtexthistory.Insert(PlayerLine);
		else
			a_PLtexthistory.Insert("null");
	}
	
	//Retrieves history
	void GetTextHistory(out array <string> hist, out array <string> PLhist)
	{
		//if (!a_texthistory.IsEmpty())
			//hist.Copy(a_texthistory);
		for (int i = 0; i < a_texthistory.Count(); i++)
		{
			hist.Insert(a_texthistory[i]);
		}
		for (int i = 0; i < a_PLtexthistory.Count(); i++)
		{
			PLhist.Insert(a_PLtexthistory[i]);
		}
	}
	
	void ArchiveTextHistory(IEntity Owner)
	{
		texthistorymap.Insert(Owner.GetID().ToString(), a_texthistory);
		PLtexthistorymap.Insert(Owner.GetID().ToString(), a_PLtexthistory);
		//a_texthistory.Clear();
		//a_PLtexthistory.Clear();
	}
	
	
	///////////////////////////////////////////Static helper functions//////////////////////////////////////////////////
	//----------------------------------------------------------------------------------------------------------------//
	static string GetEditableEntName(IEntity ent)
	{
		SCR_EditableEntityComponent editComp = SCR_EditableEntityComponent.Cast(ent.FindComponent(SCR_EditableEntityComponent));
		SCR_UIInfo uiinfo = editComp.GetInfo(ent);
		return uiinfo.GetName();
	}
	
	//CHARACTER FULL NAME
	static string GetCharacterName(notnull IEntity Character)
	{
		SCR_CharacterIdentityComponent IdentityComponent = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		
		string CharacterFullName;
		if(IdentityComponent)
		{
			Identity ID = IdentityComponent.GetIdentity();
			CharacterFullName = ID.GetFullName();
		}
		return CharacterFullName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER FIRST NAME
	static string GetCharacterFirstName(notnull IEntity Character)
	{
		SCR_CharacterIdentityComponent IdentityComponent = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		
		string CharacterFullName;
		if(IdentityComponent)
		{
			Identity ID = IdentityComponent.GetIdentity();
			CharacterFullName = ID.GetName();
			CharacterFullName = CharacterFullName.Substring(17, CharacterFullName.Length() - 17);
		}
		return CharacterFullName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER SURNAME
	static string GetCharacterSurname(notnull IEntity Character)
	{
		SCR_CharacterIdentityComponent IdentityComponent = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		
		string CharacterFullName;
		if(IdentityComponent)
		{
			Identity ID = IdentityComponent.GetIdentity();
			if(ID)
			{
				CharacterFullName = ID.GetSurname();
				CharacterFullName = CharacterFullName.Substring(15, CharacterFullName.Length() - 15)
			}
		}
		return CharacterFullName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER REPUTATION
	static int GetCharacterRep(notnull IEntity Character)
	{
		SCR_CharacterIdentityComponent Idcomp = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		int MyRep = Idcomp.GetRep();
		return MyRep;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER RANKINIGNIA
	static string GetCharacterRankInsignia(notnull IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		SCR_FactionManager factman = SCR_FactionManager.Cast(GetGame().GetFactionManager());
		SCR_Faction fact = SCR_Faction.Cast(GetCharacterFaction(Character));
		if(RankComponent)
		{
			SCR_ECharacterRank curRank = RankComponent.GetCharacterRank(Character);
			return fact.GetRankInsignia(curRank);
		}
		return STRING_EMPTY;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER RANK
	static SCR_ECharacterRank GetCharacterRank(notnull IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER RANK NAME SHORT
	static string GetCharacterRankName(notnull IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		string CharacterRank;
		if(RankComponent)
		{
			CharacterRank = RankComponent.GetCharacterRankNameShort(Character);
		}
		return CharacterRank;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER RANK NAME FULL
	static string GetCharacterRankNameFull(notnull IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		string CharacterRank;
		if(RankComponent)
		{
			CharacterRank = RankComponent.GetCharacterRankName(Character);
		}
		return CharacterRank;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER FACTION
	static Faction GetCharacterFaction(notnull IEntity Character)
	{
		FactionAffiliationComponent FactionComponent = FactionAffiliationComponent.Cast(Character.FindComponent(FactionAffiliationComponent));
		if(FactionComponent)
		{
			return FactionComponent.GetAffiliatedFaction();
		}
		return null;	
	}
	//----------------------------------------------------------------------------------------------------------------//
	static string GetCharacterLocation(IEntity Character, bool unlockloc = 0)
	{
		int m_iGridSizeX;
		int m_iGridSizeY;
		float angleA = 0.775;
		const float angleB = 0.325;
		vector mins,maxs;
		GenericWorldEntity world = GetGame().GetWorldEntity();
		if (!world)
			return STRING_EMPTY;
		world.GetWorldBounds(mins, maxs);
			
		m_iGridSizeX = maxs[0]/3;
		m_iGridSizeY = maxs[2]/3;
	 
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		vector posPlayer = Character.GetOrigin();
		
		SCR_CampaignMilitaryBaseManager BaseMan = GameMode.GetBaseManager();
		SCR_CampaignMilitaryBaseComponent nearest = BaseMan.GetClosestBase(Character.GetOrigin());
		if (!nearest)
			return STRING_EMPTY;
		if (unlockloc)
		{
			if (!BaseMan.IsBaseVisible(nearest))
			BaseMan.SetBaseVisible(nearest);
		}
		
		
		GenericEntity nearestLocation = nearest.GetOwner();
		SCR_MapDescriptorComponent mapDescr = SCR_MapDescriptorComponent.Cast(nearestLocation.FindComponent(SCR_MapDescriptorComponent));
		string closestLocationName;
		closestLocationName = nearest.GetBaseName();

		vector lastLocationPos = nearestLocation.GetOrigin();
		float lastDistance = vector.DistanceSqXZ(lastLocationPos, posPlayer);
	
		string closeLocationAzimuth;
		vector result = posPlayer - lastLocationPos;
		result.Normalize();
		
		float angle1 = vector.DotXZ(result,vector.Forward);
		float angle2 = vector.DotXZ(result,vector.Right);
				
		if (angle2 > 0)
		{
			if (angle1 >= angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionNorth";
			if (angle1 < angleA && angle1 >= angleB )
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionNorthEast";
			if (angle1 < angleB && angle1 >=-angleB)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionEast";
			if (angle1 < -angleB && angle1 >=-angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionSouthEast";
			if (angle1 < -angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionSouth";
		}
		else
		{
			if (angle1 >= angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionNorth";
			if (angle1 < angleA && angle1 >= angleB )
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionNorthWest";
			if (angle1 < angleB && angle1 >=-angleB)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionWest";
			if (angle1 < -angleB && angle1 >=-angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionSouthWest";
			if (angle1 < -angleA)
				closeLocationAzimuth = "#AR-MapLocationHint_DirectionSouth";
		}
		int playerGridPositionX = posPlayer[0]/m_iGridSizeX;
		int playerGridPositionY = posPlayer[2]/m_iGridSizeY;
			
		int playerGridID = GetGridIndex(playerGridPositionX,playerGridPositionY);
	 	string m_sLocationName = m_WorldDirections.GetQuadHint(playerGridID) + ", " + closestLocationName;
		return m_sLocationName;
	}
	//used in GetCharacterLocation, duuno
	static protected int GetGridIndex(int x, int y)
	{
		return 3*y + x;
	}
}
//----------------------------------------------------------------------------------------------------------------//
modded enum ChimeraMenuPreset
{
	DialogueMenu,
}