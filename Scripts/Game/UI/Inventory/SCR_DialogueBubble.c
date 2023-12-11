//------------------------------------------------------------------------------------------------
//! UI Script
//! Inventory Slot UI Layout
[EntityEditorProps(category: "GameScripted/UI/Inventory", description: "Inventory Item Info UI class")]
class SCR_DialogueBubble : ScriptedWidgetComponent
{
	protected Widget 						m_wRoot; 
	TextWidget 								m_CharacterName;
	TextWidget								m_DialogueT;
	
	string 											CharName;
	string 											CharRank;
	string 											rank;
	string 											rankText;
	FactionKey 										faction;
	SCR_CharacterIdentityComponent	m_CharIDComp;

	
	protected Widget								m_wInfoWidget;
	protected TextWidget							m_wTextName;
	protected SCR_SlotUIComponent					m_pFrameSlotUI;
	protected Widget 								m_wWidgetUnderCursor;
	protected bool 									m_bForceShow;

	void SetText(string text)
	{
		m_DialogueT.SetText(text);
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
		
		m_DialogueT = TextWidget.Cast(wItemInfo.FindAnyWidget("DialogueText"));
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
