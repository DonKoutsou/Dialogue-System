class DS_IDUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return false;

		// Don't inspect dead people
		SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
		if (controller.GetLifeState() == ECharacterLifeState.DEAD)
			return false;
		
		// Don't show inspect userAction when already showing SCR_InspectCasualtyWidget
		SCR_IDWidget casualtyInspectDisplay;
		array<BaseInfoDisplay> infoDisplays = {};
		
		PlayerController playerController = PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return false;
		
		HUDManagerComponent hudMan = playerController.GetHUDManagerComponent();
		if (!hudMan)
			return false;
		
		hudMan.GetInfoDisplays(infoDisplays);
		
		foreach (BaseInfoDisplay baseInfoDisplays : infoDisplays)
		{
			casualtyInspectDisplay = SCR_IDWidget.Cast(baseInfoDisplays);
			if (!casualtyInspectDisplay)
				continue;
			
			if (casualtyInspectDisplay.IsActive() && casualtyInspectDisplay.m_Target == GetOwner())
				return false;
		}
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		array<BaseInfoDisplay> infoDisplays = {};
		GetGame().GetPlayerController().GetHUDManagerComponent().GetInfoDisplays(infoDisplays);
		foreach (BaseInfoDisplay baseInfoDisplays : infoDisplays)
		{
			SCR_IDWidget casualtyInspectDisplay = SCR_IDWidget.Cast(baseInfoDisplays);
			if (!casualtyInspectDisplay)
				continue;

			casualtyInspectDisplay.SetTarget(pOwnerEntity);
			casualtyInspectDisplay.ShowInspectCasualtyWidget(pOwnerEntity);
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnActionStart(IEntity pUserEntity)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(pUserEntity);
		if (!char)
			return;

		CharacterControllerComponent contr = char.GetCharacterController();
		if(!contr)
			return;
		
		if (contr.CanPartialLower() && !contr.IsPartiallyLowered())
			contr.SetPartialLower(true);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Can this action be shown in the UI for the user?
	override bool HasLocalEffectOnlyScript() 
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	//! If HasLocalEffectOnly() is true this method tells if the server is supposed to broadcast this action to clients.
	override bool CanBroadcastScript()
	{
		return false;
	};
}
