class SP_DialogueAction : ScriptedUserAction
{
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected BaseGameMode GameMode;
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		AIControlComponent comp = AIControlComponent.Cast(pOwnerEntity.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		
		SCR_AIConverseBehavior action = new SCR_AIConverseBehavior(utility, null, pUserEntity.GetOrigin());
		
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		AIGroup group = AIGroup.Cast(agent.GetParentGroup());
		string NoTalkText = "Cant talk to you now";
		FactionKey SenderFaction = DiagComp.GetCharacterFaction(pOwnerEntity).GetFactionKey();
		BaseChatChannel Channel;
		string name = DiagComp.GetCharacterName(pOwnerEntity);
        if (group)
		{
			agent = group.GetLeaderAgent();
		}
		if (!DiagComp.LocateDialogueArchetype(pOwnerEntity, pUserEntity))
		{
			DiagComp.SendText(NoTalkText, Channel, 0, name, DiagComp.GetCharacterRankName(pOwnerEntity));
			return;
		}
		utility.AddAction(action);
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		GetGame().GetInputManager().ActivateContext("DialogueMenuContext");
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		DiagUI.Init(pOwnerEntity, pUserEntity);
		DiagUI.UpdateEntries(pOwnerEntity, pUserEntity);
	}
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
	};
	//------------------------------------------------------------------//
	override bool CanBeShownScript(IEntity user)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return false;
		SCR_CharacterDamageManagerComponent damageMan = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMan)
			return false;
		
		if (damageMan.GetState() == EDamageState.DESTROYED)
			return false;
		if(damageMan.GetIsUnconscious() == true)
		{
			return false;
		}
		
		return super.CanBePerformedScript(user);
	}
}