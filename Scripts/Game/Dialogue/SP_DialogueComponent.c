class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	[Attribute( defvalue: "", desc: "Description of the task", category: "Task",  )]			//TODO: make config, memory
	protected ref array<ref SP_DialogueArchetype> m_CharacterArchetypeList;
	[Attribute()]
	ref BaseChatChannel m_ChatChannel;
	
	protected string m_DialogTexttoshow;
	protected int senderId;
	protected string senderName;
	protected ECharacterRank senderRank;
	protected FactionKey senderFaction;
	int DiagCount;
	protected int GlobalDiagStage;
	protected SCR_CharacterIdentityComponent CharIDComp;
	protected SCR_CharacterRankComponent CharRankComp;
	protected FactionAffiliationComponent FactComp;
	protected EDiagIdentifier diagid;
	
	void SendText(IEntity pOwnerEntity, IEntity pUserEntity, int BranchID, bool MultipleChoise, int CharID)
	{
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			
			diagid = m_CharacterArchetypeList[i].GetIdentifier();
			switch (diagid) 
			{
			case 0:
				if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
				{
					m_DialogTexttoshow = m_CharacterArchetypeList[i].GetDialogueText(BranchID, MultipleChoise);
				}
				break;
		    case 1:
				CharIDComp = SCR_CharacterIdentityComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterIdentityComponent));
				senderName = CharIDComp.GetCharacterFullName();
				if (m_CharacterArchetypeList[i].GetCharacterName() == senderName)
				{
					m_DialogTexttoshow = m_CharacterArchetypeList[i].GetDialogueText(BranchID, MultipleChoise);
				}
				break;
		    case 2:
				CharRankComp = SCR_CharacterRankComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterRankComponent));
				senderRank = CharRankComp.GetCharacterRank(pOwnerEntity);
				if (m_CharacterArchetypeList[i].GetCharacterRank() == senderRank)
				{
					m_DialogTexttoshow = m_CharacterArchetypeList[i].GetDialogueText(BranchID, MultipleChoise);
				}
				break;	
			case 3:
				FactComp = FactionAffiliationComponent.Cast(pOwnerEntity.FindComponent(FactionAffiliationComponent));
				senderFaction = FactComp.GetAffiliatedFaction().GetFactionKey();
				if (m_CharacterArchetypeList[i].GetCharacterFaction() == senderFaction)
				{
					m_DialogTexttoshow = m_CharacterArchetypeList[i].GetDialogueText(BranchID, MultipleChoise);
				}
				break;
			}
		}
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(m_DialogTexttoshow, m_ChatChannel, senderId, senderName);
	}
	string GetActionName(int CharID, int BranchID, bool MultipleChoise, IEntity Owner)
	{
		string m_sActionName;
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			diagid = m_CharacterArchetypeList[i].GetIdentifier();
			switch (diagid) 
			{
				case 0:
					if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
					{
						m_sActionName = m_CharacterArchetypeList[i].GetActionTitle(BranchID, MultipleChoise);
					}
					break;
			    case 1:
					CharIDComp = SCR_CharacterIdentityComponent.Cast(Owner.FindComponent(SCR_CharacterIdentityComponent));
					senderName = CharIDComp.GetCharacterFullName();
					if (m_CharacterArchetypeList[i].GetCharacterName() == senderName)
					{
						m_sActionName = m_CharacterArchetypeList[i].GetActionTitle(BranchID, MultipleChoise);
					}
					break;
			    case 2:
					CharRankComp = SCR_CharacterRankComponent.Cast(Owner.FindComponent(SCR_CharacterRankComponent));
					senderRank = CharRankComp.GetCharacterRank(Owner);
					if (m_CharacterArchetypeList[i].GetCharacterRank() == senderRank)
					{
						m_sActionName = m_CharacterArchetypeList[i].GetActionTitle(BranchID, MultipleChoise);
					}
					break;	
				case 3:
					FactComp = FactionAffiliationComponent.Cast(Owner.FindComponent(FactionAffiliationComponent));
					senderFaction = FactComp.GetAffiliatedFaction().GetFactionKey();
					if (m_CharacterArchetypeList[i].GetCharacterFaction() == senderFaction)
					{
						m_sActionName = m_CharacterArchetypeList[i].GetActionTitle(BranchID, MultipleChoise);
					}
					break;
			}
			return m_sActionName;
		}
		return STRING_EMPTY;
	
	}
	int GetDiagStage(int CharID, int BrachiD)
	{
		int m_iDiagStage;
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
			{
				m_iDiagStage = m_CharacterArchetypeList[i].GetDiagStage(BrachiD);
			}
		}
		return m_iDiagStage;
	}
	bool IncrementDiagStage(int CharID, int BranchID, int incrementamount, bool MultipleChoise)
	{
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
			{
				m_CharacterArchetypeList[i].IncrementStage(BranchID, incrementamount, MultipleChoise);
			}
			
		}
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
};
enum EDiagIdentifier
	{
		OriginalCharID,
		Name,
		Rank,
		FactionK
	};