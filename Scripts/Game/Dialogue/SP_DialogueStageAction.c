[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageHealAction : DialogueStage
{
	override void Perform(IEntity Character, IEntity Player)
	{
		SCR_CharacterDamageManagerComponent CharDamageMnagr = SCR_CharacterDamageManagerComponent.Cast(Player.FindComponent(SCR_CharacterDamageManagerComponent));
		CharDamageMnagr.FullHeal();
	};
};

[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageTeleportAction : DialogueStage
{
	[Attribute("", UIWidgets.Coords, params: "inf inf inf purpose=coords space=world", desc: "")]
	private vector m_TeleportToWorldPosition;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		vector mat[4];
		BaseGameEntity char = BaseGameEntity.Cast(Player);
		if (char)
		{
			char.GetWorldTransform(mat);
			mat[3] = m_TeleportToWorldPosition;
			SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(char.FindComponent(SCR_EditableEntityComponent));
			if (editable)
			{
				editable.SetTransform(mat);
			}
			else
				char.Teleport(mat);
		}		
	};
	override bool CanBePerformed()
	{
		return false;
	}
};