modded class SCR_ListBoxComponent
{
	int AddDiagItem(string item, string CharName, Color color, Managed data = null, ResourceName itemLayout = string.Empty)
	{	
		SCR_ListBoxElementComponent comp;
		
		int id = _AddDiagItem(item, CharName, color, data, comp, itemLayout);
		
		return id;
	}
	protected int _AddDiagItem(string item, string CharName, Color color, Managed data, out SCR_ListBoxElementComponent compOut, ResourceName itemLayout = string.Empty)
	{	
		// Create widget for this item
		// The layout can be provided either as argument or through attribute
		ResourceName selectedLayout = m_sElementLayout;
		if (!itemLayout.IsEmpty())
			selectedLayout = itemLayout;
		Widget newWidget = GetGame().GetWorkspace().CreateWidgets(selectedLayout, m_wList);
		
		SCR_ListBoxElementComponent comp = SCR_ListBoxElementComponent.Cast(newWidget.FindHandler(SCR_ListBoxElementComponent));
		
		comp.SetText(item);
		comp.SetToggleable(true);
		comp.SetData(data);
		if (CharName)
		{
			comp.SetCharNameText(CharName, color);
		}
			
		// Pushback to internal arrays
		int id = m_aElementComponents.Insert(comp);
		
		
		// Setup event handlers
		comp.m_OnClicked.Insert(OnItemClick);
		
		// Set up explicit navigation rules for elements. Otherwise we can't navigate
		// Through separators when we are at the edge of scrolling if there is an element
		// directly above/below the list box which intercepts focus
		string widgetName = this.GetUniqueWidgetName();
		newWidget.SetName(widgetName);
		if (m_aElementComponents.Count() > 1)
		{
			Widget prevWidget = m_aElementComponents[m_aElementComponents.Count() - 2].GetRootWidget();
			prevWidget.SetNavigation(WidgetNavigationDirection.DOWN, WidgetNavigationRuleType.EXPLICIT, newWidget.GetName());
			newWidget.SetNavigation(WidgetNavigationDirection.UP, WidgetNavigationRuleType.EXPLICIT, prevWidget.GetName());
		}
		
		compOut = comp;
		
		return id;
	}
}
modded class SCR_ListBoxElementComponent
{
	void SetCharNameText(string text, Color clr)
	{
		TextWidget w = TextWidget.Cast(m_wRoot.FindAnyWidget("CharName"));
		
		if (w)
		{
			w.SetText(text);
			w.SetColor(clr);
		}
			
	}
}