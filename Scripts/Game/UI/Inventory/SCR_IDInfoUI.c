//------------------------------------------------------------------------------------------------
//! UI Script
//! Inventory Slot UI Layout
[EntityEditorProps(category: "GameScripted/UI/Inventory", description: "Inventory Item Info UI class")]
class SCR_IDInfoUI : ScriptedWidgetComponent
{
	protected Widget 						m_wRoot; 
	TextWidget 								m_CharacterName;
	ImageWidget 							m_CharacterRank;
	TextWidget 								m_TextCharacterRank;
	ImageWidget 							m_CharacterFactionIcon;
	TextWidget								m_CharacterRep;
	
	string 											CharName;
	string 											CharRank;
	string 											rank;
	string 											rankText;
	FactionKey 										faction;
	SCR_CharacterIdentityComponent	m_CharIDComp;
	ResourceName RANK_ICON_IMAGESET = "{5D7F0C1AB551F610}UI/Textures/MilitaryIcons/MilitaryIcons.imageset";
	
	protected Widget								m_wInfoWidget;
	protected TextWidget							m_wTextName;
	protected SCR_SlotUIComponent					m_pFrameSlotUI;
	protected Widget 								m_wWidgetUnderCursor;
	protected bool 									m_bForceShow;
	
	
	void AssignFactionIcons(FactionKey factioncharacter)
	{
		
		if (m_CharacterFactionIcon)
		{
			switch(factioncharacter)
			{
				case "US":
					m_CharacterFactionIcon.LoadImageTexture(0, "{EB7F65DBC9392557}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "USSR":
					m_CharacterFactionIcon.LoadImageTexture(0, "{40B12B0DF911B856}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds");
				break;
				case "FIA":
					m_CharacterFactionIcon.LoadImageTexture(0, "{FB266CDD4502D60B}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_Fia.edds");
				break;
				case "BANDITS":
					m_CharacterFactionIcon.LoadImageTexture(0, "{855625FE8D6A87A8}UI/Textures/DamageState/damageState-dead.edds");
				break;
				case "SPEIRA":
					m_CharacterFactionIcon.LoadImageTexture(0, "{CEAC8771342689C5}Assets/Data/Faction_SPEIRA.edds");
				break;	
			}
			m_CharacterFactionIcon.SetVisible(true);
		}
	}
	void AssignRankIcons(string rankinsigniacharacter)
	{
		if (m_CharacterRank)
		{
			m_CharacterRank.LoadImageFromSet(0, RANK_ICON_IMAGESET, rankinsigniacharacter);
			m_CharacterRank.SetVisible(true);
		}
	}
	void SetCharacterName(IEntity Char)
	{
		m_CharIDComp = SCR_CharacterIdentityComponent.Cast(Char.FindComponent(SCR_CharacterIdentityComponent));
		if(Char)
		{
			CharName = DS_DialogueComponent.GetCharacterName(Char);
			rank = DS_DialogueComponent.GetCharacterRankInsignia(Char);
			faction = DS_DialogueComponent.GetCharacterFaction(Char).GetFactionKey();
			rankText = DS_DialogueComponent.GetCharacterRankNameFull(Char);
		}
		
		m_TextCharacterRank.SetText(rankText);
		m_TextCharacterRank.SetVisible(true);
		int CharRep = m_CharIDComp.GetRep();
		
		if(CharRep > 80)
		{
			m_CharacterRep.SetColor(Color.DarkGreen);
			m_CharacterRep.SetText("Briliant");
		}
		else if(CharRep > 60)
		{
			m_CharacterRep.SetColor(Color.DarkGreen);
			m_CharacterRep.SetText("Good");
		}
		else if(CharRep > 40)
		{
			m_CharacterRep.SetColor(Color.DarkYellow);
			m_CharacterRep.SetText("Neuteral");
		}
		else if(CharRep > 20)
		{
			m_CharacterRep.SetColor(Color.DarkRed);
			m_CharacterRep.SetText("Bad");
		}
		else if(CharRep > 1)
		{
			m_CharacterRep.SetColor(Color.DarkRed);
			m_CharacterRep.SetText("Horrible");
		}
		m_CharacterRep.SetVisible(true);		
		m_CharacterName.SetText(CharName);
		m_CharacterName.SetVisible(true);
		
	}
	//------------------------------------------------------------------------------------------------
	void Show(float fDelay = 0.0, Widget w = null, bool forceShow = false)
	{
		m_bForceShow = forceShow;
		m_wWidgetUnderCursor = w;
		if (fDelay == 0)
		{
			ShowInfoWidget(true);
		}
		else
		{
			// CallLater delays showing up of damageInfoUI
			GetGame().GetCallqueue().Remove(ShowInfoWidget);
			GetGame().GetCallqueue().CallLater(ShowInfoWidget, fDelay * 1000, false, true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ShowInfoWidget(bool bShow)
	{
			if (m_bForceShow)
		{
			m_wInfoWidget.SetVisible(true);
			return;
		}

		if (!m_wWidgetUnderCursor)
			return;
		
		if (WidgetManager.GetWidgetUnderCursor() != m_wWidgetUnderCursor)
			return; //the cursor is on different position already
		
		m_wInfoWidget.SetVisible(true);
	}

	//------------------------------------------------------------------------------------------------
	/*void SetDamageStateVisible(bool visible, bool regenerating, string iconIntensity, string damageText)
	{
		if (!m_sMedicalIconsImageSet || !m_wDamageIconWidget || !m_wDamageTextWidget || !m_wDamageRegenTextWidget)
			return;
		
		if (!iconIntensity.IsEmpty())
			m_wDamageIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, iconIntensity);
	
		string regenText = m_sDamageRegenText;
		m_wDamageIconWidget.SetVisible(visible);

		if (!regenerating)
			regenText = "";

		m_wDamageTextWidget.SetText(damageText);
		m_wDamageRegenTextWidget.SetText(regenText);		
		m_wDamageTextWidget.SetVisible(visible);
		m_wDamageRegenTextWidget.SetVisible(visible);
	}

	//------------------------------------------------------------------------------------------------
	void SetBleedingStateVisible(bool visible, string bleedingText)
	{
		if (!m_sMedicalIconsImageSet || !m_wBleedingIconWidget || !m_wBleedingTextWidget)
			return;
		
		m_wBleedingIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sBloodIcon);
		m_wBleedingIconWidget.SetVisible(visible);
	
		m_wBleedingTextWidget.SetVisible(visible);
		m_wBleedingTextWidget.SetText(bleedingText);
	}

	//------------------------------------------------------------------------------------------------
	void SetTourniquetStateVisible(bool visible)
	{
		if (!m_sMedicalIconsImageSet || !m_wTourniquetIconWidget || !m_wTourniquetTextWidget)
			return;

		m_wTourniquetIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sTourniquetIcon);
		m_wTourniquetIconWidget.SetVisible(visible);
		
		m_wTourniquetTextWidget.SetText(m_sTourniquetText);
		m_wTourniquetTextWidget.SetVisible(visible);
		SetTourniquetHintVisible(visible);
	}

	//------------------------------------------------------------------------------------------------
	void SetSalineBagStateVisible(bool visible)
	{
		if (!m_sMedicalIconsImageSet || !m_wSalineBagIconWidget || !m_wSalineBagTextWidget)
			return;
		
		m_wSalineBagIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sSalineBagIcon);
		m_wSalineBagIconWidget.SetVisible(visible);
		
		m_wSalineBagTextWidget.SetText(m_sSalinebagText);
		m_wSalineBagTextWidget.SetVisible(visible);
	}

	//------------------------------------------------------------------------------------------------
	void SetTourniquetHintVisible(bool visible)
	{
		if (!m_wTourniquetHintIconWidget || !m_wTourniquetHintTextWidget)
			return;
		
		m_wTourniquetHintIconWidget.SetVisible(visible);

		m_wTourniquetHintTextWidget.SetText(m_sTourniquetHintText);
		m_wTourniquetHintTextWidget.SetVisible(visible);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMorphineStateVisible(bool visible)
	{
		if (!m_wMorphineIconWidget || !m_wMorphineTextWidget)
			return;
		
		m_wMorphineIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sMorphineIcon);
		m_wMorphineIconWidget.SetVisible(visible);
		
		m_wMorphineTextWidget.SetText(m_sMorphineText);
		m_wMorphineTextWidget.SetVisible(visible);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetFractureStateVisible(bool armFractured, bool legFractured)
	{
		if (!m_wFractureIconWidget || !m_wFractureIcon2Widget || !m_wFractureTextWidget || !m_wFractureText2Widget)
			return;
		
		m_wFractureIconWidget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sFractureIcon);
		m_wFractureIcon2Widget.LoadImageFromSet(0, m_sMedicalIconsImageSet, m_sFractureIcon);
		
		m_wFractureIconWidget.SetVisible(armFractured);
		m_wFractureIcon2Widget.SetVisible(legFractured);

		m_wFractureTextWidget.SetText(m_sArmFractureText);
		m_wFractureText2Widget.SetText(m_sLegFractureText);
				
		m_wFractureTextWidget.SetVisible(armFractured);
		m_wFractureText2Widget.SetVisible(legFractured);
	}*/

	//------------------------------------------------------------------------------------------------
	void Hide()
	{
		if (!m_wInfoWidget)
			return;
		
		m_wInfoWidget.SetVisible(false);
		m_wInfoWidget.SetEnabled(false);
	}

	//------------------------------------------------------------------------------------------------
	void Move(float x, float y)
	{
		if (!m_pFrameSlotUI)
			return;
		
		m_pFrameSlotUI.SetPosX(x);
		m_pFrameSlotUI.SetPosY(y);
	}

	//------------------------------------------------------------------------------------------------
	void SetName(string sName)
	{
		if (!m_wTextName)
			return;
		
		if (sName.IsEmpty())
			m_wTextName.GetParent().SetVisible(false);
		else
			m_wTextName.SetText(sName);
	}

	//------------------------------------------------------------------------------------------------
	override void HandlerAttached(Widget w)
	{
		if (!w)
			return;
		m_wInfoWidget	= w;
		m_wTextName	= TextWidget.Cast( w.FindAnyWidget( "ItemInfo_name" ) );
		Widget wItemInfo	= m_wInfoWidget.FindAnyWidget( "ItemInfo" );
		if (!wItemInfo)
			return;
		m_pFrameSlotUI = SCR_SlotUIComponent.Cast(wItemInfo.FindHandler(SCR_SlotUIComponent));
		
		m_CharacterRep = TextWidget.Cast(wItemInfo.FindAnyWidget("CharReputationText"));
		m_CharacterName = TextWidget.Cast(wItemInfo.FindAnyWidget("CharacterName"));
		m_CharacterRank = ImageWidget.Cast(wItemInfo.FindAnyWidget("RankIcon"));
		m_CharacterFactionIcon = ImageWidget.Cast(wItemInfo.FindAnyWidget("FactionIcon"));
		m_TextCharacterRank = TextWidget.Cast(wItemInfo.FindAnyWidget("CharRankText"));
		
		
		
		/*m_wDamageIconWidget 			= ImageWidget.Cast( w.FindAnyWidget( "DamageInfo_icon" ) );
		m_wDamageTextWidget 			= TextWidget.Cast( w.FindAnyWidget( "DamageInfo_text" ) );
		m_wFractureIconWidget 			= ImageWidget.Cast( w.FindAnyWidget( "FractureInfo_icon" ) );
		m_wFractureIcon2Widget 			= ImageWidget.Cast( w.FindAnyWidget( "FractureInfo_icon2" ) );
		m_wFractureTextWidget			= TextWidget.Cast( w.FindAnyWidget( "FractureInfo_text" ) );
		m_wFractureText2Widget			= TextWidget.Cast( w.FindAnyWidget( "FractureInfo_text2" ) );
		m_wDamageRegenTextWidget 		= TextWidget.Cast( w.FindAnyWidget( "DamageRegenInfo_text" ) );
		m_wBleedingIconWidget 			= ImageWidget.Cast( w.FindAnyWidget( "BleedingInfo_icon" ) );
		m_wBleedingTextWidget 			= TextWidget.Cast( w.FindAnyWidget( "BleedingInfo_text" ) );
		m_wTourniquetIconWidget 		= ImageWidget.Cast( w.FindAnyWidget( "TourniquetInfo_icon" ) );
		m_wTourniquetTextWidget 		= TextWidget.Cast( w.FindAnyWidget( "TourniquetInfo_text" ) );
		m_wTourniquetHintIconWidget 	= RichTextWidget.Cast( w.FindAnyWidget( "TourniquetHint_icon" ) );
		m_wTourniquetHintTextWidget 	= TextWidget.Cast( w.FindAnyWidget( "TourniquetHint_text" ) );
		m_wSalineBagIconWidget 			= ImageWidget.Cast( w.FindAnyWidget( "SalineBagInfo_icon" ) );
		m_wSalineBagTextWidget 			= TextWidget.Cast( w.FindAnyWidget( "SalineBagInfo_text" ) );
		m_wMorphineIconWidget			= ImageWidget.Cast( w.FindAnyWidget( "MorphineInfo_icon" ) );
		m_wMorphineTextWidget 			= TextWidget.Cast( w.FindAnyWidget( "MorphineInfo_text" ) );*/
	}

	//------------------------------------------------------------------------------------------------
	void Destroy()
	{
		if (m_wInfoWidget)
		{
			GetGame().GetCallqueue().Remove(ShowInfoWidget);
			m_wInfoWidget.RemoveHandler(m_pFrameSlotUI);
			m_wInfoWidget.RemoveHandler(this);
			m_wInfoWidget.RemoveFromHierarchy();
		}
	}

	//------------------------------------------------------------------------------------------------
	Widget GetInfoWidget()
	{
		return m_wInfoWidget.FindAnyWidget("size");
	}
};
