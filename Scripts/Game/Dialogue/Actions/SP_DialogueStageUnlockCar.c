[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageLockUnlockCarAction : DialogueStage
{
	[Attribute(defvalue: "true", desc: "If set to true, it will lock the vehicle, if set to false it will unlock the vehicle")];
	protected bool m_bLock;
	
	[Attribute(desc :"Name of Car in world")]
	string m_sCarName;	
	
	override void Perform(IEntity Character, IEntity Player)
	{
		IEntity Car = GetGame().GetWorld().FindEntityByName(m_sCarName);
		if (!Car || !Vehicle.Cast(Car))
			return;

		SCR_VehicleSpawnProtectionComponent spawnProtectionComponent = SCR_VehicleSpawnProtectionComponent.Cast(Car.FindComponent(SCR_VehicleSpawnProtectionComponent));
		if (!spawnProtectionComponent)
			return;

		if (m_bLock)
		{
			spawnProtectionComponent.SetProtectOnlyDriverSeat(false);
			spawnProtectionComponent.SetReasonText("Locked");
			spawnProtectionComponent.SetVehicleOwner(-2);
		}
		else
		{
			spawnProtectionComponent.SetProtectOnlyDriverSeat(true);
			spawnProtectionComponent.SetReasonText("#AR-Campaign_Action_CannotEnterVehicle-UC");
			spawnProtectionComponent.ReleaseProtection();
		}
		super.Perform(Character, Player);
	};
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		return true;
	}

};
[BaseContainerProps(configRoot:true), DialogueStageTitleAttribute()]
class DialogueStageBuyExistingVehicleAction : DialogueStage
{
	[Attribute("Item needed to be delivered", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_WantedItem;
	
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	
	[Attribute(desc :"Name of Car in world")]
	string m_sCarName;	
	
	override void Perform(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return;		
		
		SP_PrefabResource_Predicate pred = new SP_PrefabResource_Predicate(m_WantedItem);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		
		int movedCount = 0;
		ref array<IEntity> givenItems = {};
		
		for (int i = 0; i < entitiesToDrop.Count(); i++)
		{
			IEntity item = entitiesToDrop[i];
			if (!item) continue;
			InventoryItemComponent pInvComp = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
			InventoryStorageSlot parentSlot = pInvComp.GetParentSlot();
			bool removed = inv.TryRemoveItemFromStorage(item,parentSlot.GetStorage());
			if (removed)
			{
				movedCount++;				
				givenItems.Insert(item);
				
				if (movedCount >= m_WantedAmount)
					break;
			}
		}				
		
		// todo: what to do next
		if (movedCount >= m_WantedAmount)
		{	
			for (int i = 0; i < movedCount; i++)
			{			
				delete givenItems.Get(i);
			}
		}
		givenItems.Clear();;
		IEntity Car = GetGame().GetWorld().FindEntityByName(m_sCarName);
		if (!Car || !Vehicle.Cast(Car))
			return;

		SCR_VehicleSpawnProtectionComponent spawnProtectionComponent = SCR_VehicleSpawnProtectionComponent.Cast(Car.FindComponent(SCR_VehicleSpawnProtectionComponent));
		if (!spawnProtectionComponent)
			return;

		spawnProtectionComponent.SetProtectOnlyDriverSeat(true);
		spawnProtectionComponent.SetReasonText("#AR-Campaign_Action_CannotEnterVehicle-UC");
		spawnProtectionComponent.ReleaseProtection();
		super.Perform(Character, Player);
	};
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return false;
		
		SP_PrefabResource_Predicate pred = new SP_PrefabResource_Predicate(m_WantedItem);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		if (entitiesToDrop.Count() < m_WantedAmount)
		{
			m_sCantBePerformedReason = "[Missing Item]";
			return false;
		}		
		return true;
	}
};