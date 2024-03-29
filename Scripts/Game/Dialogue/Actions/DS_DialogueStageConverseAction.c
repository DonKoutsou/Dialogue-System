[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageConverseAction : DS_BaseDialogueStageAction
{
	[Attribute("Item needed to be delivered", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_WantedItem;
	[Attribute("", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_ItemToGive;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute("", UIWidgets.Coords, params: "", desc: "")]
	vector m_SpawnOffset;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return;		
		
		DS_PrefabResource_Predicate pred = new DS_PrefabResource_Predicate(m_WantedItem);
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
		vector mat[4];
		Character.GetWorldTransform(mat);
		vector spawnPos = m_SpawnOffset.Multiply3(mat) + mat[3];
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = spawnPos; 
		Resource res = Resource.Load(m_ItemToGive);
		if (res)
		{	
			GetGame().SpawnEntityPrefab(res, Character.GetWorld(), params);
		}
		super.Perform(Character, Player);
	};

};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageBuyVehicleAction : DS_BaseDialogueStageAction
{
	[Attribute("Item needed to be delivered", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_WantedItem;
	[Attribute("", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_ItemToGive;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute("", UIWidgets.Coords, params: "", desc: "")]
	vector m_SpawnOffset;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		if (!inv)
			return;		
		
		DS_PrefabResource_Predicate pred = new DS_PrefabResource_Predicate(m_WantedItem);
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
		vector mat[4];
		Character.GetWorldTransform(mat);
		vector spawnPos = m_SpawnOffset.Multiply3(mat) + mat[3];
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = spawnPos; 
		Resource res = Resource.Load(m_ItemToGive);
		if (res)
		{	
			GetGame().SpawnEntityPrefab(res, Character.GetWorld(), params);
		}
		super.Perform(Character, Player);
	};

};