[EntityEditorProps(category: "GameScripted/UI/Inventory", description: "Inventory Item Info UI class")]
class SCR_IDInfoUI : ScriptedWidgetComponent
{
	protected Widget 					m_wRoot; 
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
	protected static const ResourceName RANK_ICON_IMAGESET = "{5D7F0C1AB551F610}UI/Textures/MilitaryIcons/MilitaryIcons.imageset";
	
	protected Widget								m_wInfoWidget;
	protected TextWidget							m_wTextName;
	protected SCR_SlotUIComponent					m_pFrameSlotUI;
	protected Widget 								m_wWidgetUnderCursor;
	protected bool 									m_bForceShow;
	
	
	void AssignFactionIcons( FactionKey factioncharacter )
	{
		if ( m_CharacterFactionIcon )
		{
			switch( factioncharacter )
			{
				case "US":
					m_CharacterFactionIcon.LoadImageTexture( 0, "{EB7F65DBC9392557}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds" );
				break;
				case "USSR":
					m_CharacterFactionIcon.LoadImageTexture( 0, "{40B12B0DF911B856}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_USA.edds" );
				break;
				case "FIA":
					m_CharacterFactionIcon.LoadImageTexture( 0, "{FB266CDD4502D60B}UI/Textures/Editor/EditableEntities/Factions/EditableEntity_Faction_Fia.edds" );
				break;
				case "BANDITS":
					m_CharacterFactionIcon.LoadImageTexture( 0, "{855625FE8D6A87A8}UI/Textures/DamageState/damageState-dead.edds" );
				break;
				case "SPEIRA":
					m_CharacterFactionIcon.LoadImageTexture( 0, "{CEAC8771342689C5}Assets/Data/Faction_SPEIRA.edds" );
				break;	
			}
			m_CharacterFactionIcon.SetVisible( true );
		}
	}
	
	void AssignRankIcons( string rankinsigniacharacter )
	{
		if (m_CharacterRank && rankinsigniacharacter)
		{
			m_CharacterRank.LoadImageFromSet( 0, RANK_ICON_IMAGESET, rankinsigniacharacter );
			m_CharacterRank.SetVisible( true );
		}
		else
			m_CharacterRank.SetVisible( false );
	}
	
	void SetCharacterName( IEntity Char )
	{
		m_CharIDComp = SCR_CharacterIdentityComponent.Cast( Char.FindComponent( SCR_CharacterIdentityComponent ) );
		if( Char )
		{
			CharName = DS_DialogueComponent.GetCharacterName( Char );
			rank = DS_DialogueComponent.GetCharacterRankInsignia( Char );
			faction = DS_DialogueComponent.GetCharacterFaction( Char ).GetFactionKey();
			rankText = DS_DialogueComponent.GetCharacterRankNameFull( Char );
		}
		
		m_TextCharacterRank.SetText( rankText );
		m_TextCharacterRank.SetVisible( true );
		int CharRep = m_CharIDComp.GetRep();
		
		if(CharRep > 80)
		{
			m_CharacterRep.SetColor( Color.DarkGreen );
			m_CharacterRep.SetText( "Briliant" );
		}
		else if(CharRep > 60)
		{
			m_CharacterRep.SetColor( Color.DarkGreen );
			m_CharacterRep.SetText( "Good" );
		}
		else if(CharRep > 40)
		{
			m_CharacterRep.SetColor( Color.DarkYellow );
			m_CharacterRep.SetText( "Neuteral" );
		}
		else if(CharRep > 20)
		{
			m_CharacterRep.SetColor( Color.DarkRed );
			m_CharacterRep.SetText( "Bad" );
		}
		else if(CharRep > 1)
		{
			m_CharacterRep.SetColor( Color.DarkRed );
			m_CharacterRep.SetText( "Horrible" );
		}
		m_CharacterRep.SetVisible( true );		
		m_CharacterName.SetText( CharName );
		m_CharacterName.SetVisible( true );
	}
	
	//------------------------------------------------------------------------------------------------
	void Show( float fDelay = 0.0, Widget w = null, bool forceShow = false )
	{
		m_bForceShow = forceShow;
		m_wWidgetUnderCursor = w;
		if (fDelay == 0)
		{
			ShowInfoWidget( true );
		}
		else
		{
			GetGame().GetCallqueue().Remove( ShowInfoWidget );
			GetGame().GetCallqueue().CallLater( ShowInfoWidget, fDelay * 1000, false, true );
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void ShowInfoWidget( bool bShow )
	{
		if ( m_bForceShow )
		{
			m_wInfoWidget.SetVisible( true );
			return;
		}

		if ( ! m_wWidgetUnderCursor )
			return;
		
		if ( WidgetManager.GetWidgetUnderCursor() != m_wWidgetUnderCursor )
			return;
		
		m_wInfoWidget.SetVisible( true );
	}

	//------------------------------------------------------------------------------------------------
	void Hide()
	{
		if ( ! m_wInfoWidget )
			return;
		
		m_wInfoWidget.SetVisible( false );
		m_wInfoWidget.SetEnabled( false );
	}

	//------------------------------------------------------------------------------------------------
	void Move( float x, float y )
	{
		if ( ! m_pFrameSlotUI )
			return;
		
		m_pFrameSlotUI.SetPosX( x );
		m_pFrameSlotUI.SetPosY( y );
	}

	//------------------------------------------------------------------------------------------------
	void SetName( string sName )
	{
		if ( ! m_wTextName )
			return;
		
		if (sName.IsEmpty())
			m_wTextName.GetParent().SetVisible( false );
		else
			m_wTextName.SetText( sName );
	}

	//------------------------------------------------------------------------------------------------
	override void HandlerAttached( Widget w )
	{
		if (!w)
			return;
		m_wInfoWidget	= w;
		m_wTextName	= TextWidget.Cast( w.FindAnyWidget( "ItemInfo_name" ) );
		Widget wItemInfo	= m_wInfoWidget.FindAnyWidget( "ItemInfo" );
		if (!wItemInfo)
			return;
		m_pFrameSlotUI = SCR_SlotUIComponent.Cast( wItemInfo.FindHandler( SCR_SlotUIComponent ) );
		
		m_CharacterRep = TextWidget.Cast( wItemInfo.FindAnyWidget( "CharReputationText" ) );
		m_CharacterName = TextWidget.Cast( wItemInfo.FindAnyWidget( "CharacterName" ) );
		m_CharacterRank = ImageWidget.Cast( wItemInfo.FindAnyWidget( "RankIcon" ) );
		m_CharacterFactionIcon = ImageWidget.Cast( wItemInfo.FindAnyWidget( "FactionIcon" ) );
		m_TextCharacterRank = TextWidget.Cast( wItemInfo.FindAnyWidget( "CharRankText" ) );
	}

	//------------------------------------------------------------------------------------------------
	void Destroy()
	{
		if (m_wInfoWidget)
		{
			GetGame().GetCallqueue().Remove( ShowInfoWidget );
			m_wInfoWidget.RemoveHandler( m_pFrameSlotUI );
			m_wInfoWidget.RemoveHandler( this );
			m_wInfoWidget.RemoveFromHierarchy();
		}
	}

	//------------------------------------------------------------------------------------------------
	Widget GetInfoWidget()
	{
		return m_wInfoWidget.FindAnyWidget( "size" );
	}
};
