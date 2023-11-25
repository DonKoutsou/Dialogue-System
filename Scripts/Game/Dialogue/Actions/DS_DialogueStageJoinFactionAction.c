[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageJoinFactionAction : DS_BaseDialogueStageAction
{
	[Attribute()]
	FactionKey m_FactionKey;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		FactionAffiliationComponent FactComp = FactionAffiliationComponent.Cast(Player.FindComponent(FactionAffiliationComponent));
		FactComp.SetAffiliatedFactionByKey(m_FactionKey);
		SCR_CharacterRankComponent Rankcomp = SCR_CharacterRankComponent.Cast(Player.FindComponent(SCR_CharacterRankComponent));
		Rankcomp.SetCharacterRank(SCR_ECharacterRank.PRIVATE);
		super.Perform(Character, Player);
	};
	

};
