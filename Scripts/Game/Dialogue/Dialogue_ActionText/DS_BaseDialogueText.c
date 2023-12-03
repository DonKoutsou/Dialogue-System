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
[BaseContainerProps(configRoot:true)]
class DS_NamedAttributeText : DS_BaseDialogueText
{
	[Attribute(desc: "Inclue the %1 sign on the place where character name will be placed")]
	string m_sActionTextOverride;
	
	[Attribute(defvalue:"0")]
	bool m_bAddRankName;
	
	[Attribute(defvalue:"1")]
	bool m_bAddFirstName;
	
	[Attribute(defvalue:"1")]
	bool m_bAddLastName;
	
	[Attribute(defvalue:"1")]
	bool m_bUsePlayer;
	
	override string GetText(IEntity Character, IEntity Player)
	{
		IEntity char = Character;
		if (m_bUsePlayer)
			char = Player;
		
		string name;
		
		if (m_bAddRankName)
		{
			string rankname = DS_DialogueComponent.GetCharacterRankName(char);
			name = rankname + " ";
		}
		if (m_bAddFirstName)
		{
			string firstkname = DS_DialogueComponent.GetCharacterName(char);
			name = name + firstkname + " ";
		}
		if (m_bAddLastName)
		{
			string lastkname = DS_DialogueComponent.GetCharacterSurname(char);
			name = name + lastkname;
		}
		return string.Format(m_sActionTextOverride, name);;
	};
}
[BaseContainerProps(configRoot:true)]
class DS_3RDNamedAttributeText : DS_BaseDialogueText
{
	[Attribute("0",UIWidgets.ComboBox, enums : ParamEnumArray.FromEnum(SP_BaseEn))]
	SP_BaseEn m_TaskBaseNameFrom;
	
	[Attribute("20",UIWidgets.ComboBox, enums : ParamEnumArray.FromEnum(SCR_ECharacterRank) ,params:  "20 23 1")]
	SCR_ECharacterRank m_CharPost;
	
	[Attribute(desc: "Inclue the %1 sign on the place where character name will be placed")]
	string m_sActionTextOverride;
	
	[Attribute(defvalue:"0")]
	bool m_bAddRankName;
	
	[Attribute(defvalue:"1")]
	bool m_bAddFirstName;
	
	[Attribute(defvalue:"1")]
	bool m_bAddLastName;
	
	
	override string GetText(IEntity Character, IEntity Player)
	{
		SCR_CampaignMilitaryBaseManager baseman = SCR_GameModeCampaign.Cast(GetGame().GetGameMode()).GetBaseManager();
		SCR_CampaignMilitaryBaseComponent base = baseman.GetNamedBase(SP_BaseNames.Get(m_TaskBaseNameFrom));
		IEntity char;
		base.GetCharacterOfPost(m_CharPost, char);
		
		string name;
		
		if (m_bAddRankName)
		{
			string rankname = DS_DialogueComponent.GetCharacterRankName(char);
			name = rankname + " ";
		}
		if (m_bAddFirstName)
		{
			string firstkname = DS_DialogueComponent.GetCharacterName(char);
			name = name + firstkname + " ";
		}
		if (m_bAddLastName)
		{
			string lastkname = DS_DialogueComponent.GetCharacterSurname(char);
			name = name + lastkname;
		}
		return string.Format(m_sActionTextOverride, name);;
	};
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
