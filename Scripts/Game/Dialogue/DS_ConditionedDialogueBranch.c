[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class ItemConditionedDialogueBranchInfo : DialogueBranchInfo
{
	ResourceName m_WantedItem;
	
	int m_WantedAmount;
	
	void GetWantedAmount(out int amount){amount = m_WantedAmount;}

	override bool CheckCondition(IEntity Character, IEntity Player)
	{
		if (!m_WantedItem)
		{
			SetupCondition();
		}
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return false;
		
		DS_PrefabResource_Predicate pred = new DS_PrefabResource_Predicate(m_WantedItem);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		if (entitiesToDrop.Count() < m_WantedAmount)
		{
			return false;
		}
		return true;
		
	};
	void GetCondition(IEntity Character, IEntity Player, out string condition, out int amoutnt)
	{
		if (!m_WantedItem || !m_WantedAmount)
		{
			SetupCondition();
		}
		condition = m_WantedItem;
		amoutnt = m_WantedAmount;
	}
	override void SetupCondition()
	{
		m_WantedAmount = Math.RandomInt(1 ,10);
		DS_DialogueComponent Diagcomp = DS_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(DS_DialogueComponent));
		//m_WantedItem = Diagcomp.GetRandomItem();
	};
};