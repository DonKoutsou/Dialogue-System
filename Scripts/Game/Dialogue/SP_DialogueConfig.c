[BaseContainerProps(), SP_DialogueArchetype()]
class SP_DialogueConfig: ScriptAndConfig
{
	
	
	[Attribute( defvalue: "", desc: "ActionText", category: "Dialogue",  )]
	protected string m_sActionText;
	[Attribute( defvalue: "", desc: "DialogueText", category: "Dialogue",  )]
	protected string m_sDialogueText;
	[Attribute( defvalue: "", desc: "DialogueBranchKey, Key used to define wich action should use this config", category: "Dialogue",  )]			//TODO: make config, memory
	protected int m_iDialogueBranchKey;
	[Attribute( defvalue: "", desc: "DialogueStageKey, defines at wich stage of the dialogue this config will be used", category: "Dialogue",  )]			//TODO: make config, memory
	protected int m_iDialogueStageKey;
	[Attribute()]
	protected bool m_bMultipleChoise;
	[Attribute()]
	protected bool m_bGlobalStageInfluance;
	[Attribute()]
	protected bool m_bIsPermanent;
	int GetDialogueStageKey()
	{
		return m_iDialogueStageKey;
	}

	string GetDialogueText(int stageid)
	{
		string diagtext;
		diagtext = m_sDialogueText;
		return diagtext;
	}
	
	int GetDialogueBranchKey()
	{
		return m_iDialogueBranchKey;
	}
	
	string GetActionText(int stageid)
	{
		string actiontext;
		actiontext = m_sActionText;
		
		return actiontext;
	}
	
	bool IsthisMultiple()
	{
		return m_bMultipleChoise;
	}
	
	bool IsInfluanceGlobal()
	{
		return m_bGlobalStageInfluance;
	}
};
