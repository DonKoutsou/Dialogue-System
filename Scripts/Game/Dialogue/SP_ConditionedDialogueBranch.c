[BaseContainerProps(configRoot:true),  DialogueBranchConfigTitleAttribute()]
class SP_ConditionedDialogueBranch : SP_DialogueBranch
{
	[Attribute()]
	ref ItemConditionedDialogueBranchInfo 								CondBranchInfoConfig;
	protected ref map<string, ref ItemConditionedDialogueBranchInfo> 	CondBranchInfoConfigMap;
	
	bool CheckCondition(IEntity Character, IEntity Player)
	{
		ItemConditionedDialogueBranchInfo CondConf = ItemConditionedDialogueBranchInfo.Cast(LocateConfig(Character));
		return CondConf.CheckCondition(Character, Player);
	};
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		return CheckCondition(Character,Player);
	};
	override DialogueBranchInfo LocateConfig(IEntity Character)
	{
		ItemConditionedDialogueBranchInfo config;
		if(!CondBranchInfoConfigMap)
		{
			CondBranchInfoConfigMap = new map<string, ref ItemConditionedDialogueBranchInfo>();
		}
		SCR_BaseGameMode GameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		SP_DialogueComponent DiagComp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		string key = DiagComp.GetCharacterName(Character);
		if (CondBranchInfoConfigMap.Find(key, config))
		{
			return config;
		}
		else
		{
			config = CopyCondConfig(CondBranchInfoConfig);
			CondBranchInfoConfigMap.Insert(key, config);
			return config;
		}
	}
	ItemConditionedDialogueBranchInfo CopyCondConfig(ItemConditionedDialogueBranchInfo OriginalConfig)
	{
		ItemConditionedDialogueBranchInfo DiagConfigCopy = new ItemConditionedDialogueBranchInfo(OriginalConfig, true);
		return DiagConfigCopy;
	}
};
[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class ConditionedDialogueBranchInfo : DialogueBranchInfo
{
	bool CheckCondition(IEntity Character, IEntity Player)
	{
		
	};
};

[BaseContainerProps(configRoot:true), SCR_BaseContainerCustomTitleField("ActionText", "DialogueText")]
class ItemConditionedDialogueBranchInfo : ConditionedDialogueBranchInfo
{
	ResourceName m_WantedItem;
	
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	
	void GetWantedAmount(out int amout){amout = m_WantedAmount;}
	void ItemConditionedDialogueBranchInfo(ItemConditionedDialogueBranchInfo original, bool isNew = false)
    {
		if (isNew) 
		{
			ItemConditionedDialogueBranchInfo inf = ItemConditionedDialogueBranchInfo.Cast(original);
        	inf.GetWantedAmount(m_WantedAmount);
		}
    }
	override bool CheckCondition(IEntity Character, IEntity Player)
	{
		if (!m_WantedItem)
		{
			m_WantedItem = SetupCondition();
		}
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return false;
		
		SP_PrefabResource_Predicate pred = new SP_PrefabResource_Predicate(m_WantedItem);
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
		if (!m_WantedItem)
		{
			m_WantedItem = SetupCondition();
		}
		condition = m_WantedItem;
		amoutnt = m_WantedAmount;
	}
	ResourceName SetupCondition()
	{
		int index;
		SP_DialogueComponent Diagcomp = SP_DialogueComponent.Cast(GetGame().GetGameMode().FindComponent(SP_DialogueComponent));
		ResourceName WantedItem = Diagcomp.GetRandomItem();
		return WantedItem;
	};
};