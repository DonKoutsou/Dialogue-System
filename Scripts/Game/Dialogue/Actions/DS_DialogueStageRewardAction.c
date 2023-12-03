[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageRewardAction : DS_BaseDialogueStageAction
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ref array <ref ResourceName> m_ItemToGive;
	[Attribute("", UIWidgets.Coords, params: "", desc: "")]
	vector m_SpawnOffset;
	
	override void Perform(IEntity Character, IEntity Player)
	{
		vector mat[4];
		Character.GetWorldTransform(mat);
		vector spawnPos = m_SpawnOffset.Multiply3(mat) + mat[3];
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = spawnPos; 
		for (int i = 0; i < m_ItemToGive.Count(); i++)
		{
			Resource res = Resource.Load(m_ItemToGive[i]);
			if (res)
			{	
				GetGame().SpawnEntityPrefab(res, Character.GetWorld(), params);
			}
		}
		super.Perform(Character, Player);
	};
	
};
[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageTakeItemAction : DS_BaseDialogueStageAction
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, params: "et", desc: "")]
	ResourceName m_ItemToTake;
	[Attribute("1", UIWidgets.EditBox, params: "1 1000", desc: "")]
	int m_WantedAmount;
	[Attribute(defvalue: "1", desc : "if set to false character will be checked instead of player")]
	bool m_bCheckPlayer
	override void Perform(IEntity Character, IEntity Player)
	{
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(Player.FindComponent(InventoryStorageManagerComponent));
		DS_PrefabResource_Predicate pred = new DS_PrefabResource_Predicate(m_ItemToTake);
		array<IEntity> entitiesToDrop = new array<IEntity>;
		inv.FindItems(entitiesToDrop, pred);
		InventoryStorageManagerComponent Cinv = InventoryStorageManagerComponent.Cast(Character.FindComponent(InventoryStorageManagerComponent));
		for (int i; i < m_WantedAmount;i++)
		{
			BaseInventoryStorageComponent pItemComp = BaseInventoryStorageComponent.Cast( entitiesToDrop[i].GetParent().FindComponent( BaseInventoryStorageComponent ) );
			inv.TryRemoveItemFromStorage(entitiesToDrop[i], pItemComp);
			Cinv.TryInsertItem(entitiesToDrop[i]);
		}
		
		super.Perform(Character, Player);
	};
	
};
