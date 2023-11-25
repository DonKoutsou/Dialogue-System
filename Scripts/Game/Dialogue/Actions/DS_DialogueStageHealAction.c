[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageHealAction : DS_BaseDialogueStageAction
{
	SCR_FadeInOutEffect FadeInOutEffect;
	override void Perform(IEntity Character, IEntity Player)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return;
		
		SCR_HUDManagerComponent hudManager = SCR_HUDManagerComponent.Cast(playerController.GetHUDManagerComponent());
		if (!hudManager)
			return;
		FadeInOutEffect = SCR_FadeInOutEffect.Cast(hudManager.FindInfoDisplay(SCR_FadeInOutEffect));
		FadeInOutEffect.FadeOutEffect(false, 1000);
		GetGame().GetCallqueue().CallLater(Fadeout, 1000, false);
		
		SCR_CharacterDamageManagerComponent CharDamageMnagr = SCR_CharacterDamageManagerComponent.Cast(Player.FindComponent(SCR_CharacterDamageManagerComponent));
		CharDamageMnagr.FullHeal();
		
		super.Perform(Character, Player);
	};
	
	void Fadeout()
	{
		FadeInOutEffect.FadeOutEffect(false, 10);
	}
};

