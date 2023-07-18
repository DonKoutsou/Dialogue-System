/*
Element of a listbox.
*/
class SP_ListBoxElementComponent : SCR_ListBoxElementComponent
{
	protected const string WIDGET_TEXTNUMBER = "TextNumber";
	
	[Attribute("TextNumber")]
	protected string m_sWidgetTextNumber;

	ref ScriptInvoker m_OnKeyDown = new ScriptInvoker();
	
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
	ScriptInvoker GetOnClick()
	{
		if (!m_OnClicked)
			m_OnClicked = new ScriptInvoker();
		return OnClick();
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
		PanelWidget panel = PanelWidget.Cast(m_wRoot.FindAnyWidget("Border"));
		panel.SetVisible(true);
		
		m_OnClicked.Invoke(this);
			
		return eventReturnValue;
	}
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		#ifdef DEBUG_MODULAR_BUTTON
		_print("OnMouseEnter");
		#endif
		
		if (m_bIgnoreStandardInputs)
			return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_ENTER;
		
		m_bMouseOver = true;
		
		UpdateCurrentState();
		
		EInputDeviceType lastInput = GetGame().GetInputManager().GetLastUsedInputDevice();
		bool mouseInput = lastInput == EInputDeviceType.MOUSE;
		
		if (m_bFocusOnMouseEnter)
		{
			if (mouseInput)
				GetGame().GetWorkspace().SetFocusedWidget(m_wRoot);
		}
		
		InvokeEffectsEvent(EModularButtonEventFlag.EVENT_MOUSE_ENTER);
		PanelWidget panel = PanelWidget.Cast(m_wRoot.FindAnyWidget("Border"));
		panel.SetVisible(true);
		m_OnMouseEnter.Invoke(this, mouseInput);
			
		return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_ENTER;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		#ifdef DEBUG_MODULAR_BUTTON
		_print("OnMouseLeave");
		#endif
		
		if (m_bIgnoreStandardInputs)
			return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_LEAVE;
		
		m_bMouseOver = false;
		
		UpdateCurrentState();
		
		EInputDeviceType lastInput = GetGame().GetInputManager().GetLastUsedInputDevice();
		bool mouseInput = lastInput == EInputDeviceType.MOUSE;
		
		InvokeEffectsEvent(EModularButtonEventFlag.EVENT_MOUSE_LEAVE);
		PanelWidget panel = PanelWidget.Cast(m_wRoot.FindAnyWidget("Border"));
		panel.SetVisible(false);
		m_OnMouseLeave.Invoke(this, mouseInput);
		
		return m_eEventReturnValue & EModularButtonEventHandler.MOUSE_LEAVE;
	}
	override event bool OnKeyDown(Widget w, int x, int y, int key)
	{
		if (key == 1)
		{
			OnClick(w, x, y, 0);
			m_OnKeyDown.Invoke(this);
			return true;
		}
		return false;
	}
	
};