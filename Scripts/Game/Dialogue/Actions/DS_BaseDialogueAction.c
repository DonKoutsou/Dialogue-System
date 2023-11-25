[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_BaseDialogueStageAction
{
	DialogueStage OwnerStage;
	int m_iIndex;
	void Perform(IEntity Character, IEntity Player){};
	sealed void Init(DialogueStage Owner, int Index)
	{
		OwnerStage = Owner;
		m_iIndex = Index;
	};
}