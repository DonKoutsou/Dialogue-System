/*
Element of a listbox.
*/
class DS_ListBoxElementComponent : SCR_ListBoxElementComponent
{
	
	[Attribute("TextNumber")]
	protected string m_sWidgetTextNumber;
	
	int branch;
	
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
	override bool OnClick(Widget w, int x, int y, int button)
	{
		#ifdef DEBUG_MODULAR_BUTTON
		_print("OnClick");
		#endif
		
		// Auto focus is focusable
		if (m_wRoot.IsFocusable())
		{
			auto workspace = GetGame().GetWorkspace();
			Widget currentFocus = workspace.GetFocusedWidget();
			if (currentFocus != m_wRoot)
				workspace.SetFocusedWidget(m_wRoot);
		}
		
		bool eventReturnValue = m_eEventReturnValue & EModularButtonEventHandler.CLICK;
		
		if (m_bIgnoreStandardInputs)
			return eventReturnValue;
		
		if (m_bCanBeToggled && !m_bToggledOnlyThroughApi)
			Internal_SetToggled(!m_bToggled);
		
		InvokeEffectsEvent(EModularButtonEventFlag.EVENT_CLICKED);
		
		EModularButtonState state = GetCurrentState();
		InvokeEffectsEvent(state);
		TextWidget tw = TextWidget.Cast(m_wRoot.FindAnyWidget(m_sWidgetTextNumber));
		m_OnClicked.Invoke(this);
			
		return eventReturnValue;
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
		m_OnClicked.Invoke(this);
	}
	
};