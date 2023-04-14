[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class DialogueStageHealActionAction : DialogueStage
{
	override void Perform(IEntity Character, IEntity Player)
	{
		SCR_CharacterDamageManagerComponent CharDamageMnagr = SCR_CharacterDamageManagerComponent.Cast(Player.FindComponent(SCR_CharacterDamageManagerComponent));
		CharDamageMnagr.FullHeal();
	};
};