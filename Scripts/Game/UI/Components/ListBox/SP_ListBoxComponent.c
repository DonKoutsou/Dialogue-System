/*
Listbox component which supports multi selection, custom element layouts.

!!! This is not entirely finished, use at own risk. Or ping me if you need that functionality.
*/
class SP_ListBoxComponent : SCR_ListBoxComponent
{
	int GetElementIndex(SP_ListBoxElementComponent element)
	{
		for (int i = 0; i < m_aElementComponents.GetSizeOf(); i++)
		{
			if (m_aElementComponents[i] == element)
				return i;
		}
		return -1;
	}
	
};