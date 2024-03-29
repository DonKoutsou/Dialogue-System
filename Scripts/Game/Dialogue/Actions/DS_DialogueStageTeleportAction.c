[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageTeleportAction : DS_BaseDialogueStageAction
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
		super.Perform(Character, Player);	
	};
};