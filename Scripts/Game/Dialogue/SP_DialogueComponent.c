class SP_DialogueComponentClass: ScriptComponentClass
{
};
class SP_DialogueComponent: ScriptComponent
{
	[Attribute( defvalue: "", desc: "Description of the task", category: "Task",  )]			//TODO: make config, memory
	protected ref array<ref SP_CharacterArchetype> m_CharacterArchetypeList;
	
	protected string m_DialogTexttoshow;
	protected BaseChatChannel m_ChatChannel;
	protected int senderId;
	protected string senderName;
	int DiagCount;
	protected int GlobalDiagStage;
	
	
	void SendText(IEntity pOwnerEntity, IEntity pUserEntity, BaseChatChannel ChatChannel, int CharID, int BranchID, bool MultipleChoise)
	{
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
			{
					m_DialogTexttoshow = m_CharacterArchetypeList[i].GetDialogueText(BranchID, MultipleChoise);
			}
			
		}
		
		SCR_CharacterIdentityComponent IDcomp = SCR_CharacterIdentityComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterIdentityComponent));
		
		senderName = IDcomp.GetCharacterFullName();
		SCR_ChatPanelManager.GetInstance().ShowDiagMessage(m_DialogTexttoshow, ChatChannel, senderId, senderName);
	}
	string GetActionName(int CharID, int BranchID, bool MultipleChoise)
	{
		string m_sActionName;
		
		for (int i, count = m_CharacterArchetypeList.Count(); i < count; i++)
		{
			if (m_CharacterArchetypeList[i].GetCharacterID() == CharID)
			{
				m_sActionName = m_CharacterArchetypeList[i].GetActionTitle(BranchID, MultipleChoise);
			}
		}
		return m_sActionName;
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
		foreach (SP_CharacterArchetype config: m_CharacterArchetypeList)
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