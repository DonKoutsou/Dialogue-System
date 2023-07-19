/*
Element of a listbox.
*/
class SP_ListBoxElementComponent : SCR_ListBoxElementComponent
{
	
	[Attribute("TextNumber")]
	protected string m_sWidgetTextNumber;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_bToggledOnlyThroughApi = true;
	}
	
	void SetTextNumber(string Number)
	{
		TextWidget w = TextWidget.Cast(m_wRoot.FindAnyWidget(m_sWidgetTextNumber));
		
		if (w)
			w.SetText(Number);
	}
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);
		InvokeEffectsEvent(EModularButtonEventFlag.EVENT_MOUSE_ENTER);
		PanelWidget panel = PanelWidget.Cast(m_wRoot.FindAnyWidget("Border"));
		panel.SetVisible(true);
		m_OnMouseEnter.Invoke(this, true);
			
		return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_ENTER;
	}
	//------------------------------------------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		InvokeEffectsEvent(EModularButtonEventFlag.EVENT_MOUSE_LEAVE);
		PanelWidget panel = PanelWidget.Cast(m_wRoot.FindAnyWidget("Border"));
		panel.SetVisible(false);
		m_OnMouseLeave.Invoke(this, true);
		
		return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_LEAVE;
	}
	void OnKeyPressed(float value = 0.0, EActionTrigger reason = 0)
	{
		DialogueUIClass MyUI = DialogueUIClass.Cast(ChimeraMenuBase.CurrentChimeraMenu());
		SP_ListBoxComponent list = MyUI.m_ListBoxComponent;
		MyUI.ExecuteDialogue(list.GetElementIndex(this));
	}
	
};