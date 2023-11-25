[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_BaseDialogueStageAction
{
	DS_DialogueStage OwnerStage;
	int m_iIndex;
	void Perform(IEntity Character, IEntity Player){};
	sealed void Init(DS_DialogueStage Owner, int Index)
	{
		OwnerStage = Owner;
		m_iIndex = Index;
	};
}