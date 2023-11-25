//--------------------------------------------------------------------------------------------//
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageFactionCheckCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute()]
	string m_sFactionKey;
	
	
	
	[Attribute(desc:"Return resault when faction is the same", defvalue : "1")]
	bool m_bSameFaction;
	
	[Attribute(desc:"Used incase above bool is set to false. Reason why same faction cane use it")]
	string m_bSameFactionCantBePerformedReason;
	
	
	
	[Attribute(desc:"Return resault when faction is the Friendly", defvalue : "1")]
	bool m_bFriendlyFaction;
	
	[Attribute(desc:"Used incase above bool is set to false. Reason why friendly faction cant use it")]
	string m_bFriendlyFactionCantBePerformedReason;
	
	
	
	[Attribute(desc:"Return resault when faction is the Enemy", defvalue : "1")]
	bool m_bEnemyFaction;
	
	[Attribute(desc:"Used incase above bool is set to false. Reason why enemy faction cant use it")]
	string m_bEnemyFactionCantBePerformedReason;
	
	
	ref SCR_Faction faction;
	override bool CanBePerformed(IEntity Character, IEntity Player, DialogueStage ParentStage)
	{
		if (!faction)
		{
			SCR_FactionManager man = SCR_FactionManager.Cast(GetGame().GetFactionManager());
			faction = SCR_Faction.Cast(man.GetFactionByKey(m_sFactionKey));
		}

		FactionAffiliationComponent FactComp = FactionAffiliationComponent.Cast(Player.FindComponent(FactionAffiliationComponent));
		
		SCR_Faction Plfaction = SCR_Faction.Cast(FactComp.GetAffiliatedFaction());
		if (Plfaction.GetFactionKey() == m_sFactionKey)
		{
			m_sCantBePerformedReason = m_bSameFactionCantBePerformedReason;
			return m_bSameFaction;
		}
		if (Plfaction.DoCheckIfFactionFriendly(faction))
		{
			m_sCantBePerformedReason = m_bFriendlyFactionCantBePerformedReason;
			return m_bFriendlyFaction;
		}
		else
		{
			m_sCantBePerformedReason = m_bEnemyFactionCantBePerformedReason;
			return m_bEnemyFaction;
		}
		return true;
	}
};
//----------------------------------------------------------------------------------------------//
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageSingleUseCondition : DS_BaseDialogueStageActionCondition
{
	override bool CanBePerformed(IEntity Character, IEntity Player, DialogueStage ParentStage)
	{	
		if (ParentStage.timesused > 0)
		{
			m_sCantBePerformedReason = "Already claimed";
			return false;
		}
		return true;
	}
};
//--------------------------------------------------------------------------------------------//

[BaseContainerProps(configRoot:true)]
class DS_DialogueStageHealthCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute()]
	bool m_bResaultWhenMaxHP;
	[Attribute("Used incase above bool is set to false.")]
	string m_sMaxHPUnperformableReason;
	[Attribute("10", UIWidgets.Slider, "HP threashold", "0.0 100.0 1.0")]
	float m_fHPThreashold;
	[Attribute()]
	bool m_bResaultWhenThreasholdIsMet;
	[Attribute("Used incase above bool is set to false.")]
	string m_sThreasholdMetUnperformableReason;
	[Attribute()]
	bool m_bResaultWhenThreasholdIsNotMet;
	[Attribute("Used incase above bool is set to false.")]
	string m_sThreasholdNotMetUnperformableReason;
	
	override bool CanBePerformed(IEntity Character, IEntity Player, DialogueStage ParentStage)
	{
		SCR_CharacterDamageManagerComponent dmgmngr = SCR_CharacterDamageManagerComponent.Cast(Player.FindComponent(SCR_CharacterDamageManagerComponent));
		if (dmgmngr.CanBeHealed() == false)
		{
			SetCannotPerformReason("(Full Health)");
			return false;
		}
		return true;
	}
};
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageItemCheckActionCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute("Item needed to be delivered", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_WantedItem;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute(defvalue: "1", desc : "if set to false character will be checked instead of player")]
	bool m_bCheckPlayer
	
	override bool CanBePerformed(IEntity Character, IEntity Player, DialogueStage ParentStage)
	{
		IEntity CharToCheck;
		if (m_bCheckPlayer){CharToCheck = Player;}
		else CharToCheck = Character;
		
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(CharToCheck.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return false;
		
		DS_PrefabResource_Predicate pred = new DS_PrefabResource_Predicate(m_WantedItem);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		
		if (entitiesToDrop.Count() < m_WantedAmount)
		{
			if (m_bCheckPlayer)
				m_sCantBePerformedReason = string.Format("  [Missing X%1 %2] ", m_WantedAmount - entitiesToDrop.Count() , m_WantedItem);
			else
			{
				string Name = DS_DialogueComponent.GetCharacterName(Character);
				m_sCantBePerformedReason = string.Format("  [%1 is missing X%2 %3] ", Name ,m_WantedAmount - entitiesToDrop.Count() , m_WantedItem);
			}
				
			return false;
		}		
		return true;
	}
}
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageItemTypeCheckActionCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ECommonItemType))]
	ECommonItemType m_WantedItem;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute(defvalue: "1", desc : "if set to false character will be checked instead of player")]
	bool m_bCheckPlayer
	
	override bool CanBePerformed(IEntity Character, IEntity Player, DialogueStage ParentStage)
	{
		IEntity CharToCheck;
		if (m_bCheckPlayer){CharToCheck = Player;}
		else CharToCheck = Character;
		
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(CharToCheck.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return false;
		
		SCR_ItemTypeSearchPredicate pred = new SCR_ItemTypeSearchPredicate(InventoryItemComponent, m_WantedItem, null);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		
		if (entitiesToDrop.Count() < m_WantedAmount)
		{
			if (m_bCheckPlayer)
				m_sCantBePerformedReason = string.Format("  [Missing X%1 %2] ", m_WantedAmount - entitiesToDrop.Count() , m_WantedItem);
			else
			{
				string Name = DS_DialogueComponent.GetCharacterName(Character);
				m_sCantBePerformedReason = string.Format("  [%1 is missing X%2 %3] ", Name ,m_WantedAmount - entitiesToDrop.Count() , m_WantedItem);
			}
				
			return false;
		}		
		return true;
	}
}
