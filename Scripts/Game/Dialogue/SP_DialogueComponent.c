class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	[Attribute()]
	protected ref array<ref SP_DialogueArchetype> m_CharacterArchetypeList;
	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	
	int DiagCount;
	protected string m_DialogTexttoshow;
	protected int senderId;
	protected string senderName;
	protected ECharacterRank senderRank;
	protected FactionKey senderFaction;
	protected int GlobalDiagStage;
	protected SCR_CharacterIdentityComponent CharIDComp;
	protected SCR_CharacterRankComponent CharRankComp;
	protected FactionAffiliationComponent FactComp;
	protected EDiagIdentifier diagid;
	protected SP_DialogueArchetype DiagArch;
	
	void SendText(IEntity pOwnerEntity, int BranchID, bool MultipleChoise, int CharID)
	{
		CharIDComp = SCR_CharacterIdentityComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterIdentityComponent));
		senderName = CharIDComp.GetCharacterFullName();
		DiagArch = MatchDiagArchetype(pOwnerEntity, BranchID, MultipleChoise, CharID, 1);
		m_DialogTexttoshow = DiagArch.GetDialogueText(BranchID, MultipleChoise);
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(m_DialogTexttoshow, m_ChatChannel, senderId, senderName);
	}
	string GetActionName(int CharID, int BranchID, bool MultipleChoise, IEntity Owner)
	{
		string m_sActionName;
		DiagArch = MatchDiagArchetype(Owner, BranchID, MultipleChoise, CharID, 1);
		m_sActionName = DiagArch.GetActionTitle(BranchID, MultipleChoise);
		return m_sActionName;
	
	}
	bool IncrementDiagStage(IEntity owner, int CharID, int BranchID, int incrementamount, bool MultipleChoise)
	{
		DiagArch = MatchDiagArchetype(owner, BranchID, MultipleChoise, CharID, 1);
		DiagArch.IncrementStage(BranchID, incrementamount, MultipleChoise);
		return false;
	}
	override void EOnInit(IEntity owner)
	{
		
		foreach (SP_DialogueArchetype config: m_CharacterArchetypeList)
		{
			config.Init();
		}
	}
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
	SP_DialogueArchetype MatchDiagArchetype(IEntity pOwnerEntity, int BranchID, bool MultipleChoise, int CharID, int incrementamount)
	{
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			
			diagid = m_CharacterArchetypeList[i].GetIdentifier();
			switch (diagid) 
			{
			case 0:
				if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
				{
					DiagArch = m_CharacterArchetypeList[i];
				}
				break;
		    case 1:
				CharIDComp = SCR_CharacterIdentityComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterIdentityComponent));
				senderName = CharIDComp.GetCharacterFullName();
				if (m_CharacterArchetypeList[i].GetCharacterName() == senderName)
				{
					DiagArch = m_CharacterArchetypeList[i];
				}
				break;
		    case 2:
				CharRankComp = SCR_CharacterRankComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterRankComponent));
				senderRank = CharRankComp.GetCharacterRank(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetCharacterRank() == senderRank)
				{
					DiagArch = m_CharacterArchetypeList[i];
				}
				break;
			case 3:
				FactComp = FactionAffiliationComponent.Cast(pOwnerEntity.FindComponent(FactionAffiliationComponent));
				senderFaction = FactComp.GetAffiliatedFaction().GetFactionKey();
				if (m_CharacterArchetypeList[i].GetCharacterFaction() == senderFaction)
				{
					DiagArch = m_CharacterArchetypeList[i];
				}
				break;
			case 4:
				FactComp = FactionAffiliationComponent.Cast(pOwnerEntity.FindComponent(FactionAffiliationComponent));
				CharRankComp = SCR_CharacterRankComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterRankComponent));
				if (FactComp && CharRankComp)
				{
					senderFaction = FactComp.GetAffiliatedFaction().GetFactionKey();
					senderRank = CharRankComp.GetCharacterRank(pOwnerEntity);
				}
				if (m_CharacterArchetypeList[i].GetCharacterFaction() == senderFaction && m_CharacterArchetypeList[i].GetCharacterRank() == senderRank)
				{
					DiagArch = m_CharacterArchetypeList[i];
				}
				break;
			}
		}
		return DiagArch;
	}
};
enum EDiagIdentifier
	{
		OriginalCharID,
		Name,
		Rank,
		"FactionKey",
		"Faction and Rank"
	};