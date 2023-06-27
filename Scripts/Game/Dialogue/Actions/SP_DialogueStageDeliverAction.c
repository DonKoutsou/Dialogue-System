class SP_ConsumablePredicate : InventorySearchPredicate
{
	EConsumableType m_ConsumableFilter;
	
	void SP_ConsumablePredicate(EConsumableType type)
	{
		QueryComponentTypes.Insert(SCR_ConsumableItemComponent);
		m_ConsumableFilter = type;
	}

	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{
		return (SCR_ConsumableItemComponent.Cast(queriedComponents[0])).GetConsumableType() == m_ConsumableFilter);
	}
}

class SP_PrefabResource_Predicate : InventorySearchPredicate
{
	ResourceName m_WantedPrefabName;
	void SP_PrefabResource_Predicate(ResourceName prefabName)
	{
		m_WantedPrefabName = prefabName;
	}

	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{
		return item.GetPrefabData().GetPrefabName() == m_WantedPrefabName;
	}
}
[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageDeliverTaskAction : DialogueStage
{
	override void Perform(IEntity Character, IEntity Player)
	{
		SP_RequestManagerComponent requestman = SP_RequestManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SP_RequestManagerComponent));
		if(requestman.CharHasTask(Character))
		{
			array<ref SP_Task> MyTasks = new array<ref SP_Task>();
			requestman.GetCharTasks(Character, MyTasks);
			for (int i, count = MyTasks.Count(); i < count; i++)
			{
				if(MyTasks[i].ReadyToDeliver(Character, Player))
				{
					MyTasks[i].CompleteTask(Player);
				}
			}
		}
		if(requestman.CharIsTarget(Character))
		{
			array<ref SP_Task> MyTasks = new array<ref SP_Task>();
			requestman.GetCharTargetTasks(Character, MyTasks);
			for (int i, count = MyTasks.Count(); i < count; i++)
			{
				if(MyTasks[i].ReadyToDeliver(Character, Player))
				{
					MyTasks[i].CompleteTask(Player);
				}
			}
		}
	};
	override bool CanBeShown(IEntity Character, IEntity Player)
	{
		SP_RequestManagerComponent requestman = SP_RequestManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SP_RequestManagerComponent));
		if(requestman.CharHasTask(Character))
		{
			array<ref SP_Task> MyTasks = new array<ref SP_Task>();
			requestman.GetCharTasks(Character, MyTasks);
			foreach (SP_Task Task : MyTasks)
			{
				if(Task.ReadyToDeliver(Character, Player))
				{
					return true;
				}
			}
		}
		if(requestman.CharIsTarget(Character))
		{
			array<ref SP_Task> MyTasks = new array<ref SP_Task>();
			requestman.GetCharTargetTasks(Character, MyTasks);
			foreach (SP_Task Task : MyTasks)
			{
				if(Task.ReadyToDeliver(Character, Player))
				{
					return true;
				}
			}
		}
		return false;
		
	}

};