
[BaseContainerProps(configRoot:true)]
class DS_BaseDialogueStageActionCondition
{
	[Attribute()]
	string m_sCantBePerformedDialogue;
	
	[Attribute()]
	string m_sCantBePerformedText;
	
	DS_DialogueStage OwnerStage;
	
	int m_iIndex;
	
	string m_sCantBePerformedReason = "(Cant Be Performed)";
	
	bool CanBePerformed(IEntity Character, IEntity Player){return true;};
	
	void GetCannotPerformReason(out string CantBePReason){CantBePReason = m_sCantBePerformedReason;}
	
	void SetCannotPerformReason(string reason){m_sCantBePerformedReason = reason;}
	
	void GetCannotPerformDialogue(out string CantBePDialogue){CantBePDialogue = m_sCantBePerformedDialogue;}
	
	sealed void Init(DS_DialogueStage Owner,int Index)
	{
		if (Owner)
			OwnerStage = Owner;
		m_iIndex = Index;
		if (m_sCantBePerformedText)
			m_sCantBePerformedReason = m_sCantBePerformedText;
	};
}
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
	override bool CanBePerformed(IEntity Character, IEntity Player)
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
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{	
		if (OwnerStage.m_iTimesPerformed > 0)
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
	[Attribute(desc :"Used incase above bool is set to false.")]
	string m_sMaxHPUnperformableReason;
	[Attribute("10", UIWidgets.Slider, "HP threashold", "0.0 100.0 1.0")]
	float m_fHPThreashold;
	[Attribute()]
	bool m_bResaultWhenThreasholdIsMet;
	[Attribute(desc :"Used incase above bool is set to false.")]
	string m_sThreasholdMetUnperformableReason;
	[Attribute()]
	bool m_bResaultWhenThreasholdIsNotMet;
	[Attribute(desc :"Used incase above bool is set to false.")]
	string m_sThreasholdNotMetUnperformableReason;
	
	override bool CanBePerformed(IEntity Character, IEntity Player)
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
class DS_DialogueStageInBaseRangeCondition : DS_BaseDialogueStageActionCondition
{
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		SCR_GameModeCampaign gamemod = SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
		SCR_CampaignMilitaryBaseManager Baseman = gamemod.GetBaseManager();
		vector pos = Character.GetOrigin();
		SCR_CampaignMilitaryBaseComponent base = Baseman.FindClosestBase(pos);
		if (base)
		{
			if (vector.Distance(pos, base.GetOwner().GetOrigin()) < base.GetRadius())
			{
				return true;
			}
		}
		return false;
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
	
	override bool CanBePerformed(IEntity Character, IEntity Player)
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
				m_sCantBePerformedReason = string.Format("  [Missing X%1 %2] ", m_WantedAmount - entitiesToDrop.Count() , FilePath.StripPath( m_WantedItem ));
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
	
	override bool CanBePerformed(IEntity Character, IEntity Player)
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
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageVehicleTypeCheckWithinBaseActionCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute("0", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EVehicleType))]
	EVehicleType m_VehicleType;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute()]
	int m_iRange;
	ref array <Vehicle> vehs = {};
	ref array <Vehicle> correctveh = {};
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		GetGame().GetWorld().QueryEntitiesBySphere(Character.GetOrigin(), m_iRange, QueryEntitiesForVeh);
		if (vehs.IsEmpty())
			return false;
		for (int i; i < vehs.Count();i ++)
		{
			if (vehs[i].m_eVehicleType == m_VehicleType)
			{
				correctveh.Insert(vehs[i]);
			}
		}
		if (correctveh.Count() < m_WantedAmount)
			return false;
		return true;
	}
	private bool QueryEntitiesForVeh(IEntity e)
	{
		Vehicle veh = Vehicle.Cast(e);
		if (!veh)
			return true;
		
		ScriptedDamageManagerComponent dmg = ScriptedDamageManagerComponent.Cast(e.FindComponent(ScriptedDamageManagerComponent));
		if (dmg.GetState() == EDamageState.DESTROYED)
			return true;

		vehs.Insert(veh);
		return true;
	}
}
//---------------------------------------------------------------------------------------------------//
class DialogueStageActionConditionTitleAttribute : BaseContainerCustomTitleField
{
	override bool _WB_GetCustomTitle(BaseContainer source, out string title)
	{
		
		string classname = ToString();
		title = classname;
		return true;
	}
}
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageResupplyKitActionCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute()]
	ResourceName m_KitPrefab;
	
	[Attribute()]
	ResourceName m_CurrencyPrefab;
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		PrefabResource_Predicate Pred = new PrefabResource_Predicate(m_KitPrefab);
		array <IEntity> foundkits = new array <IEntity>();
		inv.FindItems(foundkits, Pred);
		bool haskittofill;
		int ammounttifill;
		for (int i = 0; i < foundkits.Count(); i++)
		{
			SCR_ResourceComponent rscrs = SCR_ResourceComponent.Cast(foundkits[i].FindComponent(SCR_ResourceComponent));
			if (rscrs)
			{
				SCR_ResourceContainer container = rscrs.GetContainer(EResourceType.SUPPLIES);
				if (container.GetMaxResourceValue() > container.GetResourceValue())
				{
					haskittofill = true;
					ammounttifill = ammounttifill + container.GetMaxResourceValue() - container.GetResourceValue();
				}
			}
		}
		if (!haskittofill)
			return false;
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(Player);
		WalletEntity wallet = char.GetWallet();
		if (!wallet)
			return false;
		int cutam = wallet.GetCurrencyAmmount();
		if (cutam == 1)
			return false;
		
		return true;
	}
}
enum SP_EDialogueConditionSet
{
	ANY,
	ALL
}
