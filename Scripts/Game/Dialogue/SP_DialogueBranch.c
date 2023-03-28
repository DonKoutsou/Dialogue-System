[BaseContainerProps(), SP_DialogueArchetype()]
class SP_DialogueBranch: ScriptAndConfig
{
	
	[Attribute()]
	protected int CharacterDiagBranchStage;
	
	[Attribute()]
	protected ref array<ref SP_DialogueStage> DialogueStage;
	
	SP_DialogueStage GetDialogueStage()
	{
		return DialogueStage[CharacterDiagBranchStage];
	}
	void IncrementBranchStage(int incrementamount)
	{
		CharacterDiagBranchStage = CharacterDiagBranchStage + 1;
	}
	int GetBranchStage()
	{
		return CharacterDiagBranchStage;
	}
	string GetDialogueText(int StageKey)
	{
		if (StageKey >= 0 && StageKey < DialogueStage.GetRefCount())
		{
			return DialogueStage[StageKey].GetDialogueText();
		}
		else
		{
			 return STRING_EMPTY;
		}
	}
	string GetDialogueActionTitle(int StageKey)
	{
		if (StageKey >= 0 && StageKey < DialogueStage.GetRefCount())
		{
			return DialogueStage[StageKey].GetActionText();
		}
		else
		{
			 return STRING_EMPTY;
		}
	}
}