[BaseContainerProps(configRoot:true), DialogueStageActionTitleAttribute()]
class DS_DialogueStageTimedRewardAction : DS_BaseDialogueStageAction
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
[BaseContainerProps(configRoot:true)]
class DS_DialogueStageTimedRewardActionCondition : DS_BaseDialogueStageActionCondition
{
	[Attribute()]
  int TimeOffset;
	bool TimeChecked;
	int TimeToBeAvailable;
	int DayToBeAvailable;
	
	override bool CanBePerformed(IEntity Character, IEntity Player)
	{
		GenericEntity ent = GenericEntity.Cast(Character);
		ChimeraWorld world = ent.GetWorld();
		TimeAndWeatherManagerEntity weatherManager = world.GetTimeAndWeatherManager();
		if(TimeChecked == false)
		{
			TimeToBeAvailable = weatherManager.GetTime().m_iHours + TimeOffset;
			DayToBeAvailable = weatherManager.GetDay();
			if(TimeToBeAvailable > 24)
			{
				TimeToBeAvailable = TimeToBeAvailable - 24;
				DayToBeAvailable = DayToBeAvailable + 1;
			}
		}
		TimeChecked = true;
		int currentday = weatherManager.GetDay();
		if (DayToBeAvailable == currentday)
		{
			int currenttime = weatherManager.GetTime().m_iHours;
			if (TimeToBeAvailable <= currenttime)
			{
				return true;
			}
		}
		if (DayToBeAvailable < currentday)
		{
			return true;
		}
		m_sCantBePerformedReason = "[" + "Not ready, come back at" + " " + TimeToBeAvailable + " " + "o'clock" + "]";
		return false;
	}
}