[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class SP_DialogueStagePaperCheckAction : SP_DialogueStageItemCheckAction
{
	
	override void Perform(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		SP_PrefabResource_Predicate pred = new SP_PrefabResource_Predicate(m_WantedItem);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		if (entitiesToDrop.Count() < m_WantedAmount)
		{
			SCR_FactionManager factman = SCR_FactionManager.Cast(GetGame().GetFactionManager());
			factman.SetFactionsHostile(SCR_Faction.Cast(factman.GetFactionByKey("SPEIRA")), SCR_Faction.Cast(factman.GetFactionByKey("USSR")));
			SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
			DiagComp.DoAnouncerDialogue("You were caught without papers, you will be considered enemy to USSR faction from now on");
			DiagComp.DoBackDialogue(Character, Player);
			DiagComp.ReleaseAI(Character, Player);
			GetGame().GetMenuManager().CloseAllMenus();
		}
		super.Perform(Character, Player);
	};

};