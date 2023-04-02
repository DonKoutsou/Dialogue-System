[BaseContainerProps(configRoot: true)]
class SP_RadialChoiceConfig: ScriptAndConfig
{
	[Attribute("", UIWidgets.ComboBox, "What is going to be the behavior of radial menu choise", "", ParamEnumArray.FromEnum(EChoiseBehavior) )]
	protected EChoiseBehavior ChoiseBehavior;
	//Action title
	[Attribute(defvalue: "", desc: "Action Title and Dialogue Text for Radial menu choice", category: "Dialogue")]
	protected ref array<ref RadialChoiceText> m_RadialChoiceTexts;
	//Get action title besed on ID provided
	string GetActionText(int TextID)
    {
        if (TextID >= 0 && TextID < m_RadialChoiceTexts.Count()) 
        {
            return m_RadialChoiceTexts[TextID].ActionText;
        }
        else 
        {
            return STRING_EMPTY;
        }
    }
	//Get dialogue besed on ID provided
	string GetDialogueText(int TextID)
    {
        if (TextID >= 0 && TextID < m_RadialChoiceTexts.Count()) 
        {
            return m_RadialChoiceTexts[TextID].DialogueText;
        }
        else 
        {
            return STRING_EMPTY;
        }
    }
	EChoiseBehavior GetChoiseBehavior()
	{
		return ChoiseBehavior;
	}
	
};

class RadialChoiceText: ScriptAndConfig
{
	[Attribute(defvalue: "", desc: "Action Title", category: "Dialogue")]
	string ActionText;
	[Attribute(defvalue: "", desc: "Dialogue Text", category: "Dialogue")]
    string DialogueText;
}