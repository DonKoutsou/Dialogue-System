enum EArchetypeIdentifier
{
	Name,
	Rank,
	"FactionKey",
	"Faction and Rank",
	"Radio"
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
	[Attribute()]
	protected ref SCR_MapLocationQuadHint m_aWorldDirections;
	//Channels
	[Attribute()]
	ref BaseChatChannel m_ChatChannelFIA;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelUSSR;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelUS;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannelANOUNCER;
	
	static protected ref SCR_MapLocationQuadHint m_WorldDirections;
	
	ref map <string, ref array <string>> texthistorymap;
	ref map <string, ref array <string>> PLtexthistorymap;
	ref array <string>				a_texthistory;
	ref array <string>				a_PLtexthistory;
	static ref array <IEntity> a_PLcontactList;
	//----------------------------------------------------------------------------------------------------------------//
	SCR_BaseGameMode GameMode;
	static SP_DialogueComponent GetInstance(){return SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));};
	void RegisterCharInHistory(IEntity Owner)
	{
		array <string>	texthistory = {};
		if (!texthistorymap.Get(Owner.GetID().ToString()))
		{
			texthistorymap.Insert(Owner.GetID().ToString(), texthistory);
		}
		array <string>	PLtexthistory = {};
		if (!PLtexthistorymap.Get(Owner.GetID().ToString()))
		{
			PLtexthistorymap.Insert(Owner.GetID().ToString(), PLtexthistory);
		}
		if (a_texthistory != texthistorymap.Get(Owner.GetID().ToString()))
		{
			a_texthistory = texthistorymap.Get(Owner.GetID().ToString());
		}
		if (a_PLtexthistory != PLtexthistorymap.Get(Owner.GetID().ToString()))
		{
			a_PLtexthistory = PLtexthistorymap.Get(Owner.GetID().ToString());
		}
	}
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
			a_texthistory.Insert(m_DialogTexttoshow);
			if (actiontext)
				a_PLtexthistory.Insert(actiontext);
			else
				a_PLtexthistory.Insert("null");
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
		a_texthistory.Insert(m_DialogTexttoshow);
		if (actiontext)
			a_PLtexthistory.Insert(actiontext);
		else
			a_PLtexthistory.Insert("null");
		
		
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
		//act.SetActionState(EAIActionState.FAILED);
		if (act)
			act.SetActiveConversation(false);
		//SCR_AIGroup group = SCR_AIGroup.Cast(agent.GetParentGroup());
		//if (!group)
		//	return;
		//SCR_DefendWaypoint wp;
		//wp = SCR_DefendWaypoint.Cast(group.GetCurrentWaypoint());
		//if (wp)
		//{
		//	group.RemoveWaypoint(wp);
		//	group.AddWaypoint(wp);
		//}
		ArchiveTextHistory(Character);
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
	static string GetEditableEntName(IEntity ent)
	{
		SCR_EditableEntityComponent editComp = SCR_EditableEntityComponent.Cast(ent.FindComponent(SCR_EditableEntityComponent));
		SCR_UIInfo uiinfo = editComp.GetInfo(ent);
		return uiinfo.GetName();
	}
	//----------------------------------------------------------------------------------------------------------------//
	//CHARACTER NAME
	static string GetCharacterName(IEntity Character)
	{
		if (!Character)
			return STRING_EMPTY;
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
	static string GetCharacterFirstName(IEntity Character)
	{
		if (!Character)
			return STRING_EMPTY;
		SCR_CharacterIdentityComponent IdentityComponent = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		
		string CharacterFullName;
		if(IdentityComponent)
		{
			Identity ID = IdentityComponent.GetIdentity();
			if(ID)
			{
				CharacterFullName = ID.GetName();
				CharacterFullName = CharacterFullName.Substring(17, CharacterFullName.Length() - 17)
			}
		}
		return CharacterFullName;
	}
	static string GetCharacterSurname(IEntity Character)
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
	//CHARACTER RANK
	static SCR_ECharacterRank GetCharacterRank(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
	static int GetCharacterRep(IEntity Character)
	{
		if (!Character)
			return -1;
		SCR_CharacterIdentityComponent Idcomp = SCR_CharacterIdentityComponent.Cast(Character.FindComponent(SCR_CharacterIdentityComponent));
		int MyRep = Idcomp.GetRep();
		return MyRep;
	}
	static string GetCharacterRankInsignia(IEntity Character)
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
	static string GetCharacterRankName(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		string CharacterRank;
		if(RankComponent)
		{
			CharacterRank = RankComponent.GetCharacterRankNameShort(Character);
		}
		return CharacterRank;
	}
	static string GetCharacterRankNameFull(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		string CharacterRank;
		if(RankComponent)
		{
			CharacterRank = RankComponent.GetCharacterRankName(Character);
		}
		return CharacterRank;
	}
	//CHARACTER FACTION
	static Faction GetCharacterFaction(IEntity Character)
	{
		if (!Character)
			return null;
		FactionAffiliationComponent FactionComponent = FactionAffiliationComponent.Cast(Character.FindComponent(FactionAffiliationComponent));
		if(FactionComponent)
		{
			return FactionComponent.GetAffiliatedFaction();
		}
		return null;	
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
		GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
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
	static string GetCharacterLocation(IEntity Character)
	{
		int m_iGridSizeX;
		int m_iGridSizeY;
		float angleA = 0.775;
		const float angleB = 0.325;
		vector mins,maxs;
		GetGame().GetWorldEntity().GetWorldBounds(mins, maxs);
			
		m_iGridSizeX = maxs[0]/3;
		m_iGridSizeY = maxs[2]/3;
	 
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
		vector posPlayer = Character.GetOrigin();
			
		SCR_EditableEntityComponent nearest = core.FindNearestEntity(posPlayer, EEditableEntityType.COMMENT);
		if (!nearest)
			return STRING_EMPTY;
		GenericEntity nearestLocation = nearest.GetOwner();
		SCR_MapDescriptorComponent mapDescr = SCR_MapDescriptorComponent.Cast(nearestLocation.FindComponent(SCR_MapDescriptorComponent));
		string closestLocationName;
		closestLocationName = nearest.GetDisplayName();

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