/*
Element of a listbox.
*/
class SCR_ListBoxElementComponent : SCR_ModularButtonComponent
{
	protected const string WIDGET_IMAGE = "Image";
	protected const string WIDGET_TEXT = "Text";
	protected const string WIDGET_TEXTNUMBER = "TextNumber";
	
	[Attribute("Text")]
	protected string m_sWidgetTextName;
	[Attribute("TextNumber")]
	protected string m_sWidgetTextNumber;
	
	//------------------------------------------------------------------------------------------------
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
	
		m_bToggledOnlyThroughApi = true;
	}
	
	void SetImage(ResourceName imageOrImageset, string iconName)
	{
		ImageWidget w = ImageWidget.Cast(m_wRoot.FindAnyWidget(WIDGET_IMAGE));
		
		if (imageOrImageset.IsEmpty())
			return;
		
		if (w)
		{
			if (imageOrImageset.EndsWith("imageset"))
			{
				if (!iconName.IsEmpty())
					w.LoadImageFromSet(0, imageOrImageset, iconName);
			}
			else
				w.LoadImageTexture(0, imageOrImageset);
		}
	}
	
	void SetText(string text)
	{
		TextWidget w = TextWidget.Cast(m_wRoot.FindAnyWidget(m_sWidgetTextName));
		
		if (w)
			w.SetText(text);
	}
	void SetTextNumber(string Number)
	{
		TextWidget w = TextWidget.Cast(m_wRoot.FindAnyWidget(m_sWidgetTextNumber));
		
		if (w)
			w.SetText(Number);
	}
	
	
};