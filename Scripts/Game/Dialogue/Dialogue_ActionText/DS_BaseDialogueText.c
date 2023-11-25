[BaseContainerProps(configRoot:true)]
class DS_BaseDialogueText
{
	DialogueStage OwnerStage;
	int m_iIndex;
	string m_sActionText;
	string GetText(IEntity Character, IEntity Player){return m_sActionText;};
	sealed void Init(DialogueStage Owner,int Index)
	{
		if (Owner)
			OwnerStage = Owner;
		m_iIndex = Index;
	};
}
[BaseContainerProps(configRoot:true)]
class DS_AttributeText : DS_BaseDialogueText
{
	[Attribute()]
	string m_sActionTextOverride;
	
	override string GetText(IEntity Character, IEntity Player){return m_sActionTextOverride;};
}
