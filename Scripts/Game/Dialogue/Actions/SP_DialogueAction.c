class SP_DialogueAction : ScriptedUserAction
{
	[Attribute()]
	bool isradio;
	//------------------------------------------------------------------//
	protected SP_DialogueComponent DiagComp;
	protected BaseGameMode GameMode;
	//------------------------------------------------------------------//
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		GameMode = BaseGameMode.Cast(GetGame().GetGameMode());
		DiagComp = SP_DialogueComponent.Cast(GameMode.FindComponent(SP_DialogueComponent));
		if (SCR_EntityHelper.IsPlayer(pUserEntity))
		{
			if (!DiagComp.a_PLcontactList.Contains(pOwnerEntity))
				DiagComp.a_PLcontactList.Insert(pOwnerEntity)
		}
		
		AIControlComponent comp = AIControlComponent.Cast(pOwnerEntity.FindComponent(AIControlComponent));
		if (!comp)
			return;
		AIAgent agent = comp.GetAIAgent();
		if (!agent)
			return;
		SCR_AIUtilityComponent utility = SCR_AIUtilityComponent.Cast(agent.FindComponent(SCR_AIUtilityComponent));
		if (!utility)
			return;
		
		SCR_AIConverseBehavior action = new SCR_AIConverseBehavior(utility, null, pUserEntity.GetOrigin(), false);
		
		
		string NoTalkText = "Cant talk to you now";
		FactionKey SenderFaction = DiagComp.GetCharacterFaction(pOwnerEntity).GetFactionKey();
		BaseChatChannel Channel;
		string name = DiagComp.GetCharacterName(pOwnerEntity);
		if (!DiagComp.LocateDialogueArchetype(pOwnerEntity, pUserEntity))
		{
			DiagComp.SendText(NoTalkText, Channel, 0, name, DiagComp.GetCharacterRankName(pOwnerEntity));
			return;
		}
		if (!utility.FindActionOfType(SCR_AIFollowBehavior))
			utility.AddAction(action);
		MenuBase myMenu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.DialogueMenu);
		GetGame().GetInputManager().ActivateContext("DialogueMenuContext");
		DialogueUIClass DiagUI = DialogueUIClass.Cast(myMenu);
		//DiagComp.IntroducitonSting(pOwnerEntity, pUserEntity);
		DiagUI.Init(pOwnerEntity, pUserEntity);
		DiagUI.UpdateEntries(pOwnerEntity, pUserEntity);
	}
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		
	};
	//------------------------------------------------------------------//
	override bool CanBeShownScript(IEntity user)
	{
		if (user == GetOwner())
			return false;
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (char)
		{
			SCR_CharacterDamageManagerComponent damageMan = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
			if (!damageMan)
				return false;
			
			if (damageMan.GetState() == EDamageState.DESTROYED)
				return false;
			if(damageMan.GetIsUnconscious() == true)
			{
				return false;
			}
		}
		
		
		return super.CanBePerformedScript(user);
	}
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
	
	//----------------------------------------------------------------------------------
	override bool CanBroadcastScript() 
	{ 
		return false; 
	};
}