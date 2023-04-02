class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	//----------------------------------------------------------------------------------------------------------------//
	[Attribute()]
	protected ref array<ref SP_DialogueArchetype> m_CharacterArchetypeList;
	
	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	//----------------------------------------------------------------------------------------------------------------//
	// Character Info
	protected SCR_CharacterIdentityComponent CharIDComp;
	protected SCR_CharacterRankComponent CharRankComp;
	protected FactionAffiliationComponent FactComp;
	protected ECharacterRank senderRank;
	protected FactionKey senderFaction;
	protected string senderName;
	protected int senderId;
	IEntity PlayerCharacter;
	IEntity TalkingCharacter;
	//----------------------------------------------------------------------------------------------------------------//
	//Dialogue System
	protected EDiagIdentifier diagid;
	protected ref map<int, ref SP_DialogueArchetype> DialogueArchetypeMap;
    protected SP_DialogueArchetype DiagArch;
	protected string m_DialogTexttoshow;
	//Stage state???
	protected int GlobalDiagStage;
	SCR_RadialMenuComponent RadComp;
	SP_RadialMenuDiags RadMenuDiags;
	SCR_BaseGameMode GameMode;
	SP_DialogueConfig DialogueConfig;
	SP_RadialChoiceConfig RadialConfig;
	
	//----------------------------------------------------------------------------------------------------------------//
	void DoDialogue(IEntity Character, IEntity Player, int BranchID, int IncrementAmount)
	{
		senderName = GetCharacterName(Character);
		DiagArch = GetArchetype(Character);
		m_DialogTexttoshow = DiagArch.GetDialogueText(BranchID);
		if(DiagArch.GetDialogueConfigLite(BranchID) && DiagArch.CheckIfDialogueBranches(DiagArch.GetDialogueConfigLite(BranchID)) == true)
		{
			if (RadMenuDiags.IsOpen() == false)
			{
				RadMenuDiags.Start(Character, Player);
			}
			return;
		}
		else
			if (RadMenuDiags.IsOpen() == true)
			{
				RadMenuDiags.Close(Character)
			}
			SendText(m_DialogTexttoshow, m_ChatChannel, senderId, senderName);
			IncrementDiagStage(Character, BranchID, IncrementAmount);
			
	}
	//----------------------------------------------------------------------------------------------------------------//
	void DoRadialDialogue(IEntity Character, IEntity Player, int BranchID, int IncrementAmount, int EntryID)
	{
		//Get name of character that will send message to chat
		senderName = GetCharacterName(Character);
		
		//Find our Diialogue Archetype by matching it to characters info
		DiagArch = GetArchetype(Character);
		
		//Get Dialogue config with same branch id. Gives you config that has same stage id as current stage
		DialogueConfig = DiagArch.GetDialogueConfigLite(BranchID);
		
		if (DialogueConfig)
		{
			//find the configuration that contains the text we want
			RadialConfig = DialogueConfig.GetRadialChoiceConfig();
			if(RadialConfig)
			{	
				m_DialogTexttoshow = RadialConfig.GetDialogueText(EntryID);
				EChoiseBehavior ChoiseBehavior = RadialConfig.GetChoiseBehavior();
				switch (ChoiseBehavior) 
					{
				    case 0:
							//Accept & Close
							RadMenuDiags.Close(Player);
						break;
					case 1:
							//Alternate Dialogue WIP
						break;
					case 2:
							//Stay
						break;
					}
			}
		}
		
		SendText(m_DialogTexttoshow, m_ChatChannel, senderId, senderName);
	}
	// Used in action to send the text to chat
	void SendText(string Text, BaseChatChannel Chanell, int SenderID, string SenderName)
	{
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(m_DialogTexttoshow, m_ChatChannel, senderId, senderName);
	}
	//----------------------------------------------------------------------------------------------------------------//
	// returns string from dialogue configuration under field Action Text, ment to be used for text shown on the action
	string GetActionName(int BranchID, IEntity Owner)
	{
		string m_sActionName;
		DiagArch = GetArchetype(Owner);
		m_sActionName = DiagArch.GetActionTitle(BranchID);
		return m_sActionName;
	}
	//Get title for radial meny action
	string GetRadialActionName(int BranchID, IEntity Owner, int RadialID)
	{
		string m_sMActionName;
		SP_DialogueConfig DialogueConfiguration;
		SP_RadialChoiceConfig RadialConfiguration;
		DiagArch = GetArchetype(Owner);
		DialogueConfiguration = DiagArch.GetDialogueConfigLite(BranchID);
		if (DialogueConfiguration)
		{
			RadialConfiguration = DialogueConfiguration.GetRadialChoiceConfig();
			if(!RadialConfiguration)
			{
				return STRING_EMPTY;
			}
			m_sMActionName = RadialConfiguration.GetActionText(RadialID);
			if(!m_sMActionName)
			{
				return STRING_EMPTY;
			}
		}
		
		return m_sMActionName;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//progress dialogue stage int, need to figure out where/how to store the stage
	bool IncrementDiagStage(IEntity owner, int BranchID, int incrementamount)
	{
		DiagArch = GetArchetype(owner);
		DiagArch.IncrementStage(BranchID, incrementamount);
		return false;
	}
	//----------------------------------------------------------------------------------------------------------------//
	//initialise configuration to crate map of configuration's contents
	override void EOnInit(IEntity owner)
	{
		GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		RadComp = SCR_RadialMenuComponent.Cast(GameMode.FindComponent(SCR_RadialMenuComponent));
		RadMenuDiags = SP_RadialMenuDiags.Cast(RadComp.GetRadialMenuHandler());
		foreach (SP_DialogueArchetype config: m_CharacterArchetypeList)
		{
			config.Init();
		}
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
	//----------------------------------------------------------------------------------------------------------------//
	//Getters for character info for identifiers, character name also used on sent text
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
	ECharacterRank GetCharacterRank(IEntity Character)
	{
		SCR_CharacterRankComponent RankComponent = SCR_CharacterRankComponent.Cast(Character.FindComponent(SCR_CharacterRankComponent));
		if(RankComponent)
		{
			return RankComponent.GetCharacterRank(Character);
		}
		return null;
	}
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
	SP_DialogueArchetype GetArchetype(IEntity pOwnerEntity)
	{
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			diagid = m_CharacterArchetypeList[i].GetIdentifier();
			switch (diagid) 
			{
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 0 means we look for Character Name
		    case 0:
				senderName = GetCharacterName(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeName() == senderName)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 1 means we look for Character Rank
		    case 1:
				senderRank = GetCharacterRank(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeRank() == senderRank)
				{
					DiagArch = m_CharacterArchetypeList[i];
					return DiagArch;
				}
				break;
			//-----------------------------------------------------------------------------------------------------------//
			// diagid 2 means we look for Character Faction
			case 2:
				senderFaction = GetCharacterFaction(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetArchetypeFaction() == senderFaction)
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
				if (m_CharacterArchetypeList[i].GetArchetypeFaction() == senderFaction && m_CharacterArchetypeList[i].GetArchetypeRank() == senderRank)
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
}
//----------------------------------------------------------------------------------------------------------------//
enum EDiagIdentifier
	{
		Name,
		Rank,
		"FactionKey",
		"Faction and Rank"
	};