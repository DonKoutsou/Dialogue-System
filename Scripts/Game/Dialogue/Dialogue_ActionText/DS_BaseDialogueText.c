[BaseContainerProps(configRoot:true)]
class DS_BaseDialogueText
{
	DS_DialogueStage OwnerStage;
	int m_iIndex;
	string m_sActionText;
	string GetText(IEntity Character, IEntity Player){return m_sActionText;};
	sealed void Init(DS_DialogueStage Owner,int Index)
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
class DialogueActionTextTitleAttribute : BaseContainerCustomTitle
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		string text;
		source.Get("m_sActionText", text);
		title = text;
		return true;
	}
};
