[BaseContainerProps(), SP_DialogueBranch()]
class SP_DialogueStage: ScriptAndConfig
{
	//------------------------------------------------------------------//
	[Attribute( defvalue: "", desc: "ActionText", category: "Dialogue",  )]
	protected string m_sActionText;
	[Attribute( defvalue: "", desc: "DialogueText", category: "Dialogue",  )]
	protected string m_sDialogueText;
	[Attribute()]
	protected bool m_bMultipleChoise;
	[Attribute()]
	protected bool m_bGlobalStageInfluance;
	//------------------------------------------------------------------//
	string GetDialogueText()
	{
		return m_sDialogueText;
	}
	//------------------------------------------------------------------//
	string GetActionText()
	{
		return m_sActionText;
	}
	//------------------------------------------------------------------//
	bool IsthisMultiple()
	{
		return m_bMultipleChoise;
	}
	//------------------------------------------------------------------//
	bool IsInfluanceGlobal()
	{
		return m_bGlobalStageInfluance;
	}
};
