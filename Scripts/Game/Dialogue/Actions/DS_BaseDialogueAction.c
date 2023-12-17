[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_BaseDialogueStageAction
{
	[Attribute(defvalue: "0")]
	bool m_bUseCustomIndex;
	[Attribute(defvalue: "0")]
	int m_iCustomIndex;
	DS_DialogueStage OwnerStage;
	int m_iIndex;
	void Perform(IEntity Character, IEntity Player){};
	sealed void Init(DS_DialogueStage Owner, int Index)
	{
		OwnerStage = Owner;
		if (m_bUseCustomIndex)
			m_iIndex = m_iCustomIndex;
		else
			m_iIndex = Index;
		
		PostInit();
	};
	void PostInit(){};
}