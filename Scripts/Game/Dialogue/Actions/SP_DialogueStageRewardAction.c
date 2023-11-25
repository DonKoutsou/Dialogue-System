[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class SP_DialogueStageRewardAction : SP_BaseDialogueStageAction
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
