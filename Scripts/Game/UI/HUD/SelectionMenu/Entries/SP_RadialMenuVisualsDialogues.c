class SP_RadialMenuVisualsDialogues : SCR_RadialMenuVisuals
{
	
	//------------------------------------------------------------------------------------------------
	//override void SetContent(array<BaseSelectionMenuEntry> allEntries, array<BaseSelectionMenuEntry> disabledEntries, bool clearData = false)
	//{
	//	super.SetContent(allEntries, disabledEntries, clearData);
	//}
	//
	
	[Attribute("0", UIWidgets.CheckBox, "Distance of entries from center")]
	protected float m_fTransformEntries;
	//------------------------------------------------------------------------------------------------
	override protected void SetElementData(SCR_RadialMenuPair<Widget, BaseSelectionMenuEntry> element, bool canBePerformed, SCR_SelectionEntryWidgetComponent widgetComp)
	{
		super.SetElementData(element, canBePerformed, widgetComp);
		
		// Update entry visuals 
		ScriptedSelectionMenuEntry entry = ScriptedSelectionMenuEntry.Cast(element.m_pEntry);
		if (entry)
			entry.UpdateVisuals();
	}
	protected vector GetPointOnAxis(float distance, float angleDegrees)
	{
	    float angleRadians = angleDegrees * (Math.PI / 180.0);
	    float x = 0.0;
	    float y = distance * Math.Cos(angleRadians);
	    float z = distance * Math.Sin(angleRadians);
	
	    return Vector(x, y, z);
	}
	protected vector GetElementPositionDistance(int index, int count, float transformFactor)
	{
	    float angle = m_fEntryAngleDistance * index;
	    float distance = transformFactor;
	
	    float angleRadians = angle * (Math.PI / 180.0);
	    float totalHeight = count * 10.0; // Total height required for all elements
	    float spacing = totalHeight / count; // Spacing between elements
	    float y = spacing * index; // Calculate y position based on spacing and index
	
	    float z = 0.0;
	
	    return Vector(0.0, y * transformFactor, z);
	}
	override void SetContent(array<BaseSelectionMenuEntry> allEntries, array<BaseSelectionMenuEntry> disabledEntries, bool clearData = false)
{
    // clearing items 
    if (clearData)
        DestroyElementWidgets();
    
    for (int i = 0; i < m_aBorderLines.Count(); i++)
    {
        RemoveBorderLine(m_aBorderLines[i]);
    }
    
    // Adding items  
    int originalCount = m_pWidgetPairs.Count();
    int incomingCount = allEntries.Count();
    
    // If we need to shrink the content, do so
    if (incomingCount < originalCount)
    {
        for (int i = originalCount-1; i >= incomingCount; i--)
        {
            DestroyElementWidgetAt(i);
        }
    }
    
    for (int i = 0; i < incomingCount; i++)
    {
        SCR_SelectionEntryWidgetComponent entryWidget = null;
        if (m_aEntryElements.Count() > i)
        {
            entryWidget = m_aEntryElements[i];
        }
        
        auto currentEntry = allEntries[i];
        bool canBePerformed = disabledEntries.Find(currentEntry) == -1;
        
        // For widgets which already exist, 
        // just update entries
        if (i < originalCount)
        {
            m_pWidgetPairs.SetEntryAt(i, currentEntry);
            
            // Check if we have a widget already,
            // if not proceed with creating new one
            auto pair = m_pWidgetPairs.GetAt(i);
            auto widget = pair.m_pWidget;
            if (widget)
            {
                auto position = GetElementPosition(i, incomingCount, m_fRadiusEntries);
                SetElementPosition(widget, position);
                SetElementData(pair, canBePerformed, entryWidget);
                continue;
            }
        }
        
        // For widgets which do not exist,
        // create new ones
        ScriptedSelectionMenuEntry selectionEntry = ScriptedSelectionMenuEntry.Cast(currentEntry);
        
        Widget widget = CreateElementWidget(m_wRoot, selectionEntry);
        
        // Prepare pair
        ref auto pair = new ref SCR_RadialMenuPair<Widget, BaseSelectionMenuEntry>(null, currentEntry);
        
        // If we created widget, create container with child widgets for it
        if (widget)
        {
            // Set pair value
            pair.m_pEntry = currentEntry;
            pair.m_pWidget = widget;
            
            auto position = GetElementPosition(i, incomingCount, m_fRadiusEntries);
            SetElementPosition(widget, position);
            SetElementData(pair, canBePerformed, entryWidget);
            currentEntry.GetEntryIconPath();
        }
        
        m_pWidgetPairs.AddEntry(pair);
        
        // Add lines between entries
        if (!m_rBorderLinePath.IsEmpty())
        {
            float angle = m_fEntryAngleDistance * i + m_fEntryAngleDistance/2;
            CreateBorderLine(m_wLayoutContent, angle, m_rBorderLinePath);
        }
    }
    
    // Update angle on change
    UpdateBorderLinesAngle();
    
    // Set select size
    SetWidgetSelectSize(m_wSelectCurrent, 0.5);
    SetWidgetSelectSize(m_wSelectLast, 0.5);
}
	
}